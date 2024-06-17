//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "AnimModelMeshRenderer.h"

#include "gameobject.h"
#include "ShaderComponent.h"
//-----------------------------------------------------------------------------
// �����������ł�.
// ModelMeshLoad���烂�f���̏����擾
//-----------------------------------------------------------------------------
void AnimModelMeshRenderer::Init(AnimModelMeshLoad& mesh)
{
	// ���_�o�b�t�@����
	m_VertexBuffer.Create(mesh.GetVertices());

	// �C���f�b�N�X�o�b�t�@����
	m_IndexBuffer.Create(mesh.GetIndices());

	// �}�e���A�������擾���}�e���A�����ɒ萔�o�b�t�@�𐶐�����
	const std::vector<MATERIAL>& materials = mesh.GetMaterials();

	for (auto& mtrl : materials) {
		CMaterial* m = new CMaterial();
		m->Init(mtrl);
		m_Materials.emplace_back(m);
	}

	// �f�B�t���[�Y�e�N�X�`���e�[�u�������b�V������擾
	m_DiffuseSRVtable = mesh.GetSRVtable();

	// SUBSET���擾
	m_Subsets = mesh.GetSubsets();

	// �萔�o�b�t�@�����i�{�[���R���r�l�[�V�����i�[�p�j
	bool sts = CreateConstantBufferWrite(											
		Renderer::GetDevice(),					    // �f�o�C�X�I�u�W�F�N�g			
		sizeof(CBBoneCombMatrix),					// �R���X�^���g�o�b�t�@�T�C�Y	
		&m_BoneCombMtxCBuffer);						// �R���X�^���g�o�b�t�@			

	assert(m_BoneCombMtxCBuffer);
}

// �X�V
void AnimModelMeshRenderer::Update(AnimModelMeshLoad& mesh)
{
	// ���b�V������X�V���ꂽ�{�[���R���r�l�[�V�����s��Q���擾
	const std::vector<aiMatrix4x4> mtxcontainer = mesh.GetBoneCombinationMatrix();

	// �萔�o�b�t�@�X�V
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrix* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr)) {
		pData = (CBBoneCombMatrix*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			mtxcontainer.data(),
			sizeof(aiMatrix4x4) * mtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}
}
//-----------------------------------------------------------------------------
// �`�揈���ł�.
//-----------------------------------------------------------------------------
void AnimModelMeshRenderer::Draw()
{
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();

	// ���_�o�b�t�@�ݒ�
	m_VertexBuffer.SetGPU();
	// �C���f�b�N�X�o�b�t�@�ݒ�
	m_IndexBuffer.SetGPU();

	// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���W�X�^�ɃZ�b�g
	devicecontext->VSSetConstantBuffers(1, 1, &m_BoneCombMtxCBuffer);

	// �T�u�Z�b�g�̐������[�v�i�T�u�Z�b�g���̓}�e���A���̐��j
	for (auto& subset : m_Subsets) {

		// (�e�N�X�`��)SRV���Z�b�g
		devicecontext->PSSetShaderResources(0, 1, &m_DiffuseSRVtable[subset.MaterialIdx]);

		// �}�e���A�����擾����
		if (m_isMaterial)
		{
			m_Materials[subset.MaterialIdx]->SetShader(m_GameObject->GetComponent<ShaderComponent>()->GetVSShader());		// ���݂�VS�V�F�[�_�[���擾

			m_Materials[subset.MaterialIdx]->SetGPU(2);	// GPU�ɃZ�b�g
		}

		// �h���[�R�[��
		devicecontext->DrawIndexed(
			subset.IndexNum,	// �`�悷��C���f�b�N�X��
			subset.IndexBase,	// �C���f�b�N�X�o�b�t�@�x�[�X�J�n�ʒu�i���̏ꏊ���C���f�b�N�X�O�ԖڂɂȂ�j	
			subset.VertexBase);	// ���_�o�b�t�@�x�[�X�J�n�ʒu�i���̏ꏊ�����_�̂O�ԖڂɂȂ�j
	}
}
//-----------------------------------------------------------------------------
// ��������ł�.
//-----------------------------------------------------------------------------
void AnimModelMeshRenderer::Uninit()
{
	m_BoneCombMtxCBuffer->Release();

	for (auto& mtrl : m_Materials)
	{
		delete mtrl;
	}
}
//-----------------------------------------------------------------------------
// �}�e���A�����Z�b�g����֐��ł�.
//-----------------------------------------------------------------------------
void AnimModelMeshRenderer::SetMaterialShader(bool _is)
{
	m_isMaterial = _is;
}

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "ModelMeshRenderer.h"

#include "gameobject.h"
#include "ShaderComponent.h"
//-----------------------------------------------------------------------------
// �����������ł�.
// ModelMeshLoad���烂�f���̏����擾
//-----------------------------------------------------------------------------
void ModelMeshRenderer::Init(ModelMeshLoad& mesh)
{
	// ���_�o�b�t�@����
	m_VertexBuffer.Create(mesh.GetVertices());

	// �C���f�b�N�X�o�b�t�@����
	m_IndexBuffer.Create(mesh.GetIndices());

	// �}�e���A�������擾���}�e���A�����ɒ萔�o�b�t�@�𐶐�����
	const std::vector<MATERIAL>& materials = mesh.GetMaterials();

	for (auto& mtrl : materials)
	{
		CMaterial* m = new CMaterial();
		m->Init(mtrl);
		m_Materials.emplace_back(m);
	}

	// �f�B�t���[�Y�e�N�X�`���e�[�u�������b�V������擾
	m_DiffuseSRVtable = mesh.GetSRVtable();

	// SUBSET���擾
	m_Subsets = mesh.GetSubsets();
}
//-----------------------------------------------------------------------------
// �`�揈���ł�.
//-----------------------------------------------------------------------------
void ModelMeshRenderer::Draw()
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

	// �T�u�Z�b�g�̐������[�v�i�T�u�Z�b�g���̓}�e���A���̐��j
	for (auto& subset : m_Subsets) {

		// (�e�N�X�`��)SRV���Z�b�g
		devicecontext->PSSetShaderResources(0, 1, &m_DiffuseSRVtable[subset.MaterialIdx]);

		// �}�e���A�����擾����
		if (m_isMaterial)
		{
			m_Materials[subset.MaterialIdx]->SetShader(m_GameObject->GetComponent<ShaderComponent>()->GetVSShader());		// ���݂�VS�V�F�[�_�[���擾

			m_Materials[subset.MaterialIdx]->SetGPU(1);	// GPU�ɃZ�b�g
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
void ModelMeshRenderer::Uninit()
{
	for (auto& mtrl : m_Materials)
	{
		delete mtrl;
	}
}
//-----------------------------------------------------------------------------
// �}�e���A�����Z�b�g����֐��ł�.
//-----------------------------------------------------------------------------
void ModelMeshRenderer::SetMaterialShader(bool _is)
{
	m_isMaterial = _is;
}

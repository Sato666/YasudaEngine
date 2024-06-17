#include "AnimationModelRenderer.h"

void AnimationModelRenderer::Draw()
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

	// 20230909-02 ���W�X�^2�ɃZ�b�g
	Renderer::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_BoneCombMtxCBuffer);

	// �T�u�Z�b�g�̐������[�v�i�T�u�Z�b�g���̓}�e���A���̐��j
	for (auto& subset : m_Subsets) {

		// (�e�N�X�`��)SRV���Z�b�g
		devicecontext->PSSetShaderResources(0, 1, &m_DiffuseSRVtable[subset.MaterialIdx]);

		// �}�e���A���萔�o�b�t�@��GPU�ɃZ�b�g
		m_Materials[subset.MaterialIdx]->SetGPU();

		// �h���[�R�[��
		devicecontext->DrawIndexed(
			subset.IndexNum,	// �`�悷��C���f�b�N�X��
			subset.IndexBase,	// �C���f�b�N�X�o�b�t�@�x�[�X�J�n�ʒu�i���̏ꏊ���C���f�b�N�X�O�ԖڂɂȂ�j	
			subset.VertexBase);	// ���_�o�b�t�@�x�[�X�J�n�ʒu�i���̏ꏊ�����_�̂O�ԖڂɂȂ�j

	}
}

void AnimationModelRenderer::Init(AnimaionModelLoad& mesh)
{
	// ���_�o�b�t�@����
	m_VertexBuffer.Create(mesh.GetVertices());

	// �C���f�b�N�X�o�b�t�@����
	m_IndexBuffer.Create(mesh.GetIndices());

	// �}�e���A�������擾���}�e���A�����ɒ萔�o�b�t�@�𐶐�����
	const std::vector<MATERIAL>& materials = mesh.GetMaterials();

	// �}�e���A�������擾���}�e���A�����ɒ萔�o�b�t�@�𐶐�����
	m_Bone = mesh.GetBone();

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

	Model = &mesh;
}

void AnimationModelRenderer::Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate)
{
	// �A�j���[�V�������肩�H
	if (m_Animation.count(AnimationName1) == 0)
		return;
	if (m_Animation.count(AnimationName2) == 0)
		return;

	if (!m_Animation[AnimationName1]->HasAnimations())
		return;
	if (!m_Animation[AnimationName2]->HasAnimations())
		return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	// ���݂̃A�j���[�V�����ɂ��Ċ֘A����{�[����S�čX�V
	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosFrom = pos;
		bone->BlendRFrom = rot;
	}

	// ���݂̃A�j���[�V����2�ɂ��Ċ֘A����{�[����S�čX�V
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = Frame1 % nodeAnim->mNumRotationKeys;//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = Frame1 % nodeAnim->mNumPositionKeys;//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->BlendPosTo = pos;
		bone->BlendRTo = rot;
	}

	// �u�����h
	for (unsigned int c = 0; c < animation2->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation2->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		// �ʒu�̃u�����h
		aiVector3D pos1 = bone->BlendPosFrom;
		aiVector3D pos2 = bone->BlendPosTo;

		aiVector3D pos = pos1 * (1.0f - BlendRate) + pos2 * BlendRate;//���`���

		// �p���̃u�����h
		aiQuaternion rot1 = bone->BlendRFrom;
		aiQuaternion rot2 = bone->BlendRTo;

		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//���ʐ��`���

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// 20230909 �{�[���R���r�l�[�V�����s��̔z��𐶐�����
	std::vector<aiMatrix4x4> bonecombmtxcontainer;					// 20230909
	bonecombmtxcontainer.resize(m_Bone.size());						// 20230909
	for (auto data : m_Bone) {										// 20230909
		bonecombmtxcontainer[data.second.idx] = data.second.Matrix;	// 20230909
	}																// 20230909

	// 20230909 �]�u
	for (auto& bcmtx : bonecombmtxcontainer)
	{
		// �]�u����
		bcmtx.Transpose();
	}

	// 20230909-02 �萔�o�b�t�@�ɔ��f������
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CBBoneCombMatrx* pData = nullptr;

	HRESULT hr = Renderer::GetDeviceContext()->Map(
		m_BoneCombMtxCBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&MappedResource);

	if (SUCCEEDED(hr))
	{
		pData = (CBBoneCombMatrx*)MappedResource.pData;
		memcpy(pData->BoneCombMtx,
			bonecombmtxcontainer.data(),
			sizeof(aiMatrix4x4) * bonecombmtxcontainer.size());
		Renderer::GetDeviceContext()->Unmap(m_BoneCombMtxCBuffer, 0);
	}
}

void AnimationModelRenderer::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// �����œn���ꂽ�m�[�h�����L�[�Ƃ��ă{�[�������擾����
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;					// �����l�͒P�ʍs��

	//	worldMatrix *= matrix;						
	worldMatrix = matrix;						// �e�̈ʒu�A�p����������ԁi�t�{�[���I�t�Z�b�g�s��j
	worldMatrix *= bone->AnimationMatrix;		// �����œn���ꂽ�s����|���Z�i���m�[�h�̃A�j���[�V�����𔽉f������j�i�{�[����Ԃł̂��́j

	bone->Matrix = worldMatrix;					// �v���O�������ɗp�ӂ��Ă���s��ɔ��f������
	bone->Matrix *= bone->OffsetMatrix;			// �I�t�Z�b�g�s��𔽉f������

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimationModelRenderer::Uninit()
{
	for (auto& mtrl : m_Materials)
	{
		mtrl->Uninit();
		delete mtrl;
	}

	for (auto& texres : m_DiffuseSRVtable)
	{
		if (texres != nullptr) {
			texres->Release();
		}
	}

	m_BoneCombMtxCBuffer->Release();						// 20230909-02

	aiReleaseImport(m_AiScene);

	// �A�j���[�V���������݂��Ă�����������
	if (m_AiScene->HasAnimations())
	{
		for (std::pair<const std::string, const aiScene*> pair : m_Animation)
		{
			aiReleaseImport(pair.second);
		}
	}

	m_Bone.clear();
}

void AnimationModelRenderer::LoadAnimation(const char* FileName, const char* Name)
{
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}
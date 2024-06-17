//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "AnimModelMeshLoad.h"
#include <SimpleMath.h>
#include <DirectXTex.h>
#include <iostream>		// std::string�p
//-----------------------------------------------------------------------------
// �K�v�ȋ@�\
#include "main.h"
#include "renderer.h"
#include "utility.h"
//-----------------------------------------------------------------------------
// ���[�h�����ł�.
//-----------------------------------------------------------------------------
bool AnimModelMeshLoad::Load(std::string filename)
{
	bool sts{};

	m_filename = filename;			// �e�N�X�`���ǂݍ��ݗp�ɕۑ�
	//-----------------------------------------------------------------------------
	// �V�[�����\�z
	Assimp::Importer importer;

	m_AiScene = importer.ReadFile(
		filename.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs |
		aiProcess_PreTransformVertices |
		aiProcess_MakeLeftHanded);

	// �G���[����
	if (m_AiScene == nullptr)
	{
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
		return false;
	}

	// Model���\�L
	std::filesystem::path p = std::filesystem::u8path(filename);
	std::cout << std::endl;
	std::cout << "Model��  : " << p.stem() << std::endl;
	std::cout << "�g���q : " << p.extension() << std::endl;

	//-----------------------------------------------------------------------------
	// ���f�����擾
	
	//�ċA�I�Ƀ{�[�������i��ŊK�w�\�������j
	CreateBone(m_AiScene->mRootNode);

	// �{�[���̔z��ʒu���i�[����	
	unsigned int num = 0;			
	for (auto& data : m_Bone) {		
		data.second.idx = num;		
		num++;						
	}								

	// ���b�V�����Ń��b�V���R���e�i�z�񐔂�����
	m_subsets.resize(m_AiScene->mNumMeshes);

	// ���b�V���������[�v
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {

		aiMesh* mesh{};
		mesh = m_AiScene->mMeshes[m];

		// ���b�V���P�ʂŒ��_�f�[�^���擾
		int vnum = GetVertices(mesh);

		// ���b�V���P�ʂŃC���f�b�N�X�f�[�^�擾
		int idxnum = GetIndices(mesh);

		// ���b�V���Ŏg�p���Ă���}�e���A�������擾
		int mtrlidx = mesh->mMaterialIndex;

		// �C���f�b�N�X����ۑ�
		m_subsets[m].IndexNum = idxnum;

		// ���_����ۑ�
		m_subsets[m].VertexNum = vnum;

		// �}�e���A���C���f�b�N�X��ۑ�
		m_subsets[m].MaterialIdx = mtrlidx;

		// �{�[�������擾����(�T�u�Z�b�g�P�ʂ�)
		m_SubsetBones.emplace_back(GetBoneInfo(mesh));
	}

	// ���_���\��
	std::cout << "���_�� : " << m_vertices.size() << std::endl;
	std::cout << std::endl;

	// ���b�V���̃x�[�X�����\�z
	CalcMeshBaseIndex();

	// �{�[��ID�ƃE�G�C�g�l�𒸓_�f�[�^�ɔ��f������
	// �K��CalcMeshBaseIndex()�̌�ōs����
	SetBoneDataToVertices();

	// �S�e�N�X�`�����擾
	GetTextures(m_AiScene);

	// �}�e���A�����擾
	GetMaterials(m_AiScene);

	std::cout << "----------------------------------------" << std::endl;

	return true;
}
//-----------------------------------------------------------------------------
// ��������ł�.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::Uninit()
{
	// SRV���
	for (auto& texres : m_Texturedic)
	{
		if (texres.second != nullptr)
		{
			texres.second->Release();
		}
	}

	for (auto bone : m_Bone)
	{
		if (&bone.second != nullptr)
		{
			bone.second;
		}
	}

	m_AllTexture.clear();
	m_vertices.clear();
	m_indices.clear();
	m_materials.clear();
	m_subsets.clear();
	m_SubsetBones.clear();
	m_bonecombmtxcontainer.clear();
}
//-----------------------------------------------------------------------------
// ���b�V���P�ʂŒ��_�f�[�^���擾���ĕԂ�
//-----------------------------------------------------------------------------
unsigned int AnimModelMeshLoad::GetVertices(const aiMesh* aimesh)
{
	// ���_�������[�v
	for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++)
	{
		ANIMMODELVERTEX v;

		// �ʒu
		v.Position.x = aimesh->mVertices[vidx].x;
		v.Position.y = aimesh->mVertices[vidx].y;
		v.Position.z = aimesh->mVertices[vidx].z;

		// �@������H
		if (aimesh->HasNormals()) {
			v.Normal.x = aimesh->mNormals[vidx].x;
			v.Normal.y = aimesh->mNormals[vidx].y;
			v.Normal.z = aimesh->mNormals[vidx].z;
		}
		else {
			v.Normal = DirectX::SimpleMath::Vector3(0, 0, 0);
		}

		// �e�N�X�`������H
		if (aimesh->HasTextureCoords(0)) {
			v.TexCoord.x = aimesh->mTextureCoords[0][vidx].x;				// 20230809 bug fix by ts
			v.TexCoord.y = aimesh->mTextureCoords[0][vidx].y;				// 20230809 bug fix by ts
		}
		else {
			v.TexCoord = DirectX::SimpleMath::Vector2(0, 0);
		}

		// ���_�J���[����H
		if (aimesh->HasVertexColors(0)) {
			v.Diffuse.x = aimesh->mColors[0][vidx].r;
			v.Diffuse.y = aimesh->mColors[0][vidx].g;
			v.Diffuse.z = aimesh->mColors[0][vidx].b;
			v.Diffuse.w = aimesh->mColors[0][vidx].a;
		}
		else {
			v.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);								// ���������ꍇ�͔�
		}

		// ���_�f�[�^��ۑ�
		m_vertices.emplace_back(v);
	}

	// aiMesh���ێ����钸�_����߂�
	return aimesh->mNumVertices;
}
//-----------------------------------------------------------------------------
// ���b�V���P�ʂŃC���f�b�N�X�f�[�^���擾���ĕԂ�
//-----------------------------------------------------------------------------
unsigned int AnimModelMeshLoad::GetIndices(const aiMesh* aimesh)
{
	// �ʐ������[�v
	for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
	{
		const aiFace* face = &aimesh->mFaces[f];
		assert(face->mNumIndices <= 3);				// �|���S����3���_�ł͂Ȃ���΃G���[

		// �C���f�b�N�X�f�[�^��ۑ�
		m_indices.emplace_back(face->mIndices[0]);
		m_indices.emplace_back(face->mIndices[1]);
		m_indices.emplace_back(face->mIndices[2]);
	}

	return aimesh->mNumFaces * 3;	// 3����
}
//-----------------------------------------------------------------------------
// ���b�V���̃x�[�X�𐶐����鏈��
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::CalcMeshBaseIndex()
{
	// �T�u�Z�b�g�������[�v
	for (int m = 0; m < m_subsets.size(); m++)
	{
		// ���_�o�b�t�@�̃x�[�X���v�Z
		m_subsets[m].VertexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].VertexBase += m_subsets[i].VertexNum;
		}

		// �C���f�b�N�X�o�b�t�@�̃x�[�X���v�Z
		m_subsets[m].IndexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].IndexBase += m_subsets[i].IndexNum;
		}
	}
}
//-----------------------------------------------------------------------------
// ���f���̃e�N�X�`���[���擾���鏈��.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::GetTextures(const aiScene* aiscene)
{
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// ���̃V�[�������e�N�X�`����S�ēǂݍ���
	for (int texidx = 0; texidx < aiscene->mNumTextures; texidx++)
	{
		// �e�N�X�`�����擾
		aiTexture* aitexture = aiscene->mTextures[texidx];

		// �����e�N�X�`�����H
		if (isEmbeddedTexure(aitexture))
		{
			ID3D11ShaderResourceView* srv;

			aiTexture* aitexture = aiscene->mTextures[texidx];

			const size_t size = aitexture->mWidth;

			// SRV����
			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				device,
				devicecontext,
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&srv);

			assert(srv);

			// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
			m_Texturedic[aitexture->mFilename.data] = srv;
		}
		else
		{
			// �����e�N�X�`���ł͂Ȃ��ꍇ

			// �e�N�X�`���t�@�C�������擾����
			std::string filename = GetFileName(aitexture->mFilename.C_Str());

			// �e�N�X�`���t�@�C����ǂݍ���SRV�𐶐�
			// SRV
			ID3D11ShaderResourceView* srv = nullptr;

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				device,
				devicecontext,
				&srv);

			if (sts) {
				// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
				m_Texturedic[aitexture->mFilename.data] = srv;
			}
			else {
				m_Texturedic[aitexture->mFilename.data] = nullptr;
			}
		}
	}
}
//-----------------------------------------------------------------------------
// ���f���ɓ����e�N�X�`�������邩��Ԃ�����.
//-----------------------------------------------------------------------------
bool AnimModelMeshLoad::isEmbeddedTexure(const aiTexture* aitexture)
{
	if (aitexture->mHeight == 0 ||
		aitexture->mWidth == 0 ||
		aitexture->pcData == nullptr) {
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
// ���f���̃}�e���A�������擾���ăZ�b�g���鏈��.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::GetMaterials(const aiScene* aiscene)
{
	// ���̃��f���Ŏg�p���Ă��邷�ׂẴ}�e���A�������擾
	for (unsigned int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {

		// ���̃��b�V���p�̃}�e���A�����擾
		aiMaterial* mtl;
		mtl = aiscene->mMaterials[mrlidx];

		// �}�e���A�����擾
		GetaMatreial(mtl);
	}
}
//-----------------------------------------------------------------------------
// ���f���̃}�e���A�������擾���ăZ�b�g���鏈��.
//-----------------------------------------------------------------------------
void AnimModelMeshLoad::GetaMatreial(const aiMaterial* aimtrl)
{
	// �f�o�C�X�A�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// �}�e���A�����
	MATERIAL mymaterial{};

	// �e��p�����[�^�[
	aiColor3D color(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 ambient(0.3f, 0.3f, 0.3f, 1.0f);
	float shinness = 0.0f;

	// �������ː���
	mymaterial.Ambient = aimtrl->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;
	// �g�U���ː���
	mymaterial.Diffuse = aimtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;
	// ���ʔ��ː���
	mymaterial.Specular = aimtrl->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, shinness) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shinness);
	// ���Ɣ�������
	mymaterial.Emission = aimtrl->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS ?
		DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	// ���ʔ��ˋ��x
	mymaterial.Shininess = aimtrl->Get(AI_MATKEY_SHININESS, shinness) == AI_SUCCESS ? shinness : 0.0f;

	// �f�B�t���[�Y�e�N�X�`�����擾
	aiTextureType type = aiTextureType_DIFFUSE;
	int texnum = aimtrl->GetTextureCount(type);

	// 1���b�V���Ƀf�B�t�[�Y�e�N�X�`���P������������
	assert(texnum <= 1);

	// �}�e���A����
	aiString str;

	// assimp����e�N�X�`���t�@�C�������擾
	aimtrl->GetTexture(type, 0, &str);

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	aiString path;
	if (aimtrl->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		// SRV�����ɑ��݂��邩�H
		if (m_Texturedic.find(str.data) == m_Texturedic.end())
		{
			mymaterial.TextureEnable = FALSE;
			srv = nullptr;

			// pmx �̏ꍇ�V�[���ɓo�^����Ă��Ȃ��e�N�X�`�������݂���
			aimtrl->GetTexture(type, 0, &str);

			// �e�N�X�`���擾
			srv = LoadDiffuseTexture(path);

			if (srv == nullptr)
			{
				mymaterial.TextureEnable = FALSE;
			}
			else
			{
				mymaterial.TextureEnable = TRUE;
			}
		}
	}
	else
	{
		mymaterial.TextureEnable = TRUE;
		srv = m_Texturedic[str.data];
	}

	// SRV�ۑ�
	m_AllTexture.emplace_back(srv);

	// �}�e���A������ۑ�
	m_materials.emplace_back(mymaterial);
}
//-----------------------------------------------------------------------------
// ���f���ɓ����e�N�X�`�����Ȃ��ꍇ�����f�B���N�g������e�N�X�`�����擾���鏈��.
//-----------------------------------------------------------------------------
ID3D11ShaderResourceView* AnimModelMeshLoad::LoadDiffuseTexture(aiString _path)
{
	// u8�G���R�[�h��path���R���X�g���N�g
	std::filesystem::path p = std::filesystem::u8path(m_filename);

	// �t�@�C���̒T��
	std::string dir = p.string();
	dir = dir.substr(0, dir.find_last_of('/') + 1);		// �ŏ��̃t�@�C���p�X�́`/�܂Ŏ���Ă���

	// �e�N�X�`��
	aiString path = _path;	// ���f���ɃA�^�b�`����Ă���e�N�X�`�����擾
	HRESULT hr = texture.Create(path.C_Str());		// ���̂܂ܒT��

	// ���f���Ɠ����K�w��T��
	if (FAILED(hr)) {
		std::string file = dir;
		file += GetFileName(path.C_Str());
		hr = texture.Create(file.c_str());

		// �t�@�C���p�X�擾
		p = file;
		std::cout << "�f�B���N�g���� : " << file.c_str() << std::endl;
		std::cout << "�t�@�C����  : " << p.filename() << std::endl;
		std::cout << "�g���q : " << p.extension() << std::endl;
	}
	// ���s
	if (FAILED(hr)) {
		std::cout << "LoadTextureFile Error (" << GetFileName(path.C_Str()) << ")" << std::endl;
		return nullptr;
	}
	return texture.GetResource();
}

// �{�[����񐶐�(�X�^�e�B�b�N���b�V�����̑S�Ẵ{�[�������𐶐�����)
void AnimModelMeshLoad::CreateBone(aiNode* node)
{
	std::cout << "�{�[���� : " << GetFileName(node->mName.C_Str()) << std::endl;

	// �{�[�����ŎQ�Ƃł���悤�ɋ�̃{�[�������Z�b�g����
	m_Bone[node->mName.C_Str()];

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}

// �{�[�����A�{�[���C���f�b�N�X�A�{�[���E�F�C�g�𒸓_�ɃZ�b�g����
void AnimModelMeshLoad::SetBoneDataToVertices()
{
	// �{�[���C���f�b�N�X��������
	for (auto& v : m_vertices) {
		v.bonecnt = 0;
		for (int b = 0; b < 4; b++) {
			v.BoneIndex[b] = -1;
			v.BoneWeight[b] = 0.0f;
		}
	}

	// �T�u�Z�b�g���b�V�����̃R���e�i
	int subsetid = 0;
	for (auto& bones : m_SubsetBones) {

		// ���̃X�^�e�B�b�N���b�V�����̒��_�f�[�^�̃X�^�[�g�ʒu���擾
		int vertexbase = m_subsets[subsetid].VertexBase;

		// ���̃T�u�Z�b�g���̃{�[�����ЂƂÂ��o��
		for (auto& bone : bones)
		{
			for (auto& w : bone.weights) {
				int& idx = m_vertices[vertexbase + w.vertexindex].bonecnt;
				m_vertices[vertexbase + w.vertexindex].BoneWeight[idx] = w.weight;	// weight�l���Z�b�g
				m_vertices[vertexbase + w.vertexindex].BoneIndex[idx] = m_Bone[w.bonename].idx;
				//�{�[���̔z��ԍ����Z�b�g
				idx++;
				assert(idx <= 4);
			}
		}
		subsetid++;				// ���̃T�u�Z�b�g��
	}
}

// �T�u�Z�b�g�ɕR�Â��Ă���{�[�������擾����
std::vector<BONE> AnimModelMeshLoad::GetBoneInfo(const aiMesh* mesh)
{
	std::vector<BONE> bones;		// ���̃T�u�Z�b�g���b�V���Ŏg�p����Ă���{�[���R���e�i

	// �{�[���������[�v
	for (unsigned int bidx = 0; bidx < mesh->mNumBones; bidx++) {

		BONE bone{};

		// �{�[�����擾
		bone.bonename = mesh->mBones[bidx]->mName.C_Str();
		// ���b�V���m�[�h��
		bone.meshname = mesh->mBones[bidx]->mNode->mName.C_Str();
		// �A�[�}�`���A�m�[�h��
		bone.armaturename = mesh->mBones[bidx]->mArmature->mName.C_Str();

		// �{�[���I�t�Z�b�g�s��擾
		bone.OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;

		// �E�F�C�g��񒊏o
		bone.weights.clear();
		for (unsigned int widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {

			WEIGHT w;
			w.meshname = bone.meshname;										// ���b�V����
			w.bonename = bone.bonename;										// �{�[����

			w.weight = mesh->mBones[bidx]->mWeights[widx].mWeight;			// �d��
			w.vertexindex = mesh->mBones[bidx]->mWeights[widx].mVertexId;	// ���_�C���f�b�N�X
			bone.weights.emplace_back(w);
		}

		// �R���e�i�ɓo�^
		bones.emplace_back(bone);

		// �{�[�������ɂ����f������
		m_Bone[mesh->mBones[bidx]->mName.C_Str()].OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;
	}

	return bones;
}

void AnimModelMeshLoad::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	// �����œn���ꂽ�m�[�h�����L�[�Ƃ��ă{�[�������擾����
	BONE* bone = &m_Bone[GetFileName(node->mName.C_Str())];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix;					// �����l�͒P�ʍs��			

	worldMatrix = matrix;						// �e�̈ʒu�A�p����������ԁi�t�{�[���I�t�Z�b�g�s��j
	worldMatrix *= bone->AnimationMatrix;		// �����œn���ꂽ�s����|���Z�i���m�[�h�̃A�j���[�V�����𔽉f������j�i�{�[����Ԃł̂��́j

	bone->Matrix = worldMatrix;					// �v���O�������ɗp�ӂ��Ă���s��ɔ��f������
	bone->Matrix *= bone->OffsetMatrix;			// �I�t�Z�b�g�s��𔽉f������

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimModelMeshLoad::Update(CAnimationData& animdata,const char* animname ,int frmae)
{
	// ���[�g�s�񐶐�
	aiMatrix4x4 rootMatrix = aiMatrix4x4(
		aiVector3D(1.0f, 1.0f, 1.0f),						// scale
		aiQuaternion(0.0f, 0.0f, 0.0f),						// rotation
		aiVector3D(0.0f, 0.0f, 0.0f));						// transform

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = animdata.GetAnimation(animname, 0);

	for (unsigned int c = 0; c < animation1->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim = animation1->mChannels[c];

		BONE* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;

		f = frmae % nodeAnim->mNumRotationKeys;				//�ȈՎ���
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;

		f = frmae % nodeAnim->mNumPositionKeys;				//�ȈՎ���
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���R���r�l�[�V�����s����X�V
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);

	// �萔�o�b�t�@�ɏ������ނ��߂̏��𐶐�
	m_bonecombmtxcontainer.clear();
	m_bonecombmtxcontainer.resize(m_Bone.size());
	for (auto data : m_Bone) {
		m_bonecombmtxcontainer[data.second.idx] = data.second.Matrix;
	}

	// DirectX�p�ɓ]�u����
	for (auto& bcmtx : m_bonecombmtxcontainer)
	{
		// �]�u����
		bcmtx.Transpose();
	}
}

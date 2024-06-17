#include "AnimaionModelLoad.h"

bool AnimaionModelLoad::Load(std::string filename)
{
	bool sts{};

	m_filename = filename;			// �e�N�X�`���ǂݍ��ݗp�ɕۑ�
	//-----------------------------------------------------------------------------
	// �V�[�����\�z
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(
		filename.c_str(),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate);
	// �G���[����
	if (pScene == nullptr)
	{
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
		return false;
	}

	//�ό`�㒸�_�z�񐶐�
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[pScene->mNumMeshes];

	//�ċA�I�Ƀ{�[������
	CreateBone(pScene->mRootNode);

	// �{�[���̔z��ʒu���i�[����				// 20230909
	unsigned int num = 0;						// 20230909
	for (auto& data : m_Bone) {					// 20230909
		data.second.idx = num;					// 20230909
		num++;									// 20230909
	}											// 20230909

	// ���b�V�����Ń��b�V���R���e�i�z�񐔂�����
	m_subsets.resize(pScene->mNumMeshes);

	// ���b�V���������[�v
	for (unsigned int m = 0; m < pScene->mNumMeshes; m++) {

		aiMesh* mesh{};
		mesh = pScene->mMeshes[m];

		//�ό`�㒸�_�f�[�^������
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;

			// �ʒu
			deformVertex.Position.x = mesh->mVertices[v].x;
			deformVertex.Position.y = mesh->mVertices[v].y;
			deformVertex.Position.z = mesh->mVertices[v].z;

			// �@��
			deformVertex.Normal.x = mesh->mNormals[v].x;
			deformVertex.Normal.y = mesh->mNormals[v].y;
			deformVertex.Normal.z = mesh->mNormals[v].z;

			// �{�[����������
			deformVertex.BoneNum = 0;

			// �{�[��������
			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			// �ό`�㒸�_�f�[�^�i�[�z��ɓ����
			m_DeformVertex[m].push_back(deformVertex);
		}

		// ���b�V���P�ʂŒ��_�f�[�^���擾
		int vnum = GetVertices(mesh, m);

		// ���b�V���P�ʂŃC���f�b�N�X�f�[�^�擾
		int idxnum = GetIndices(mesh);

		//�{�[���f�[�^������
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			// ���O����{�[������
			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();

				// 20230909
				m_DeformVertex[m][weight.mVertexId].BoneIdx[num] = m_Bone[bone->mName.C_Str()].idx;

				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
			}
		}

		// ���b�V���Ŏg�p���Ă���}�e���A�������擾
		int mtrlidx = mesh->mMaterialIndex;

		// �C���f�b�N�X����ۑ�
		m_subsets[m].IndexNum = idxnum;

		// ���_����ۑ�
		m_subsets[m].VertexNum = vnum;

		// �{�[������ۑ�
		m_subsets[m].BoneNum = mesh->mNumBones;;

		// �}�e���A���C���f�b�N�X��ۑ�
		m_subsets[m].MaterialIdx = mtrlidx;
	}

	printf("�A�j���[�V�������f��Load�R���|�[�l���g\n");
	printf("�{�[���� : %d\n", m_Bone.size());
	printf("���_�� : %d\n", m_vertices.size());

	// ���b�V���̃x�[�X�����\�z
	CalcMeshBaseIndex();

	// �S�e�N�X�`�����擾
	GetTextures(pScene);

	// �}�e���A�����擾
	GetMaterials(pScene);

	return true;
}

void AnimaionModelLoad::Uninit()
{
	// SRV���
	for (auto& texres : m_Texturedic)
	{
		if (texres.second != nullptr) {
			texres.second->Release();
		}
	}

	delete[] m_DeformVertex;
}


unsigned int AnimaionModelLoad::GetVertices(const aiMesh* aimesh, int i)
{
	// ���_�������[�v
	for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++)
	{

		ANIMMODELVERTEX v;

		DEFORM_VERTEX* deformVertex = &m_DeformVertex[i][vidx];

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

		v.BoneIndex[0] = deformVertex->BoneIdx[0];
		v.BoneIndex[1] = deformVertex->BoneIdx[1];
		v.BoneIndex[2] = deformVertex->BoneIdx[2];
		v.BoneIndex[3] = deformVertex->BoneIdx[3];

		v.BoneWeight[0] = deformVertex->BoneWeight[0];
		v.BoneWeight[1] = deformVertex->BoneWeight[1];
		v.BoneWeight[2] = deformVertex->BoneWeight[2];
		v.BoneWeight[3] = deformVertex->BoneWeight[3];

		// ���_�f�[�^��ۑ�
		m_vertices.emplace_back(v);
	}

	// aiMesh���ێ����钸�_����߂�
	return aimesh->mNumVertices;
}

unsigned int AnimaionModelLoad::GetIndices(const aiMesh* aimesh)
{
	// �ʐ������[�v
	for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
	{
		const aiFace* face = &aimesh->mFaces[f];
		assert(face->mNumIndices == 3);				// �|���S����3���_�ł͂Ȃ���΃G���[

		// �C���f�b�N�X�f�[�^��ۑ�
		m_indices.emplace_back(face->mIndices[0]);
		m_indices.emplace_back(face->mIndices[1]);
		m_indices.emplace_back(face->mIndices[2]);
	}

	return aimesh->mNumFaces * 3;	// 3����
}

void AnimaionModelLoad::CalcMeshBaseIndex()
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

		// �{�[���o�b�t�@�̃x�[�X���v�Z
		m_subsets[m].BoneBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].BoneBase += m_subsets[i].BoneNum;
		}
	}
}

void AnimaionModelLoad::GetTextures(const aiScene* aiscene)
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

			// mHeight is 0, so try to load a compressed texture of mWidth bytes
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

bool AnimaionModelLoad::isEmbeddedTexure(const aiTexture* aitexture)
{
	if (aitexture->mHeight == 0 ||
		aitexture->mWidth == 0 ||
		aitexture->pcData == nullptr) {
		return true;
	}
	return false;
}

void AnimaionModelLoad::GetMaterials(const aiScene* aiscene)
{
	// ���̃��f���Ŏg�p���Ă��邷�ׂẴ}�e���A�������擾
	for (int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {

		// ���̃��b�V���p�̃}�e���A�����擾
		aiMaterial* mtl;
		mtl = aiscene->mMaterials[mrlidx];

		// �}�e���A�����擾
		GetaMatreial(mtl);
	}
}

void AnimaionModelLoad::GetaMatreial(const aiMaterial* aimtrl)
{
	// �f�o�C�X�A�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// �}�e���A�����
	MATERIAL mymaterial{};

	// �g�U���ː���
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
			mymaterial.Diffuse.x = color.r;
			mymaterial.Diffuse.y = color.g;
			mymaterial.Diffuse.z = color.b;
			mymaterial.Diffuse.w = 1.0f;
		}
		else {
			mymaterial.Diffuse.x = 0.5f;
			mymaterial.Diffuse.y = 0.5f;
			mymaterial.Diffuse.z = 0.5f;
			mymaterial.Diffuse.w = 1.0f;
		}
	}

	// ���ʔ��ː���
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
			mymaterial.Specular.x = color.r;
			mymaterial.Specular.y = color.g;
			mymaterial.Specular.z = color.b;
			mymaterial.Specular.w = 1.0f;
		}
		else {
			mymaterial.Specular.x = 0.0f;
			mymaterial.Specular.y = 0.0f;
			mymaterial.Specular.z = 0.0f;
			mymaterial.Specular.w = 0.0f;
		}
	}

	// ���ʔ��ˋ��x
	{
		float Shinness = 0.0f;

		if (aimtrl->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS) {
			mymaterial.Shininess = Shinness;
		}
		else {
			mymaterial.Shininess = 0.0f;
		}
	}

	// �������ː���
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
			mymaterial.Ambient.x = color.r;
			mymaterial.Ambient.y = color.g;
			mymaterial.Ambient.z = color.b;
			mymaterial.Ambient.w = 1.0f;
		}
		else {
			mymaterial.Ambient.x = 0.0f;
			mymaterial.Ambient.y = 0.0f;
			mymaterial.Ambient.z = 0.0f;
			mymaterial.Ambient.w = 0.0f;
		}
	}

	// ���Ɣ�������
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (aimtrl->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
			mymaterial.Emission.x = color.r;
			mymaterial.Emission.y = color.g;
			mymaterial.Emission.z = color.b;
			mymaterial.Emission.w = 1.0f;
		}
		else {
			mymaterial.Emission.x = 0.0f;
			mymaterial.Emission.y = 0.0f;
			mymaterial.Emission.z = 0.0f;
			mymaterial.Emission.w = 0.0f;
		}
	}

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

	// SRV�����ɑ��݂��邩�H
	if (m_Texturedic.find(str.data) == m_Texturedic.end()) {
		mymaterial.TextureEnable = FALSE;
		srv = nullptr;

		// pmx �̏ꍇ�V�[���ɓo�^����Ă��Ȃ��e�N�X�`�������݂���
		aimtrl->GetTexture(type, 0, &str);

		// �t�@�C�������擾����
		std::string filename = GetFileName(str.C_Str());

		// �e�N�X�`����ǂݍ��ݎ擾
		// SRV
		srv = LoadDiffuseTexture(filename);

		if (srv == nullptr)
		{
			mymaterial.TextureEnable = FALSE;
		}
		else
		{
			mymaterial.TextureEnable = TRUE;
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

ID3D11ShaderResourceView* AnimaionModelLoad::LoadDiffuseTexture(std::string filename)
{
	// �t�@�C���̊g���q���擾
	std::string fileext = GetFileExt_s(filename);

	// �t�@�C����(UTF-16)���擾����
	std::wstring widefilename = GetFileNameWide(filename.c_str());

	// ���f���f�[�^�̐e�f�B���N�g�����擾
	std::wstring parentdirectory = GetParentDirectoryWide(m_filename);// +L"";

	// �e�N�X�`���̃p�X���擾
	std::wstring filepath = parentdirectory + widefilename;

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// TGA�H
	if (fileext == ".tga")
	{
		// TGA�̏ꍇ
		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filepath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);

		HRESULT hr = LoadFromTGAFile(filepath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			std::cout << "LoadFromTGAFile Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		ID3D11Resource* tex = nullptr;

		// �e�N�X�`������
		hr = CreateTexture(
			Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&tex);

		tex->Release();

		if (FAILED(hr)) {
			std::cout << "CreateTexture Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		// �V�F�[�_�[���\�[�X�r���[�쐬
		hr = DirectX::CreateShaderResourceView(Renderer::GetDevice(),
			image->GetImages(),
			image->GetImageCount(),
			meta,
			&srv);

		if (FAILED(hr)) {
			std::wcout << L"CreateShaderResourceView Error (" << filepath.c_str() << L")" << std::endl;
			return nullptr;
		}
	}
	else {
		// �e�N�X�`���ǂݍ���
		HRESULT hr = DirectX::CreateWICTextureFromFile(
			Renderer::GetDevice(),
			filepath.c_str(),
			nullptr,
			&srv);
		if (FAILED(hr)) {
			std::wcout << L"CreateWICTextureFromFile Error (" << filepath.c_str() << widefilename.c_str() << L")" << std::endl;
			return nullptr;
		}
	}

	return srv;
}

void AnimaionModelLoad::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}
}
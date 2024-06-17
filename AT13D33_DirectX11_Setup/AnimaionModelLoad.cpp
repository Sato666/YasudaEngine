#include "AnimaionModelLoad.h"

bool AnimaionModelLoad::Load(std::string filename)
{
	bool sts{};

	m_filename = filename;			// テクスチャ読み込み用に保存
	//-----------------------------------------------------------------------------
	// シーン情報構築
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(
		filename.c_str(),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate);
	// エラー処理
	if (pScene == nullptr)
	{
		printf("load error %s ( %s ) \n ", filename.c_str(), importer.GetErrorString());
		return false;
	}

	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[pScene->mNumMeshes];

	//再帰的にボーン生成
	CreateBone(pScene->mRootNode);

	// ボーンの配列位置を格納する				// 20230909
	unsigned int num = 0;						// 20230909
	for (auto& data : m_Bone) {					// 20230909
		data.second.idx = num;					// 20230909
		num++;									// 20230909
	}											// 20230909

	// メッシュ数でメッシュコンテナ配列数を決定
	m_subsets.resize(pScene->mNumMeshes);

	// メッシュ数分ループ
	for (unsigned int m = 0; m < pScene->mNumMeshes; m++) {

		aiMesh* mesh{};
		mesh = pScene->mMeshes[m];

		//変形後頂点データ初期化
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_VERTEX deformVertex;

			// 位置
			deformVertex.Position.x = mesh->mVertices[v].x;
			deformVertex.Position.y = mesh->mVertices[v].y;
			deformVertex.Position.z = mesh->mVertices[v].z;

			// 法線
			deformVertex.Normal.x = mesh->mNormals[v].x;
			deformVertex.Normal.y = mesh->mNormals[v].y;
			deformVertex.Normal.z = mesh->mNormals[v].z;

			// ボーン数初期化
			deformVertex.BoneNum = 0;

			// ボーン初期化
			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			// 変形後頂点データ格納配列に入れる
			m_DeformVertex[m].push_back(deformVertex);
		}

		// メッシュ単位で頂点データを取得
		int vnum = GetVertices(mesh, m);

		// メッシュ単位でインデックスデータ取得
		int idxnum = GetIndices(mesh);

		//ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			// 名前からボーン生成
			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//変形後頂点にボーンデータ格納
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

		// メッシュで使用しているマテリアル情報を取得
		int mtrlidx = mesh->mMaterialIndex;

		// インデックス数を保存
		m_subsets[m].IndexNum = idxnum;

		// 頂点数を保存
		m_subsets[m].VertexNum = vnum;

		// ボーン数を保存
		m_subsets[m].BoneNum = mesh->mNumBones;;

		// マテリアルインデックスを保存
		m_subsets[m].MaterialIdx = mtrlidx;
	}

	printf("アニメーションモデルLoadコンポーネント\n");
	printf("ボーン数 : %d\n", m_Bone.size());
	printf("頂点数 : %d\n", m_vertices.size());

	// メッシュのベース情報を構築
	CalcMeshBaseIndex();

	// 全テクスチャを取得
	GetTextures(pScene);

	// マテリアルを取得
	GetMaterials(pScene);

	return true;
}

void AnimaionModelLoad::Uninit()
{
	// SRV解放
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
	// 頂点数分ループ
	for (unsigned int vidx = 0; vidx < aimesh->mNumVertices; vidx++)
	{

		ANIMMODELVERTEX v;

		DEFORM_VERTEX* deformVertex = &m_DeformVertex[i][vidx];

		// 位置
		v.Position.x = aimesh->mVertices[vidx].x;
		v.Position.y = aimesh->mVertices[vidx].y;
		v.Position.z = aimesh->mVertices[vidx].z;

		// 法線あり？
		if (aimesh->HasNormals()) {
			v.Normal.x = aimesh->mNormals[vidx].x;
			v.Normal.y = aimesh->mNormals[vidx].y;
			v.Normal.z = aimesh->mNormals[vidx].z;
		}
		else {
			v.Normal = DirectX::SimpleMath::Vector3(0, 0, 0);
		}

		// テクスチャあり？
		if (aimesh->HasTextureCoords(0)) {
			v.TexCoord.x = aimesh->mTextureCoords[0][vidx].x;				// 20230809 bug fix by ts
			v.TexCoord.y = aimesh->mTextureCoords[0][vidx].y;				// 20230809 bug fix by ts
		}
		else {
			v.TexCoord = DirectX::SimpleMath::Vector2(0, 0);
		}

		// 頂点カラーあり？
		if (aimesh->HasVertexColors(0)) {
			v.Diffuse.x = aimesh->mColors[0][vidx].r;
			v.Diffuse.y = aimesh->mColors[0][vidx].g;
			v.Diffuse.z = aimesh->mColors[0][vidx].b;
			v.Diffuse.w = aimesh->mColors[0][vidx].a;
		}
		else {
			v.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);								// 無かった場合は白
		}

		v.BoneIndex[0] = deformVertex->BoneIdx[0];
		v.BoneIndex[1] = deformVertex->BoneIdx[1];
		v.BoneIndex[2] = deformVertex->BoneIdx[2];
		v.BoneIndex[3] = deformVertex->BoneIdx[3];

		v.BoneWeight[0] = deformVertex->BoneWeight[0];
		v.BoneWeight[1] = deformVertex->BoneWeight[1];
		v.BoneWeight[2] = deformVertex->BoneWeight[2];
		v.BoneWeight[3] = deformVertex->BoneWeight[3];

		// 頂点データを保存
		m_vertices.emplace_back(v);
	}

	// aiMeshが保持する頂点数を戻す
	return aimesh->mNumVertices;
}

unsigned int AnimaionModelLoad::GetIndices(const aiMesh* aimesh)
{
	// 面数分ループ
	for (unsigned int f = 0; f < aimesh->mNumFaces; f++)
	{
		const aiFace* face = &aimesh->mFaces[f];
		assert(face->mNumIndices == 3);				// ポリゴンが3頂点ではなければエラー

		// インデックスデータを保存
		m_indices.emplace_back(face->mIndices[0]);
		m_indices.emplace_back(face->mIndices[1]);
		m_indices.emplace_back(face->mIndices[2]);
	}

	return aimesh->mNumFaces * 3;	// 3分割
}

void AnimaionModelLoad::CalcMeshBaseIndex()
{
	// サブセット数分ループ
	for (int m = 0; m < m_subsets.size(); m++)
	{
		// 頂点バッファのベースを計算
		m_subsets[m].VertexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].VertexBase += m_subsets[i].VertexNum;
		}

		// インデックスバッファのベースを計算
		m_subsets[m].IndexBase = 0;
		for (int i = m - 1; i >= 0; i--) {
			m_subsets[m].IndexBase += m_subsets[i].IndexNum;
		}

		// ボーンバッファのベースを計算
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

	// このシーンが持つテクスチャを全て読み込む
	for (int texidx = 0; texidx < aiscene->mNumTextures; texidx++)
	{
		// テクスチャを取得
		aiTexture* aitexture = aiscene->mTextures[texidx];

		// 内蔵テクスチャか？
		if (isEmbeddedTexure(aitexture))
		{
			ID3D11ShaderResourceView* srv;

			aiTexture* aitexture = aiscene->mTextures[texidx];

			// mHeight is 0, so try to load a compressed texture of mWidth bytes
			const size_t size = aitexture->mWidth;

			// SRV生成
			HRESULT hr = DirectX::CreateWICTextureFromMemory(
				device,
				devicecontext,
				reinterpret_cast<const unsigned char*>(aitexture->pcData),
				aitexture->mWidth,
				nullptr,
				&srv);

			assert(srv);

			// assimpの内部データにセットされているテクスチャ名をキーとして保存
			m_Texturedic[aitexture->mFilename.data] = srv;
		}
		else
		{
			// 内蔵テクスチャではない場合

			// テクスチャファイル名を取得する
			std::string filename = GetFileName(aitexture->mFilename.C_Str());

			// テクスチャファイルを読み込みSRVを生成
			// SRV
			ID3D11ShaderResourceView* srv = nullptr;

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				device,
				devicecontext,
				&srv);

			if (sts) {
				// assimpの内部データにセットされているテクスチャ名をキーとして保存
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
	// このモデルで使用しているすべてのマテリアル情報を取得
	for (int mrlidx = 0; mrlidx < aiscene->mNumMaterials; mrlidx++) {

		// このメッシュ用のマテリアルを取得
		aiMaterial* mtl;
		mtl = aiscene->mMaterials[mrlidx];

		// マテリアル情報取得
		GetaMatreial(mtl);
	}
}

void AnimaionModelLoad::GetaMatreial(const aiMaterial* aimtrl)
{
	// デバイス、デバイスコンテキスト取得
	ID3D11DeviceContext* devicecontext;
	ID3D11Device* device;

	device = Renderer::GetDevice();
	devicecontext = Renderer::GetDeviceContext();

	// マテリアル情報
	MATERIAL mymaterial{};

	// 拡散反射成分
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

	// 鏡面反射成分
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

	// 鏡面反射強度
	{
		float Shinness = 0.0f;

		if (aimtrl->Get(AI_MATKEY_SHININESS, Shinness) == AI_SUCCESS) {
			mymaterial.Shininess = Shinness;
		}
		else {
			mymaterial.Shininess = 0.0f;
		}
	}

	// 環境光反射成分
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

	// 自家発光成分
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

	// ディフューズテクスチャ数取得
	aiTextureType type = aiTextureType_DIFFUSE;
	int texnum = aimtrl->GetTextureCount(type);

	// 1メッシュにディフーズテクスチャ１枚しか許可せず
	assert(texnum <= 1);

	// マテリアル名
	aiString str;

	// assimpからテクスチャファイル名を取得
	aimtrl->GetTexture(type, 0, &str);

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// SRV辞書に存在するか？
	if (m_Texturedic.find(str.data) == m_Texturedic.end()) {
		mymaterial.TextureEnable = FALSE;
		srv = nullptr;

		// pmx の場合シーンに登録されていないテクスチャが存在する
		aimtrl->GetTexture(type, 0, &str);

		// ファイル名を取得する
		std::string filename = GetFileName(str.C_Str());

		// テクスチャを読み込み取得
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

	// SRV保存
	m_AllTexture.emplace_back(srv);

	// マテリアル情報を保存
	m_materials.emplace_back(mymaterial);
}

ID3D11ShaderResourceView* AnimaionModelLoad::LoadDiffuseTexture(std::string filename)
{
	// ファイルの拡張子を取得
	std::string fileext = GetFileExt_s(filename);

	// ファイル名(UTF-16)を取得する
	std::wstring widefilename = GetFileNameWide(filename.c_str());

	// モデルデータの親ディレクトリを取得
	std::wstring parentdirectory = GetParentDirectoryWide(m_filename);// +L"";

	// テクスチャのパス名取得
	std::wstring filepath = parentdirectory + widefilename;

	// SRV
	ID3D11ShaderResourceView* srv = nullptr;

	// TGA？
	if (fileext == ".tga")
	{
		// TGAの場合
		DirectX::TexMetadata meta;
		DirectX::GetMetadataFromTGAFile(filepath.c_str(), meta);

		std::unique_ptr<DirectX::ScratchImage> image(new DirectX::ScratchImage);

		HRESULT hr = LoadFromTGAFile(filepath.c_str(), &meta, *image);
		if (FAILED(hr)) {
			std::cout << "LoadFromTGAFile Error (" << filename << ")" << std::endl;
			return nullptr;
		}

		ID3D11Resource* tex = nullptr;

		// テクスチャ生成
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

		// シェーダーリソースビュー作成
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
		// テクスチャ読み込み
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
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SceneEdit.h"
#include "main.h"		// ウィンドウサイズ取得用
#include "dx11mathutil.h"

// このシーンで使うGameObjectインクルード
#include "sky.h"		// スカイボックス
#include "camera.h"		// カメラ
#include "MoveLight.h"	// ライト

/// <summary>
/// 次回起動時に現在のサブシーンを引き続く
/// </summary>
struct SceneEditSetting
{
	float AlphaLen;			// アルファディザ適応距離
	float OutLineVal;		// 輪郭線の幅
	float GammaValue;		// ガンマ値
	float ViewClipping01;	// 被写界深度の距離設定用
	float ViewClipping02;	// 被写界深度の距離設定用
	float blurStrength;

	bool Outline;			// 輪郭線の描画
	bool GBuffer;			// Gバッファの描画
	bool AlwaysGamma;		// 常にガンマ補正適応描画フラグ
	bool IsShadow;			// 影の描画フラグ
	bool DoF;				// 被写界深度フラグ
	bool FrustumCulling;	// フラスタムカリング
	bool isMulti;			// 画面塗りつぶしフラグ

	int ShadowQualityLevel;	// 影の品質
	int index;				// ポストエフェクトの番号
};
// ファイルパスを指定
const char* EditSettingFileName = "asset/Setting/SceneEditSetting.Didyouthinktext";

void SceneEdit::Init()
{
	// シェーダー読み込み
	Shader* shader[] = {
		CreateObj<VertexShader>("VS_Object"),			// デフォルトの頂点シェーダー（行列返還など）
		CreateObj<VertexShader>("VS_Shadow"),			// 影用頂点シェーダー
		CreateObj<VertexShader>("VS_Outline"),			// 輪郭線用頂点シェーダー
		CreateObj<VertexShader>("VS_AlphaDither"),		// アルファディザ用頂点シェーダー
		CreateObj<VertexShader>("VS_ScreenPosition"),	// スクリーン座標取得用
		CreateObj<VertexShader>("VS_AnimObject"),		// アニメーション用

		CreateObj<PixelShader>("PS_TexColor"),			// デフォルトのピクセルシェーダー（そのまま色を出す）
		CreateObj<PixelShader>("PS_Material"),			// デフォルトModelのピクセルシェーダー
		CreateObj<PixelShader>("PS_FXAA"),				// ポストエフェクト（アンチエイリアシング）
		CreateObj<PixelShader>("PS_Monochrome"),		// ポストエフェクト（モノクロ）
		CreateObj<PixelShader>("PS_Sepia"),				// ポストエフェクト（セピア）
		CreateObj<PixelShader>("PS_Posterization"),		// ポストエフェクト（ポスタリゼーション）
		CreateObj<PixelShader>("PS_RGBSplit"),			// ポストエフェクト（色収差）
		CreateObj<PixelShader>("PS_Noise"),				// ポストエフェクト（ノイズ）
		CreateObj<PixelShader>("PS_Vignette"),			// ポストエフェクト（ビネット）
		CreateObj<PixelShader>("PS_ScanningLine"),		// ポストエフェクト（走査線）
		CreateObj<PixelShader>("PS_GammaCorrection"),	// ポストエフェクト（ガンマ補正）
		CreateObj<PixelShader>("PS_DepthWrite"),		// 影用に深度値を取得するためのGbufferに書き込むピクセルシェーダー
		CreateObj<PixelShader>("PS_WriteDoF"),			// レンダーターゲット受け取り用
		CreateObj<PixelShader>("PS_Blur"),				// ブラー
		CreateObj<PixelShader>("PS_DoF"),				// 被写界深度

		CreateObj<PixelShader>("PS_AlphaDither"),		// アルファディザ用ピクセルシェーダー
		CreateObj<PixelShader>("PS_ModelToon"),			// モデル（トゥーン）

		CreateObj<PixelShader>("PS_Shadow"),			// 影用ピクセルシェーダー
		CreateObj<PixelShader>("PS_ObjShadow"),			// Obj影用ピクセルシェーダー
		CreateObj<PixelShader>("PS_Outline"),			// 輪郭線用ピクセルシェーダー
		CreateObj<PixelShader>("PS_AlphaDitherOutline"),// アルファディザ用輪郭線考慮ピクセルシェーダー

	};
	// シェーダーを読み込んでくる
	const char* filePath[] = {
	"Shader/VS/Default/VS_Object.cso",
	"Shader/VS/AddRenderer/VS_Shadow.cso",
	"Shader/VS/AddRenderer/VS_Outline.cso",
	"Shader/VS/AddRenderer/VS_AlphaDither.cso",
	"Shader/VS/AddRenderer/VS_ScreenPosition.cso",
	"Shader/VS/AddRenderer/VS_AnimObject.cso",

	"Shader/PS/Default/PS_TexColor.cso",
	"Shader/PS/Default/PS_Material.cso",
	"Shader/PS/PostProcessing/PS_FXAA.cso",
	"Shader/PS/PostProcessing/PS_Monochrome.cso",
	"Shader/PS/PostProcessing/PS_Sepia.cso",
	"Shader/PS/PostProcessing/PS_Posterization.cso",
	"Shader/PS/PostProcessing/PS_RGBSplit.cso",
	"Shader/PS/PostProcessing/PS_Noise.cso",
	"Shader/PS/PostProcessing/PS_Vignette.cso",
	"Shader/PS/PostProcessing/PS_ScanningLine.cso",
	"Shader/PS/PostProcessing/PS_GammaCorrection.cso",
	"Shader/PS/PostProcessing//PS_DepthWrite.cso",
	"Shader/PS/PostProcessing/PS_WriteDoF.cso",
	"Shader/PS/PostProcessing/PS_Blur.cso",
	"Shader/PS/PostProcessing/PS_DoF.cso",

	"Shader/PS/Model/Effect/PS_AlphaDither.cso",
	"Shader/PS/Model/Effect/PS_ModelToon.cso",

	"Shader/PS/Model/AddRenderer/PS_Shadow.cso",
	"Shader/PS/Model/AddRenderer/PS_ObjShadow.cso",
	"Shader/PS/Model/AddRenderer/PS_Outline.cso",
	"Shader/PS/Model/AddRenderer/PS_AlphaDitherOutline.cso",
	};
	static_assert(_countof(shader) == _countof(filePath), "");
	for (int i = 0; i < _countof(shader); ++i)
		shader[i]->Load(filePath[i]);

	// 高画質影用レンダーターゲット
	RenderTarget* pHighDepthWriteRTV = CreateObj<RenderTarget>("HighDepthWriteRTV");
	pHighDepthWriteRTV->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4);
	// 高画質影用深度ステンシルの生成と設定
	DepthStencil* pHighDepthWriteDSV = CreateObj<DepthStencil>("HighDepthWriteDSV");
	pHighDepthWriteDSV->Create(SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4, false);
	// 中画質影用レンダーターゲット
	RenderTarget* pDepthWriteRTV = CreateObj<RenderTarget>("DepthWriteRTV");
	pDepthWriteRTV->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH, SCREEN_HEIGHT);
	// 中画質深度ステンシルの生成と設定
	DepthStencil* pDepthWriteDSV = CreateObj<DepthStencil>("DepthWriteDSV");
	pDepthWriteDSV->Create(SCREEN_WIDTH, SCREEN_HEIGHT, false);
	// 低画質影用レンダーターゲット
	RenderTarget* pLowDepthWriteRTV = CreateObj<RenderTarget>("LowDepthWriteRTV");
	pLowDepthWriteRTV->Create(DXGI_FORMAT_R32_FLOAT, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	// 低画質深度ステンシルの生成と設定
	DepthStencil* pLowDepthWriteDSV = CreateObj<DepthStencil>("LowDepthWriteDSV");
	pLowDepthWriteDSV->Create(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, false);

	// レンダーターゲット作成
	RenderTarget* pRTV[] = {
		CreateObj<RenderTarget>("albedoRTV"),				// アルベド
		CreateObj<RenderTarget>("DepthRTV"),				// 深度マップ
		CreateObj<RenderTarget>("NomalRTV"),				// 法線マップ
		CreateObj<RenderTarget>("BlurH"),					// ブラーH
		CreateObj<RenderTarget>("BlurV"),					// ブラーV
		CreateObj<RenderTarget>("GammaCorrectionRTV"),		// ガンマ補正
		CreateObj<RenderTarget>("LastRTV"),					// 最終結果のスクリーン画像
	};
	DirectX::XMFLOAT2 rtvSize[] = {
		{SCREEN_WIDTH,SCREEN_HEIGHT},
		{SCREEN_WIDTH,SCREEN_HEIGHT},
		{SCREEN_WIDTH,SCREEN_HEIGHT},
		{SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT},
		{SCREEN_WIDTH * 0.25f,SCREEN_HEIGHT * 0.25f},
		{SCREEN_WIDTH,SCREEN_HEIGHT},
		{SCREEN_WIDTH,SCREEN_HEIGHT},
	};
	for (int i = 0; i < 7; ++i){
		if (i == 1){
			// レンダーターゲット作成(Rを深度地として使用)
			pRTV[i]->Create(DXGI_FORMAT_R32_FLOAT, rtvSize[i].x, rtvSize[i].y);
		}
		else if (i == 2) {
			// レンダーターゲット作成(Rを深度地として使用)
			pRTV[i]->Create(DXGI_FORMAT_R8G8B8A8_SNORM, rtvSize[i].x, rtvSize[i].y);
		}
		else{
			pRTV[i]->Create(DXGI_FORMAT_R8G8B8A8_UNORM, rtvSize[i].x, rtvSize[i].y);
		}
	}

	// モデルの生成
	// スカイボックス作成
	Sky* pSkyBack = CreateObj<Sky>("SkyBox");
	pSkyBack->Init();
	pSkyBack->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vsシェーダーセット
	pSkyBack->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));		// 保存Vsシェーダーセット
	pSkyBack->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));		// Psシェーダーセット
	pSkyBack->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// 保存Psシェーダーセット

	// 空のオブジェクトを入れておく
	Model* pNone = CreateObj<Model>("None");
	pNone->SetID("None");
	pNone->Init();
	pNone->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vsシェーダーセット
	pNone->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// 保存Vsシェーダーセット
	pNone->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));		// Psシェーダーセット
	pNone->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// 保存Psシェーダーセット
	ObjeList.push_back(pNone);		// リストに追加

	// Modelを取得
	Model* pModel = CreateObj<Model>("Model");
	pModel->SetID("Model");
	pModel->SetName(u8"課題用モデル君");
	pModel->Init();
	pModel->GetComponent<ModelSetting>()->SetCharacterFlag(true);
	pModel->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vsシェーダーセット
	pModel->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// 保存Vsシェーダーセット
	pModel->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_ModelToon"));	// Psシェーダーセット
	pModel->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_ModelToon"));// 保存Psシェーダーセット
	// モデルを読み込み
	pModel->GetComponent<ModelMeshLoad>()->Load(u8"asset/model/assimp/Tda式初音ミク・アペンドVer1.00/ミクアペ.pmx");
	pModel->GetComponent<ModelMeshRenderer>()->Init(*pModel->GetComponent<ModelMeshLoad>());
	pModel->SetScale(DirectX::SimpleMath::Vector3(0.05f, 0.05f, 0.05f));	// サイズ
	ObjeList.push_back(pModel);		// リストに追加

	// Modelを取得
	Model* pModel2 = CreateObj<Model>("None2");
	pModel2->SetID("None2");
	pModel2->SetName(u8"課題用モデル君2");
	pModel2->Init();
	pModel2->GetComponent<ModelSetting>()->SetCharacterFlag(false);
	pModel2->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));					// Vsシェーダーセット
	pModel2->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));				// 保存Vsシェーダーセット
	pModel2->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_Material"));				// Psシェーダーセット
	pModel2->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Material"));			// 保存Psシェーダーセット
	// モデルを読み込み
	pModel2->GetComponent<ModelMeshLoad>()->Load(u8"asset/model/assimp/吹き抜けのある本棚付きペントハウスReTex/ペントハウスReTex_昼間接照明.pmx");
	pModel2->GetComponent<ModelMeshRenderer>()->Init(*pModel2->GetComponent<ModelMeshLoad>());
	pModel2->SetScale(DirectX::SimpleMath::Vector3(0.05f, 0.05f, 0.05f));								// サイズ
	ObjeList.push_back(pModel2);		// リストに追加

	// テクスチャの読み込み
	// ランプテクスチャの読込
	Texture* pRamp =
		CreateObj <Texture>("RampTex");
	pRamp->Create("asset/ShaderTexture/RampTex.bmp");
	// ディザパターンの読込
	Texture* pDither =
		CreateObj<Texture>("DitherTex");
	pDither->Create("asset/ShaderTexture/BayerMatrix.png");

	// アイコンテクスチャ読み込み
	Texture* pIcon =
		CreateObj <Texture>("IconTex");
	pIcon->Create("asset/texture/Objecticon.png");

	// 保存データの読み込み
	SceneEditSetting setting ={
		0.75f,0.001f,1.5f,0.2f,2.0f,0.25f,
		true,true,false,true,true,true,false,
		ShadowQuality::HIGH,
		0
	};
	FILE* fp;
	fopen_s(&fp, EditSettingFileName, "rb");
	if (fp){
		fread(&setting, sizeof(SceneEditSetting), 1, fp);	// 書き込み
		fclose(fp);	// 閉じる
	}

	m_AlphaLen = setting.AlphaLen;				// アルファディザ適応距離
	m_OutLineVal = setting.OutLineVal;			// 輪郭線の幅
	m_GammaValue = setting.GammaValue;			// ガンマ値
	m_ViewClipping[0] = setting.ViewClipping01;	// 被写界深度の距離設定用
	m_ViewClipping[1] = setting.ViewClipping02;	// 被写界深度の距離設定用
	m_blurStrength = setting.blurStrength;		// ブラーの強さ
	m_Outline = setting.Outline;				// 輪郭線の描画
	m_GBuffer = setting.GBuffer;				// Gバッファの描画
	m_AlwaysGamma = setting.AlwaysGamma;		// 常にガンマ補正適応描画フラグ
	m_IsShadow = setting.IsShadow;				// 影の描画フラグ
	m_DoF = setting.DoF;						// 被写界深度フラグ
	m_FrustumCulling = setting.FrustumCulling;	// フラスタムカリング
	m_isMulti = setting.isMulti;				// 画面塗りつぶしフラグ
	m_ShadowQualityLevel = setting.ShadowQualityLevel;	// 影の品質
	m_index = setting.index;					// ポストエフェクト
}

void SceneEdit::Uninit()
{
	// Editの設定をセーブする
	SceneEditSetting setting ={
		m_AlphaLen,m_OutLineVal,m_GammaValue,m_ViewClipping[0],m_ViewClipping[1],m_blurStrength,
		m_Outline,m_GBuffer,m_AlwaysGamma,m_IsShadow,m_DoF,m_FrustumCulling,m_isMulti,
		m_ShadowQualityLevel,
		m_index
	};
	FILE* fp;
	fopen_s(&fp, EditSettingFileName, "wb");
	if (fp){
		fwrite(&setting, sizeof(SceneEditSetting), 1, fp);
		fclose(fp);
	}

	// シーン内で生成したオリストに追加されていないオブジェクトの解放
	Sky* pSkyBack = GetObj<Sky>("SkyBox");
	pSkyBack->Uninit();

	// 生成したオブジェクト分削除を行う
	for (int i = 0; i < ObjeList.size(); i++)
	{
		// リストに入っているオブジェクトの解放
		ObjeList[i]->Uninit();
	}

	// リストの解放
	ObjeList.clear();
}

void SceneEdit::Update(float tick)
{
	// 矢印でポストプロセス切り替え
	const int max = g_posteffectname_size + 1;
	if (Input::Get()->GetKeyTrigger(VK_LEFT)) { m_index = (m_index - 1 + max) % max; }
	if (Input::Get()->GetKeyTrigger(VK_RIGHT)) { m_index = (m_index + 1) % max; }
	if (Input::Get()->GetKeyTrigger(VK_RETURN)) { m_isMulti ^= true; }

	// 生成したオブジェクト分Loopする
	for (int i = 1; i < ObjeList.size(); i++)
	{
		// リストに入っているオブジェクトのアップデート
		ObjeList[i]->Update();
	}

	// 時間経過をシェーダに渡す
	m_Time += tick / 60;

	if (m_Time > 61)
	{
		m_Time = 0;
	}

	// 定数バッファに時間を渡す
	GetObj<Shader>("PS_ScanningLine")->WriteBuffer(0, &m_Time);
}

void SceneEdit::Draw()
{
	// 共有オブジェクトの取得
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// テクスチャ読み込み
	Texture* pRampTex = GetObj<Texture>("RampTex");
	Texture* pDitherTex = GetObj<Texture>("DitherTex");

	// テクスチャ取得
	RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");
	DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");

	// 変換行列設定
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// ワールド
	mat[1] = pCamera->GetViewMatrix();				// ビュー
	mat[2] = pCamera->GetProjMatrix();				// プロジェクション

	// 定数バッファに渡すカメラの情報
	DirectX::XMFLOAT3 camPos =
		pCamera->GetComponent<DXCamera>()->GetPos();
	DirectX::XMFLOAT4 camera[] = {
		{camPos.x,camPos.y,camPos.z,0.0f}
	};
	// 定数バッファに渡すライトの情報
	DirectX::XMFLOAT3 plightDir =
		pLight->GetComponent<Light>()->GetDirection();		// ライトの方向
	DirectX::XMFLOAT4 plight[] = {
		pLight->GetComponent<Light>()->GetDiffuse(),		// 光の拡散
		pLight->GetComponent<Light>()->GetAmbient(),		// 光の色
		{plightDir.x,plightDir.y,plightDir.z,0.0f}
	};

	// シェーダーに定数バッファを渡す
	GetObj<Shader>("PS_AlphaDither")->WriteBuffer(0, camera);
	GetObj<Shader>("PS_AlphaDither")->WriteBuffer(1, &m_AlphaLen);			// アルファディザ適応距離を定数バッファに渡す
	GetObj<Shader>("PS_AlphaDither")->SetTexture(1, pDitherTex);
	GetObj<Shader>("PS_AlphaDitherOutline")->WriteBuffer(0, camera);
	GetObj<Shader>("PS_AlphaDitherOutline")->WriteBuffer(1, &m_AlphaLen);	// アルファディザ適応距離を定数バッファに渡す
	GetObj<Shader>("PS_AlphaDitherOutline")->SetTexture(1, pDitherTex);
	GetObj<Shader>("PS_ModelToon")->WriteBuffer(0, plight);					// PSToon（光源の情報）
	GetObj<Shader>("PS_ModelToon")->SetTexture(1, pRampTex);				// PSToon（テクスチャ）
	GetObj<Shader>("PS_GammaCorrection")->WriteBuffer(0, &m_GammaValue);	// PSガンマ補正

	// Gbuffer
	switch (m_ShadowQualityLevel) {		// 影の品質
	default:
	case HIGH:	GBufferRender(m_GBuffer, u8"HighDepthWriteRTV", u8"HighDepthWriteDSV"); break;
	case MEDIUM:GBufferRender(m_GBuffer, u8"DepthWriteRTV", u8"DepthWriteDSV"); break;
	case LOW:	GBufferRender(m_GBuffer, u8"LowDepthWriteRTV", u8"LowDepthWriteDSV"); break;
	}

	// 被写界深度
	DepthOfFieldRender(true);

	// ポストエフェクトをかける
	PostEffectRender(true);

	// 常にガンマ補正して描画
	AlwaysGammaRender(m_AlwaysGamma);

	// 最終結果のレンダーターゲット処理
	LastRender();

	// 画面塗りつぶし
	if (m_isMulti){
		// レンダーターゲット切り替え
		float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		if (m_isMulti) color[3] = 1.0f;
		pDefRTV->Clear(color);
	}
}

/// <summary>
/// 十字ガイド線を出す
/// </summary>
void SceneEdit::GuideLine()
{
	// 共有オブジェクトの取得
	Camera* pCamera = GetObj<Camera>("Camera");

	// nullチェック
	if (m_Number != 0) {

		DirectX::XMFLOAT4X4 fmat;
		DirectX::XMStoreFloat4x4(&fmat, DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
			DirectX::XMMatrixTranslation(ObjeList[m_Number]->GetPosition().x, ObjeList[m_Number]->GetPosition().y, ObjeList[m_Number]->GetPosition().z)
		));
		Geometory::SetWorld(fmat);
		Geometory::SetView(pCamera->GetViewMatrix());
		Geometory::SetProjection(pCamera->GetProjMatrix());

		// 網掛け描画
		int GridSize = 1;
		// 軸描画
		Geometory::SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		Geometory::AddLine(DirectX::XMFLOAT3(-GridSize, 0.0f, 0.0f),
			DirectX::XMFLOAT3(GridSize, 0.0f, 0.0f));
		Geometory::SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		Geometory::AddLine(DirectX::XMFLOAT3(0.0f, -GridSize, 0.0f),
			DirectX::XMFLOAT3(0.0f, GridSize, 0.0f));
		Geometory::SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		Geometory::AddLine(DirectX::XMFLOAT3(0.0f, 0.0f, -GridSize),
			DirectX::XMFLOAT3(0.0f, 0.0f, GridSize));

		Geometory::DrawLines();
	}
}

/// <summary>
/// 最終結果をレンダーターゲットに書き込む
/// </summary>
void SceneEdit::LastRender()
{
	// テクスチャ取得
	RenderTarget* pRTV = GetObj<RenderTarget>("RTV");						// デフォルトRTV
	DepthStencil* pDSV = GetObj<DepthStencil>("DSV");						// デフォルトDSV
	RenderTarget* pLastRTV = GetObj<RenderTarget>("LastRTV");				// 最終結果RTV

	// 画面効果の最終結果を描画する
	// レンダーターゲット切り替え
	Renderer::SetRenderTargets(1, &pLastRTV, nullptr);
	pLastRTV->Clear();
	Sprite::SetSize(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::Draw();						// 最終結果を描画
	Sprite::SetPixelShader(nullptr);	// 最終結果のエフェクトはNull（加工なし）にしておく
	Sprite::SetTexture(pLastRTV);		// 画像テクスチャ反映

	// 本来の描画に戻す
	Renderer::SetRenderTargets(1, &pRTV, pDSV);

	Sprite::SetSize(DirectX::XMFLOAT2(1.0f, 1.0f));
	Sprite::Draw();					// 最終結果を描画
	Sprite::SetTexture(pRTV);		// 画像テクスチャ反映
}

/// <summary>
/// デフォルトのレンダリング
/// </summary>
/// <param name="p_active">描画するかどうか</param>
void SceneEdit::DefaultRender(bool p_active)
{
	if (p_active) {
		// このシーンのオブジェクト取得
		Camera* pCamera = GetObj<Camera>("Camera");
		MoveLight* pLight = GetObj<MoveLight>("Light");
		Sky* pSkyBack = GetObj<Sky>("SkyBox");

		// 変換行列設定
		DirectX::XMFLOAT4X4 mat[3];
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// ワールド
		mat[1] = pCamera->GetViewMatrix();				// ビュー
		mat[2] = pCamera->GetProjMatrix();				// プロジェクション

		pCamera->Draw(GetObj<Shader>("VS_Object"), mat);		// Skyボックスの描画
		pLight->Draw(GetObj<Shader>("VS_Object"), mat);
		pSkyBack->Draw(GetObj<Shader>("VS_Object"), mat);		// Skyボックスの描画

		// 見るかか見えないかを判断
		DirectX::XMFLOAT4X4 vpmtx;
		DX11MtxMultiply(vpmtx, pCamera->GetViewMatrix().Transpose(),
			pCamera->GetProjMatrix().Transpose());		// 転置しないと計算結果おかしくなる

		// 輪郭線の表示
		Renderer::SetCullingMode(D3D11_CULL_NONE);			// 裏面のポリボンを表示する
		// 生成したオブジェクト分Loopする
		for (int i = 1; i < ObjeList.size(); i++) {
			// 影がオンの時、影付きのモデルは描画をしない
			if (m_IsShadow && m_GBuffer) {
				// キャラクターモデルなら描画
				if (ObjeList[i]->GetComponent<ModelSetting>()->GetCharacterFlag()) {
					ModelMaterialSet(i, true);		// マテリアルセット

					// フラスタムカリングが有効
					if (m_FrustumCulling) {
						XMFLOAT3 pos = { ObjeList[i]->GetPosition().x,ObjeList[i]->GetPosition().y + 0.5f,ObjeList[i]->GetPosition().z };	// 位置（点）
						bool sts = IsInFrustum(pos, vpmtx);	// フラスタム内かどうかをチェック
						ObjeList[i]->SetInFrustumn(sts);
						if (sts) {
							ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(
								ObjeList[i]->GetComponent<ShaderComponent>()->GetOldVSShader());	// 保存Vsシェーダーセット
							ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(
								ObjeList[i]->GetComponent<ShaderComponent>()->GetOldPSShader());	// 前のPsシェーダーセット
							ObjeList[i]->Draw(ObjeList[i]->GetComponent<ShaderComponent>()->GetOldVSShader(), mat);					// ObjListの描画
						}
					}
					// フラスタムカリングが無効
					else {
						ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(
							ObjeList[i]->GetComponent<ShaderComponent>()->GetOldVSShader());	// 保存Vsシェーダーセット
						ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(
							ObjeList[i]->GetComponent<ShaderComponent>()->GetOldPSShader());	// 前のPsシェーダーセット
						ObjeList[i]->Draw(ObjeList[i]->GetComponent<ShaderComponent>()->GetOldVSShader(), mat);					// ObjListの描画
					}

					ModelMaterialSet(i, false);		// マテリアルセット解除
				}
			}
			// 影の描画をしない時すべてのオブジェクト描画
			else {
				ModelMaterialSet(i, true);			// マテリアルセット

				ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(
					ObjeList[i]->GetComponent<ShaderComponent>()->GetOldVSShader());	// 保存Vsシェーダーセット

				ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(
					ObjeList[i]->GetComponent<ShaderComponent>()->GetOldPSShader());	// 前のPsシェーダーセット
				ObjeList[i]->Draw(ObjeList[i]->GetComponent<ShaderComponent>()->GetOldVSShader(), mat);					// ObjListの描画

				ModelMaterialSet(i, false);			// マテリアルセット解除
			}
		}
		Renderer::SetCullingMode(D3D11_CULL_NONE);		// ポリボンの表示を戻す
	}
}

/// <summary>
/// 輪郭線の描画
/// </summary>
/// <param name="p_active">描画するかどうか</param>
void SceneEdit::OutlineRender(bool p_active)
{
	if (p_active) {
		Camera* pCamera = GetObj<Camera>("Camera");

		// 変換行列設定
		DirectX::XMFLOAT4X4 mat[3];
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// ワールド
		mat[1] = pCamera->GetViewMatrix();				// ビュー
		mat[2] = pCamera->GetProjMatrix();				// プロジェクション

		GetObj<Shader>("VS_Outline")->WriteBuffer(0, mat);	// 輪郭線用頂点シェーダーに変換行列を渡す

		// 生成したオブジェクト分Loopする
		for (int i = 1; i < ObjeList.size(); i++) {

			// Assimpで読み込んだモデルかどうか
			if(ObjeList[i]->GetComponent<ModelMeshRenderer>())	
				Renderer::SetCullingMode(D3D11_CULL_FRONT);			// 表面のポリボンを表示する
			else	
				Renderer::SetCullingMode(D3D11_CULL_BACK);			// 裏面のポリボンを表示する

			// フラスタム外なら処理をしない
			if (ObjeList[i]->IsInFrustumn()) {
				// i番目がオリジナル設定を持っているかどうか
				if (ObjeList[i]->GetComponent<ModelSetting>()->GetOriginalSetting()) {
					// i番目が輪郭線の描画する場合
					if (ObjeList[i]->GetComponent<ModelSetting>()->GetOutLineDraw()) {
						m_ModelOutLineVal = ObjeList[i]->GetComponent<ModelSetting>()->GetOutLineVal();
						m_ModelOutLineColor = ObjeList[i]->GetComponent<ModelSetting>()->GetOutLineColor();

						GetObj<Shader>("VS_Outline")->WriteBuffer(1, &m_ModelOutLineVal);	// 輪郭線の太さを定数バッファに渡す

						ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Outline"));	// Vsシェーダーセット

						// アルファディザを適応している場合
						if (GetObj<Shader>("PS_AlphaDither") == ObjeList[i]->GetComponent<ShaderComponent>()->GetOldPSShader()) {
							GetObj<Shader>("PS_AlphaDitherOutline")->WriteBuffer(2, &m_ModelOutLineColor);	// 輪郭線の色を定数バッファに渡す

							ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_AlphaDitherOutline"));	// アルファディザを考慮したPsシェーダーセット
						}
						// していない場合
						else {
							GetObj<Shader>("PS_Outline")->WriteBuffer(0, &m_ModelOutLineColor);	// 輪郭線の色を定数バッファに渡す

							ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_Outline"));	// Psシェーダーセット
						}
						ObjeList[i]->Draw(GetObj<Shader>("VS_Outline"), mat);	// ObjListの描画
					}
				}
				// 持っていない場合
				else {
					ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Outline"));	// Vsシェーダーセット
					GetObj<Shader>("VS_Outline")->WriteBuffer(1, &m_OutLineVal);	// 輪郭線の太さを定数バッファに渡す

					// アルファディザを適応している場合
					if (GetObj<Shader>("PS_AlphaDither") == ObjeList[i]->GetComponent<ShaderComponent>()->GetOldPSShader()) {
						GetObj<Shader>("PS_AlphaDitherOutline")->WriteBuffer(2, &m_OutLineColor);	// 輪郭線の色を定数バッファに渡す

						ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_AlphaDitherOutline"));	// アルファディザを考慮したPsシェーダーセット
					}
					// していない場合
					else {
						GetObj<Shader>("PS_Outline")->WriteBuffer(0, &m_OutLineColor);	// 輪郭線の色を定数バッファに渡す

						ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_Outline"));	// Psシェーダーセット
					}
					ObjeList[i]->Draw(GetObj<Shader>("VS_Outline"), mat);	// ObjListの描画
				}
			}
		}
		Renderer::SetCullingMode(D3D11_CULL_NONE);		// ポリボンの表示を戻す
	}
}

/// <summary>
/// Gバッファの描画
/// </summary>
/// <param name="p_active">描画するかどうか</param>
void SceneEdit::GBufferRender(bool p_active, std::string p_rtv, std::string p_dsv)
{
	if (p_active) {
		Camera* pCamera = GetObj<Camera>("Camera");
		MoveLight* pLight = GetObj<MoveLight>("Light");

		RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");
		DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");

		RenderTarget* pDepthWriteRTV = GetObj<RenderTarget>(p_rtv.c_str());	// 影RTV
		DepthStencil* pDepthWriteDSV = GetObj<DepthStencil>(p_dsv.c_str());	// 影DSV

		// 変換行列設定
		DirectX::XMFLOAT4X4 mat[3];
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// ワールド
		mat[1] = pCamera->GetViewMatrix();				// ビュー
		mat[2] = pCamera->GetProjMatrix();				// プロジェクション

		// ライトの方向と位置を取得
		DirectX::XMFLOAT3 Ldir = pLight->GetComponent<Light>()->GetDirection();
		DirectX::XMFLOAT3 Lpos = pLight->GetComponent<Light>()->GetPos();
		DirectX::XMFLOAT4 light[3] = {
			{Ldir.x,Ldir.y,Ldir.z,10.0f},
			pLight->GetComponent<Light>()->GetDiffuse(),
			pLight->GetComponent<Light>()->GetAmbient()
		};
		// ライト行列
		DirectX::XMFLOAT4X4 lightMat[3];
		lightMat[0] = mat[0];		// ワールド行列
		DirectX::XMStoreFloat4x4(&lightMat[1], DirectX::XMMatrixTranspose(		// ビュー行列
			DirectX::XMMatrixLookAtLH(
				DirectX::XMVectorSet(Lpos.x, Lpos.y, Lpos.z, 0.0f),
				DirectX::XMVectorSet(Lpos.x + Ldir.x, Lpos.y + Ldir.y, Lpos.z + Ldir.z, 0.0f),
				DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			)));
		DirectX::XMStoreFloat4x4(&lightMat[2], DirectX::XMMatrixTranspose(		// プロジェクション行列
			DirectX::XMMatrixOrthographicLH(10.0f, 10.0f, 1.0f, 10000.0f)
		));

		// レンダーターゲット設定
		float rcolor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };	// 深度地を初期化(Rを深度地として使用)
		pDepthWriteRTV->Clear(rcolor);
		pDepthWriteDSV->Clear();
		// レンダーターゲット切り替え
		Renderer::SetRenderTargets(1, &pDepthWriteRTV, pDepthWriteDSV);

		// 生成したオブジェクト分Loopする
		for (int i = 1; i < ObjeList.size(); i++) {
			// キャラクターモデルを描画
			if (ObjeList[i]->GetComponent<ModelSetting>()->GetCharacterFlag())
			{
				ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vsシェーダーセット
				ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_DepthWrite"));	// Psシェーダーセット
				ObjeList[i]->Draw(GetObj<Shader>("VS_Object"), lightMat);				// ObjListの描画
			}
		}
		// デフォルトに戻す
		Renderer::SetRenderTargets(1, &pDefRTV, pDefDSV);
	}
}

/// <summary>
/// 常にガンマ補正した描画
/// </summary>
/// <param name="p_active">描画するかどうか</param>
void SceneEdit::AlwaysGammaRender(bool p_active)
{
	if (p_active) {
		// テクスチャ取得
		RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");
		DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");
		RenderTarget* pGammaRTV = GetObj<RenderTarget>("GammaCorrectionRTV");

		// レンダーターゲット切り替え
		Renderer::SetRenderTargets(1, &pGammaRTV, pDefDSV);		// ガンマ補正用RTVに切り替え

		// 変換行列設定
		DirectX::XMFLOAT4X4 mat[3];
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// 情報打消し
		DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());		// 情報打消し
		DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(
			DirectX::XMMatrixOrthographicLH(1.0f, 1.0f, 0.0f, 1.0f)));
		Sprite::SetWorld(mat[0]);
		Sprite::SetView(mat[1]);
		Sprite::SetProjection(mat[2]);
		Sprite::Draw();

		Sprite::SetPixelShader(GetObj<Shader>("PS_GammaCorrection"));	// ガンマ補正PSシェーダー設定
		Sprite::SetTexture(pGammaRTV);
	}
}

/// <summary>
/// 影付きのモデルの描画
/// </summary>
/// <param name="p_active">描画するかどうか</param>
/// <param name="p_rtv">レンダーターゲットの名前</param>
void SceneEdit::ShadowRender(bool p_active, std::string p_rtv)
{
	if (p_active && m_GBuffer) {
		// 共有オブジェクトの取得
		Camera* pCamera = GetObj<Camera>("Camera");
		MoveLight* pLight = GetObj<MoveLight>("Light");

		RenderTarget* pDepthWriteRTV = GetObj<RenderTarget>(p_rtv.c_str());

		// 変換行列設定
		DirectX::XMFLOAT4X4 mat[3];
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// ワールド
		mat[1] = pCamera->GetComponent<DXCamera>()->GetView();				// ビュー
		mat[2] = pCamera->GetComponent<DXCamera>()->GetProj();				// プロジェクション

		// ライトの方向と位置を取得
		DirectX::XMFLOAT3 Ldir = pLight->GetComponent<Light>()->GetDirection();
		DirectX::XMFLOAT3 Lpos = pLight->GetComponent<Light>()->GetPos();

		// ライト行列
		DirectX::XMFLOAT4X4 plightMat[2];
		DirectX::XMStoreFloat4x4(&plightMat[0], DirectX::XMMatrixTranspose(		// ビュー行列
			DirectX::XMMatrixLookAtLH(
				DirectX::XMVectorSet(Lpos.x, Lpos.y, Lpos.z, 0.0f),
				DirectX::XMVectorSet(Lpos.x + Ldir.x, Lpos.y + Ldir.y, Lpos.z + Ldir.z, 0.0f),
				DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			)
		));

		DirectX::XMStoreFloat4x4(&plightMat[1], DirectX::XMMatrixTranspose(		// プロジェクション行列
			DirectX::XMMatrixOrthographicLH(10.0f, 10.0f, 1.0f, 10000.0f)
		));

		GetObj<Shader>("VS_Shadow")->WriteBuffer(0, mat);				// モデル変換行列
		GetObj<Shader>("VS_Shadow")->WriteBuffer(2, plightMat);			// 光源からの変換行列（ビュー行列とプロジェクション行列）
		GetObj<Shader>("PS_Shadow")->SetTexture(1, pDepthWriteRTV);		// 深度テクスチャ

		// 生成したオブジェクト分Loopする
		for (int i = 1; i < ObjeList.size(); i++) {
			// 影付きのモデルを描画
			if (!ObjeList[i]->GetComponent<ModelSetting>()->GetCharacterFlag()) {
				ModelMaterialSet(i, true);			// マテリアルセット

				ObjeList[i]->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Shadow"));	// 保存Vsシェーダーセット
				ObjeList[i]->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_Shadow"));	// 前のPsシェーダーセット

				ObjeList[i]->Draw(GetObj<Shader>("VS_Shadow"), mat);	// ObjListの描画
				ModelMaterialSet(i, false);			// マテリアルセット解除
			}
		}
	}
}

/// <summary>
/// 被写界深度
/// </summary>
/// <param name="p_active">描画するかどうか</param>
void SceneEdit::DepthOfFieldRender(bool p_active)
{
	if (p_active) {
		Camera* pCamera = GetObj<Camera>("Camera");

		RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");			// デフォRTV
		DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");			// デフォDSV

		RenderTarget* pAlbedoRTV = GetObj<RenderTarget>("albedoRTV");	// アルベド情報
		RenderTarget* pDepthRTV = GetObj<RenderTarget>("DepthRTV");		// 深度マップ
		RenderTarget* pNomalRTV = GetObj<RenderTarget>("NomalRTV");		// 法線マップ
		RenderTarget* pBlurH = GetObj<RenderTarget>("BlurH");			// ブラー横半分
		RenderTarget* pBlurV = GetObj<RenderTarget>("BlurV");			// ブラー縦半分

		// 変換行列設定
		DirectX::XMFLOAT4X4 mat[3];
		DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());
		mat[1] = pCamera->GetViewMatrix();
		mat[2] = pCamera->GetProjMatrix();

		// レンダーターゲットのクリア
		float clearColor[] = { 1.0f,1.0f,1.0f,0.0f };
		pAlbedoRTV->Clear(clearColor);
		pDepthRTV->Clear(clearColor);
		pNomalRTV->Clear(clearColor);
		pBlurH->Clear(clearColor);
		pBlurV->Clear(clearColor);

		// 色と深度値を出力するレンダーターゲットを設定
		RenderTarget* pRTV[] = {
			pAlbedoRTV,pDepthRTV,pNomalRTV,
		};
		Renderer::SetRenderTargets(3, pRTV, pDefDSV);

		// デフォルトの描画
		DefaultRender(true);

		// 輪郭線
		OutlineRender(m_Outline);

		// 影の描画
		switch (m_ShadowQualityLevel) {
		default:
		case HIGH:	ShadowRender(m_IsShadow, "HighDepthWriteRTV"); break;
		case MEDIUM:ShadowRender(m_IsShadow, "DepthWriteRTV"); break;
		case LOW:	ShadowRender(m_IsShadow, "LowDepthWriteRTV"); break;
		}

		// ぼかし処理の前にスプライトの初期設定
		DirectX::XMFLOAT4X4 identity;
		DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());
		mat[0] = mat[1] = mat[2] = identity;
		Sprite::SetWorld(mat[0]);
		Sprite::SetView(mat[1]);
		Sprite::SetProjection(mat[2]);
		Sprite::SetSize(DirectX::XMFLOAT2(2.0f, 2.0f));

		// ブラー
		if (m_blur) {
			Sprite::SetPixelShader(GetObj<Shader>("PS_Blur"));
			// 横半分のレンダーターゲットへぼかしながら書き込み
			Renderer::SetRenderTargets(1, &pBlurH, nullptr);
			float blurParam[] = {
				SCREEN_WIDTH * m_blurStrength,SCREEN_HEIGHT,1.0f,0.0f
			};
			GetObj<Shader>("PS_Blur")->WriteBuffer(0, blurParam);
			Sprite::SetTexture(pAlbedoRTV);
			Sprite::Draw();

			// さらに縦半分のレンダーターゲットへぼかしながら書き込み
			Renderer::SetRenderTargets(1, &pBlurV, nullptr);
			blurParam[1] *= m_blurStrength;
			blurParam[2] = 0.0f;
			blurParam[3] = 1.0f;
			GetObj<Shader>("PS_Blur")->WriteBuffer(0, blurParam);
			Sprite::SetTexture(pBlurH);
			Sprite::Draw();
		}
		// 被写界深度
		if (m_DoF && m_blur) {
			// ここまでで生成したレンダーターゲットを利用して被写界深度を適応
			Renderer::SetRenderTargets(1, &pDefRTV, nullptr);
			Sprite::SetPixelShader(GetObj<Shader>("PS_DoF"));
			Sprite::SetTexture(pAlbedoRTV);
			GetObj<Shader>("PS_DoF")->WriteBuffer(0, m_ViewClipping);
			GetObj<Shader>("PS_DoF")->SetTexture(1, pDepthRTV);
			GetObj<Shader>("PS_DoF")->SetTexture(2, pBlurV);
			Sprite::Draw();
		}
		Sprite::SetTexture(pAlbedoRTV);
		Sprite::Draw();		// 描画
		Sprite::SetSize(DirectX::XMFLOAT2(1.0f, 1.0f));	// サイズ初期化
	}
}

/// <summary>
/// 画面にポストエフェクトを描画
/// </summary>
/// <param name="p_active">描画するかどうか</param>
void SceneEdit::PostEffectRender(bool p_active)
{
	// 各種シェーダー取得(ポストeffect用)
	Shader* shader[] = {
		GetObj<Shader>("PS_TexColor"),
		GetObj<Shader>("PS_FXAA"),
		GetObj<Shader>("PS_Monochrome"),
		GetObj<Shader>("PS_Sepia"),
		GetObj<Shader>("PS_Posterization"),
		GetObj<Shader>("PS_RGBSplit"),
		GetObj<Shader>("PS_Noise"),
		GetObj<Shader>("PS_Vignette"),
		GetObj<Shader>("PS_ScanningLine"),
		GetObj<Shader>("PS_GammaCorrection"),
	};

	// テクスチャ取得
	RenderTarget* pDefRTV = GetObj<RenderTarget>("RTV");
	DepthStencil* pDefDSV = GetObj<DepthStencil>("DSV");
	RenderTarget* pAlbedoRTV = GetObj<RenderTarget>("albedoRTV");

	// レンダーターゲット切り替え
	Renderer::SetRenderTargets(1, &pAlbedoRTV, pDefDSV);		// ポストプロセス

	// 変換行列設定
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixIdentity());		// 情報打消し
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());		// 情報打消し
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixOrthographicLH(1.0f, 1.0f, 0.0f, 1.0f)));
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::Draw();
	// スプライトで描画（ポストプロセス）
	Renderer::SetRenderTargets(1, &pDefRTV, pDefDSV);
	Sprite::SetPixelShader(shader[m_index]);
	Sprite::SetTexture(pAlbedoRTV);
}

/// <summary>
/// マテリアル情報を持っているモデルをセットするかどうか
/// </summary>
/// <param name="p_number">リストの番号</param>
/// <param name="p_set">セットするかどうか</param>
void SceneEdit::ModelMaterialSet(int p_number, bool p_set)
{
	if (ObjeList[p_number]->GetComponent<ModelMeshRenderer>() != nullptr) {
		ObjeList[p_number]->GetComponent<ModelMeshRenderer>()->SetMaterialShader(p_set);
	}

	if (ObjeList[p_number]->GetComponent<AnimModelMeshRenderer>() != nullptr) {
		ObjeList[p_number]->GetComponent<AnimModelMeshRenderer>()->SetMaterialShader(p_set);
	}
}

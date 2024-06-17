//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "Application.h"
#include "renderer.h"
#include <io.h>

using namespace DirectX::SimpleMath;

//-----------------------------------------------------------------------------
// ドライバ系
ID3D11Device* Renderer::m_Device{};						// ドライブ
ID3D11DeviceContext* Renderer::m_DeviceContext{};		// ドライブコンテキスト
IDXGISwapChain* Renderer::m_SwapChain{};				// スワップチェーン
ID3D11RasterizerState* Renderer::g_pRasterizerState[3];
ID3D11BlendState* Renderer::g_pBlendState[BLEND_MAX];
ID3D11SamplerState* Renderer::g_pSamplerState[SAMPLER_MAX];
ID3D11DepthStencilState* Renderer::g_pDepthStencilState[DEPTH_MAX];

//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
HRESULT  Renderer::Init(HWND hWnd, UINT width, UINT height, bool fullscreen)
{
	HRESULT	hr = E_FAIL;
	//-----------------------------------------------------------------------------
	// swapChainDesc.BufferCount						: バックバッファー表示モード		
	// swapChainDesc.BufferDesc.Width					: 解像度の幅を表す値
	// swapChainDesc.BufferDesc.Height					: 解像度の幅を表す値
	// swapChainDesc.BufferDesc.Format					: 表示形式を記述するDXGI_FORMAT構造体
	// swapChainDesc.BufferDesc.RefreshRate				: リフレッシュ レートをヘルツ単位で記述するDXGI_RATIONAL構造体
	// swapChainDesc.BufferUsage						: バック バッファーのサーフェス使用量と CPU アクセス オプションを記述する、 DXGI_USAGE 列挙型のメンバー
	// swapChainDesc.OutputWindow						: 出力ウィンドウへの HWND ハンドル
	// swapChainDesc.SampleDesc							: マルチサンプリング パラメーターを記述する DXGI_SAMPLE_DESC 構造体。
	// swapChainDesc.Windowed							: 出力がウィンドウ モードであるかどうかを示すブール値。 出力 がウィンドウ モードの場合は TRUE。それ以外の場合は FALSE。
	//-----------------------------------------------------------------------------
	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = width;							// 解像度の幅設定
	swapChainDesc.BufferDesc.Height = height;						// 解像度の高さ設定
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 0~1に正規化されたRGBA 32bitフォーマット
	swapChainDesc.SampleDesc.Count = 1;								// ピクセルあたりのマルチサンプルの数。
	swapChainDesc.SampleDesc.Quality = 0;							// クオリティは最低
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1000;			// 有理数の先頭を表す符号なし整数値。
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			// 有理数の底を表す符号なし整数値。
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// サーフェスまたはリソースを出力レンダー ターゲットとして使用します。
	swapChainDesc.BufferCount = 1;									// バックバッファー表示モード
	swapChainDesc.OutputWindow = hWnd;								// ApplicationのHWNDハンドル
	swapChainDesc.Windowed = fullscreen ? FALSE : TRUE;				// 出力がウィンドウ モード
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPUで描画
		D3D_DRIVER_TYPE_WARP,		// 高精度(低速)
		D3D_DRIVER_TYPE_REFERENCE,	// CPUで描画
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1対応GPUレベル
		D3D_FEATURE_LEVEL_11_0,		// DirectX11対応GPUレベル
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1対応GPUレベル
		D3D_FEATURE_LEVEL_10_0,		// DirectX10対応GPUレベル
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3対応GPUレベル
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2対応GPUレベル
		D3D_FEATURE_LEVEL_9_1		// Direct9.1対応GPUレベル
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,							// ディスプレイデバイスのアダプタ（NULLの場合最初に見つかったアダプタ）
			D3D_DRIVER_TYPE_HARDWARE,		// デバイスドライバのタイプ
			NULL,							// ソフトウェアラスタライザを使用する場合に指定する
			createDeviceFlags,				// デバイスフラグ
			featureLevels,					// 機能レベル
			numFeatureLevels,				// 機能レベル数
			D3D11_SDK_VERSION,				// SDKのバージョン
			&swapChainDesc,					// スワップチェインの設定
			&m_SwapChain,					// IDXGIDwapChainインタフェース	
			&m_Device,						// ID3D11Deviceインタフェース
			&featureLevel,					// サポートされている機能レベル
			&m_DeviceContext);				// デバイスコンテキスト
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		return hr;
	}

	//-----------------------------------------------------------------------------
	// FillMode					: レンダリング時に使用する塗りつぶしモード
	// CullMode					: 指定した方向に向いている三角形が描画モード
	// FrontCounterClockwise	: 三角形が正面向きか背面向きかを設定
	// DepthClipEnable			: 距離に基づいてクリッピングを有効にするか
	// MultisampleEnable		: マルチサンプル アンチエイリアシング (MSAA) レンダー ターゲットで四角形またはアルファ線のアンチエイリアシング アルゴリズムを使用するか
	//-----------------------------------------------------------------------------
	// カリング設定
	D3D11_RASTERIZER_DESC rasterizer = {};
	D3D11_CULL_MODE cull[] = {
		D3D11_CULL_NONE,
		D3D11_CULL_FRONT,
		D3D11_CULL_BACK
	};
	rasterizer.FillMode = D3D11_FILL_SOLID;		// 頂点によって形成された三角形を塗りつぶす（ソリッド
	rasterizer.FrontCounterClockwise = true;	// 正面向き
	rasterizer.DepthClipEnable = TRUE;			// クリッピングを有効
	rasterizer.MultisampleEnable = FALSE;		// アンチエイリアシング アルゴリズムを使用しない
	for (int i = 0; i < 3; ++i)
	{
		rasterizer.CullMode = cull[i];
		hr = m_Device->CreateRasterizerState(&rasterizer, &g_pRasterizerState[i]);
	}
	SetCullingMode(D3D11_CULL_NONE);

	//--- 深度テスト
	// -----------------------------------------------------------------------------
	// DepthEnable					: 深度テストを有効にするかどうか
	// DepthFunc					: 深度データと既存の深度データを比較する
	// DepthWriteMask				: レ深度データと変更できる深度ステンシルバッファの一部の識別
	// StencilEnable				: ステンシルステートを有効にするかどうか 
	// StencilReadMask				: ステンシル データを読み取るための深度ステンシルバッファーの一部を識別
	// StencilWriteMask				: ステンシル データを書き込むための深度ステンシルバッファーの一部を識別
	// FrontFace					: 深度テストの結果とサーフェスの法線がカメラに向いているピクセルのステンシルテストの使用方法を特定
	// FrontFace.StencilFailOp		: ステンシルテストが失敗したときに実行するステンシル操作
	// FrontFace.StencilDepthFailOp	: ステンシルテストが成功して深度テストが失敗したときに実行するステンシル操作
	// FrontFace.StencilPassOp		: ステンシルテストと深度テストの両方が成功した場合の実行するステンシル操作
	// FrontFace.StencilFunc		: ステンシル データと既存のステンシル データを比較する
	// BackFace						: 深度テストの結果とサーフェスの法線がカメラから離れているピクセルのステンシルテストの使用方法を特定
	// https://tositeru.github.io/ImasaraDX11/part/ZBuffer-and-depth-stencil
	// -----------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	// 深度テストを行う深度ステンシルステートの作成とステンシルテストを行う深度ステンシルステートの作成
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
	dsDesc.BackFace = dsDesc.FrontFace;
	bool enablePattern[] = { true, true, false };
	D3D11_DEPTH_WRITE_MASK maskPattern[] = {
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_DEPTH_WRITE_MASK_ZERO,
	};
	for (int i = 0; i < DEPTH_MAX; ++i)
	{
		dsDesc.DepthEnable = dsDesc.StencilEnable = enablePattern[i];
		dsDesc.DepthWriteMask = maskPattern[i];
		hr = m_Device->CreateDepthStencilState(&dsDesc, &g_pDepthStencilState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetDepthTest(DEPTH_ENABLE_WRITE_TEST);

	// アルファブレンディング
	// -----------------------------------------------------------------------------
	// AlphaToCoverageEnable				: ピクセルをレンダー ターゲットに設定するときに、マルチサンプリング手法としてアルファからカバレッジを使用するかどうか
	// IndependentBlendEnable				: 同時レンダー ターゲットで独立したブレンドを有効にするかどうか
	// RenderTarget[8]						: レンダー ターゲットのブレンド状態
	// RenderTarget.BlendEnable				: ブレンドを有効 (または無効) 
	// RenderTarget.BlendOp					: SrcBlend 操作と DestBlend 操作を組み合わせる方法
	// RenderTarget.BlendOpAlpha			: SrcBlendAlpha 操作と DestBlendAlpha 操作を組み合わせる方法
	// RenderTarget.SrcBlend				: 描画元のピクセル シェーダーが出力する RGB 値に対して実行する操作を指定 
	// RenderTarget.SrcBlendAlpha			: 描画元のピクセル シェーダーが出力するアルファ値に対して実行する操作を指定
	// RenderTarget.DestBlend				: 描画先のレンダー ターゲットの現在の RGB 値に対して実行する操作を指定
	// RenderTarget.DestBlendAlpha			: 描画先のレンダー ターゲットの現在のアルファ値に対して実行する操作を指定
	// RenderTarget.RenderTargetWriteMask	: 書き込みマスク
	// -----------------------------------------------------------------------------
	// https://pgming-ctrl.com/directx11/blend/
	// PMXを描画するときにRenderTexture内のアルファブレンドを行ったあと、
	// 実際にフレームバッファに描く、再度計算される時にα値のブレンド係数がおかしくなり、本来見えないモデルのテクスチャが見えてしまうので
	// 描画先のアルファ値に対してD3D11_BLEND_ONE（素材の色）かD3D11_BLEND_INV_SRC_ALPHA（α値の反転色）を設定する
	// DestBlendAlphaがα値のブレンド係数なのでDestBlendも合わせる必要がある
	// https://github.com/benikabocha/saba/issues/15 参考資料
	// SrcBlendとDestBlendの設定はのちに切り替えれるように実装
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;						// カバレッジを使用しない
	blendDesc.IndependentBlendEnable = FALSE;						// 独立ブレンド無効
	blendDesc.RenderTarget[0].BlendEnable = TRUE;					// 有効
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			// 加算
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	// 加算
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		// 描画元のα値は素材の元の色
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;		// 描画先のα値は素材の元の色		※D3D11_BLEND_INV_SRC_ALPHAでもよい
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	// 全色対応
	// ブレンドの係数組み合わせ
	// D3D11_BLEND_ZERO				0乗算 : なんでもゼロになる
	// D3D11_BLEND_ONE				1乗算 : 素材の元の色となる
	// D3D11_BLEND_SRC_COLOR		描画元の色と乗算する
	// D3D11_BLEND_INV_SRC_COLOR	描画元の反転色と乗算する
	// D3D11_BLEND_SRC_ALPHA		描画元のα値と乗算する
	// D3D11_BLEND_INV_SRC_ALPHA	描画元のα値の反転色と乗算する
	// D3D11_BLEND_DEST_ALPHA		描画先のα値と乗算する
	// D3D11_BLEND_INV_DEST_ALPHA	描画先のα値の反転色と乗算する
	// D3D11_BLEND_DEST_COLOR		描画先の色と乗算する
	// D3D11_BLEND_INV_DEST_COLOR	描画先の反転色と乗算する
	// D3D11_BLEND_SRC_ALPHA_SAT	αが1-αの小さいほうを係数として採用
	// D3D11_BLEND_BLEND_FACTOR		OMSetBlendFactor()関数から与えられる係数
	// D3D11_BLEND_INV_BLEND_FACTOR	OMSetBlendFactor()関数から与えられる係数の反転色
	// D3D11_BLEND_SRC1_COLOR		target1の色を係数とする
	// D3D11_BLEND_INV_SRC1_COLOR	target1の色の反転値を係数とする
	// D3D11_BLEND_SRC1_ALPHA		target1のα値を係数とする
	// D3D11_BLEND_INV_SRC1_ALPHA	target1のα値の反転色を係数とする
	D3D11_BLEND blend[BLEND_MAX][2] = {
		{D3D11_BLEND_ONE, D3D11_BLEND_ZERO},					// 1乗算 + 0乗
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},		// 描画元のα値 + 描画元のα値の反転色
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ZERO},		// 描画元のα値 + 描画元のα値の反転色
		{D3D11_BLEND_ONE, D3D11_BLEND_ONE},						// 1乗算 + 1乗算
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE},				// 描画元のα値 + 1乗算
		{D3D11_BLEND_ZERO, D3D11_BLEND_INV_SRC_COLOR},			// 0乗 + 描画元の反転色
		{D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE},			// 描画元の反転色 + 1乗算
	};
	for (int i = 0; i < BLEND_MAX; ++i)
	{
		blendDesc.RenderTarget[0].SrcBlend = blend[i][0];
		blendDesc.RenderTarget[0].DestBlend = blend[i][1];
		hr = m_Device->CreateBlendState(&blendDesc, &g_pBlendState[i]);
		if (FAILED(hr)) { return hr; }
	}
	// SrcBlendとDestBlend設定
	// 	BLEND_NONE	ブレンドをしない
	//	BLEND_ALPHA	ピクセルシェーダーから渡されるカラーデータに従ってブレンド
	//	BLEND_NONEALPHA ピクセルシェーダーから渡されるカラーデータに1従わずブレンド
	//	BLEND_ADD	加算合成
	//	BLEND_ADDALPHA  α値を加算合成する
	//	BLEND_SUB		減算合成
	//	BLEND_SCREEN    スクリーンブレンド
	//	BLEND_TEST
	//	BLEND_MAX	最大数count用
	SetBlendMode(BLEND_ALPHA);

	// サンプラー
	D3D11_SAMPLER_DESC samplerDesc = {};
	D3D11_FILTER filter[] = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_MIP_POINT,
	};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	for (int i = 0; i < SAMPLER_MAX; ++i)
	{
		samplerDesc.Filter = filter[i];
		hr = m_Device->CreateSamplerState(&samplerDesc, &g_pSamplerState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetSamplerState(SAMPLER_LINEAR);

	return S_OK;
}
//-----------------------------------------------------------------------------
//      解放処理です.
//-----------------------------------------------------------------------------
void Renderer::Uninit()
{
	for (int i = 0; i < SAMPLER_MAX; ++i)
		g_pSamplerState[i]->Release();
	for (int i = 0; i < BLEND_MAX; ++i)
		g_pBlendState[i]->Release();
	for (int i = 0; i < 3; ++i)
		g_pRasterizerState[i]->Release();

	m_DeviceContext->ClearState();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->QueryInterface(&m_Device);
	m_Device->Release();
}
//-----------------------------------------------------------------------------
//      描画削除処理です.
//-----------------------------------------------------------------------------
void Renderer::SwapEnd()
{
	m_SwapChain->Present(0, 0);		// モデルキューを反転させる
}

//-----------------------------------------------------------------------------
//      カリング設定　指定した方向に向いている三角形が描画モードを設定する.
//-----------------------------------------------------------------------------
void Renderer::SetCullingMode(D3D11_CULL_MODE cull)
{
	switch (cull)
	{
	case D3D11_CULL_NONE: m_DeviceContext->RSSetState(g_pRasterizerState[0]); break;
	case D3D11_CULL_FRONT: m_DeviceContext->RSSetState(g_pRasterizerState[1]); break;
	case D3D11_CULL_BACK: m_DeviceContext->RSSetState(g_pRasterizerState[2]); break;
	}
}

//-----------------------------------------------------------------------------
//      ブレンド設定.
//-----------------------------------------------------------------------------
void Renderer::SetBlendMode(BlendMode blend)
{
	if (blend < 0 || blend >= BLEND_MAX) return;
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };	// clear
	m_DeviceContext->OMSetBlendState(g_pBlendState[blend], blendFactor, 0xffffffff);
}

//-----------------------------------------------------------------------------
//      深度テスト設定.
//-----------------------------------------------------------------------------
void Renderer::SetDepthTest(DepthState state)
{
	m_DeviceContext->OMSetDepthStencilState(g_pDepthStencilState[state], 0);
}

//-----------------------------------------------------------------------------
//      サンプラー設定.
//-----------------------------------------------------------------------------
void Renderer::SetSamplerState(SamplerState state)
{
	if (state < 0 || state >= SAMPLER_MAX) return;
	m_DeviceContext->VSSetSamplers(0, 1, &g_pSamplerState[state]);	// 頂点シェーダー
	m_DeviceContext->GSSetSamplers(0, 1, &g_pSamplerState[state]);	// ジオメトリシェーダー
	m_DeviceContext->HSSetSamplers(0, 1, &g_pSamplerState[state]);	// ハルシェーダー
	m_DeviceContext->DSSetSamplers(0, 1, &g_pSamplerState[state]);	// ドメインシェーダー
	m_DeviceContext->PSSetSamplers(0, 1, &g_pSamplerState[state]);	// ピクセルシェーダー
}

//-----------------------------------------------------------------------------
//      レンダリング設定.
//-----------------------------------------------------------------------------
void Renderer::SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView)
{
	static ID3D11RenderTargetView* rtvs[4];

	if (num > 4) num = 4;
	for (UINT i = 0; i < num; ++i)
		rtvs[i] = ppViews[i]->GetView();
	m_DeviceContext->OMSetRenderTargets(num, rtvs, pView ? pView->GetView() : nullptr);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (float)ppViews[0]->GetWidth();
	vp.Height = (float)ppViews[0]->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_DeviceContext->RSSetViewports(1, &vp);
}
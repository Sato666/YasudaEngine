//-------------------------------------------------------------------------------------
// Class : Renderer
// 
// 描画関係の機能のもとを持つクラス
// シェーダに情報を渡す機能を持つ
// DirectX11の機能を返す機能を持つ
// 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<d3d11.h>
#include	<SimpleMath.h>
#include	<string>
#include	"Texture.h"


// リンクすべき外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

//-----------------------------------------------------------------------------
// マテリアル
struct MATERIAL
{
	DirectX::SimpleMath::Color	Ambient;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Color	Specular;
	DirectX::SimpleMath::Color	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2]{};
};
//-----------------------------------------------------------------------------
// 平行光源
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	DirectX::SimpleMath::Vector3	Direction;		// ライトの位置
	DirectX::SimpleMath::Color		Diffuse;		// ライトの拡散
	DirectX::SimpleMath::Color		Ambient;		// ライトの色
};
//-----------------------------------------------------------------------------
// メッシュ（マテリアル毎にサブセットが存在する）
struct SUBSET {
	std::string		Name;							//サブセット名
	unsigned int	IndexNum = 0;					// インデックス数
	unsigned int	VertexNum = 0;					// 頂点数
	unsigned int	IndexBase = 0;					// 開始インデックス
	unsigned int	VertexBase = 0;					// 頂点ベース
	unsigned int	MaterialIdx = 0;				// マテリアルインデックス
};

// 全部消すよう
#define SAFE_RELEASE(p) do{if(p){p->Release(); p = nullptr;}}while(0)

// 前方宣言
class RenderTarget;		// レンダーターゲット
class DepthStencil;		// 深度テクスチャ

// ブレンドモード定義
enum BlendMode
{
	BLEND_NONE,
	BLEND_ALPHA,
	BLEND_NONEALPHA,
	BLEND_ADD,
	BLEND_ADDALPHA,
	BLEND_SUB,
	BLEND_SCREEN,
	BLEND_MAX
};

// サンプラーステート定義
enum SamplerState
{
	SAMPLER_LINEAR,
	SAMPLER_POINT,
	SAMPLER_MAX
};

enum DepthState
{
	DEPTH_ENABLE_WRITE_TEST,
	DEPTH_ENABLE_TEST,
	DEPTH_DISABLE,
	DEPTH_MAX
};

//-----------------------------------------------------------------------------
// レンダラ
class Renderer
{
private:
	//-----------------------------------------------------------------------------
	// ドライバ系
	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RasterizerState* g_pRasterizerState[3];
	static ID3D11BlendState* g_pBlendState[BLEND_MAX];
	static ID3D11SamplerState* g_pSamplerState[SAMPLER_MAX];
	static ID3D11DepthStencilState* g_pDepthStencilState[DEPTH_MAX];

public:
	// 初期化処理
	static HRESULT Init(HWND hWnd, UINT width, UINT height, bool fullscreen);
	// 解放処理
	static void Uninit();
	// 描画終了処理
	static void SwapEnd();

	/// <summary>
	/// <para>指定した方向に向いている三角形の描画モードを設定する</para>
	/// <para>D3D11_CULL_NONE  : すべての三角形を描画</para>
	/// <para>D3D11_CULL_FRONT : 表面のみ描画</para>
	/// <para>D3D11_CULL_BACK  : 裏面のみ描画</para>
	/// </summary>
	/// <param name="cull">D3D11_CULL_MODE</param>
	static void SetCullingMode(D3D11_CULL_MODE cull);

	/// <summary>
	/// <para>ブレンド</para>
	/// </summary>
	/// <param name="blend"></param>
	static void SetBlendMode(BlendMode blend);
	static void SetDepthTest(DepthState state);

	static void SetSamplerState(SamplerState state);
	static void SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView);

	/// <summary>
	/// ドライブ取得
	/// </summary>
	/// <returns>ID3D11Device</returns>
	static ID3D11Device* GetDevice(void) { return m_Device; }
	/// <summary>
	/// コンテキスト取得
	/// </summary>
	/// <returns>ID3D11DeviceContext</returns>
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }
	/// <summary>
	/// スワップチェーン取得
	/// </summary>
	/// <returns>IDXGISwapChain</returns>
	static IDXGISwapChain* GetSwapChain(void) { return m_SwapChain; }
};

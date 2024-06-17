//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Sprite.h"

Sprite::Data Sprite::m_data;
std::shared_ptr<VertexShader> Sprite::m_defVS;
std::shared_ptr<PixelShader> Sprite::m_defPS;
std::shared_ptr<Texture> Sprite::m_whiteTex;

/// <summary>
/// 初期化
/// </summary>
void Sprite::Init()
{
	const char* VS = R"EOT(
struct VS_IN {
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	float4 wPos : POSITION0;
};
cbuffer Matrix : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
cbuffer Param : register(b1) {
	float2 offset;
	float2 size;
	float2 uvPos;
	float2 uvScale;
	float4 color;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos.xy *= size;
	vout.pos.xy += offset;
	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	vout.uv *= uvScale;
	vout.uv += uvPos;
	vout.color = color;
	return vout;
})EOT";
	const char* PS = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET {
	return tex.Sample(samp, pin.uv) * pin.color;
})EOT";

	struct Vertex
	{
		float pos[3];
		float uv[2];
	} vtx[] = {
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
		{{ 0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},
		{{-0.5f,-0.5f, 0.0f}, {0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.0f}, {1.0f, 1.0f}},
	};

	// メッシュ
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxSize = sizeof(Vertex);
	desc.vtxCount = _countof(vtx);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_data.mesh = std::make_shared<MeshBuffer>(desc);

	// パラメーター
	m_data.param[0] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_data.param[1] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_data.param[2] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&m_data.matrix[0], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_data.matrix[1], DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_data.matrix[2], DirectX::XMMatrixIdentity());

	// シェーダー
	m_defVS = std::make_shared<VertexShader>();
	m_defVS->Compile(VS);
	m_data.vs = m_defVS.get();
	m_defPS = std::make_shared <PixelShader>();
	m_defPS->Compile(PS);
	m_data.ps = m_defPS.get();

	// デフォルトテクスチャ
	BYTE color[] = { 255,255,255,255 };
	m_whiteTex = std::make_shared<Texture>();
	m_whiteTex->Create(DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, color);
}
/// <summary>
/// 解放
/// </summary>
void Sprite::Uninit()
{
}
/// <summary>
/// 描画
/// </summary>
void Sprite::Draw()
{
	m_data.vs->WriteBuffer(0, m_data.matrix);		// 頂点シェーダーにマトリックスデータを送る
	m_data.vs->WriteBuffer(1, m_data.param);		// 頂点シェーダーにデータを送る
	m_data.vs->Bind();								// VS描画
	m_data.ps->SetTexture(0, m_data.texture);		// シーンを画像化して渡している
	m_data.ps->Bind();								// PS描画
	m_data.mesh->Draw();							// メッシュ描画
}

/// <summary>
/// オフセットをセット
/// </summary>
/// <param name="offset">flaot2のデータ</param>
void Sprite::SetOffset(DirectX::XMFLOAT2 offset)
{
	m_data.param[0].x = offset.x;
	m_data.param[0].y = offset.y;
}
/// <summary>
/// サイズをセット
/// </summary>
/// <param name="size">flaot2のデータ</param>
void Sprite::SetSize(DirectX::XMFLOAT2 size)
{
	m_data.param[0].z = size.x;
	m_data.param[0].w = size.y;
}
/// <summary>
/// 位置をセット
/// </summary>
/// <param name="pos">float2のデータ</param>
void Sprite::SetUVPos(DirectX::XMFLOAT2 pos)
{
	m_data.param[1].x = pos.x;
	m_data.param[1].y = pos.y;
}
/// <summary>
/// UVのサイズセット
/// </summary>
/// <param name="scale">float2のデータ</param>
void Sprite::SetUVScale(DirectX::XMFLOAT2 scale)
{
	m_data.param[1].x = scale.x;
	m_data.param[1].y = scale.y;
}
/// <summary>
/// 色をセットする
/// </summary>
/// <param name="color">float4のデータ</param>
void Sprite::SetColor(DirectX::XMFLOAT4 color)
{
	m_data.param[2] = color;
}
/// <summary>
/// テクスチャをセット
/// </summary>
/// <param name="tex">テクスチャのRTVポインタ</param>
void Sprite::SetTexture(Texture* tex)
{
	m_data.texture = tex ? tex : m_whiteTex.get();
}
/// <summary>
/// ワールド行列をセット
/// </summary>
/// <param name="world">4x4のワールド行列</param>
void Sprite::SetWorld(DirectX::XMFLOAT4X4 world)
{
	m_data.matrix[0] = world;
}
/// <summary>
/// ビュー行列をセット
/// </summary>
/// <param name="view">4x4のビュー行列</param>
void Sprite::SetView(DirectX::XMFLOAT4X4 view)
{
	m_data.matrix[1] = view;
}
/// <summary>
/// プロジェクション行列をセット
/// </summary>
/// <param name="proj">4x4のプロジェクション行列</param>
void Sprite::SetProjection(DirectX::XMFLOAT4X4 proj)
{
	m_data.matrix[2] = proj;
}

/// <summary>
/// VSシェーダーをセットする
/// </summary>
/// <param name="vs">VSシェーダーのポインタ</param>
void Sprite::SetVertexShader(Shader* vs)
{
	if (vs && typeid(VertexShader) == typeid(*vs))
		m_data.vs = vs;
	else
		m_data.vs = m_defVS.get();
}
/// <summary>
/// PSシェーダーをセットする
/// </summary>
/// <param name="ps">PSシェーダーのポインタ</param>
void Sprite::SetPixelShader(Shader* ps)
{
	if (ps && typeid(PixelShader) == typeid(*ps))
		m_data.ps = ps;
	else
		m_data.ps = m_defPS.get();
}
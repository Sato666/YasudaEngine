#include "ShaderComponent.h"
//-------------------------------------------------------
// オプション関係
#include "renderer.h"
#include "Shader.h"
#include "gameobject.h"
#include "ModelMeshRenderer.h"

std::shared_ptr<VertexShader> ShaderComponent::m_defVS = nullptr;
std::shared_ptr<PixelShader> ShaderComponent::m_defPS = nullptr;

//-----------------------------------------------------------------------------
//      ロード処理です.
//-----------------------------------------------------------------------------
void ShaderComponent::Load()
{
	MakeDefaultShader();

	m_pVS = m_defVS.get();
	m_pPS = m_defPS.get();
}
//-----------------------------------------------------------------------------
//      描画処理です.
//-----------------------------------------------------------------------------
void ShaderComponent::Draw()
{
    m_pVS->Bind();
    m_pPS->Bind();
}

void ShaderComponent::Uninit()
{

}

void ShaderComponent::MakeDefaultShader()
{
	const char* vsCode = R"EOT(
struct VS_IN {
    float3 pos : POSITION0; 
    float3 normal : NORMAL0; 
    float4 diff : COLOR0;
    float2 uv : TEXCOORD0; 
};
struct VS_OUT {
    float4 pos : SV_POSITION;
    float4 diff : COLOR0;
    float2 uv : TEXCOORD0;
    float4 normal : NORMAL0;
};
cbuffer Matrix0 : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};
VS_OUT main(VS_IN vin) {
    VS_OUT vout;
    matrix wvp;
    wvp = mul(world, view);
    wvp = mul(wvp, proj);
    float4 worldNormal, normal;
    normal = float4(vin.normal, 0.0f);
    worldNormal = mul(normal, world);
    worldNormal = normalize(worldNormal);
    vout.pos = mul(float4(vin.pos.xyz, 1.0f), wvp);
    vout.uv = vin.uv;
    return vout;
})EOT";
	const char* psCode = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
};
float4 main(PS_IN pin) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
})EOT";
	m_defVS = std::make_shared<VertexShader>();
	m_defVS->Compile(vsCode);
	m_defPS = std::make_shared<PixelShader>();
	m_defPS->Compile(psCode);
}
//-----------------------------------------------------------------------------
//      頂点シェーダにセットする.
//-----------------------------------------------------------------------------
void ShaderComponent::SetVSShader(Shader* _vs)
{
	VertexShader* cast = reinterpret_cast<VertexShader*>(_vs);
	if (cast)
		m_pVS = cast;
}
//-----------------------------------------------------------------------------
//      ピクセルシェーダにセットする.
//-----------------------------------------------------------------------------
void ShaderComponent::SetPSShader(Shader* _ps)
{
	PixelShader* cast = reinterpret_cast<PixelShader*>(_ps);
	if (cast)
		m_pPS = cast;
}

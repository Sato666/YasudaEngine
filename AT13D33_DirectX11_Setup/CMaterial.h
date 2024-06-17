//-------------------------------------------------------------------------------------
// Class : CMaterial
// 
// マテリアルを管理するクラス
// マテリアルを受け取る機能を持つ
// マテリアルを更新する機能を持つ
// GPUにセットする機能を持つ
// マテリアルをセットする機能を持つ
// 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<Windows.h>
#include	<directxmath.h>
#include	<d3d11.h>
//-----------------------------------------------------------------------------
// その他
#include "utility.h"
#include "renderer.h"

#include "Shader.h"

class CMaterial 
{
	struct ConstantBufferMaterial 
	{
		MATERIAL	Material;
	};

	MATERIAL	m_Material{};

	VertexShader* m_pVS;	// 現在の頂点シェーダー
	std::shared_ptr<VertexShader> m_defVS;
public:
	//-----------------------------------------------------------------------------
	// 初期化処理です.
	//-----------------------------------------------------------------------------
	bool Init(MATERIAL mtrl) 
	{
		// 初期値
		m_Material.Ambient = mtrl.Ambient;
		m_Material.Diffuse = mtrl.Diffuse;
		m_Material.Specular = mtrl.Specular;
		m_Material.Emission = mtrl.Emission;
		m_Material.Shininess = mtrl.Shininess;
		m_Material.TextureEnable = mtrl.TextureEnable;

		MakeVertexShader();

		m_pVS->WriteBuffer(1, &m_Material);

		return true;
	}
	//-----------------------------------------------------------------------------
	// シェーダーを取得する処理です.
	//-----------------------------------------------------------------------------
	void SetShader(Shader* _vs)
	{
		VertexShader* castvs = reinterpret_cast<VertexShader*>(_vs);
		if (castvs)
			m_pVS = castvs;
	}
	//-----------------------------------------------------------------------------
	// GPUにセット処理です.
	//-----------------------------------------------------------------------------
	void SetGPU(int _number) 
	{
		m_pVS->WriteBuffer(_number, &m_Material);
		//m_pVS->Bind();
	}
	//-----------------------------------------------------------------------------
	// マテリアルをセットする処理です.
	//-----------------------------------------------------------------------------
	void SetMaterial(const MATERIAL& mtrl)
	{
		m_Material = mtrl;
	}

	void SetDiffuse(DirectX::XMFLOAT4 diffuse) 
	{
		m_Material.Diffuse = diffuse;
	}
	DirectX::XMFLOAT4 GetDiffuse() { return m_Material.Diffuse; }

	void SetAmbient(DirectX::XMFLOAT4 ambient) 
	{
		m_Material.Ambient = ambient;
	}
	DirectX::XMFLOAT4 GetAmbient() { return m_Material.Ambient; }

	void SetSpecular(DirectX::XMFLOAT4 specular) 
	{
		m_Material.Specular = specular;
	}
	DirectX::XMFLOAT4 GetSpecular() { return m_Material.Specular; }

	void SetEmission(DirectX::XMFLOAT4 emission)
	{
		m_Material.Emission = emission;
	}
	DirectX::XMFLOAT4 GetEmission() { return m_Material.Emission; }

	void SetShiness(float shiness) 
	{
		m_Material.Shininess = shiness;
	}
	float GetShiness() { return m_Material.Shininess; }

	void MakeVertexShader()
	{
		const char* VSCode = R"EOT(
struct VS_IN
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};
cbuffer Matrix0 : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};
cbuffer MaterialBuffer : register(b1)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
}
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
        
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    vout.normal = mul(vin.normal, (float3x3) world);
    
    vout.color.xyz = vin.color.xyz * Diffuse.xyz;
    vout.color.xyz += vin.color.xyz * Ambient.xyz;
    vout.color.xyz += Emission.xyz;
    vout.color.a = vin.color.a * Diffuse.a;

    vout.uv = vin.uv;
    
    return vout;
}
)EOT";
		m_defVS = std::make_shared<VertexShader>();
		HRESULT hr = m_defVS->Compile(VSCode);
		m_pVS = m_defVS.get();
	}
};

// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float4 color : COLOR0; // 色情報
};

cbuffer MaterialBuffer : register(b0)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
}

Texture2D tex : register(t0); // ベースのテクスチャ
SamplerState samp : register(s0); // テクスチャサンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 0.0f);
    color = tex.Sample(samp, pin.uv);
    
    if (TextureEnable)
    {
        color = tex.Sample(samp, pin.uv);
        color *= Diffuse;
    }
    else
    {
        color += Diffuse;
        color.a = Diffuse.a;
    }
    
    return color;
}
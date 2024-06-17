// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION;   // ピクセルの位置
    float2 uv : TEXCOORD0;      // テクスチャ座標
};

// 定数バッファ
cbuffer GammaValue : register(b0)
{
    float1 gammavalue;
};

Texture2D tex : register(t0); // ベースのテクスチャ
SamplerState samp : register(s0); // テクスチャサンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    // テクスチャから色をサンプリング
    float4 color = tex.Sample(samp, pin.uv);
    
    // ガンマ補正を適用
    color.rgb = pow(color.rgb, (1.0f / gammavalue));
    
    return color;
}
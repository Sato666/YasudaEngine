// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

// テクスチャを受け取る
Texture2D<float4> tex : register(t0);
Texture2D<float4> depth : register(t1);
Texture2D<float4> normal : register(t2);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

float4 main(PS_IN pin) : SV_TARGET
{
    // テクスチャから色をサンプリング
    float4 color = tex.Sample(samp, pin.uv);
    
    return color;
}
// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // ピクセルの位置
    float2 uv : TEXCOORD0; // テクスチャ座標
};

Texture2D tex : register(t0); // ベースのテクスチャ
SamplerState samp : register(s0); // テクスチャサンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    // テクスチャから色をサンプリング
    float4 color = tex.Sample(samp, pin.uv);

    // r成分とg成分をずらす
    color.r = tex.Sample(samp, pin.uv + 0.002f).r;  // 0.002fずらす
    color.g = tex.Sample(samp, pin.uv - 0.002f).g;  // -0.002fずらす

    return color;
}
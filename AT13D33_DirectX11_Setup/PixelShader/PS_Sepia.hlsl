// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // ピクセルの位置
    float2 uv : TEXCOORD0;    // テクスチャ座標
};

Texture2D tex : register(t0); // ベースのテクスチャ
SamplerState samp : register(s0); // テクスチャサンプラー

float4 main(PS_IN pin) : SV_TARGET
{
    // テクスチャから色をサンプリング
    float4 color = tex.Sample(samp, pin.uv);

    // セピア色に色を乗算
    color.rgb = color.r * 0.3f + color.g * 0.6f + color.b * 0.1f;
	// セピアっぽく見せる重み付け
    color.g *= 74.0f / 107.0f;
    color.b *= 43.0f / 107.0f;

    return color;
}
// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // ピクセルの位置
    float2 uv : TEXCOORD0; // テクスチャ座標
};

Texture2D tex : register(t0); // ベースのテクスチャ
SamplerState samp : register(s0); // テクスチャサンプラー

// 定数バッファ
cbuffer Time : register(b0)
{
    float1 time;    // 現在時間
};

float4 main(PS_IN pin) : SV_TARGET
{    
    float2 samplePoint = pin.uv; // テクスチャの座標を取得
    float4 color = tex.Sample(samp, samplePoint); // テクスチャから色をサンプリング
    
    // 周期的に波を生成
    float sinv = sin(pin.uv.y * 2 + time * -0.1f); // 正弦波を取得
    float steped = step(0.99f, sinv * sinv); // 大きいほうを返す
    // ノイズを走らせる
    color.rgb -= (1 - steped) * abs(sin(pin.uv.y * 50.0 + time * 1.0)) * 0.05f;
    color.rgb -= (1 - steped) * abs(sin(pin.uv.y * 100.0 - time * 2.0)) * 0.08f;
    // 波の高いほうを光らせる
    color.rgb += steped * 0.1f;
    return color;
}
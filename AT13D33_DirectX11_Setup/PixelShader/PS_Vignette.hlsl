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
    float2 samplePoint = pin.uv;    // テクスチャの座標を取得
    float4 color = tex.Sample(samp, samplePoint); // テクスチャから色をサンプリング
    
    // テクスチャの座標から0.5f引いたベクトルの長さを取得
    float vignette = length(float2(0.5f, 0.5f) - pin.uv);
    // 0~1に収まるように変換する
    vignette = clamp(vignette - 0.2f, 0, 1);
    // 逆色にして色を返す
    color.rgb -= vignette;
    return color;
}
// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // ピクセルの位置
    float2 uv : TEXCOORD0; // テクスチャ座標
};

Texture2D tex : register(t0); // ベースのテクスチャ
SamplerState samp : register(s0); // テクスチャサンプラー

// 2D疑似乱数ジェネレータ関数
float rand(float2 co)
{
	// 2Dの疑似乱数ジェネレータを使用します。
	// 時間変数を追加することで各フレームで異なる結果を得ます。
    float time = 0.0f;
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453 + time);
}

float4 main(PS_IN input) : SV_TARGET
{
    // テクスチャから色をサンプリング
    float4 color = tex.Sample(samp, input.uv);

    // 乱数を生成し、色に加算
    float noise = rand(input.uv);
    color.rgb += noise;
    
    // 色を0から1の範囲にクランプ
    color.rgb = saturate(color.rgb);

    return color;
}
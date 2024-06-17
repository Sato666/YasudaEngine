// 頂点シェーダーから受け取るためのデータを定義
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

cbuffer Param : register(b0)
{
    float2 texSize; // テクスチャサイズ
    float2 blurDir; // ブラーをかける方向
};

// テクスチャを受け取る
Texture2D tex : register(t0);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

float4 main(PS_IN pin) : SV_TARGET
{
    float valume = 1.0f / 5.0f; // 5x5のフィルター値
    float filter[5] =
    {
        valume, valume, valume, valume, valume,
    };
    
    // フィルターに基づいて色を取得
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);  // 出力色の初期化
    float2 uvOffset = 1.0f / texSize;               // 1ピクセルのUV上での移動量
    uvOffset *= blurDir;                            // 指定された方向のみ移動させる
    float2 uv = pin.uv - uvOffset * 2;              // 初期位置からずれたUV座標を計算
    
    // フィルターを適応して色を計算
    for (int i = 0; i < 5; ++i)
    {
        // テクスチャから色をサンプリングしてフィルターをかける
        color += tex.Sample(samp, uv) * filter[i];
        uv += uvOffset;
    }
    
    return color;
}
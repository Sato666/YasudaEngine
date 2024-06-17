// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線情報
};

// 定数バッファ
cbuffer OutLineColorValue : register(b0)
{
    float4 outlinecolorvalue;
};

// ピクセルシェーダのメイン処理
// 頂点で囲まれているピクセル分だけ実行する
float4 main(PS_IN pin) : SV_TARGET
{
    // 輪郭の色を指定
    float4 color = outlinecolorvalue;
  
    return color;
}
// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // ピクセルの位置
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL; // 法線ベクトル
};

// 各ピクセルのz位置を赤色の強さで表す
float4 main(PS_IN pin) : SV_Target
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // 深度値算出ピクセルのz位置をRGBのR要素として保存する
    color.r = pin.pos.z;
    
    return color;
}
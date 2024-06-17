struct VS_IN
{
    float3 pos : POSITION0; // セマンティクス
    float3 normal : NORMAL0; // データの用途を定義する
    float4 color : COLOR0; // 色情報
    float2 uv : TEXCOORD0; // テクスチャ座標(UV)
};

struct VS_OUT
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線情報
    
    // スクリーン座標とは別にワールド座標のデータを用意
    // ピクセルシェーダーに渡すとピクセルごとのワールド座標のデータになる
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// 定数バッファ
cbuffer WVP : register(b0)
{
    float4x4 world; // ワールド行列
    float4x4 view; // ビュー行列
    float4x4 proj; // プロジェクション行列
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    
    // ピクセルシェーダーに渡すために途中で変数にコピー
    vout.worldPos = vout.pos;
    
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    vout.screenPos = vout.pos;
    
    vout.uv = vin.uv;
    
    vout.normal = mul(vin.normal, (float3x3) world);
    
    return vout; // voutをピクセルシェーダに返す   
}
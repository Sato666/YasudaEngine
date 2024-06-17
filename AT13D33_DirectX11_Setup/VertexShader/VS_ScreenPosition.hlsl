struct VS_IN
{
    float3 pos : POSITION0; // セマンティクス
    float3 normal : NORMAL0; // データの用途を定義する
    float4 color : COLOR0; // 色情報
    float2 uv : TEXCOORD0; // テクスチャ座標(UV)
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    // スクリーン座標
    float4 screenPos : TEXCOORD1;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    vout.pos = float4(vin.pos, 1.0f); // ローカル座標
    vout.pos = mul(vout.pos, world); // ワールド座標
    vout.pos = mul(vout.pos, view); // ビュー座標
    vout.pos = mul(vout.pos, proj); // プロジェクション座標
    
    vout.screenPos = vout.pos;

    vout.uv = vin.uv;
    vout.normal = mul(vin.normal, (float3x3) world);

    return vout;
}
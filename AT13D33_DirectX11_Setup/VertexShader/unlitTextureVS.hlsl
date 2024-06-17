struct VS_IN
{
    float3 pos : POSITION0; // セマンティクス
    float3 normal : NORMAL0; // データの用途を定義する
    float4 color : COLOR0; // 色情報
    float2 uv : TEXCOORD0; // テクスチャ座標(UV)
};

// ピクセルシェーダへの入力
struct VS_OUT
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // データの用途を定義する
    float4 color : COLOR0; // 色情報
};

cbuffer Matrix0 : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
}

VS_OUT main(in VS_IN vin)
{
    VS_OUT vout;
    
    vout.pos = float4(vin.pos, 1.0f); // ローカル座標
    vout.pos = mul(vout.pos, world); // ワールド座標
    
    vout.pos = mul(vout.pos, view); // ビュー座標
    vout.pos = mul(vout.pos, proj); // プロジェクション座標
    
    vout.normal = mul(vin.normal, (float3x3) world);

	// 頂点データのUV座標をピクセルシェーダへの出力データとして設定
    vout.uv = vin.uv;
    
    vout.color = vin.color * Diffuse;

    return vout;
}


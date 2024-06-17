// 頂点シェーダへの入力
// 必要に合わせて変更していく
struct VS_IN
{
    float3 pos : POSITION0; // セマンティクス
    float3 normal : NORMAL0; // データの用途を定義する
    float4 color : COLOR0; // 色情報
    float2 uv : TEXCOORD0; // テクスチャ座標(UV)
};

// 頂点シェーダからの出力→ピクセルシェーダに出力
struct VS_OUT
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // ワールド変換後の頂点座標
    float4 color : COLOR0; // 色情報
    float4 shadowPos : POSITION0; // 頂点座標（ワールド空間）
    float4 screenPosTex : TEXCOORD1;
};

// 定数バッファ
cbuffer Matrix0 : register(b0)
{
    float4x4 world; // ワールド行列
    float4x4 view; // ビュー行列
    float4x4 proj; // プロジェクション行列
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

// 光源からの視点の変換行列
cbuffer LightMat : register(b2)
{
    float4x4 Lview; // 光源からのビュー変換行列
    float4x4 Lproj; // 光源からのプロジェクション変換行列
};

// 影計算用の位置情報出力
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    // 頂点位置のワールド変換
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    
    // 影の計算用に位置をコピー
    vout.shadowPos = vout.pos;
    
    // 頂点位置のビュー変換とプロジェクション変換
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    // スクリーン座標を渡す
    vout.screenPosTex = vout.pos;
    
    // 影を光源からの視点でビュー変換とプロジェクション変換
    vout.shadowPos = mul(vout.shadowPos, Lview);
    vout.shadowPos = mul(vout.shadowPos, Lproj);
    
    // 法線のワールド変換
    vout.normal = mul(vin.normal, (float3x3) world);
    
    // Material色適応
    vout.color.xyz = vin.color.xyz * Diffuse.xyz;
    vout.color.a = vin.color.a * Diffuse.a;
    
    // UVをそのまま出力
    vout.uv = vin.uv;
    
    return vout; // voutをピクセルシェーダに返す
}
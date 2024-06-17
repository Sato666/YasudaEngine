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
    float3 normal : NORMAL0; // 法線情報    
    float4 color : COLOR0; // マテリアル情報
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

// 頂点シェーダのメイン処理
// この処理を「頂点」ごとに実行する
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    vout.pos = float4(vin.pos, 1.0f); // ローカル座標
    vout.pos = mul(vout.pos, world); // ワールド座標
        
    vout.pos = mul(vout.pos, view); // ビュー座標
    vout.pos = mul(vout.pos, proj); // プロジェクション座標
    
	// ピクセルシェーダーで利用する法線を渡す
	// 法線はオブジェクトの回転に併せて回転させないとおかしな見た目になる。
	// ワールド行列には移動成分も含まれるが、移動を法線に適用してもおかしく
	// なるので、回転成分のみの行列となるように縮小して計算する。
    vout.normal = mul(vin.normal, (float3x3) world);
    
    // Material色適応
    vout.color.xyz = vin.color.xyz * Diffuse.xyz;
    vout.color.xyz += vin.color.xyz * Ambient.xyz;
    vout.color.xyz += Emission.xyz;
    vout.color.a = vin.color.a * Diffuse.a;

	// 頂点データのUV座標を加工せずにピクセルシェーダへの出力データとして設定
    vout.uv = vin.uv;
    
    return vout; // voutをピクセルシェーダに返す   
}

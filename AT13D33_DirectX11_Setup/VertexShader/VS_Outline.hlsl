// インプットレイアウト
// CPUから渡される頂点の情報を定義したもの
struct VS_IN
{
    float3 pos : POSITION0; // セマンティクス
    float3 normal : NORMAL0; // データの用途を定義する
    float4 color : COLOR0; // 色情報
    float2 uv : TEXCOORD0; // テクスチャ座標(UV)
};

// 頂点シェーダーからピクセルシェーダーへ送るデータの定義
struct VS_OUT
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線情報

     // スクリーン座標
    float4 screenPosTex : TEXCOORD1;
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// 定数バッファ
// CPUとGPUでやり取りする、頂点以外のデータを格納する先
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// 定数バッファ
cbuffer OutLine : register(b1)
{
    float1 outline;
};

// テクスチャを受け取る
Texture2D tex : register(t0);
Texture2D<float4> rampTex : register(t1);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

VS_OUT main(VS_IN vin)
{    
    VS_OUT vout;

	// ローカル座標をスクリーン座標へ変換
    vout.pos = float4(vin.pos, 1.0f); // ローカル座標
    vout.pos = mul(vout.pos, world); // ワールド座標
    
    // ワールド座標を渡す
    vout.worldPos = vout.pos;
    
    // 輪郭線として表示するために、法線方向へ頂点を移動
    // (単純に拡縮で大きくすると見た目がおかしくなる)
    // 正規化された法線は1(メートル)で拡大が大きくなりすぎるため補正を入れる
    vout.pos.xyz += normalize(vin.normal) * outline;
    
    vout.pos = mul(vout.pos, view); // ビュー座標
    vout.pos = mul(vout.pos, proj); // プロジェクション座標
    
    // スクリーン座標を渡す
    vout.screenPosTex = vout.pos;
    vout.screenPos = vout.pos;

	// ピクセルシェーダーで利用するUV座標を渡す
    vout.uv = vin.uv;

	// ピクセルシェーダーで利用する法線を渡す
	// 法線はオブジェクトの回転に併せて回転させないとおかしな見た目になる。
	// ワールド行列には移動成分も含まれるが、移動を法線に適用してもおかしく
	// なるので、回転成分のみの行列となるように縮小して計算する。
    vout.normal = mul(vin.normal, (float3x3) world);

    return vout;
}
// 頂点シェーダへの入力
// 必要に合わせて変更していく
struct VS_IN
{
    float3 pos : POSITION0; // セマンティクス
    float3 normal : NORMAL0; // データの用途を定義する
    float4 color : COLOR0; // 色情報
    float2 uv : TEXCOORD0; // テクスチャ座標(UV)
    int4 BoneIndex : BONEINDEX;
    float4 BoneWeight : BONEWEIGHT;
};

// 頂点シェーダからの出力→ピクセルシェーダに出力
struct VS_OUT
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線情報
    float4 color : COLOR0; // 色情報
    // スクリーン座標
    float4 screenPosTex : TEXCOORD1;
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// 定数バッファ
cbuffer Matrix0 : register(b0)
{
    float4x4 world; // ワールド行列
    float4x4 view; // ビュー行列
    float4x4 proj; // プロジェクション行列
};

cbuffer BoneMatrixBuffer : register(b1)
{
    matrix BoneMatrix[400];
}

cbuffer MaterialBuffer : register(b2)
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
    
    // ワンスキン頂点ブレンドの処理
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
		// 重みを計算しながら行列生成
        comb += BoneMatrix[vin.BoneIndex[i]] * vin.BoneWeight[i];
    }
    
    float4 Pos;
    Pos = mul((float4x4) comb, float4(vin.pos, 1.0f));
    vin.pos = Pos;
    vout.pos = Pos; // ローカル座標
    vout.pos = mul(vout.pos, world); // ワールド座標
    
    // ワールド座標を渡す
    vout.worldPos = vout.pos;
    
    vout.pos = mul(vout.pos, view); // ビュー座標
    vout.pos = mul(vout.pos, proj); // プロジェクション座標
    
    // スクリーン座標を渡す
    vout.screenPosTex = vout.pos;
    vout.screenPos = vout.pos;
    
 	// ピクセルシェーダーで利用する法線を渡す
	// 法線はオブジェクトの回転に併せて回転させないとおかしな見た目になる。
	// ワールド行列には移動成分も含まれるが、移動を法線に適用してもおかしく
	// なるので、回転成分のみの行列となるように縮小して計算する。
    vout.normal = mul(vin.normal, (float3x3) world);
    
     // Material色適応
    vout.color.xyz = vin.color.xyz * Diffuse.xyz;
    vout.color.xyz = normalize(vout.color.xyz);
    vout.color.xyz = vout.color.xyz + Ambient.xyz;
    vout.color.a = Diffuse.a;

	// 頂点データのUV座標をピクセルシェーダへの出力データとして設定
    vout.uv = vin.uv;
    
    return vout; // voutをピクセルシェーダに返す   
}

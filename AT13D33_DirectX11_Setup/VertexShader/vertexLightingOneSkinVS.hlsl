struct VSONESKIN_IN
{
    float3 pos : POSITION0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    int4 BoneIndex : BONEINDEX;
    float4 BoneWeight : BONEWEIGHT;
};

struct VS_OUT
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線情報
};

cbuffer Matrix0 : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer BoneMatrixBuffer : register(b2)
{
    matrix BoneMatrix[400];
}

VS_OUT main(VSONESKIN_IN vin)
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
    
    Pos = mul(comb, (float4)(vin.pos, 1.0f));
    vin.pos = Pos;

    matrix wvp;
    wvp = mul(world, view);
    wvp = mul(wvp, proj);
	
    float4 worldNormal, normal;
    normal = float4(vin.normal.xyz, 0.0);
    worldNormal = mul(normal, world);
    worldNormal = normalize(worldNormal);
    
    vout.pos = mul(float4(vin.pos.xyz, 1.0f), wvp);

	// 頂点データのUV座標をピクセルシェーダへの出力データとして設定
    vout.uv = vin.uv;
    
    return vout; // voutをピクセルシェーダに返す  
}

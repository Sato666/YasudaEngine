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
    
    float2 tex0 : TEXCOORD0; // テクスチャ座標    
    float4 tex1 : TEXCOORD1;
    float4 tex2 : TEXCOORD2;
    float4 tex3 : TEXCOORD3;
    float4 tex4 : TEXCOORD4;
    float4 tex5 : TEXCOORD5;
    float4 tex6 : TEXCOORD6;
    float4 tex7 : TEXCOORD7;
    float4 tex8 : TEXCOORD8;
};
cbuffer Matrix0 : register(b0)
{
    float4x4 world; // ワールド行列
    float4x4 view; // ビュー行列
    float4x4 proj; // プロジェクション行列
};
cbuffer Param : register(b1)
{
    float2 offset;
    float2 size;
    float2 uvPos;
    float2 uvScale;
    float4 color;
};

// テクスチャを受け取る
Texture2D<float4> tex : register(t0);
Texture2D<float4> depth : register(t1);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

VS_OUT main(VS_IN vin)
{
    float2 texSize;
    float level;
	//テクスチャーのサイズを取得する
    tex.GetDimensions(0, texSize.x, texSize.y, level);
    
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f); // ローカル座標
    vout.pos.xy *= size;
    vout.pos.xy += offset;
    vout.pos = mul(vout.pos, world);

    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    float2 tex = vin.uv;
    
    float offset = 0.2f;
	//法線
	{
		//真ん中のピクセル
        vout.tex0 = tex;

		//右上のピクセル
        vout.tex1.xy = tex + float2(offset / texSize.x, -offset / texSize.y);

		//上のピクセル
        vout.tex2.xy = tex + float2(0.0f, -offset / texSize.y);

		//左上のピクセル
        vout.tex3.xy = tex + float2(-offset / texSize.x, -offset / texSize.y);
		
		//右のピクセル
        vout.tex4.xy = tex + float2(offset / texSize.x, 0.0f);

		//左のピクセル
        vout.tex5.xy = tex + float2(-offset / texSize.x, 0.0f);

		//右下のピクセル
        vout.tex6.xy = tex + float2(offset / texSize.x, offset / texSize.y);

		//下のピクセル
        vout.tex7.xy = tex + float2(0.0f, offset / texSize.y);

		//左下のピクセル
        vout.tex8.xy = tex + float2(-offset / texSize.x, offset / texSize.y);
    }

    //深度値
	{
		//深度値を取り出すときに使うUV座標
        offset = 1.0f;
		//右上のピクセル
        vout.tex1.zw = tex + float2(offset / texSize.x, -offset / texSize.y);

		//上のピクセル
        vout.tex2.zw = tex + float2(0.0f, -offset / texSize.y);

		//左上のピクセル
        vout.tex3.zw = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//右のピクセル
        vout.tex4.zw = tex + float2(offset / texSize.x, 0.0f);

		//左のピクセル
        vout.tex5.zw = tex + float2(-offset / texSize.x, 0.0f);

		//右下のピクセル
        vout.tex6.zw = tex + float2(offset / texSize.x, offset / texSize.y);

		//下のピクセル
        vout.tex7.zw = tex + float2(0.0f, offset / texSize.y);

		//左下のピクセル
        vout.tex8.zw = tex + float2(-offset / texSize.x, offset / texSize.y);
    }
    
    return vout;
}
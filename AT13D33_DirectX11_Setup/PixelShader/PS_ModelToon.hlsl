// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION; // プロジェクション変換後の頂点座標
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 normal : NORMAL0; // 法線情報
    float4 color : COLOR0; // 色情報
    float4 screenPosTex : TEXCOORD1;    // スクリーン座標
};

// 複数の出力を持つため構造体にまとめる（出力先はSV_TARGETxで指定）
struct PS_OUT
{
    float4 albedo : SV_Target0; // アルベド（色）出力
    float4 depth : SV_Target1; // 深度値
    float4 nomal : SV_Target2; // 法線マップ
};

// ライトの情報
cbuffer Light : register(b0)
{
    float4 lightColor;
    float4 lightAmbient;
    float4 lightDir;
}

// テクスチャを受け取る
Texture2D tex : register(t0);
Texture2D<float4> rampTex : register(t1);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

PS_OUT main(PS_IN pin) : SV_TARGET
{
    PS_OUT pout;

    // 法線ベクトルとライトの方向ベクトルを取得し、ライトの方向を反転
    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);
    L = -L;
    
    // 拡散反射の計算
    float diffuse = saturate(dot(N, L));

#if 1
    // ランプテクスチャを使用して拡散反射を補正
    float2 rampUV = float2(diffuse * 0.98f + 0.01f, 0.5f);
    diffuse = rampTex.Sample(samp, rampUV);
#else
    // diffuseの値を階調化（0 ~ 1 → 0,0.abort,1.0）
    diffuse *= 2.0f; // 一度数値の範囲を広げる
    // 切り捨ての関係上、2になるのは1の時だけ(0.9 * 2 = 1.8, 切り捨ては1となる)
    diffuse += 0.5f;
    // 少数を切り捨てた値を取得
    diffuse = floor(diffuse);
    // 広げた範囲と同等の数値を割る
    diffuse /= 2.0f;
#endif
    
	// テクスチャの色を考慮して陰を付ける
    pout.albedo = tex.Sample(samp, pin.uv) * pin.color;
    pout.albedo.rgb *= (diffuse + lightAmbient.rgb);
    
    // スクリーン座標から深度を取得して深度値を出力
    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // 法線ベクトルを正規化し、w成分を1に設定
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
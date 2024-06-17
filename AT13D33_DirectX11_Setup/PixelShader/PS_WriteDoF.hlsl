struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float4 screenPosTex : TEXCOORD1;
};

// 複数の出力を持つため構造体にまとめる（出力先はSV_TARGETxで指定）
struct PS_OUT
{
    float4 albedo : SV_Target0; // アルベド（色）出力
    float4 depth : SV_Target1; // 深度値
    float4 nomal : SV_Target2;// 深度値
};

// テクスチャを受け取る
Texture2D tex : register(t0);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    
    // アルベド（色）をテクスチャからサンプリング
    pout.albedo = tex.Sample(samp, pin.uv);
    
    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // 法線ベクトルを正規化し、w成分を1に設定
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
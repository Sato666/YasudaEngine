struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float4 worldPos : POSITION0;
};

// 複数の出力を持つため構造体にまとめる（出力先はSV_TARGETxで指定）
struct PS_OUT
{
    float4 albedo : SV_Target0; // アルベド（色）出力
    float4 nomal : SV_Target1; // 法線ベクトル出力
    float4 world : SV_Target2; // ワールド座標出力
};

// テクスチャを受け取る
Texture2D tex : register(t0);
SamplerState samp : register(s0); // テクスチャの繰り返し設定

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    
    // アルベド（色）をテクスチャからサンプリング
    pout.albedo = tex.Sample(samp, pin.uv);
    
    // 法線ベクトルを正規化し、w成分を1に設定
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    // ワールド座標を出力
    pout.world = pin.worldPos;
    
    return pout;
}
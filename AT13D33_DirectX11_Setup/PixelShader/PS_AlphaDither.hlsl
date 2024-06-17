// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float4 screenPosTex : TEXCOORD1;
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// 複数の出力を持つため構造体にまとめる（出力先はSV_TARGETxで指定）
struct PS_OUT
{
    float4 albedo : SV_Target0; // アルベド（色）出力
    float4 depth : SV_Target1; // 深度値
    float4 nomal : SV_Target2; // 深度値
};

// 定数バッファ
cbuffer Camera : register(b0)
{
    float4 camPos;
};

// 定数バッファ
cbuffer AlphaLen : register(b1)
{
    float1 alphalen;
};

// テクスチャのバインド
Texture2D tex : register(t0);
Texture2D dither : register(t1);
// サンプラーへのバインド
SamplerState samp : register(s0);

// ピクセルシェーダのメイン処理
// 頂点で囲まれているピクセル分だけ実行する
PS_OUT main(PS_IN pin) : SV_TARGET
{
    PS_OUT pout;
    
    // テクスチャから色をサンプリングしてマテリアル色を乗算する
    pout.albedo = tex.Sample(samp, pin.uv) * pin.color;
    
    // 距離に応じて透明度を変更
    float len = length(pin.worldPos.xyz - camPos.xyz);
    float alpha = pout.albedo.a * saturate(len * alphalen);
    
    // スクリーンの座標に応じてディザパターンを適応する位置を計算
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV *= float2(1920.0f, 1080.0f) / 4.0f;    // スクリーンサイズ
    
    float mask = dither.Sample(samp, screenUV).r;
    
    if(alpha < mask)
    {
        // 透明度0の場合は完全に表示を行わない
        discard;
    }

    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // 法線ベクトルを正規化し、w成分を1に設定
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
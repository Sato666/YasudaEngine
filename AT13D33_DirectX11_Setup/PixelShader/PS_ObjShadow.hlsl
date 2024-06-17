// ピクセルシェーダへの入力
struct PS_IN
{
    float4 pos : SV_POSITION;   // ピクセルの位置
    float2 uv : TEXCOORD0;      // テクスチャ座標
    float3 normal : NORMAL;     // 法線ベクトル
    float4 color : COLOR0;      // 色情報
    float4 shadowPos : POSITION0; // ワールド座標
    float4 screenPosTex : TEXCOORD1;
};

// 複数の出力を持つため構造体にまとめる（出力先はSV_TARGETxで指定）
struct PS_OUT
{
    float4 albedo : SV_Target0; // アルベド（色）出力
    float4 depth : SV_Target1; // 深度値
    float4 nomal : SV_Target2; // 深度値
};

// サンプラーへのバインド
SamplerState samp : register(s0); // サンプラーステートが入っている
// テクスチャのバインド
Texture2D tex : register(t0); // テクスチャのリソースが入っている
// テクスチャのバインド
Texture2D depthTex : register(t1); // テクスチャのリソースが入っている

// ピクセルシェーダのメイン処理
// 頂点で囲まれているピクセル分だけ実行する
PS_OUT main(PS_IN pin) : SV_TARGET
{
    PS_OUT pout;
    
    // テクスチャから色を取得
    pout.albedo = tex.Sample(samp, pin.uv);
    
    float2 depthUV = pin.shadowPos.xy / pin.shadowPos.w; // 影で使うUVを計算
    float pixelDepth = pin.shadowPos.z / pin.shadowPos.w; // 深度地を取得
    
    // UV座標を-1から1の範囲から0から1の範囲に変換する
    depthUV = max(-1.0, min(1.0, depthUV)) * 0.5f + 0.5f;
    // Y座標の向きを反転
    depthUV.y = -depthUV.y;
    
    // 深度テクスチャから深度値を取得
    float depthshadow = depthTex.Sample(samp, depthUV);
    
    // 取得した深度値から影の補正を行う
    pout.albedo.rgb *= saturate(step(pixelDepth, depthshadow) + 0.5f);
    
    // スクリーン座標から深度値取得
    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // 法線ベクトルを正規化し、w成分を1に設定
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0; // 法線情報
};

// ディファードレンダリングにおいてGBufferに書き込まれた情報をもとに
// レンダリング計算を行うためのサンプラー及びテクスチャ
SamplerState samp : register(s0);
Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D worldTex : register(t2);

// ライトの情報
struct Light
{
    float4 color;
    float3 pos;
    float range;
};

// ライトの情報
cbuffer LightBuf : register(b0)
{
    Light lights[30];
};


float4 main(PS_IN pin) : SV_TARGET
{
     // テクスチャからアルベド（色）情報をサンプリング
    float4 albedo = albedoTex.Sample(samp, pin.uv);
    
     // テクスチャから法線ベクトルをサンプリング
    float3 N = normalize(normalTex.Sample(samp, pin.uv).rgb);
    
     // テクスチャからワールド座標をサンプリング
    float3 worldPos = worldTex.Sample(samp, pin.uv).rgb;
    
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // ピクセルの1点に対して、すべての点光源と明るさを計算する
    for (int i = 0; i < 30; ++i)
    {
        // 各ピクセルから点光源に向かうベクトルを計算
        float3 toLightVec = lights[i].pos - worldPos;
        float3 V = normalize(toLightVec); // 正規化したベクトル
        float toLightLen = length(toLightVec); // 光源までの距離
        
        // 点光源に向かうベクトルと法線から明るさを計算
        float dotNV = saturate(dot(N, V));
        
        // 距離に応じて光の強さを変える
        // 距離が近ければ明るく離れていれば暗くなるよう計算
        float attenuation = saturate(1.0f - toLightLen / lights[i].range);
        
        // 自然な減衰の計算
        // 距離に応じて光の当たる面が乗算で増えていく
        attenuation = pow(attenuation, 2.0f);
        
        // 複数の点光源の光を合算していく
        color.rgb += lights[i].color.rgb * dotNV * attenuation;
    }
    // アルベド情報を基に最終的な色を求める
    color.rgb *= albedo.rbg;
    color.a = albedo.a;

    // 最終的な色を返す
    return color;
}
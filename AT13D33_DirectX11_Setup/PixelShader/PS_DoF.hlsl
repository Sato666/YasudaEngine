struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

cbuffer Focus : register(b0)
{
    float near;
    float far;
};

// テクスチャを受け取る
SamplerState samp : register(s0);
Texture2D albedoTex : register(t0);
Texture2D depthTex : register(t1);
Texture2D blurTex : register(t2);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // 各情報を取得
    float4 albedo = albedoTex.Sample(samp, pin.uv);
    float depth = depthTex.Sample(samp, pin.uv).r;
    float4 blur = blurTex.Sample(samp, pin.uv);
    float rate = saturate(abs(depth - near) / far);
    rate = pow(rate, 2.0f) * 0.8f;
    color = lerp(albedo, blur, rate);
    
    return color;
}
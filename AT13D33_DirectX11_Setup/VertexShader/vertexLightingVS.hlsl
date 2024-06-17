struct VS_IN
{
    float3 pos : POSITION0; // ���_���W
    float3 normal : NORMAL0; // �@�����
    float4 diff : COLOR0; // �F���
    float2 uv : TEXCOORD0; // �e�N�X�`�����W(UV)
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

cbuffer Matrix0 : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

cbuffer Light : register(b1)
{
    float4 lightDir;
    float4 lightDiffuse;
    float4 lightAmbient;
};


VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);

    vout.uv = vin.uv;

    float3 N = normalize(mul(vin.normal, (float3x3) world));
    float3 L = normalize(-lightDir.xyz);

    float diffuse = saturate(dot(N, L));
    vout.color = diffuse * lightDiffuse + lightAmbient;

    return vout;
}
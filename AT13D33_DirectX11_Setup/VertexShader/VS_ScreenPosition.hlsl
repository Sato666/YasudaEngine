struct VS_IN
{
    float3 pos : POSITION0; // �Z�}���e�B�N�X
    float3 normal : NORMAL0; // �f�[�^�̗p�r���`����
    float4 color : COLOR0; // �F���
    float2 uv : TEXCOORD0; // �e�N�X�`�����W(UV)
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    // �X�N���[�����W
    float4 screenPos : TEXCOORD1;
};

cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    vout.pos = float4(vin.pos, 1.0f); // ���[�J�����W
    vout.pos = mul(vout.pos, world); // ���[���h���W
    vout.pos = mul(vout.pos, view); // �r���[���W
    vout.pos = mul(vout.pos, proj); // �v���W�F�N�V�������W
    
    vout.screenPos = vout.pos;

    vout.uv = vin.uv;
    vout.normal = mul(vin.normal, (float3x3) world);

    return vout;
}
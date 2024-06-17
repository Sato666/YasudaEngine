// ���_�V�F�[�_�ւ̓���
// �K�v�ɍ��킹�ĕύX���Ă���
struct VS_IN
{
    float3 pos : POSITION0; // �Z�}���e�B�N�X
    float3 normal : NORMAL0; // �f�[�^�̗p�r���`����
    float4 color : COLOR0; // �F���
    float2 uv : TEXCOORD0; // �e�N�X�`�����W(UV)
};

// ���_�V�F�[�_����̏o�́��s�N�Z���V�F�[�_�ɏo��
struct VS_OUT
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0; // ���[���h�ϊ���̒��_���W
    float4 color : COLOR0; // �F���
    float4 shadowPos : POSITION0; // ���_���W�i���[���h��ԁj
    float4 screenPosTex : TEXCOORD1;
};

// �萔�o�b�t�@
cbuffer Matrix0 : register(b0)
{
    float4x4 world; // ���[���h�s��
    float4x4 view; // �r���[�s��
    float4x4 proj; // �v���W�F�N�V�����s��
};

cbuffer MaterialBuffer : register(b1)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
}

// ��������̎��_�̕ϊ��s��
cbuffer LightMat : register(b2)
{
    float4x4 Lview; // ��������̃r���[�ϊ��s��
    float4x4 Lproj; // ��������̃v���W�F�N�V�����ϊ��s��
};

// �e�v�Z�p�̈ʒu���o��
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    // ���_�ʒu�̃��[���h�ϊ�
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    
    // �e�̌v�Z�p�Ɉʒu���R�s�[
    vout.shadowPos = vout.pos;
    
    // ���_�ʒu�̃r���[�ϊ��ƃv���W�F�N�V�����ϊ�
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    // �X�N���[�����W��n��
    vout.screenPosTex = vout.pos;
    
    // �e����������̎��_�Ńr���[�ϊ��ƃv���W�F�N�V�����ϊ�
    vout.shadowPos = mul(vout.shadowPos, Lview);
    vout.shadowPos = mul(vout.shadowPos, Lproj);
    
    // �@���̃��[���h�ϊ�
    vout.normal = mul(vin.normal, (float3x3) world);
    
    // Material�F�K��
    vout.color.xyz = vin.color.xyz * Diffuse.xyz;
    vout.color.a = vin.color.a * Diffuse.a;
    
    // UV�����̂܂܏o��
    vout.uv = vin.uv;
    
    return vout; // vout���s�N�Z���V�F�[�_�ɕԂ�
}
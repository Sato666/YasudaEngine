struct VS_IN
{
    float3 pos : POSITION0; // �Z�}���e�B�N�X
    float3 normal : NORMAL0; // �f�[�^�̗p�r���`����
    float4 color : COLOR0; // �F���
    float2 uv : TEXCOORD0; // �e�N�X�`�����W(UV)
};

struct VS_OUT
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0; // �@�����
    
    // �X�N���[�����W�Ƃ͕ʂɃ��[���h���W�̃f�[�^��p��
    // �s�N�Z���V�F�[�_�[�ɓn���ƃs�N�Z�����Ƃ̃��[���h���W�̃f�[�^�ɂȂ�
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// �萔�o�b�t�@
cbuffer WVP : register(b0)
{
    float4x4 world; // ���[���h�s��
    float4x4 view; // �r���[�s��
    float4x4 proj; // �v���W�F�N�V�����s��
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f);
    vout.pos = mul(vout.pos, world);
    
    // �s�N�Z���V�F�[�_�[�ɓn�����߂ɓr���ŕϐ��ɃR�s�[
    vout.worldPos = vout.pos;
    
    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    vout.screenPos = vout.pos;
    
    vout.uv = vin.uv;
    
    vout.normal = mul(vin.normal, (float3x3) world);
    
    return vout; // vout���s�N�Z���V�F�[�_�ɕԂ�   
}
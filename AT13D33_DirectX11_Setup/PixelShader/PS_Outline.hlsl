// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0; // �@�����
};

// �萔�o�b�t�@
cbuffer OutLineColorValue : register(b0)
{
    float4 outlinecolorvalue;
};

// �s�N�Z���V�F�[�_�̃��C������
// ���_�ň͂܂�Ă���s�N�Z�����������s����
float4 main(PS_IN pin) : SV_TARGET
{
    // �֊s�̐F���w��
    float4 color = outlinecolorvalue;
  
    return color;
}
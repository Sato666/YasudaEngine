// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL; // �@���x�N�g��
};

// �e�s�N�Z����z�ʒu��ԐF�̋����ŕ\��
float4 main(PS_IN pin) : SV_Target
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // �[�x�l�Z�o�s�N�Z����z�ʒu��RGB��R�v�f�Ƃ��ĕۑ�����
    color.r = pin.pos.z;
    
    return color;
}
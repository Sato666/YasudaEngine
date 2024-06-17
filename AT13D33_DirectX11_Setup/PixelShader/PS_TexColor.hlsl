// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0;
    float4 color : COLOR0; // �F���
    float4 screenPosTex : TEXCOORD1;
};

// �����̏o�͂������ߍ\���̂ɂ܂Ƃ߂�i�o�͐��SV_TARGETx�Ŏw��j
struct PS_OUT
{
    float4 albedo : SV_Target0; // �A���x�h�i�F�j�o��
    float4 depth : SV_Target1; // �[�x�l
    float4 nomal : SV_Target2; // �[�x�l
};

// �e�N�X�`���̃o�C���h
Texture2D tex : register(t0); // �e�N�X�`���̃��\�[�X�������Ă���
// �T���v���[�ւ̃o�C���h
SamplerState samp : register(s0); // �T���v���[�X�e�[�g�������Ă���

// �s�N�Z���V�F�[�_�̃��C������
// ���_�ň͂܂�Ă���s�N�Z�����������s����
PS_OUT main(PS_IN pin) : SV_TARGET
{
    PS_OUT pout;
    
    // �e�N�X�`������F���擾
    pout.albedo = tex.Sample(samp, pin.uv);
    
    // �X�N���[�����W����[�x�l�擾
    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // �@���x�N�g���𐳋K�����Aw������1�ɐݒ�
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
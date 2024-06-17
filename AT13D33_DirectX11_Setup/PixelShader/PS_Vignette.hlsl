// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
};

Texture2D tex : register(t0); // �x�[�X�̃e�N�X�`��
SamplerState samp : register(s0); // �e�N�X�`���T���v���[

float4 main(PS_IN pin) : SV_TARGET
{
    float2 samplePoint = pin.uv;    // �e�N�X�`���̍��W���擾
    float4 color = tex.Sample(samp, samplePoint); // �e�N�X�`������F���T���v�����O
    
    // �e�N�X�`���̍��W����0.5f�������x�N�g���̒������擾
    float vignette = length(float2(0.5f, 0.5f) - pin.uv);
    // 0~1�Ɏ��܂�悤�ɕϊ�����
    vignette = clamp(vignette - 0.2f, 0, 1);
    // �t�F�ɂ��ĐF��Ԃ�
    color.rgb -= vignette;
    return color;
}
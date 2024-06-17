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
    // �e�N�X�`������F���T���v�����O
    float4 color = tex.Sample(samp, pin.uv);

    // r������g���������炷
    color.r = tex.Sample(samp, pin.uv + 0.002f).r;  // 0.002f���炷
    color.g = tex.Sample(samp, pin.uv - 0.002f).g;  // -0.002f���炷

    return color;
}
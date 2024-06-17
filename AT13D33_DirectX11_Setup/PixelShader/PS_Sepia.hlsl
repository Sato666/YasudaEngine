// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0;    // �e�N�X�`�����W
};

Texture2D tex : register(t0); // �x�[�X�̃e�N�X�`��
SamplerState samp : register(s0); // �e�N�X�`���T���v���[

float4 main(PS_IN pin) : SV_TARGET
{
    // �e�N�X�`������F���T���v�����O
    float4 color = tex.Sample(samp, pin.uv);

    // �Z�s�A�F�ɐF����Z
    color.rgb = color.r * 0.3f + color.g * 0.6f + color.b * 0.1f;
	// �Z�s�A���ۂ�������d�ݕt��
    color.g *= 74.0f / 107.0f;
    color.b *= 43.0f / 107.0f;

    return color;
}
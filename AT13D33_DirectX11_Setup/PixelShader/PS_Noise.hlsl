// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
};

Texture2D tex : register(t0); // �x�[�X�̃e�N�X�`��
SamplerState samp : register(s0); // �e�N�X�`���T���v���[

// 2D�^�������W�F�l���[�^�֐�
float rand(float2 co)
{
	// 2D�̋^�������W�F�l���[�^���g�p���܂��B
	// ���ԕϐ���ǉ����邱�ƂŊe�t���[���ňقȂ錋�ʂ𓾂܂��B
    float time = 0.0f;
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453 + time);
}

float4 main(PS_IN input) : SV_TARGET
{
    // �e�N�X�`������F���T���v�����O
    float4 color = tex.Sample(samp, input.uv);

    // �����𐶐����A�F�ɉ��Z
    float noise = rand(input.uv);
    color.rgb += noise;
    
    // �F��0����1�͈̔͂ɃN�����v
    color.rgb = saturate(color.rgb);

    return color;
}
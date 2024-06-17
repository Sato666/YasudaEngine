// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
};

Texture2D tex : register(t0); // �x�[�X�̃e�N�X�`��
SamplerState samp : register(s0); // �e�N�X�`���T���v���[

// �萔�o�b�t�@
cbuffer Time : register(b0)
{
    float1 time;    // ���ݎ���
};

float4 main(PS_IN pin) : SV_TARGET
{    
    float2 samplePoint = pin.uv; // �e�N�X�`���̍��W���擾
    float4 color = tex.Sample(samp, samplePoint); // �e�N�X�`������F���T���v�����O
    
    // �����I�ɔg�𐶐�
    float sinv = sin(pin.uv.y * 2 + time * -0.1f); // �����g���擾
    float steped = step(0.99f, sinv * sinv); // �傫���ق���Ԃ�
    // �m�C�Y�𑖂点��
    color.rgb -= (1 - steped) * abs(sin(pin.uv.y * 50.0 + time * 1.0)) * 0.05f;
    color.rgb -= (1 - steped) * abs(sin(pin.uv.y * 100.0 - time * 2.0)) * 0.08f;
    // �g�̍����ق������点��
    color.rgb += steped * 0.1f;
    return color;
}
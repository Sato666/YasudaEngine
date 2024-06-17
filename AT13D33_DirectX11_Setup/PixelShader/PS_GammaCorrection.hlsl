// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION;   // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0;      // �e�N�X�`�����W
};

// �萔�o�b�t�@
cbuffer GammaValue : register(b0)
{
    float1 gammavalue;
};

Texture2D tex : register(t0); // �x�[�X�̃e�N�X�`��
SamplerState samp : register(s0); // �e�N�X�`���T���v���[

float4 main(PS_IN pin) : SV_TARGET
{
    // �e�N�X�`������F���T���v�����O
    float4 color = tex.Sample(samp, pin.uv);
    
    // �K���}�␳��K�p
    color.rgb = pow(color.rgb, (1.0f / gammavalue));
    
    return color;
}
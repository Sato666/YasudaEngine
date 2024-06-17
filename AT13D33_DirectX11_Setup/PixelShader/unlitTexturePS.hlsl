// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float4 color : COLOR0; // �F���
};

cbuffer MaterialBuffer : register(b0)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
}

Texture2D tex : register(t0); // �x�[�X�̃e�N�X�`��
SamplerState samp : register(s0); // �e�N�X�`���T���v���[

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 0.0f);
    color = tex.Sample(samp, pin.uv);
    
    if (TextureEnable)
    {
        color = tex.Sample(samp, pin.uv);
        color *= Diffuse;
    }
    else
    {
        color += Diffuse;
        color.a = Diffuse.a;
    }
    
    return color;
}
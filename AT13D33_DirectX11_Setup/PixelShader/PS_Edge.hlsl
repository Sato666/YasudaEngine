// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

// �e�N�X�`�����󂯎��
Texture2D<float4> tex : register(t0);
Texture2D<float4> depth : register(t1);
Texture2D<float4> normal : register(t2);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

float4 main(PS_IN pin) : SV_TARGET
{
    // �e�N�X�`������F���T���v�����O
    float4 color = tex.Sample(samp, pin.uv);
    
    return color;
}
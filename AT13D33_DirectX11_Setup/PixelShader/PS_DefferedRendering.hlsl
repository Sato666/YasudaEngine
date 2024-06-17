struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0; // �@�����
};

// �f�B�t�@�[�h�����_�����O�ɂ�����GBuffer�ɏ������܂ꂽ�������Ƃ�
// �����_�����O�v�Z���s�����߂̃T���v���[�y�уe�N�X�`��
SamplerState samp : register(s0);
Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D worldTex : register(t2);

// ���C�g�̏��
struct Light
{
    float4 color;
    float3 pos;
    float range;
};

// ���C�g�̏��
cbuffer LightBuf : register(b0)
{
    Light lights[30];
};


float4 main(PS_IN pin) : SV_TARGET
{
     // �e�N�X�`������A���x�h�i�F�j�����T���v�����O
    float4 albedo = albedoTex.Sample(samp, pin.uv);
    
     // �e�N�X�`������@���x�N�g�����T���v�����O
    float3 N = normalize(normalTex.Sample(samp, pin.uv).rgb);
    
     // �e�N�X�`�����烏�[���h���W���T���v�����O
    float3 worldPos = worldTex.Sample(samp, pin.uv).rgb;
    
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // �s�N�Z����1�_�ɑ΂��āA���ׂĂ̓_�����Ɩ��邳���v�Z����
    for (int i = 0; i < 30; ++i)
    {
        // �e�s�N�Z������_�����Ɍ������x�N�g�����v�Z
        float3 toLightVec = lights[i].pos - worldPos;
        float3 V = normalize(toLightVec); // ���K�������x�N�g��
        float toLightLen = length(toLightVec); // �����܂ł̋���
        
        // �_�����Ɍ������x�N�g���Ɩ@�����疾�邳���v�Z
        float dotNV = saturate(dot(N, V));
        
        // �����ɉ����Č��̋�����ς���
        // �������߂���Ζ��邭����Ă���ΈÂ��Ȃ�悤�v�Z
        float attenuation = saturate(1.0f - toLightLen / lights[i].range);
        
        // ���R�Ȍ����̌v�Z
        // �����ɉ����Č��̓�����ʂ���Z�ő����Ă���
        attenuation = pow(attenuation, 2.0f);
        
        // �����̓_�����̌������Z���Ă���
        color.rgb += lights[i].color.rgb * dotNV * attenuation;
    }
    // �A���x�h������ɍŏI�I�ȐF�����߂�
    color.rgb *= albedo.rbg;
    color.a = albedo.a;

    // �ŏI�I�ȐF��Ԃ�
    return color;
}
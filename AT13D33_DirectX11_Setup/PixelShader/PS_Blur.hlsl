// ���_�V�F�[�_�[����󂯎�邽�߂̃f�[�^���`
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

cbuffer Param : register(b0)
{
    float2 texSize; // �e�N�X�`���T�C�Y
    float2 blurDir; // �u���[�����������
};

// �e�N�X�`�����󂯎��
Texture2D tex : register(t0);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

float4 main(PS_IN pin) : SV_TARGET
{
    float valume = 1.0f / 5.0f; // 5x5�̃t�B���^�[�l
    float filter[5] =
    {
        valume, valume, valume, valume, valume,
    };
    
    // �t�B���^�[�Ɋ�Â��ĐF���擾
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);  // �o�͐F�̏�����
    float2 uvOffset = 1.0f / texSize;               // 1�s�N�Z����UV��ł̈ړ���
    uvOffset *= blurDir;                            // �w�肳�ꂽ�����݈̂ړ�������
    float2 uv = pin.uv - uvOffset * 2;              // �����ʒu���炸�ꂽUV���W���v�Z
    
    // �t�B���^�[��K�����ĐF���v�Z
    for (int i = 0; i < 5; ++i)
    {
        // �e�N�X�`������F���T���v�����O���ăt�B���^�[��������
        color += tex.Sample(samp, uv) * filter[i];
        uv += uvOffset;
    }
    
    return color;
}
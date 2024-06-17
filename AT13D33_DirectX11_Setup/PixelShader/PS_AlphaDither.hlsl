// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float4 screenPosTex : TEXCOORD1;
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// �����̏o�͂������ߍ\���̂ɂ܂Ƃ߂�i�o�͐��SV_TARGETx�Ŏw��j
struct PS_OUT
{
    float4 albedo : SV_Target0; // �A���x�h�i�F�j�o��
    float4 depth : SV_Target1; // �[�x�l
    float4 nomal : SV_Target2; // �[�x�l
};

// �萔�o�b�t�@
cbuffer Camera : register(b0)
{
    float4 camPos;
};

// �萔�o�b�t�@
cbuffer AlphaLen : register(b1)
{
    float1 alphalen;
};

// �e�N�X�`���̃o�C���h
Texture2D tex : register(t0);
Texture2D dither : register(t1);
// �T���v���[�ւ̃o�C���h
SamplerState samp : register(s0);

// �s�N�Z���V�F�[�_�̃��C������
// ���_�ň͂܂�Ă���s�N�Z�����������s����
PS_OUT main(PS_IN pin) : SV_TARGET
{
    PS_OUT pout;
    
    // �e�N�X�`������F���T���v�����O���ă}�e���A���F����Z����
    pout.albedo = tex.Sample(samp, pin.uv) * pin.color;
    
    // �����ɉ����ē����x��ύX
    float len = length(pin.worldPos.xyz - camPos.xyz);
    float alpha = pout.albedo.a * saturate(len * alphalen);
    
    // �X�N���[���̍��W�ɉ����ăf�B�U�p�^�[����K������ʒu���v�Z
    float2 screenUV = pin.screenPos.xy / pin.screenPos.w;
    screenUV = screenUV * 0.5f + 0.5f;
    screenUV *= float2(1920.0f, 1080.0f) / 4.0f;    // �X�N���[���T�C�Y
    
    float mask = dither.Sample(samp, screenUV).r;
    
    if(alpha < mask)
    {
        // �����x0�̏ꍇ�͊��S�ɕ\�����s��Ȃ�
        discard;
    }

    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // �@���x�N�g���𐳋K�����Aw������1�ɐݒ�
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
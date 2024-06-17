struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float4 worldPos : POSITION0;
};

// �����̏o�͂������ߍ\���̂ɂ܂Ƃ߂�i�o�͐��SV_TARGETx�Ŏw��j
struct PS_OUT
{
    float4 albedo : SV_Target0; // �A���x�h�i�F�j�o��
    float4 nomal : SV_Target1; // �@���x�N�g���o��
    float4 world : SV_Target2; // ���[���h���W�o��
};

// �e�N�X�`�����󂯎��
Texture2D tex : register(t0);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    
    // �A���x�h�i�F�j���e�N�X�`������T���v�����O
    pout.albedo = tex.Sample(samp, pin.uv);
    
    // �@���x�N�g���𐳋K�����Aw������1�ɐݒ�
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    // ���[���h���W���o��
    pout.world = pin.worldPos;
    
    return pout;
}
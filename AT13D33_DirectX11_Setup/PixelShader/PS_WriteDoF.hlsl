struct PS_IN
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    float4 screenPosTex : TEXCOORD1;
};

// �����̏o�͂������ߍ\���̂ɂ܂Ƃ߂�i�o�͐��SV_TARGETx�Ŏw��j
struct PS_OUT
{
    float4 albedo : SV_Target0; // �A���x�h�i�F�j�o��
    float4 depth : SV_Target1; // �[�x�l
    float4 nomal : SV_Target2;// �[�x�l
};

// �e�N�X�`�����󂯎��
Texture2D tex : register(t0);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    
    // �A���x�h�i�F�j���e�N�X�`������T���v�����O
    pout.albedo = tex.Sample(samp, pin.uv);
    
    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // �@���x�N�g���𐳋K�����Aw������1�ɐݒ�
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
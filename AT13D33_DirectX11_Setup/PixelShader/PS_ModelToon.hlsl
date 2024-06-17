// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0; // �@�����
    float4 color : COLOR0; // �F���
    float4 screenPosTex : TEXCOORD1;    // �X�N���[�����W
};

// �����̏o�͂������ߍ\���̂ɂ܂Ƃ߂�i�o�͐��SV_TARGETx�Ŏw��j
struct PS_OUT
{
    float4 albedo : SV_Target0; // �A���x�h�i�F�j�o��
    float4 depth : SV_Target1; // �[�x�l
    float4 nomal : SV_Target2; // �@���}�b�v
};

// ���C�g�̏��
cbuffer Light : register(b0)
{
    float4 lightColor;
    float4 lightAmbient;
    float4 lightDir;
}

// �e�N�X�`�����󂯎��
Texture2D tex : register(t0);
Texture2D<float4> rampTex : register(t1);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

PS_OUT main(PS_IN pin) : SV_TARGET
{
    PS_OUT pout;

    // �@���x�N�g���ƃ��C�g�̕����x�N�g�����擾���A���C�g�̕����𔽓]
    float3 N = normalize(pin.normal);
    float3 L = normalize(lightDir.xyz);
    L = -L;
    
    // �g�U���˂̌v�Z
    float diffuse = saturate(dot(N, L));

#if 1
    // �����v�e�N�X�`�����g�p���Ċg�U���˂�␳
    float2 rampUV = float2(diffuse * 0.98f + 0.01f, 0.5f);
    diffuse = rampTex.Sample(samp, rampUV);
#else
    // diffuse�̒l���K�����i0 ~ 1 �� 0,0.abort,1.0�j
    diffuse *= 2.0f; // ��x���l�͈̔͂��L����
    // �؂�̂Ă̊֌W��A2�ɂȂ�̂�1�̎�����(0.9 * 2 = 1.8, �؂�̂Ă�1�ƂȂ�)
    diffuse += 0.5f;
    // ������؂�̂Ă��l���擾
    diffuse = floor(diffuse);
    // �L�����͈͂Ɠ����̐��l������
    diffuse /= 2.0f;
#endif
    
	// �e�N�X�`���̐F���l�����ĉA��t����
    pout.albedo = tex.Sample(samp, pin.uv) * pin.color;
    pout.albedo.rgb *= (diffuse + lightAmbient.rgb);
    
    // �X�N���[�����W����[�x���擾���Đ[�x�l���o��
    float depth = pin.screenPosTex.z;
    pout.depth = float4(depth, depth, depth, 1.0f);
    
    // �@���x�N�g���𐳋K�����Aw������1�ɐݒ�
    pout.nomal = float4(normalize(pin.normal), 1.0f);
    
    return pout;
}
// ���_�V�F�[�_�ւ̓���
// �K�v�ɍ��킹�ĕύX���Ă���
struct VS_IN
{
    float3 pos : POSITION0; // �Z�}���e�B�N�X
    float3 normal : NORMAL0; // �f�[�^�̗p�r���`����
    float4 color : COLOR0; // �F���
    float2 uv : TEXCOORD0; // �e�N�X�`�����W(UV)
};

// ���_�V�F�[�_����̏o�́��s�N�Z���V�F�[�_�ɏo��
struct VS_OUT
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0; // �@�����    
    float4 color : COLOR0; // �}�e���A�����
};

// �萔�o�b�t�@
cbuffer Matrix0 : register(b0)
{
    float4x4 world; // ���[���h�s��
    float4x4 view; // �r���[�s��
    float4x4 proj; // �v���W�F�N�V�����s��
};

cbuffer MaterialBuffer : register(b1)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
}

// ���_�V�F�[�_�̃��C������
// ���̏������u���_�v���ƂɎ��s����
VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    vout.pos = float4(vin.pos, 1.0f); // ���[�J�����W
    vout.pos = mul(vout.pos, world); // ���[���h���W
        
    vout.pos = mul(vout.pos, view); // �r���[���W
    vout.pos = mul(vout.pos, proj); // �v���W�F�N�V�������W
    
	// �s�N�Z���V�F�[�_�[�ŗ��p����@����n��
	// �@���̓I�u�W�F�N�g�̉�]�ɕ����ĉ�]�����Ȃ��Ƃ������Ȍ����ڂɂȂ�B
	// ���[���h�s��ɂ͈ړ��������܂܂�邪�A�ړ���@���ɓK�p���Ă���������
	// �Ȃ�̂ŁA��]�����݂̂̍s��ƂȂ�悤�ɏk�����Čv�Z����B
    vout.normal = mul(vin.normal, (float3x3) world);
    
    // Material�F�K��
    vout.color.xyz = vin.color.xyz * Diffuse.xyz;
    vout.color.xyz += vin.color.xyz * Ambient.xyz;
    vout.color.xyz += Emission.xyz;
    vout.color.a = vin.color.a * Diffuse.a;

	// ���_�f�[�^��UV���W�����H�����Ƀs�N�Z���V�F�[�_�ւ̏o�̓f�[�^�Ƃ��Đݒ�
    vout.uv = vin.uv;
    
    return vout; // vout���s�N�Z���V�F�[�_�ɕԂ�   
}

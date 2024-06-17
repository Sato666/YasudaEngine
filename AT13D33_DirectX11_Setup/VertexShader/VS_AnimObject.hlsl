// ���_�V�F�[�_�ւ̓���
// �K�v�ɍ��킹�ĕύX���Ă���
struct VS_IN
{
    float3 pos : POSITION0; // �Z�}���e�B�N�X
    float3 normal : NORMAL0; // �f�[�^�̗p�r���`����
    float4 color : COLOR0; // �F���
    float2 uv : TEXCOORD0; // �e�N�X�`�����W(UV)
    int4 BoneIndex : BONEINDEX;
    float4 BoneWeight : BONEWEIGHT;
};

// ���_�V�F�[�_����̏o�́��s�N�Z���V�F�[�_�ɏo��
struct VS_OUT
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0; // �@�����
    float4 color : COLOR0; // �F���
    // �X�N���[�����W
    float4 screenPosTex : TEXCOORD1;
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// �萔�o�b�t�@
cbuffer Matrix0 : register(b0)
{
    float4x4 world; // ���[���h�s��
    float4x4 view; // �r���[�s��
    float4x4 proj; // �v���W�F�N�V�����s��
};

cbuffer BoneMatrixBuffer : register(b1)
{
    matrix BoneMatrix[400];
}

cbuffer MaterialBuffer : register(b2)
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
    
    // �����X�L�����_�u�����h�̏���
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
		// �d�݂��v�Z���Ȃ���s�񐶐�
        comb += BoneMatrix[vin.BoneIndex[i]] * vin.BoneWeight[i];
    }
    
    float4 Pos;
    Pos = mul((float4x4) comb, float4(vin.pos, 1.0f));
    vin.pos = Pos;
    vout.pos = Pos; // ���[�J�����W
    vout.pos = mul(vout.pos, world); // ���[���h���W
    
    // ���[���h���W��n��
    vout.worldPos = vout.pos;
    
    vout.pos = mul(vout.pos, view); // �r���[���W
    vout.pos = mul(vout.pos, proj); // �v���W�F�N�V�������W
    
    // �X�N���[�����W��n��
    vout.screenPosTex = vout.pos;
    vout.screenPos = vout.pos;
    
 	// �s�N�Z���V�F�[�_�[�ŗ��p����@����n��
	// �@���̓I�u�W�F�N�g�̉�]�ɕ����ĉ�]�����Ȃ��Ƃ������Ȍ����ڂɂȂ�B
	// ���[���h�s��ɂ͈ړ��������܂܂�邪�A�ړ���@���ɓK�p���Ă���������
	// �Ȃ�̂ŁA��]�����݂̂̍s��ƂȂ�悤�ɏk�����Čv�Z����B
    vout.normal = mul(vin.normal, (float3x3) world);
    
     // Material�F�K��
    vout.color.xyz = vin.color.xyz * Diffuse.xyz;
    vout.color.xyz = normalize(vout.color.xyz);
    vout.color.xyz = vout.color.xyz + Ambient.xyz;
    vout.color.a = Diffuse.a;

	// ���_�f�[�^��UV���W���s�N�Z���V�F�[�_�ւ̏o�̓f�[�^�Ƃ��Đݒ�
    vout.uv = vin.uv;
    
    return vout; // vout���s�N�Z���V�F�[�_�ɕԂ�   
}

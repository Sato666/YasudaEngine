// �C���v�b�g���C�A�E�g
// CPU����n����钸�_�̏����`��������
struct VS_IN
{
    float3 pos : POSITION0; // �Z�}���e�B�N�X
    float3 normal : NORMAL0; // �f�[�^�̗p�r���`����
    float4 color : COLOR0; // �F���
    float2 uv : TEXCOORD0; // �e�N�X�`�����W(UV)
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�֑���f�[�^�̒�`
struct VS_OUT
{
    float4 pos : SV_POSITION; // �v���W�F�N�V�����ϊ���̒��_���W
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
    float3 normal : NORMAL0; // �@�����

     // �X�N���[�����W
    float4 screenPosTex : TEXCOORD1;
    float4 worldPos : POSITION0;
    float4 screenPos : POSITION1;
};

// �萔�o�b�t�@
// CPU��GPU�ł���肷��A���_�ȊO�̃f�[�^���i�[�����
cbuffer WVP : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// �萔�o�b�t�@
cbuffer OutLine : register(b1)
{
    float1 outline;
};

// �e�N�X�`�����󂯎��
Texture2D tex : register(t0);
Texture2D<float4> rampTex : register(t1);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

VS_OUT main(VS_IN vin)
{    
    VS_OUT vout;

	// ���[�J�����W���X�N���[�����W�֕ϊ�
    vout.pos = float4(vin.pos, 1.0f); // ���[�J�����W
    vout.pos = mul(vout.pos, world); // ���[���h���W
    
    // ���[���h���W��n��
    vout.worldPos = vout.pos;
    
    // �֊s���Ƃ��ĕ\�����邽�߂ɁA�@�������֒��_���ړ�
    // (�P���Ɋg�k�ő傫������ƌ����ڂ����������Ȃ�)
    // ���K�����ꂽ�@����1(���[�g��)�Ŋg�傪�傫���Ȃ肷���邽�ߕ␳������
    vout.pos.xyz += normalize(vin.normal) * outline;
    
    vout.pos = mul(vout.pos, view); // �r���[���W
    vout.pos = mul(vout.pos, proj); // �v���W�F�N�V�������W
    
    // �X�N���[�����W��n��
    vout.screenPosTex = vout.pos;
    vout.screenPos = vout.pos;

	// �s�N�Z���V�F�[�_�[�ŗ��p����UV���W��n��
    vout.uv = vin.uv;

	// �s�N�Z���V�F�[�_�[�ŗ��p����@����n��
	// �@���̓I�u�W�F�N�g�̉�]�ɕ����ĉ�]�����Ȃ��Ƃ������Ȍ����ڂɂȂ�B
	// ���[���h�s��ɂ͈ړ��������܂܂�邪�A�ړ���@���ɓK�p���Ă���������
	// �Ȃ�̂ŁA��]�����݂̂̍s��ƂȂ�悤�ɏk�����Čv�Z����B
    vout.normal = mul(vin.normal, (float3x3) world);

    return vout;
}
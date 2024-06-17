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
    
    float2 tex0 : TEXCOORD0; // �e�N�X�`�����W    
    float4 tex1 : TEXCOORD1;
    float4 tex2 : TEXCOORD2;
    float4 tex3 : TEXCOORD3;
    float4 tex4 : TEXCOORD4;
    float4 tex5 : TEXCOORD5;
    float4 tex6 : TEXCOORD6;
    float4 tex7 : TEXCOORD7;
    float4 tex8 : TEXCOORD8;
};
cbuffer Matrix0 : register(b0)
{
    float4x4 world; // ���[���h�s��
    float4x4 view; // �r���[�s��
    float4x4 proj; // �v���W�F�N�V�����s��
};
cbuffer Param : register(b1)
{
    float2 offset;
    float2 size;
    float2 uvPos;
    float2 uvScale;
    float4 color;
};

// �e�N�X�`�����󂯎��
Texture2D<float4> tex : register(t0);
Texture2D<float4> depth : register(t1);
SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�

VS_OUT main(VS_IN vin)
{
    float2 texSize;
    float level;
	//�e�N�X�`���[�̃T�C�Y���擾����
    tex.GetDimensions(0, texSize.x, texSize.y, level);
    
    VS_OUT vout;
    vout.pos = float4(vin.pos, 1.0f); // ���[�J�����W
    vout.pos.xy *= size;
    vout.pos.xy += offset;
    vout.pos = mul(vout.pos, world);

    vout.pos = mul(vout.pos, view);
    vout.pos = mul(vout.pos, proj);
    
    float2 tex = vin.uv;
    
    float offset = 0.2f;
	//�@��
	{
		//�^�񒆂̃s�N�Z��
        vout.tex0 = tex;

		//�E��̃s�N�Z��
        vout.tex1.xy = tex + float2(offset / texSize.x, -offset / texSize.y);

		//��̃s�N�Z��
        vout.tex2.xy = tex + float2(0.0f, -offset / texSize.y);

		//����̃s�N�Z��
        vout.tex3.xy = tex + float2(-offset / texSize.x, -offset / texSize.y);
		
		//�E�̃s�N�Z��
        vout.tex4.xy = tex + float2(offset / texSize.x, 0.0f);

		//���̃s�N�Z��
        vout.tex5.xy = tex + float2(-offset / texSize.x, 0.0f);

		//�E���̃s�N�Z��
        vout.tex6.xy = tex + float2(offset / texSize.x, offset / texSize.y);

		//���̃s�N�Z��
        vout.tex7.xy = tex + float2(0.0f, offset / texSize.y);

		//�����̃s�N�Z��
        vout.tex8.xy = tex + float2(-offset / texSize.x, offset / texSize.y);
    }

    //�[�x�l
	{
		//�[�x�l�����o���Ƃ��Ɏg��UV���W
        offset = 1.0f;
		//�E��̃s�N�Z��
        vout.tex1.zw = tex + float2(offset / texSize.x, -offset / texSize.y);

		//��̃s�N�Z��
        vout.tex2.zw = tex + float2(0.0f, -offset / texSize.y);

		//����̃s�N�Z��
        vout.tex3.zw = tex + float2(-offset / texSize.x, -offset / texSize.y);

		//�E�̃s�N�Z��
        vout.tex4.zw = tex + float2(offset / texSize.x, 0.0f);

		//���̃s�N�Z��
        vout.tex5.zw = tex + float2(-offset / texSize.x, 0.0f);

		//�E���̃s�N�Z��
        vout.tex6.zw = tex + float2(offset / texSize.x, offset / texSize.y);

		//���̃s�N�Z��
        vout.tex7.zw = tex + float2(0.0f, offset / texSize.y);

		//�����̃s�N�Z��
        vout.tex8.zw = tex + float2(-offset / texSize.x, offset / texSize.y);
    }
    
    return vout;
}
// �s�N�Z���V�F�[�_�ւ̓���
struct PS_IN
{
    float4 pos : SV_POSITION; // �s�N�Z���̈ʒu
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
};

Texture2D tex : register(t0); // �x�[�X�̃e�N�X�`��
SamplerState samp : register(s0); // �e�N�X�`���T���v���[

// RGB�F��HSV�F�ɕϊ�����֐�
float3 RGB2HSV(float3 rgb)
{
    float r = rgb.x;
    float g = rgb.y;
    float b = rgb.z;

    float max = r > g ? r : g;
    max = max > b ? max : b;
    float min = r < g ? r : g;
    min = min < b ? min : b;
    float h = max - min;

    if (h > 0.0f)
    {
        if (max == r)
        {
            h = (g - b) / h;
            if (h < 0.0f)
            {
                h += 6.0f;
            }
        }
        else if (max == g)
        {
            h = 2.0f + (b + r) / h;
        }
        else
        {
            h = 4.0f + (r - g) / h;
        }
    }

    h /= 6.0f;
    float s = (max - min);
    if (max != 0.0f)
    {
        s /= max;
    }

    float v = max;

    return float3(h, s, v);
}

// HSV�F��RGB�F�ɕϊ�����֐�
float3 HSV2RGB(float3 hsv)
{
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;

    float r = v;
    float g = v;
    float b = v;

    if (s > 0.0f)
    {
        h *= 6.0f;
        int i = (int) h;
        float f = h - (float) i;

        switch (i)
        {
            default:
            case 0:
                g *= 1 - s * (1 - f);
                b *= 1 - s;
                break;
            case 1:
                r *= 1 - s * f;
                b *= 1 - s;
                break;
            case 2:
                r *= 1 - s;
                b *= 1 - s * (1 - f);
                break;
            case 3:
                r *= 1 - s;
                g *= 1 - s * f;
                break;
            case 4:
                r *= 1 - s * (1 - f);
                g *= 1 - s;
                break;
            case 5:
                g *= 1 - s;
                b *= 1 - s * f;
                break;
        }
    }

    return float3(r, g, b);
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // �e�N�X�`�����W�̂���F�̎擾�ƒ���
    float2 i_uv = (floor(pin.uv * 100.0f) + 0.5f) / 100.0f;  // 100����
    color = tex.Sample(samp, i_uv);

	// HSV���~���\��
    float3 hsv = RGB2HSV(color.rgb);
    hsv.x = floor(hsv.x * 16.0f) / 16.0f;
    hsv.yz = floor(hsv.yz * 6.0f) / 6.0f;
    color.rgb = HSV2RGB(hsv);

    return color;
}
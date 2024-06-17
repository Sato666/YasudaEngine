//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Light.h"
#include <algorithm>		// std::max std::min�@�p

#include "Geometory.h"

//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void Light::Init()
{
	// �ϐ�������
	m_radXZ = (0.0f);
	m_radY = (DirectX::XM_PIDIV4);
	m_hue = (1.0f);
	m_saturationValue = (1.0f);

	light.Enable = true;
	light.Direction = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	light.Direction.Normalize();
	light.Ambient = DirectX::SimpleMath::Color(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = DirectX::SimpleMath::Color(1.5f, 1.5f, 1.5f, 1.0f);
}
//-----------------------------------------------------------------------------
//      �`�揈���ł�.
//-----------------------------------------------------------------------------
void Light::Draw()
{
	Geometory::SetColor(light.Diffuse);

	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)
	));
	Geometory::SetWorld(mat);
	Geometory::DrawBox();
}
//-----------------------------------------------------------------------------
//      �X�V�����ł�.
//-----------------------------------------------------------------------------
void Light::Update()
{
	// L�{�^���ňړ�
	if (Input::GetKeyPress('L'))
		UpdateParam();
	// R�{�^���ŐF�ω�
	if (Input::GetKeyPress('R'))
		UpdateColorParam();
}
//-----------------------------------------------------------------------------
//      �ړ��X�V�����ł�.
//-----------------------------------------------------------------------------
void Light::UpdateParam()
{
	// ��]
	constexpr float rotSpeed = DirectX::XMConvertToRadians(360.0f / (2.0f * 60)); // 2�b��1��]
	if (Input::GetKeyPress('A')) { m_radXZ += rotSpeed; }
	if (Input::GetKeyPress('D')) { m_radXZ -= rotSpeed; }
	if (Input::GetKeyPress('W')) { m_radY += rotSpeed; }
	if (Input::GetKeyPress('S')) { m_radY -= rotSpeed; }
	m_radY = std::min<float>(std::max<float>(m_radY, 0.0f), DirectX::XM_PIDIV2);

	// ���S�ɉ��悤�Ɍv�Z����
	const float lightLength = 5.0f;	// ����
	light.Direction.x = cosf(m_radY) * sinf(m_radXZ) * lightLength;
	light.Direction.y = -sinf(m_radY) * lightLength;
	light.Direction.z = cosf(m_radY) * cosf(m_radXZ) * lightLength;

	// �t�����̈ʒu��n��
	m_pos.x = -light.Direction.x;
	m_pos.y = -light.Direction.y;
	m_pos.z = -light.Direction.z;
}
//-----------------------------------------------------------------------------
//      �F�ω��X�V�����ł�.
//-----------------------------------------------------------------------------
void Light::UpdateColorParam()
{
	// ������
	light.Ambient = DirectX::SimpleMath::Color(0.2f, 0.2f, 0.2f, 1.0f);

	// �F�ύX
	const float DefHueSpeed = 1.0f;
	const float SVSpeed = 0.01f;
	float HueSpeed = DefHueSpeed;
	if (Input::GetKeyPress(VK_RCONTROL)) { HueSpeed *= 20.0f; }
	if (Input::GetKeyPress(VK_RIGHT)) { m_hue += HueSpeed; }
	if (Input::GetKeyPress(VK_LEFT)) { m_hue -= HueSpeed; }
	if (Input::GetKeyPress(VK_UP)) { m_saturationValue += SVSpeed; }
	if (Input::GetKeyPress(VK_DOWN)) { m_saturationValue -= SVSpeed; }
	if (m_hue < 0.0f) { m_hue += 360.0f; }
	if (m_hue > 360.0f) { m_hue -= 360.0f; }
	m_saturationValue = std::min<float>(std::max<float>(m_saturationValue, 0.0f), 1.0f);
	// �F�ʁE�ʓx�E���x��n��
	light.Diffuse = HSV2Color(m_hue, m_saturationValue);
}
//-----------------------------------------------------------------------------
//      HSV�Z�b�g�����ł�.
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector4 Light::HSV2Color(float h, float sv)
{
	DirectX::SimpleMath::Vector4 color(1.0f, 1.0f, 1.0f, 1.0f);

	h = h / 360.0f;
	sv = sv * 2.0f - 1.0f;
	float s = 1.0f - std::max<float>(sv, 0.0f);
	float v = std::min<float>(sv, 0.0f) + 1.0f;

	float r = v;
	float g = v;
	float b = v;
	if (s > 0.0f)
	{
		h *= 6.0f;
		int i = (int)h;
		float f = h - (float)i;
		switch (i)
		{
		default:
		case 0:
			g *= 1.0f - s * (1.0f - f);
			b *= 1.0f - s;
			break;
		case 1:
			r *= 1.0f - s * f;
			b *= 1.0f - s;
			break;
		case 2:
			r *= 1.0f - s;
			b *= 1.0f - s * (1.0f - f);
			break;
		case 3:
			r *= 1.0f - s;
			g *= 1.0f - s * f;
			break;
		case 4:
			r *= 1.0f - s * (1.0f - f);
			g *= 1.0f - s;
			break;
		case 5:
			g *= 1.0f - s;
			b *= 1.0f - s * f;
			break;
		}
	}

	color.x = r;
	color.y = g;
	color.z = b;
	return color;
}
// ��]�p�̐ݒ�
void Light::SetRot(float xz, float y)
{
	m_radXZ = xz;
	m_radY = y;
}
// XZ���ʊp�̎擾
float Light::GetRotXZ()
{
	return m_radXZ;
}
// Y���p�̎擾
float Light::GetRotY()
{
	return m_radY;
}
// HSV�F��Ԃ̐ݒ�
void Light::SetHSV(float h, float sv)
{
	m_hue = std::min<float>(std::max<float>(h, 0.0f), 360.0f);
	m_saturationValue = std::min<float>(std::max<float>(sv, 0.0f), 1.0f);
	light.Diffuse = HSV2Color(m_hue, m_saturationValue);
}
// Hue(�F��)�擾
float Light::GetH()
{
	return m_hue;
}
// Saturation(�ʓx),Value(���x)�擾
float Light::GetSV()
{
	return m_saturationValue;
}

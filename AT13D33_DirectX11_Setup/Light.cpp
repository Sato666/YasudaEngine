//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Light.h"
#include <algorithm>		// std::max std::min　用

#include "Geometory.h"

//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
void Light::Init()
{
	// 変数初期化
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
//      描画処理です.
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
//      更新処理です.
//-----------------------------------------------------------------------------
void Light::Update()
{
	// Lボタンで移動
	if (Input::GetKeyPress('L'))
		UpdateParam();
	// Rボタンで色変化
	if (Input::GetKeyPress('R'))
		UpdateColorParam();
}
//-----------------------------------------------------------------------------
//      移動更新処理です.
//-----------------------------------------------------------------------------
void Light::UpdateParam()
{
	// 回転
	constexpr float rotSpeed = DirectX::XMConvertToRadians(360.0f / (2.0f * 60)); // 2秒で1回転
	if (Input::GetKeyPress('A')) { m_radXZ += rotSpeed; }
	if (Input::GetKeyPress('D')) { m_radXZ -= rotSpeed; }
	if (Input::GetKeyPress('W')) { m_radY += rotSpeed; }
	if (Input::GetKeyPress('S')) { m_radY -= rotSpeed; }
	m_radY = std::min<float>(std::max<float>(m_radY, 0.0f), DirectX::XM_PIDIV2);

	// 中心に回るように計算する
	const float lightLength = 5.0f;	// 長さ
	light.Direction.x = cosf(m_radY) * sinf(m_radXZ) * lightLength;
	light.Direction.y = -sinf(m_radY) * lightLength;
	light.Direction.z = cosf(m_radY) * cosf(m_radXZ) * lightLength;

	// 逆向きの位置を渡す
	m_pos.x = -light.Direction.x;
	m_pos.y = -light.Direction.y;
	m_pos.z = -light.Direction.z;
}
//-----------------------------------------------------------------------------
//      色変化更新処理です.
//-----------------------------------------------------------------------------
void Light::UpdateColorParam()
{
	// 初期化
	light.Ambient = DirectX::SimpleMath::Color(0.2f, 0.2f, 0.2f, 1.0f);

	// 色変更
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
	// 色彩・彩度・明度を渡す
	light.Diffuse = HSV2Color(m_hue, m_saturationValue);
}
//-----------------------------------------------------------------------------
//      HSVセット処理です.
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
// 回転角の設定
void Light::SetRot(float xz, float y)
{
	m_radXZ = xz;
	m_radY = y;
}
// XZ平面角の取得
float Light::GetRotXZ()
{
	return m_radXZ;
}
// Y軸角の取得
float Light::GetRotY()
{
	return m_radY;
}
// HSV色空間の設定
void Light::SetHSV(float h, float sv)
{
	m_hue = std::min<float>(std::max<float>(h, 0.0f), 360.0f);
	m_saturationValue = std::min<float>(std::max<float>(sv, 0.0f), 1.0f);
	light.Diffuse = HSV2Color(m_hue, m_saturationValue);
}
// Hue(色相)取得
float Light::GetH()
{
	return m_hue;
}
// Saturation(彩度),Value(明度)取得
float Light::GetSV()
{
	return m_saturationValue;
}

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Sprite2D.h"
//-------------------------------------------------------
// オプション関係
#include <SimpleMath.h>
#include "ShaderComponent.h"
#include "SpriteComponent.h"
#include "CMaterial.h"
#include "input.h"

using namespace DirectX::SimpleMath;

void Sprite2D::GameObjInit()
{
	//-------------------------------------------------------
	// テクスチャー読み込み
	AddComponent<ShaderComponent>()->Load();
	m_Sprite = AddComponent<SpriteComponent>();
	m_Sprite->Init(0.0f, 0.0f, 1.0f, 1.0f, "asset\\texture\\TitleLogo.png");

	this->m_Position.x = -0.460f;
	this->m_Position.y = -0.160f;

	this->m_Scale.x = 0.590f;
	this->m_Scale.y = -0.170f;

	material.Diffuse = Color(0.0f, 0.0f, 0.0f, 0.5f);
	material.TextureEnable = false;
}

void Sprite2D::GameObjUpdate()
{
	if (Input::Get()->GetKeyTrigger(VK_SPACE)) { m_State = State::Finish; }

	// ステートごとに処理する
	switch (m_State)
	{
	case State::In:	// フェードイン中（透明度を下げていく）
	{
		m_Alpha -= 2.0f / 60.0f;	// １秒間に２ずつ透明

		if (m_Alpha < 0.0f)	// 完全に透明になったらSTOP状態へ
			m_State = State::Out;

		break;
	}
	case State::Out:// フェードアウト中
	{
		m_Alpha += 2.0f / 60.0f;// １秒間に２ずつ不透明

		if (m_Alpha > 1.0f)// 完全に不透明になったらFINISH状態へ
			m_State = State::In;

		break;
	}
	case State::Finish:// フェードアウト終了を表す
		m_Alpha = 0.0f;
		break;

	default:
		break;
	}

	material.Diffuse = Color(0.0f, 0.0f, 0.0f, m_Alpha);
	material.TextureEnable = false;
}

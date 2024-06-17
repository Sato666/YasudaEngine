//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "transition.h"
//-------------------------------------------------------
// オプション関係
#include <SimpleMath.h>
#include "main.h"
#include "renderer.h"
#include "ShaderComponent.h"
#include "SpriteComponent.h"

#include "CMaterial.h"

using namespace DirectX::SimpleMath;
//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
void Transition::GameObjInit()
{
	//-------------------------------------------------------
	// テクスチャー読み込み
	AddComponent<ShaderComponent>()->Load();
	m_Sprite = AddComponent<SpriteComponent>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\title.jpg");

	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, 1.0f);
	material.TextureEnable = false;
}
//-----------------------------------------------------------------------------
//      更新処理です.
//-----------------------------------------------------------------------------
void Transition::GameObjUpdate()
{
	// ステートごとに処理する
	switch (m_State)
	{

	case State::Stop:	// フェードイン終了を表す
		break;

	case State::In:	// フェードイン中（透明度を下げていく）
	{
		m_Alpha -= 2.0f / 60.0f;	// １秒間に２ずつ透明

		if (m_Alpha < 0.0f)	// 完全に透明になったらSTOP状態へ
			m_State = State::Stop;

		break;
	}
	case State::Out:// フェードアウト中
	{
		m_Alpha += 2.0f / 60.0f;// １秒間に２ずつ不透明

		if (m_Alpha > 1.0f)// 完全に不透明になったらFINISH状態へ
			m_State = State::Finish;

		break;
	}

	case State::Finish:// フェードアウト終了を表す
		break;

	default:
		break;
	}

	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, m_Alpha);
	material.TextureEnable = false;
}

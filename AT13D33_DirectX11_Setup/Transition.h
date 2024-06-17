//-------------------------------------------------------------------------------------
// Class : Transition
// 親Class   :   GameObject
// 
// フェードの情報を持っているクラス
// フェードインフェードアウトの機能を持つ
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "gameObject.h"

class Transition : public GameObject
{
public:
	enum class State
	{
		Stop,	// フェードイン終了
		In,	// フェードイン
		Out,	// フェードアウト
		Finish	// フェードアウト終了
	};
private:
	float m_Alpha = 0.0f;
	State m_State = State::Stop;
	class SpriteComponent* m_Sprite{};
public:

	void GameObjInit() override;
	void GameObjUpdate() override;

	State GetState() { return m_State; }

	void FadeIn()			// フェードイン初期処理			
	{
		m_Alpha = 1.0f;
		m_State = State::In;
	}

	void FadeOut() {		 // フェードアウト初期処理	
		m_Alpha = 0.0f;
		m_State = State::Out;
	}

};




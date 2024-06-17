//-------------------------------------------------------------------------------------
// Class : Transition
// �eClass   :   GameObject
// 
// �t�F�[�h�̏��������Ă���N���X
// �t�F�[�h�C���t�F�[�h�A�E�g�̋@�\������
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
		Stop,	// �t�F�[�h�C���I��
		In,	// �t�F�[�h�C��
		Out,	// �t�F�[�h�A�E�g
		Finish	// �t�F�[�h�A�E�g�I��
	};
private:
	float m_Alpha = 0.0f;
	State m_State = State::Stop;
	class SpriteComponent* m_Sprite{};
public:

	void GameObjInit() override;
	void GameObjUpdate() override;

	State GetState() { return m_State; }

	void FadeIn()			// �t�F�[�h�C����������			
	{
		m_Alpha = 1.0f;
		m_State = State::In;
	}

	void FadeOut() {		 // �t�F�[�h�A�E�g��������	
		m_Alpha = 0.0f;
		m_State = State::Out;
	}

};




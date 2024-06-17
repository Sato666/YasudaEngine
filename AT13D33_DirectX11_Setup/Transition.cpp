//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "transition.h"
//-------------------------------------------------------
// �I�v�V�����֌W
#include <SimpleMath.h>
#include "main.h"
#include "renderer.h"
#include "ShaderComponent.h"
#include "SpriteComponent.h"

#include "CMaterial.h"

using namespace DirectX::SimpleMath;
//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void Transition::GameObjInit()
{
	//-------------------------------------------------------
	// �e�N�X�`���[�ǂݍ���
	AddComponent<ShaderComponent>()->Load();
	m_Sprite = AddComponent<SpriteComponent>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\title.jpg");

	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, 1.0f);
	material.TextureEnable = false;
}
//-----------------------------------------------------------------------------
//      �X�V�����ł�.
//-----------------------------------------------------------------------------
void Transition::GameObjUpdate()
{
	// �X�e�[�g���Ƃɏ�������
	switch (m_State)
	{

	case State::Stop:	// �t�F�[�h�C���I����\��
		break;

	case State::In:	// �t�F�[�h�C�����i�����x�������Ă����j
	{
		m_Alpha -= 2.0f / 60.0f;	// �P�b�ԂɂQ������

		if (m_Alpha < 0.0f)	// ���S�ɓ����ɂȂ�����STOP��Ԃ�
			m_State = State::Stop;

		break;
	}
	case State::Out:// �t�F�[�h�A�E�g��
	{
		m_Alpha += 2.0f / 60.0f;// �P�b�ԂɂQ���s����

		if (m_Alpha > 1.0f)// ���S�ɕs�����ɂȂ�����FINISH��Ԃ�
			m_State = State::Finish;

		break;
	}

	case State::Finish:// �t�F�[�h�A�E�g�I����\��
		break;

	default:
		break;
	}

	MATERIAL material{};
	material.Diffuse = Color(0.0f, 0.0f, 0.0f, m_Alpha);
	material.TextureEnable = false;
}

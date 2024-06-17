//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Sprite2D.h"
//-------------------------------------------------------
// �I�v�V�����֌W
#include <SimpleMath.h>
#include "ShaderComponent.h"
#include "SpriteComponent.h"
#include "CMaterial.h"
#include "input.h"

using namespace DirectX::SimpleMath;

void Sprite2D::GameObjInit()
{
	//-------------------------------------------------------
	// �e�N�X�`���[�ǂݍ���
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

	// �X�e�[�g���Ƃɏ�������
	switch (m_State)
	{
	case State::In:	// �t�F�[�h�C�����i�����x�������Ă����j
	{
		m_Alpha -= 2.0f / 60.0f;	// �P�b�ԂɂQ������

		if (m_Alpha < 0.0f)	// ���S�ɓ����ɂȂ�����STOP��Ԃ�
			m_State = State::Out;

		break;
	}
	case State::Out:// �t�F�[�h�A�E�g��
	{
		m_Alpha += 2.0f / 60.0f;// �P�b�ԂɂQ���s����

		if (m_Alpha > 1.0f)// ���S�ɕs�����ɂȂ�����FINISH��Ԃ�
			m_State = State::In;

		break;
	}
	case State::Finish:// �t�F�[�h�A�E�g�I����\��
		m_Alpha = 0.0f;
		break;

	default:
		break;
	}

	material.Diffuse = Color(0.0f, 0.0f, 0.0f, m_Alpha);
	material.TextureEnable = false;
}

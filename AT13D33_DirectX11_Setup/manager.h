//-------------------------------------------------------------------------------------
// Class : Manager
// 
// ���ׂẴV�[���ɕK�v�ȋ@�\�̏�������������s��
// �V�[���@�\���Ǘ������݂̃V�[����n���@�\������
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <cinttypes>
#include	"SceneMain.h"

// �O���Q��
class Application;

class Manager
{
private:
	static std::shared_ptr<SceneMain> m_pScene;
public:
	static void Init(Application* ap);
	static void Uninit();
	static void Update(uint64_t d);
	static void Draw(uint64_t d);

	// �V�[�����Q�b�g����
	static std::shared_ptr<SceneMain> GetScene()
	{
		return m_pScene;
	}

private:
	// �S�V�[�����ʂ�ImGui�̕`��
	static void MyImGui();

private:
};
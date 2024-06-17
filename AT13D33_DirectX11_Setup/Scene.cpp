//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Scene.hpp"

Scene::Objects Scene::m_objects;

Scene::Scene()
	: m_pParent(nullptr)
	, m_pSubScene(nullptr)
{
}
Scene::~Scene()
{
	// �T�u�V�[�����폜
	RemoveSubScene();

	// �폜
	Items::iterator it = m_items.begin();
	while (it != m_items.end())
	{
		m_objects.erase(*it);
		++it;
	}
	m_items.clear();

	// �e�̎Q�Ƃ��폜
	if (m_pParent)
		m_pParent->m_pSubScene = nullptr;
}
/// <summary>
/// �T�u�V�[�����l�������X�V����
/// </summary>
/// <param name="tick">����</param>
void Scene::_update(float tick)
{
	if (m_pSubScene)
		m_pSubScene->_update(tick);
	Update(tick);
}
/// <summary>
/// �T�u�V�[�����l�������`�揈��
/// </summary>
void Scene::_draw()
{
	if (m_pSubScene)
		m_pSubScene->_draw();
	Draw();
}
/// <summary>
/// �T�u�V�[�����l������ImGui�`�揈��
/// </summary>
void Scene::_imgui()
{
	if (m_pSubScene)
		m_pSubScene->_imgui();
	ImGui();
}

/// <summary>
/// @brief �T�u�V�[���̍폜
/// </summary>
void Scene::RemoveSubScene()
{
	// �폜����T�u�V�[�������݂��邩
	if (!m_pSubScene) return;

	// �K�w���̃T�u�V�[����D�悵�č폜
	m_pSubScene->RemoveSubScene();

	// �����̃T�u�V�[�����폜
	m_pSubScene->Uninit();
	delete m_pSubScene;
	m_pSubScene = nullptr;
}

/// <summary>
/// ���������I�u�W�F�N�g�̍폜
/// </summary>
/// <param name="name">���O����</param>
void Scene::DestroyObj(const char* name)
{
	m_objects.erase(name);
	m_items.remove(name);
}
//-------------------------------------------------------------------------------------
// Class : Scene
// 
// �V�[�����Ǘ�����N���X
// �T�u�V�[���̍쐬�ƃV�[�����ŃI�u�W�F�N�g�̐����̋@�\������
// 
// Yasuda.Aoto  2023.11.09
//-------------------------------------------------------------------------------------

#ifndef __SCENE_HPP__
#define __SCENE_HPP__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <memory>
#include <map>
#include <string>
#include <list>
#include <Windows.h>
#include <array>

#include "Geometory.h"

// GameObject
#include "gameobject.h"

// resource
#include "Sprite.h"
#include "Shader.h"

// �J����
#include "camera.h"
#include "DXCamera.h"

// light
#include "MoveLight.h"
#include "Light.h"

// �V�F�[�_�[�n
#include "ShaderComponent.h"

// ���f���n
#include "Model.h"
#include "ModelMeshLoad.h"
#include "ModelMeshRenderer.h"

#include "AnimModel.h"
#include "AnimModelMeshLoad.h"
#include "AnimModelMeshRenderer.h"

#include "ModelSetting.h"

// ���͏��
#include "Input.h"

// ImGui
#include "imgui/imgui.h"

// �I�u�W�F�N�g�̒ǉ��p���N���X
class SceneObjectBase
{
public:
	virtual ~SceneObjectBase() {}
};

// ���\�[�X�̏��L�����L�N���X
template<class T>
class SceneObject : public SceneObjectBase
{
public:
	SceneObject(std::shared_ptr<T> ptr) : m_pObj(ptr) {}
	virtual ~SceneObject() {}
	std::shared_ptr<T> m_pObj;
};

// �V�[����{�N���X
class Scene
{
private:
	using Objects = std::map<std::string, std::shared_ptr<SceneObjectBase>>;
	using Items = std::list<std::string>;
private:
	static Objects m_objects;		// �I�u�W�F�N�g���X�g�i���O�ƃN���X�|�C���^�ŕۑ��j
protected:
	Scene* m_pParent;			// ���g�̃V�[��
	Scene* m_pSubScene;			// �T�u�V�[��
	Items m_items;				// �A�C�e�����X�g�i���O�ŕۑ��j

	std::vector<GameObject*> ObjeList;	// ���������I�u�W�F�N�g�̃��X�g
public:
	Scene();
	virtual ~Scene();
	void _update(float tick);
	void _draw();
	void _imgui();

	// �V�[������֐�
	template<class T> T* AddSubScene();
	void RemoveSubScene();

	// �I�u�W�F�N�g����֐�
	template<class T> T* CreateObj(const char* name);
	void DestroyObj(const char* name);
	template<class T> T* GetObj(const char* name);

	// �p���V�[���̈�ʂ�̏���
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update(float tick) {};
	virtual void Draw() {};
	virtual void ImGui() {};

	// �V�[���ɓo�^����Ă���I�u�W�F�N�g�̏���������
	void InitBase()
	{
		Init();
	}

	// �V�[���ɓo�^����Ă���I�u�W�F�N�g�̉������
	void UninitBase()
	{
		Uninit();
	}

	// �V�[���ɓo�^����Ă���I�u�W�F�N�g�̍X�V����
	void UpdateBase(float tick)
	{
		_update(tick);
	}

	// �V�[���ɓo�^����Ă���I�u�W�F�N�g�̕`�揈��
	void DrawBase()
	{
		_draw();
	}

	// �V�[���ɓo�^����Ă���I�u�W�F�N�g��ImGut�`�揈��
	void ImGuiBase()
	{
		_imgui();
	}
};

/// <summary>
/// �T�u�V�[���̒ǉ�
/// </summary>
/// <typeparam name="T">�T�u�V�[���̌^</typeparam>
/// <returns>���������T�u�V�[��</returns>
template<class T> T* Scene::AddSubScene()
{
	RemoveSubScene();
	T* pScene = new T;
	m_pSubScene = pScene;
	pScene->m_pParent = this;
	pScene->Init();
	return pScene;
}

/// <summary>
/// �I�u�W�F�N�g�̐���
/// </summary>
/// <typeparam name="T">�I�u�W�F�N�g�̌^</typeparam>
/// <param name="name">�I�u�W�F�N�g�̖���</param>
/// <returns>���������I�u�W�F�N�g</returns>
template<class T> T* Scene::CreateObj(const char* name)
{
#ifdef _DEBUG
	// �f�o�b�O���̂݁A���̃_�u�肪�Ȃ����`�F�b�N
	Objects::iterator it = m_objects.find(name);
	if (it != m_objects.end()) {
		static char buf[256];
		sprintf_s(buf, sizeof(buf), "Failed to create object. %s", name);
		MessageBox(NULL, buf, "Error", MB_OK);
		return nullptr;
	}
#endif // _DEBUG

	// �I�u�W�F�N�g����
	std::shared_ptr<T> ptr = std::make_shared<T>();
	m_objects.insert(std::pair<std::string, std::shared_ptr<SceneObjectBase>>(name, std::make_shared<SceneObject<T>>(ptr)));
	m_items.push_back(name);
	return ptr.get();
}

/// <summary>
/// �I�u�W�F�N�g�̎擾
/// </summary>
/// <typeparam name="T">�I�u�W�F�N�g�̌^</typeparam>
/// <param name="name">�I�u�W�F�N�g�̖���</param>
/// <returns>�擾�����I�u�W�F�N�g</returns>
template<class T> T* Scene::GetObj(const char* name)
{
	// �I�u�W�F�N�g�̒T��
	Objects::iterator it = m_objects.find(name);		// ���O�Ō���
	if (it == m_objects.end()) return nullptr;			// �Ȃ���΂�����nullptr�Ԃ�

	// �^�ϊ�
	std::shared_ptr<SceneObject<T>> ptr = std::reinterpret_pointer_cast<SceneObject<T>>(it->second);
	return ptr->m_pObj.get();							// �I�u�W�F�N�g�̃|�C���^�Ԃ�
}

#endif // __SCENE_BASE_HPP__
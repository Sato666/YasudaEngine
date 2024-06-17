//-------------------------------------------------------------------------------------
// Class : GameObject 
// 
// �e�Q�[���I�u�W�F�N�g�N���X
// �ʒu�E�p�x�E�T�C�Y���Z�b�g�܂��́A�Ԃ��@�\������
// �O�����x�N�g����Ԃ��@�\������
// ���g�������@�\������
// ����Class���p�������q�N���X�ɃR���|�[�l���g��ǉ��܂��͕Ԃ��@�\������
// ����Class���p�������q�N���X�Ɏq�I�u�W�F�N�g��ǉ�����@�\������
// ���ꂼ��ǉ������R���|�[�l���g�N���X���ꊇ�ɏ������E����E�X�V�E�`�悷��@�\������
// 
// �X�V�����̏��ԁi07.17)
// ���q�I�u�W�F�N�g
// ���R���|�[�l���g
// �����g
// 
// Yasuda.Aoto  2023.07.17
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "renderer.h"

#include <list>
#include <SimpleMath.h>

#include "component.h"
#include "Shader.h"

class GameObject
{
protected:
	bool		m_Destroy = false;	// ����t���O
	bool		m_Active = true;	// �`��t���O
	bool		m_isinfrustum = true;		// �t���X�^�������ǂ���

	std::string m_ObjectName;		// �I�u�W�F�N�g�̖��O
	std::string m_ID;				// �I�u�W�F�N�g��ID

	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);	// �ʒu
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);	// �p�x
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);		// �T�C�Y

	std::list<Component*> m_Component;			// �R���|�[�l���g�N���X�i�[
	std::list<GameObject*> m_ChildGameObject;	// �q�I�u�W�F�N�g�i�[
public:
	GameObject() {}//�R���X�g���N�^
	virtual ~GameObject() {}//�f�X�g���N�^�i���z�֐��j

	/// <summary>
	/// ���O���擾
	/// </summary>
	/// <returns>string�^</returns>
	std::string GetName() { return m_ObjectName; }
	/// <summary>
	/// ID���擾
	/// </summary>
	/// <returns>string�^</returns>
	std::string GetID() { return m_ID; }

	// �ʒu�E�p�x�E�T�C�Y�E�F��Ԃ��֐�
	/// <summary>
	/// �ʒu��Ԃ�
	/// </summary>
	/// <returns>Vector3�^</returns>
	DirectX::SimpleMath::Vector3 GetPosition() { return m_Position; }		// �ʒu
	/// <summary>
	/// �p�x��Ԃ�
	/// </summary>
	/// <returns>Vector3�^</returns>
	DirectX::SimpleMath::Vector3 GetRotation() { return m_Rotation; }		// �p�x
	/// <summary>
	/// �T�C�Y��Ԃ�
	/// </summary>
	/// <returns>Vector3�^</returns>
	DirectX::SimpleMath::Vector3 GetScale() { return m_Scale; }				// �T�C�Y

	// ���O���Z�b�g
	void SetName(std::string _name) { m_ObjectName = _name; }
	// ID���Z�b�g
	void SetID(std::string _id) { m_ID = _id; }

	// �ʒu�E�p�x�E�T�C�Y�E�F���Z�b�g����֐�
	/// <summary>
	/// �ʒu���Z�b�g
	/// </summary>
	/// <param name="_position">Vector3�^</param>
	void SetPosition(DirectX::SimpleMath::Vector3 _position) { m_Position = _position; }		// �ʒu
	/// <summary>
	/// �p�x���Z�b�g
	/// </summary>
	/// <param name="_rotation">Vector3�^</param>
	void SetRotation(DirectX::SimpleMath::Vector3 _rotation) { m_Rotation = _rotation; }		// �p�x
	/// <summary>
	/// �T�C�Y���Z�b�g
	/// </summary>
	/// <param name="_scale">Vector3�^</param>
	void SetScale(DirectX::SimpleMath::Vector3 _scale) { m_Scale = _scale; }					// �T�C�Y

	/// <summary>
	/// �O�����x�N�g����Ԃ��֐�
	/// </summary>
	/// <returns>Vector3�^</returns>
	DirectX::SimpleMath::Vector3 GetForward()
	{
		DirectX::SimpleMath::Matrix rot;
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

		DirectX::SimpleMath::Vector3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	// ���M���������֐�
	void SetDestroy() { m_Destroy = true; }
	void SetActive(bool _is) { m_Active = _is; }

	bool Destroy()
	{
		// �������Ȃ�
		if (m_Destroy)
		{
			Uninit();	// ���
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual void GameObjInit() {}		// ������
	virtual void GameObjUninit() {}		// ���
	virtual void GameObjUpdate() {}		// �X�V
	virtual void GameObjDraw() {}		// �`��
	virtual void PreDraw() {}

	/// <summary>
	/// �R���|�[�l���g��ǉ�����֐�
	/// </summary>
	/// <typeparam name="T">�R���|�[�l���g�̃N���X</typeparam>
	/// <returns>�R���|�[�l���g�̃N���X</returns>
	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		m_Component.push_back(component);
		((Component*)component)->Init();	// �R���|�[�l���g�̏�����

		return component;
	}
	
	/// <summary>
	/// �R���|�[�l���g��Ԃ��֐�
	/// </summary>
	/// <returns>�ǂ����v���Ȃ����Null��Ԃ�</returns>
	template <typename T>
	T* GetComponent()
	{
		for (Component* component : m_Component)
		{
			// �ǉ�����Ă���R���|�[�l���g�̖��O���ꏏ�Ȃ�
			if (typeid(*component) == typeid(T))
			{
				return (T*)component;	// �Ԃ�
			}
		}
		return nullptr;		// �ǂ����v���Ȃ����Null��Ԃ�
	}

	template <typename T>
	T* DeleteComponent()
	{
		for (Component* component : m_Component)
		{
			// �ǉ�����Ă���R���|�[�l���g�̖��O���ꏏ�Ȃ�
			if (typeid(*component) == typeid(T))
			{
				m_Component.pop_back(component);
				return true;	// ����
			}
		}
		return false;		// ���s
	}

	/// <summary>
	/// �q���I�u�W�F�N�g��ǉ�����
	/// </summary>
	template <typename T>
	T* AddChild()
	{
		T* child = new T();
		m_ChildGameObject.push_back(child);
		child->InitBase();	// ����������

		return child;
	}

	// �������x�[�X
	void Init()
	{
		GameObjInit();
	}

	// ����x�[�X
	void Uninit()
	{
		// �q�I�u�W�F�N�g�̉��
		for (GameObject* child : m_ChildGameObject)
		{
			child->Uninit();
			delete child;
		}
		m_ChildGameObject.clear();//���X�g�̃N���A

		// �e�I�u�W�F�N�g�̉��
		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();//���X�g�̃N���A

		GameObjUninit();
	}

	// �X�V�x�[�X
	void Update()
	{
		// �q�I�u�W�F�N�g�̍X�V
		for (GameObject* child : m_ChildGameObject)
		{
			child->Update();
		}
		// �e�I�u�W�F�N�g�̍X�V
		for (Component* component : m_Component)
		{
			component->Update();
		}
		// ���g�̍X�V
		GameObjUpdate();
	}

	void DrawChildBase(DirectX::SimpleMath::Matrix ParentMatrix)
	{
		// �}�g���N�X�ݒ�
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans * ParentMatrix;

		PreDraw();

		// �q�I�u�W�F�N�g�̕`��
		for (GameObject* child : m_ChildGameObject)
		{
			child->DrawChildBase(world);
		}

		// �e�I�u�W�F�N�g�̕`��
		for (Component* component : m_Component)
		{
			// �A�N�e�B�u��ԂȂ�
			if (m_Active == true)
			{
				component->Draw();
			}
		}

		GameObjDraw();
	}

	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	/// <param name="_setvsshader">�Z�b�g����Ă���VSShader</param>
	/// <param name="_mat">�ϊ��s��</param>
	void Draw(Shader* _setvsshader, DirectX::XMFLOAT4X4 _mat[3])
	{
		// �P�ʍs��ɂ���
		DirectX::XMFLOAT4X4 matrix;
		DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixIdentity());

		// �}�g���N�X�ݒ�
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot * trans;		// ���[���h�s��̍���n�̌v�Z

		PreDraw();

		// �]�u����
		world = world.Transpose();			// ����n�ɍs������ւ���

		_mat[0] = world;	// ���[���h�����X�V
		_setvsshader->WriteBuffer(0, _mat);

		// �e�I�u�W�F�N�g�̕`��
		for (Component* component : m_Component)
		{
			// �A�N�e�B�u��ԂȂ�
			if (m_Active == true)
			{
				component->Draw();
			}
		}

		GameObjDraw();
	}

	/// <summary>
	/// ���[���h�ϊ������̕`��
	/// </summary>
	void DefaultDraw()
	{
		// �e�I�u�W�F�N�g�̕`��
		for (Component* component : m_Component)
		{
			// �A�N�e�B�u��ԂȂ�
			if (m_Active == true)
			{
				component->Draw();
			}
		}

		GameObjDraw();
	}

	void SetInFrustumn(bool flag) {
		m_isinfrustum = flag;
	}

	bool IsInFrustumn() {
		return m_isinfrustum;
	}
};
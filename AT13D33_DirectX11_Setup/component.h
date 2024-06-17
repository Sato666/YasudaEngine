//-------------------------------------------------------------------------------------
// Class : Component 
// 
// �e�R���|�[�l���g�N���X
// ����Class���p�������q�N���X���ꊇ�ŊǗ�����
// 
// Yasuda.Aoto  2023.07.11
//-------------------------------------------------------------------------------------
#pragma once

class Component
{

protected:
	class  GameObject* m_GameObject = nullptr;

public:
	Component() = delete;
	Component(GameObject* Object) { m_GameObject = Object; }
	virtual ~Component() {}

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

};
//-------------------------------------------------------------------------------------
// Class : ShaderComponent
// �eClass   :   Component
// 
// �V�F�[�_�[�̃R���|�[�l���g�N���X
// �V�F�[�_�[�̏���^����
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "component.h"
#include <d3d11.h>
#include <memory>
#include "Shader.h"

class ShaderComponent : public Component
{
private:
	static std::shared_ptr<VertexShader> m_defVS;
	static std::shared_ptr<PixelShader> m_defPS;

	VertexShader* m_pVS;	// ���݂̒��_�V�F�[�_�[
	PixelShader* m_pPS;		// ���݂̃s�N�Z���V�F�[�_�[

	VertexShader* m_pOldVS;	// �ۑ��p���_�V�F�[�_�[
	PixelShader* m_pOldPS;	// �ۑ��p�s�N�Z���V�F�[�_�[
public:
	using Component::Component;

	void Load();
	void Draw() override;
	void Uninit() override;
	void MakeDefaultShader();

	/// <summary>
	/// VS�V�F�[�_�[���Z�b�g����
	/// </summary>
	/// <param name="vs">�Z�b�g������VS�V�F�[�_�[</param>
	void SetVSShader(Shader* _vs);
	/// <summary>
	/// PS�V�F�[�_�[���Z�b�g����
	/// </summary>
	/// <param name="ps">�Z�b�g������PS�V�F�[�_�[</param>
	void SetPSShader(Shader* _ps);

	/// <summary>
	/// VS�V�F�[�_�[���擾����
	/// </summary>
	/// <returns>VertexShader</returns>
	Shader* GetVSShader() { return m_pVS; }

	/// <summary>
	/// PS�V�F�[�_�[���擾����
	/// </summary>
	/// <returns>PixelShader</returns>
	Shader* GetPSShader() { return m_pPS; }

	/// <summary>
	/// �O��VS�V�F�[�_�[��ۑ����Ă���
	/// </summary>
	/// <param name="_oldps">�ۑ�������VS�V�F�[�_�[</param>
	void SetOldVSShader(Shader* _oldvs)
	{
		VertexShader* cast = reinterpret_cast<VertexShader*>(_oldvs);
		if (cast)
			m_pOldVS = cast;
	}
	/// <summary>
	/// �O�̕ۑ����ꂽVS�V�F�[�_�[���擾����
	/// </summary>
	/// <returns>VertexShader</returns>
	Shader* GetOldVSShader() { return m_pOldVS; }

	/// <summary>
	/// �O��PS�V�F�[�_�[��ۑ����Ă���
	/// </summary>
	/// <param name="_oldps">�ۑ�������PS�V�F�[�_�[</param>
	void SetOldPSShader(Shader* _oldps)
	{
		PixelShader* cast = reinterpret_cast<PixelShader*>(_oldps);
		if (cast)
			m_pOldPS = cast;
	}
	/// <summary>
	/// �O�̕ۑ����ꂽPS�V�F�[�_�[���擾����
	/// </summary>
	/// <returns>PixelShader</returns>
	Shader* GetOldPSShader() { return m_pOldPS; }

	/// <summary>
	/// �Z�b�g����Ă���PS�V�F�[�_�[���ꏏ����Ԃ�
	/// </summary>
	/// <param name="_nowps">��r����PS�V�F�[�_�[</param>
	/// <returns>bool�^</returns>
	bool ComparisonPSShader(Shader* _nowps) {

		PixelShader* cast = reinterpret_cast<PixelShader*>(_nowps);
		if (m_pPS == cast)
		{
			return true;
		}

		return false;
	}
};
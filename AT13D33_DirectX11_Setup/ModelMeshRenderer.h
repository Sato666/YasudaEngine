//-------------------------------------------------------------------------------------
// Class : ModelMeshRenderer
// �eClass   :   Component
// 
// ModelMeshLoad�N���X���烂�f���̏����󂯎��`�悷��N���X
// ���f���̕`��@�\������ 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<vector>
//-----------------------------------------------------------------------------
// ���̑�
#include "renderer.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "ModelMeshLoad.h"
#include "component.h"

class ModelMeshRenderer :
	public Component
{
public:
	using Component::Component;

	void Draw() override;				// �󂯎������񂩂�`��
	void Init(ModelMeshLoad& mesh);		// ���[�h�������f���̏����󂯎��
	void Uninit() override;				// �s�K�v�ɂȂ����f�[�^�J��

	void SetMaterialShader(bool _is);		// �}�e���A���Z�b�g���邩�ǂ����̃Z�b�^�[

	std::vector<ID3D11ShaderResourceView*> GetDiffuseRTV() { return m_DiffuseSRVtable; }	// �f�B�t���[�Y�e�N�X�`��SRV��Ԃ�
	std::vector<SUBSET> GetSubSet() { return m_Subsets; }									// �T�u�Z�b�g��Ԃ�
	std::vector<CMaterial*> GetMaterial() { return m_Materials; }							// �}�e���A���Q��Ԃ�
private:
	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;		// �f�B�t���[�Y�e�N�X�`��SRV
	std::vector<SUBSET>	m_Subsets;					// �T�u�Z�b�g���

	std::vector<CMaterial*>		m_Materials;		// �}�e���A���Q
	CVertexBuffer<MODELVERTEX>	m_VertexBuffer;		// ���_�o�b�t�@
	CIndexBuffer				m_IndexBuffer;		// �C���f�b�N�X�o�b�t�@

	bool m_isMaterial = false;	// �}�e���A������^���邩�ǂ���
};




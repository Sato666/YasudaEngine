//-------------------------------------------------------------------------------------
// Class : ModelMeshLoad
// �eClass   :   Component
// 
// Assimp5.3.0���g���ă��f�������[�h����R���|�[�l���g
// ���[�h��������Ԃ��@�\������
// 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<vector>
#include	<string>
#include	<unordered_map>
#include	<WICTextureLoader.h>
//-----------------------------------------------------------------------------
// Assimp
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

// debug��release��Assimp���C�u������ύX����
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"assimp-vc143-mtd.lib")
#else
#pragma comment (lib, "assimp-vc143-mt.lib")
#endif//defined(DEBUG) || defined(_DEBUG)
//-----------------------------------------------------------------------------
// ���̑�
#include "component.h"
#include "CIndexBuffer.h"
#include "CMaterial.h"
#include "CVertexBuffer.h"

// COMPTR���g����悤��
using Microsoft::WRL::ComPtr;

// �R�c���_�f�[�^
struct MODELVERTEX
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};

class ModelMeshLoad :
    public Component
{
public:
	using Component::Component;
private:
	const aiScene* m_AiScene = nullptr;			// assimp scene

	std::vector<MODELVERTEX>	m_vertices;		// ���_���W�Q
	std::vector<unsigned int>	m_indices;		// �C���f�b�N�X�f�[�^�Q
	std::vector<SUBSET>			m_subsets;		// �T�u�Z�b�g�Q
	std::vector<MATERIAL>		m_materials;	// �}�e���A���e�[�u��

	// ���b�V���Ɋ܂܂�钸�_�����o��
	unsigned int GetVertices(const aiMesh* aimesh);
	// ���b�V���Ɋ܂܂��C���f�b�N�X�����o��
	unsigned int  GetIndices(const aiMesh* aimesh);

	// �T�u�Z�b�g���P�ɂ܂Ƃ߂邽�߂̃x�[�X���v�Z����
	void CalcMeshBaseIndex();

	// �S�e�N�X�`�����擾(�������쐬�����)
	void GetTextures(const aiScene* aiscene);

	// �����e�N�X�`�����ǂ��������ʂ���
	bool isEmbeddedTexure(const aiTexture* aitexture);

	// assimp�̓����f�[�^�ɃZ�b�g����Ă���e�N�X�`�������L�[�Ƃ��ĕۑ�
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texturedic;

	// �}�e���A���Ɋ܂܂��f�[�^���擾����
	void GetMaterials(const aiScene* aiscene);				// �S�}�e���A�������[�v
	void GetaMatreial(const aiMaterial* aimtrl);			// �P�̃}�e���A��

	Texture texture;
	// �e�N�X�`�����\�[�X�e�[�u��
	std::vector<ID3D11ShaderResourceView*>	m_AllTexture;

	// �e�N�X�`���t�@�C����ǂݍ����SRV�𐶐�����
	ID3D11ShaderResourceView* LoadDiffuseTexture(aiString _path);

	// ���̃��f���̃p�X��
	std::string m_filename;
public:
	// ������
	bool Load(std::string filename);

	// �I������
	void Uninit() override;

	// ���_�f�[�^�擾
	const std::vector<MODELVERTEX>& GetVertices() {
		return m_vertices;
	}

	// �C���f�b�N�X�f�[�^�擾
	const std::vector<unsigned int>& GetIndices() {
		return m_indices;
	}

	// �T�u�Z�b�g�f�[�^�擾
	const std::vector<SUBSET>& GetSubsets() {
		return m_subsets;
	}

	// SRV�e�[�u���擾
	const std::vector<ID3D11ShaderResourceView*>& GetSRVtable() {
		return m_AllTexture;
	}

	// �}�e���A���f�[�^�擾
	const std::vector<MATERIAL>& GetMaterials() {
		return m_materials;
	}
};


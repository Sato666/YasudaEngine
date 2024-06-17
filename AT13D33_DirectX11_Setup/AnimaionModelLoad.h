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

// �R�c���_�f�[�^
struct ANIMMODELVERTEX
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
	int			BoneIndex[4];
	float		BoneWeight[4];
};

//�ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D		Position;
	aiVector3D		Normal;
	int				BoneNum;
	std::string		BoneName[4];
	float			BoneWeight[4];
	int				BoneIdx[4];
};

//�{�[���\����
struct BONE
{
	aiMatrix4x4 Matrix;						// ���W�ϊ��Ɏg�p����s��
	aiMatrix4x4 AnimationMatrix;			// �����̎������l�����s��
	aiMatrix4x4 OffsetMatrix;				// �{�[���I�t�Z�b�g�s��
	aiQuaternion BlendRFrom;				// ���[�V�����u�����h����ۂ�FROM��
	aiQuaternion BlendRTo;					// ���[�V�����u�����h����ۂ�TO��
	aiVector3D	BlendPosFrom;				// ���[�V�����u�����h����ۂ�FROM��
	aiVector3D	BlendPosTo;					// ���[�V�����u�����h����ۂ�TO��
	int			idx;						// 20230909
};

// ���b�V��
struct ANIMSUBSET {
	unsigned int	IndexNum = 0;					// �C���f�b�N�X��
	unsigned int	VertexNum = 0;					// ���_��
	unsigned int	BoneNum = 0;					// �{�[����
	unsigned int	IndexBase = 0;					// �J�n�C���f�b�N�X
	unsigned int	VertexBase = 0;					// ���_�x�[�X
	unsigned int	BoneBase = 0;					// �{�[���x�[�X
	unsigned int	MaterialIdx = 0;				// �}�e���A���C���f�b�N�X
};

class AnimaionModelLoad :
    public Component
{
private:
	std::vector<ANIMMODELVERTEX>			m_vertices;			// ���_���W�Q
	std::vector<unsigned int>				m_indices;			// �C���f�b�N�X�f�[�^�Q
	std::vector<ANIMSUBSET>					m_subsets;			// �T�u�Z�b�g�Q
	std::vector<MATERIAL>					m_materials;		// �}�e���A���e�[�u��
	std::vector<DEFORM_VERTEX>*				m_DeformVertex;		// �ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE>	m_Bone;				// �{�[���f�[�^�i���O�ŎQ�Ɓj

	// ���b�V���Ɋ܂܂�钸�_�����o��
	unsigned int GetVertices(const aiMesh* aimesh,int i);
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

	// �e�N�X�`�����\�[�X�e�[�u��
	std::vector<ID3D11ShaderResourceView*>	m_AllTexture;

	// �e�N�X�`���t�@�C����ǂݍ����SRV�𐶐�����
	ID3D11ShaderResourceView* LoadDiffuseTexture(std::string filename);

	// �{�[������
	void CreateBone(aiNode* node);	

	// ���̃��f���̃p�X��
	std::string m_filename;
public:
	using Component::Component;

	// ������
	bool Load(std::string filename);

	// �I������
	void Uninit() override;

	// ���_�f�[�^�擾
	const std::vector<ANIMMODELVERTEX>& GetVertices() {
		return m_vertices;
	}

	// �C���f�b�N�X�f�[�^�擾
	const std::vector<unsigned int>& GetIndices() {
		return m_indices;
	}

	// �ό`�㒸�_�f�[�^�擾
	const std::vector<DEFORM_VERTEX>* GetDeformVertex() {
		return m_DeformVertex;
	}

	// �{�[���f�[�^�擾
	const std::unordered_map<std::string, BONE> GetBone() {
		return m_Bone;
	}

	// �T�u�Z�b�g�f�[�^�擾
	const std::vector<ANIMSUBSET>& GetSubsets() {
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


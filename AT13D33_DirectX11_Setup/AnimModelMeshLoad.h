//-------------------------------------------------------------------------------------
// Class : AnimModelMeshLoad
// �eClass   :   Component
// 
// Assimp5.3.0���g���ă��f�������[�h����R���|�[�l���g
// �A�j���[�V�����@�\�t�����[�h��������Ԃ��@�\������
// 
// Yasuda.Aoto  2023.10.23
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
#include "AnimationData.h"

// COMPTR���g����悤��
using Microsoft::WRL::ComPtr;

// �R�c���_�f�[�^
struct ANIMMODELVERTEX
{
	DirectX::SimpleMath::Vector3	Position;
	DirectX::SimpleMath::Vector3	Normal;
	DirectX::SimpleMath::Color		Diffuse;
	DirectX::SimpleMath::Vector2	TexCoord;
	int		BoneIndex[4];	
	float	BoneWeight[4];	
	int		bonecnt = 0;			// 20231226
};

// �E�F�C�g���@20231225 �ǉ�
struct WEIGHT 
{
	std::string bonename;						// �{�[����
	std::string meshname;						// ���b�V����
	float weight;								// �E�F�C�g�l
	int	vertexindex;							// ���_�C���f�b�N�X
};

//�{�[���\����
struct BONE
{
	std::string bonename;						// �{�[����
	std::string meshname;						// ���b�V����
	std::string armaturename;					// �A�[�}�`���A��
	aiMatrix4x4 Matrix{};						// �e�q�֌W���l�������s��
	aiMatrix4x4 AnimationMatrix{};				// �����̎��������l�����s��
	aiMatrix4x4 OffsetMatrix{};					// �{�[���I�t�Z�b�g�s��
	int			idx;							// �z��̉��Ԗڂ�			
	std::vector<WEIGHT> weights;				// ���̃{�[�����e����^���钸�_�C���f�b�N�X�E�E�F�C�g�l
};

class AnimModelMeshLoad :
    public Component
{
private:
	const aiScene* m_AiScene = nullptr;			// assimp scene

	std::vector<ANIMMODELVERTEX>m_vertices;		// ���_���W�Q
	std::vector<unsigned int>	m_indices;		// �C���f�b�N�X�f�[�^�Q
	std::vector<SUBSET>			m_subsets;		// �T�u�Z�b�g�Q
	std::vector<MATERIAL>		m_materials;	// �}�e���A���e�[�u��
	std::vector<aiMatrix4x4>	m_bonecombmtxcontainer;		// �{�[���R���r�l�[�V�����s��̔z��

	// �{�[����񎫏��i�{�[�����ŎQ�Ɖ\�j				
	std::unordered_map<std::string, BONE> m_Bone;		
	// �T�u�Z�b�g�P�ʂŃ{�[�������W�߂�����
	std::vector<std::vector<BONE>>	m_SubsetBones;
	// �{�[����񐶐��i�{�[���̋���𐶐�����j		
	void CreateBone(aiNode* node);
	// �{�[�����A�{�[���E�F�C�g�𒸓_�ɃZ�b�g����
	void SetBoneDataToVertices();						
	// �{�[�������擾����
	std::vector<BONE> GetBoneInfo(const aiMesh* mesh);	

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
	using Component::Component;

	// ������
	bool Load(std::string filename);

	// �X�V
	void Update(CAnimationData& animdata, const char* animname, int frmae);

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

	// �{�[���R���r�l�[�V�����s����K�w�\���ɏ]���čX�V����
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	// �{�[���R���r�l�[�V�����s��Q���擾
	const std::vector<aiMatrix4x4>& GetBoneCombinationMatrix() {
		return m_bonecombmtxcontainer;
	}
};


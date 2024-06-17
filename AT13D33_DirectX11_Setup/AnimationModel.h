//-------------------------------------------------------------------------------------
// Class : AnimationModel
// �eClass   :   Component
// 
// Assimp���g���ă��[�h�ƃA�j���[�V�����X�V����������
// ���f���̕`��@�\������ 
// 
// Yasuda.Aoto  2023.09.25
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <unordered_map>
#include <d3d11.h>
#include <DirectXMath.h>
#include "component.h"
#include "CMaterial.h"
//-----------------------------------------------------------------------------
// Asiimp
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

// debug��release��Assimp���C�u������ύX����
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "assimp-vc143-mtd.lib")
#else
#pragma comment (lib, "assimp-vc143-mt.lib")
#endif//defined(DEBUG) || defined(_DEBUG)

//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// �R�c���_�f�[�^
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	 Diffuse;
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
	std::string		BoneName[4];			// �{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float			BoneWeight[4];
	int				BoneIdx[4];
	// �{�[�����ւ̃C���f�b�N�X 20230909
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
// �{�[�� 20230909-02
struct CBBoneCombMatrx
{
	DirectX::XMFLOAT4X4 BoneCombMtx[400];			// �{�[���R���r�l�[�V�����s��
};

class AnimationModel :
	public Component
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;	// �A�j���[�V�����f�[�^

	ID3D11Buffer** m_VertexBuffer;	// ���_�o�b�t�@
	ID3D11Buffer** m_IndexBuffer;	// �C���f�b�N�X�o�b�t�@

	// �e�N�X�`�����
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;				//�ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE> m_Bone;			//�{�[���f�[�^�i���O�ŎQ�Ɓj

	void CreateBone(aiNode* Node);								// �{�[������
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);	// �{�[���}�g���b�N�X�X�V

	ID3D11Buffer* m_BoneCombMtxCBuffer;						// �萔�o�b�t�@�@20230909-02

	CMaterial*	m_Material;		// �}�e���A��
public:
	using Component::Component;

	void Load(const char* FileName);
	void LoadAnimation(const char* FileName, const char* Name);
	void Uninit() override;
	void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate);
	void Draw() override;
};

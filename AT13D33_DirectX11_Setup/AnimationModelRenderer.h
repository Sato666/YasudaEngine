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
#include "AnimaionModelLoad.h"
#include "component.h"

// �{�[�� 20230909-02
struct CBBoneCombMatrx
{
    DirectX::XMFLOAT4X4 BoneCombMtx[400];			// �{�[���R���r�l�[�V�����s��
};

class AnimationModelRenderer :
    public Component
{
public:
    using Component::Component;

    void LoadAnimation(const char* FileName, const char* Name);
    void Init(AnimaionModelLoad& mesh);		// ���[�h�������f���̏����󂯎��
    void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate);
    void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);	// �{�[���}�g���b�N�X�X�V
    void Draw() override;				// �󂯎������񂩂�`��
    void Uninit() override;				// �s�K�v�ɂȂ����f�[�^�J��
private:
    AnimaionModelLoad* Model;
    std::unordered_map<std::string, BONE>	m_Bone;

    const aiScene* m_AiScene = nullptr;

    std::unordered_map<std::string, const aiScene*> m_Animation;	// �A�j���[�V�����f�[�^

    std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable;		// �f�B�t���[�Y�e�N�X�`��SRV
    std::vector<ANIMSUBSET>	m_Subsets;					// �T�u�Z�b�g���

    ID3D11Buffer* m_BoneCombMtxCBuffer;						// �萔�o�b�t�@�@20230909-02

    std::vector<CMaterial*>		    m_Materials;		    // �}�e���A���Q
    CVertexBuffer<ANIMMODELVERTEX>	m_VertexBuffer;		    // ���_�o�b�t�@
    CVertexBuffer<DEFORM_VERTEX>	m_DeformVertexBuffer;   // �ό`�㒸�_�o�b�t�@
    CIndexBuffer				    m_IndexBuffer;		    // �C���f�b�N�X�o�b�t�@

};


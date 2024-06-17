#pragma once
#include "component.h"
#include "renderer.h"

#include <string>
#include <vector>
#include <unordered_map>

#include	<d3d11.h>
#include	<SimpleMath.h>

// �����N���ׂ��O�����C�u����
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// PMX�f�[�^
struct PMXModelData
{
	static constexpr int NO_DATA_FLAG = -1;

	struct Vertex
	{
		VERTEX_3D VertexArray;

		// �ǉ�UV���W
		std::vector<DirectX::XMFLOAT4> addUV;

		// �{�[���E�F�C�g
		struct Weight
		{
			enum Type
			{
				BDEF1,
				BDEF2,
				BDEF4,
				SDEF,
			};

			Type type;
			int born1;
			int born2;
			int born3;
			int born4;

			float weight1;
			float weight2;
			float weight3;
			float weight4;

			DirectX::XMFLOAT3 c;
			DirectX::XMFLOAT3 r0;
			DirectX::XMFLOAT3 r1;
		}weight;

		// �G�b�W�{��
		float edgeMagnif;
	};

	struct Surface
	{
		int vertexIndex;
	};

	struct Materials
	{
		MATERIAL		Material;
		ID3D11ShaderResourceView* Texture;
		char		TextureName[256];

		int colorMapTextureIndex;
		int toonTextureIndex;
		// �X�t�B�A�e�N�X�`���͔�Ή�

		// �ގ����Ƃ̒��_��
		int vertexNum;
	};

	struct Bone
	{
		// �{�[����
		std::wstring name;
		// English version
		std::string nameEnglish;
		DirectX::XMFLOAT3 position;
		int parentIndex;
		int transformationLevel;
		unsigned short flag;
		DirectX::XMFLOAT3 coordOffset;
		int childrenIndex;
		int impartParentIndex;
		float impartRate;
		// �Œ莲�����x�N�g��
		DirectX::XMFLOAT3 fixedAxis;
		// ���[�J����X�������x�N�g��
		DirectX::XMFLOAT3 localAxisX;
		// ���[�J����Z�������x�N�g��
		DirectX::XMFLOAT3 localAxisZ;
		int externalParentKey;
		int ikTargetIndex;
		int ikLoopCount;
		float ikUnitAngle;
		struct IKLink
		{
			int index;
			bool existAngleLimited;
			DirectX::XMFLOAT3 limitAngleMin;
			DirectX::XMFLOAT3 limitAngleMax;
		};
		std::vector<IKLink> ikLinks;
	};

	std::vector<Vertex> vertices;
	std::vector<Surface> surfaces;
	std::vector<std::wstring> texturePaths;
	std::vector<Materials> materials;
	std::vector<Bone> bones;
};

// �`��T�u�Z�b�g�\����
struct PMXSUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	PMXModelData::Materials	Material;
};

struct PMXMODEL
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	VERTEX_3D* VertexArray;
	unsigned int	VertexNum;

	unsigned int* IndexArray;
	unsigned int	IndexNum;

	PMXSUBSET* SubsetArray;
	unsigned int	SubsetNum;
};

class PMXModel :
    public Component
{
private:
	using Component::Component;

	static std::unordered_map<std::wstring, PMXMODEL*> m_ModelPool;
	PMXMODEL* m_Model{};

	// PMX���f���f�[�^�̃��[�h�֐�
	bool loadPMX(const std::wstring& _filePath, PMXModelData& data);
	bool LoadModel(const std::wstring& _filePath, PMXMODEL* Model);

public:
	void Draw() override;
	void Load(const std::wstring& _filePath);
	static void end();
};


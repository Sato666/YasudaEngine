#pragma once
#include "component.h"
#include "renderer.h"

#include <string>
#include <vector>
#include <unordered_map>

#include	<d3d11.h>
#include	<SimpleMath.h>

// リンクすべき外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// PMXデータ
struct PMXModelData
{
	static constexpr int NO_DATA_FLAG = -1;

	struct Vertex
	{
		VERTEX_3D VertexArray;

		// 追加UV座標
		std::vector<DirectX::XMFLOAT4> addUV;

		// ボーンウェイト
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

		// エッジ倍率
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
		// スフィアテクスチャは非対応

		// 材質ごとの頂点数
		int vertexNum;
	};

	struct Bone
	{
		// ボーン名
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
		// 固定軸方向ベクトル
		DirectX::XMFLOAT3 fixedAxis;
		// ローカルのX軸方向ベクトル
		DirectX::XMFLOAT3 localAxisX;
		// ローカルのZ軸方向ベクトル
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

// 描画サブセット構造体
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

	// PMXモデルデータのロード関数
	bool loadPMX(const std::wstring& _filePath, PMXModelData& data);
	bool LoadModel(const std::wstring& _filePath, PMXMODEL* Model);

public:
	void Draw() override;
	void Load(const std::wstring& _filePath);
	static void end();
};


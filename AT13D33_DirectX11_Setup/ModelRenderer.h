//-------------------------------------------------------------------------------------
// Class : ModelRenderer
// �eClass   :   Component
// 
// Obj�t�@�C������Ń��f���ƃ}�e���A���̃��[�h�@�\������
// ���f���̕`��@�\������ 
// 
// Yasuda.Aoto  2023.07.28
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
#include "component.h"
#include "utftosjisconv.h"
#include "renderer.h"
#include "CMaterial.h"
//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// �R�c���_�f�[�^
struct OBJ_VERTEX
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Color	 Diffuse;
	DirectX::SimpleMath::Vector2 TexCoord;
};

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};
// �`��T�u�Z�b�g�\����
struct SUBSETS
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};
// ���f���\����
struct MODEL_OBJ
{
	OBJ_VERTEX*		VertexArray;
	unsigned int	VertexNum;

	unsigned int* IndexArray;
	unsigned int	IndexNum;

	SUBSETS* SubsetArray;
	unsigned int	SubsetNum;
};
// ���f���̏��
struct MODEL
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	SUBSETS* SubsetArray;
	unsigned int	SubsetNum;
};

class ModelRenderer :
	public Component
{
public:
	using Component::Component;

	static void Preload(const char* FileName);
	static void UnloadAll();

	void Load(const char* FileName);
	void Draw() override;
private:

	static std::unordered_map<std::string, MODEL*> m_ModelPool;

	static void LoadModel(const char* FileName, MODEL* Model);
	static void LoadObj(const char* FileName, MODEL_OBJ* ModelObj);
	static void LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum);

	MODEL* m_Model{};

	static std::vector<CMaterial*>	m_Materials;		// �}�e���A���Q
};
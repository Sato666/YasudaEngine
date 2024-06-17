//-------------------------------------------------------------------------------------
// Class : Polygon3D
// êeClass   :   Component
// 
// 3DÉ|ÉäÉSÉìÇê∂ê¨Ç∑ÇÈ
// 
// Yasuda.Aoto  2023.07.28
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	"component.h"
#include	"renderer.h"
#include	"utftosjisconv.h"
#include	<d3d11.h>
#include	<SimpleMath.h>

class Polygon3D :
    public Component
{
private:
	struct POLYGON_VERTEX
	{
		DirectX::SimpleMath::Vector3 Position;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Color	 Diffuse;
		DirectX::SimpleMath::Vector2 TexCoord;
	};
	
	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	MATERIAL					m_Material{};
public:

	using Component::Component;

	void Init(int x, int y, int z, int Width, int Height, const char* TextureName);
	void Uninit() override;
	void Draw() override;

	void SetMaterial(MATERIAL Material) { m_Material = Material; }
};


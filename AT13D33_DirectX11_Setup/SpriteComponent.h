//-------------------------------------------------------------------------------------
// Class : Sprite
// 親Class   :   Component
// 
// スプライトのコンポーネントクラス
// スプライト情報を与えるクラス
// マテリアルをセットする機能を持つ
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	"renderer.h"
#include	"component.h"
#include	"utftosjisconv.h"

class SpriteComponent : public Component
{
private:
	struct SPRITE_VERTEX
	{
		DirectX::SimpleMath::Vector3 Position;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Color	 Diffuse;
		DirectX::SimpleMath::Vector2 TexCoord;
	};

	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// テクスチャ読み込み変数
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
public:
	using Component::Component;

	void Init(int x, int y, int Width, int Height, const char* TextureName);
	void Uninit() override;
	void Draw() override;
};
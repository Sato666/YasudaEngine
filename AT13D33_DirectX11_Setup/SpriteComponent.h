//-------------------------------------------------------------------------------------
// Class : Sprite
// �eClass   :   Component
// 
// �X�v���C�g�̃R���|�[�l���g�N���X
// �X�v���C�g����^����N���X
// �}�e���A�����Z�b�g����@�\������
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

	// �e�N�X�`���ǂݍ��ݕϐ�
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
public:
	using Component::Component;

	void Init(int x, int y, int Width, int Height, const char* TextureName);
	void Uninit() override;
	void Draw() override;
};
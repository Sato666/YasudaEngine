//-------------------------------------------------------------------------------------
// Class : Sprite
// 
// Sprite�Ǘ�����N���X
// 
// Yasuda.Aoto  2023.11.12
//-------------------------------------------------------------------------------------
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <DirectXMath.h>
#include <memory>
#include "MeshBuffer.h"
#include "Shader.h"
#include "Texture.h"

class Sprite
{
public:
	static void Init();
	static void Uninit();
	static void Draw();

	// �e�N�X�`���֌W
	static void SetOffset(DirectX::XMFLOAT2 offset);
	static void SetSize(DirectX::XMFLOAT2 size);
	static void SetUVPos(DirectX::XMFLOAT2 pos);
	static void SetUVScale(DirectX::XMFLOAT2 scale);
	static void SetColor(DirectX::XMFLOAT4 color);
	static void SetTexture(Texture* tex);

	// �}�g���b�N�X�f�[�^
	static void SetWorld(DirectX::XMFLOAT4X4 world);
	static void SetView(DirectX::XMFLOAT4X4 view);
	static void SetProjection(DirectX::XMFLOAT4X4 proj);

	// �V�F�[�_�[
	static void SetVertexShader(Shader* vs);
	static void SetPixelShader(Shader* ps);

private:
	struct Data
	{
		std::shared_ptr<MeshBuffer> mesh;
		DirectX::XMFLOAT4X4 matrix[3];
		DirectX::XMFLOAT4 param[3];
		Texture* texture;
		Shader* vs;
		Shader* ps;
	};
	static Data m_data;								// 1���̃X�v���C�g�Ɏg���f�[�^�Q
	static std::shared_ptr<VertexShader> m_defVS;	// ���_�V�F�[�_�[
	static std::shared_ptr<PixelShader> m_defPS;	// �s�N�Z���V�F�[�_�[
	static std::shared_ptr<Texture> m_whiteTex;		// �g���e�N�X�`��
};

#endif // __SPRITE_H__
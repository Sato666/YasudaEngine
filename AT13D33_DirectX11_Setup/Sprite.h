//-------------------------------------------------------------------------------------
// Class : Sprite
// 
// Sprite管理するクラス
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

	// テクスチャ関係
	static void SetOffset(DirectX::XMFLOAT2 offset);
	static void SetSize(DirectX::XMFLOAT2 size);
	static void SetUVPos(DirectX::XMFLOAT2 pos);
	static void SetUVScale(DirectX::XMFLOAT2 scale);
	static void SetColor(DirectX::XMFLOAT4 color);
	static void SetTexture(Texture* tex);

	// マトリックスデータ
	static void SetWorld(DirectX::XMFLOAT4X4 world);
	static void SetView(DirectX::XMFLOAT4X4 view);
	static void SetProjection(DirectX::XMFLOAT4X4 proj);

	// シェーダー
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
	static Data m_data;								// 1枚のスプライトに使うデータ群
	static std::shared_ptr<VertexShader> m_defVS;	// 頂点シェーダー
	static std::shared_ptr<PixelShader> m_defPS;	// ピクセルシェーダー
	static std::shared_ptr<Texture> m_whiteTex;		// 使うテクスチャ
};

#endif // __SPRITE_H__
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SpriteComponent.h"
//-------------------------------------------------------
// オプション関係
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include <WICTextureLoader.h>
using namespace DirectX::SimpleMath;
//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
void SpriteComponent::Init(int x, int y, int Width, int Height, const char* TextureName)
{
	// ポリゴン作成
	SPRITE_VERTEX vertex[4];

	vertex[0].Position = Vector3((float)x, (float)y, 0.0f);
	vertex[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);

	vertex[1].Position = Vector3((float)(x + Width), (float)y, 0.0f);
	vertex[1].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);

	vertex[2].Position = Vector3((float)x, (float)(y + Height), 0.0f);
	vertex[2].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);

	vertex[3].Position = Vector3((float)(x + Width), (float)(y + Height), 0.0f);
	vertex[3].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SPRITE_VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	std::wstring ws = sjis_to_wide_winapi(TextureName);

	// テクスチャ読み込み
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		ws.c_str(),
		nullptr,
		&m_Texture);

	assert(m_Texture);
}
//-----------------------------------------------------------------------------
//      解放処理です.
//-----------------------------------------------------------------------------
void SpriteComponent::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}
//-----------------------------------------------------------------------------
//      描画処理です.
//-----------------------------------------------------------------------------
void SpriteComponent::Draw()
{
	// 頂点バッファ設定
	UINT stride = sizeof(SPRITE_VERTEX);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
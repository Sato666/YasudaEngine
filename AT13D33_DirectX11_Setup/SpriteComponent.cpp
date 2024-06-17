//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SpriteComponent.h"
//-------------------------------------------------------
// �I�v�V�����֌W
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include <WICTextureLoader.h>
using namespace DirectX::SimpleMath;
//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
void SpriteComponent::Init(int x, int y, int Width, int Height, const char* TextureName)
{
	// �|���S���쐬
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

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SPRITE_VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	std::wstring ws = sjis_to_wide_winapi(TextureName);

	// �e�N�X�`���ǂݍ���
	DirectX::CreateWICTextureFromFile(
		Renderer::GetDevice(),
		ws.c_str(),
		nullptr,
		&m_Texture);

	assert(m_Texture);
}
//-----------------------------------------------------------------------------
//      ��������ł�.
//-----------------------------------------------------------------------------
void SpriteComponent::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}
//-----------------------------------------------------------------------------
//      �`�揈���ł�.
//-----------------------------------------------------------------------------
void SpriteComponent::Draw()
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(SPRITE_VERTEX);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}
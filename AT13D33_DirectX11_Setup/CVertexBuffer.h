//-------------------------------------------------------------------------------------
// Class : CVertexBuffer
// 
// 頂点バッファを作るテンプレートクラス
// GPUにセットする機能を持つ
// 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<vector>
#include	<wrl/client.h>
//-----------------------------------------------------------------------------
// その他
#include "utility.h"
#include "renderer.h"

using Microsoft::WRL::ComPtr;

template <typename T>
class CVertexBuffer
{
	ComPtr<ID3D11Buffer> m_VertexBuffer;
public:
	void Create(const std::vector<T>& vertices)
	{

		// デバイス取得
		ID3D11Device* device = nullptr;
		device = Renderer::GetDevice();
		assert(device);

		// 頂点バッファ作成
		bool sts = CreateVertexBufferWrite(
			device,
			sizeof(T),						// １頂点当たりバイト数
			(unsigned int)vertices.size(),	// 頂点数
			(void*)vertices.data(),			// 頂点データ格納メモリ先頭アドレス
			&m_VertexBuffer);				// 頂点バッファ

		assert(sts == true);
	}

	void SetGPU()
	{
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext = nullptr;
		devicecontext = Renderer::GetDeviceContext();

		// 頂点バッファをセットする
		unsigned int stride = sizeof(T);
		unsigned  offset = 0;

		devicecontext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	}
};
//-------------------------------------------------------------------------------------
// Class : CIndexBuffer
// 
// インデックスバッファを作るクラス
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

class CIndexBuffer 
{
	ComPtr<ID3D11Buffer> m_IndexBuffer;
public:
	void Create(const std::vector<unsigned int>& indices) {

		// デバイス取得
		ID3D11Device* device = nullptr;

		device = Renderer::GetDevice();

		assert(device);

		// インデックスバッファ作成
		bool sts = CreateIndexBuffer(
			device,										// デバイス
			(unsigned int)(indices.size()),			// インデックス数
			(void*)indices.data(),						// インデックスデータ先頭アドレス
			&m_IndexBuffer);							// インデックスバッファ

		assert(sts == true);
	}

	void SetGPU() {

		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext = nullptr;
		devicecontext = Renderer::GetDeviceContext();

		// インデックスバッファをセット
		devicecontext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	}
};
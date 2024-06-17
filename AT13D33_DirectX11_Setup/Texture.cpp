//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Texture.h"
#include "renderer.h"

/// <summary>
/// �e�N�X�`��
/// </summary>
Texture::Texture()
	: m_width(0), m_height(0)
	, m_pTex(nullptr)
	, m_pSRV(nullptr)
{
}
Texture::~Texture()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pTex);
}

/// <summary>
/// �e�N�X�`����ǂݍ���ŃV�F�[�_�[���\�[�X�𐶐�
/// </summary>
HRESULT Texture::Create(const char* fileName)
{
	HRESULT hr = S_OK;

	// �����ϊ�
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);

	// �t�@�C���ʓǂݍ���
	DirectX::TexMetadata mdata;
	DirectX::ScratchImage image;
	if (strstr(fileName, ".tga"))
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
	else
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
	if (FAILED(hr)) {
		return E_FAIL;
	}

	// �V�F�[�_���\�[�X����
	hr = CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), mdata, &m_pSRV);
	if (SUCCEEDED(hr))
	{
		m_width = (UINT)mdata.width;
		m_height = (UINT)mdata.height;
	}
	return hr;
}


HRESULT Texture::Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	return CreateResource(desc, pData);
}

UINT Texture::GetWidth() const
{
	return m_width;
}
UINT Texture::GetHeight() const
{
	return m_height;
}
ID3D11ShaderResourceView* Texture::GetResource() const
{
	return m_pSRV;
}

/// <summary>
/// 2D�e�N�X�`���̃��\�[�X��Ԃ�
/// </summary>
D3D11_TEXTURE2D_DESC Texture::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;				// GPU �ɂ��ǂݎ��Ə������݂̃A�N�Z�X��K�v�Ƃ���
	desc.Format = format;							// �e�N�X�`���̌`��
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// �e�N�X�`�����V�F�[�_�[�X�e�[�W�Ƀo�C���h
	desc.Width = width;								// �𑜓x�̕�
	desc.Height = height;							// �𑜓x�̍���
	desc.MipLevels = 1;								// �~�j�}�b�v���x���ő吔
	desc.ArraySize = 1;								// �z����̃e�N�X�`���̐�
	desc.SampleDesc.Count = 1;						// �}���`�T���v�����O�̃T���v����(���g�p��1)
	return desc;
}


HRESULT Texture::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	HRESULT hr = E_FAIL;

	// �e�N�X�`���쐬
	D3D11_SUBRESOURCE_DATA data = {};	// �T�u���\�[�X�̏�����
	data.pSysMem = pData;				// �������f�[�^�ւ̃|�C���^�[
	data.SysMemPitch = desc.Width * 4;	// �e�N�X�`����1�s�ڂ̐擪���玟�̍s�܂ł̃o�C�g�P�ʂ̋���
	// ����(�T�u���\�[�X�������݂��Ȃ��ꍇ��̃f�[�^��n��)
	hr = Renderer::GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);
	if (FAILED(hr)) { return hr; }		// �G���[�`�F�b�N

	// �ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	switch (desc.Format)
	{
		// �e�N�X�`���v�̃t�H�[�}�b�g�̐ݒ�
	default:						srvDesc.Format = desc.Format;			break;
	case DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// ����
	hr = Renderer::GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);
	if (SUCCEEDED(hr))
	{
		m_width = desc.Width;
		m_height = desc.Height;
	}
	return hr;
}

/// <summary>
/// �����_�[�^�[�Q�b�g
/// </summary>
RenderTarget::RenderTarget()
	: m_pRTV(nullptr)
{
}
RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_pRTV);
}
void RenderTarget::Clear()
{
	static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Clear(color);
}
void RenderTarget::Clear(const float* color)
{
	// �h��Ԃ�
	Renderer::GetDeviceContext()->ClearRenderTargetView(m_pRTV, color);
}
HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	return CreateResource(desc);
}
HRESULT RenderTarget::CreateFromScreen()
{
	HRESULT hr;

	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = Renderer::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTex);
	if (FAILED(hr)) { return hr; }

	// �o�b�N�o�b�t�@�ւ̃|�C���^���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// ImGui���Â߂̕`��Ȃ̂ł���ɍ��킹��@DXGI_FORMAT_R8G8B8A8_UNORM_SRGB�̓��l�̏�񂪂���A-1/2�悵����ϊ��ł���
	rtvDesc.Texture2D.MipSlice = 0;
	hr = Renderer::GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		m_pTex->GetDesc(&desc);
		m_width = desc.Width;
		m_height = desc.Height;
	}
	return hr;
}
ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return m_pRTV;
}

HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// �e�N�X�`�����\�[�X�쐬
	HRESULT hr = Texture::CreateResource(desc, nullptr);
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// ����
	return Renderer::GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
}

/// <summary>
/// �[�x�e�N�X�`��
/// </summary>
DepthStencil::DepthStencil()
	: m_pDSV(nullptr)
{
}
DepthStencil::~DepthStencil()
{
	SAFE_RELEASE(m_pDSV);
}
void DepthStencil::Clear()
{
	Renderer::GetDeviceContext()->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	return CreateResource(desc);
}
ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return m_pDSV;
}
HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// �X�e���V���g�p����
	bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);

	// ���\�[�X����
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = Texture::CreateResource(desc, nullptr);
	if (FAILED(hr)) { return hr; }

	// �ݒ�
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// ����
	return Renderer::GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);
}
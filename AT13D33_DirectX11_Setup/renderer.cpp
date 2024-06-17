//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "Application.h"
#include "renderer.h"
#include <io.h>

using namespace DirectX::SimpleMath;

//-----------------------------------------------------------------------------
// �h���C�o�n
ID3D11Device* Renderer::m_Device{};						// �h���C�u
ID3D11DeviceContext* Renderer::m_DeviceContext{};		// �h���C�u�R���e�L�X�g
IDXGISwapChain* Renderer::m_SwapChain{};				// �X���b�v�`�F�[��
ID3D11RasterizerState* Renderer::g_pRasterizerState[3];
ID3D11BlendState* Renderer::g_pBlendState[BLEND_MAX];
ID3D11SamplerState* Renderer::g_pSamplerState[SAMPLER_MAX];
ID3D11DepthStencilState* Renderer::g_pDepthStencilState[DEPTH_MAX];

//-----------------------------------------------------------------------------
//      �����������ł�.
//-----------------------------------------------------------------------------
HRESULT  Renderer::Init(HWND hWnd, UINT width, UINT height, bool fullscreen)
{
	HRESULT	hr = E_FAIL;
	//-----------------------------------------------------------------------------
	// swapChainDesc.BufferCount						: �o�b�N�o�b�t�@�[�\�����[�h		
	// swapChainDesc.BufferDesc.Width					: �𑜓x�̕���\���l
	// swapChainDesc.BufferDesc.Height					: �𑜓x�̕���\���l
	// swapChainDesc.BufferDesc.Format					: �\���`�����L�q����DXGI_FORMAT�\����
	// swapChainDesc.BufferDesc.RefreshRate				: ���t���b�V�� ���[�g���w���c�P�ʂŋL�q����DXGI_RATIONAL�\����
	// swapChainDesc.BufferUsage						: �o�b�N �o�b�t�@�[�̃T�[�t�F�X�g�p�ʂ� CPU �A�N�Z�X �I�v�V�������L�q����A DXGI_USAGE �񋓌^�̃����o�[
	// swapChainDesc.OutputWindow						: �o�̓E�B���h�E�ւ� HWND �n���h��
	// swapChainDesc.SampleDesc							: �}���`�T���v�����O �p�����[�^�[���L�q���� DXGI_SAMPLE_DESC �\���́B
	// swapChainDesc.Windowed							: �o�͂��E�B���h�E ���[�h�ł��邩�ǂ����������u�[���l�B �o�� ���E�B���h�E ���[�h�̏ꍇ�� TRUE�B����ȊO�̏ꍇ�� FALSE�B
	//-----------------------------------------------------------------------------
	// �f�o�C�X�A�X���b�v�`�F�[���쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = width;							// �𑜓x�̕��ݒ�
	swapChainDesc.BufferDesc.Height = height;						// �𑜓x�̍����ݒ�
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 0~1�ɐ��K�����ꂽRGBA 32bit�t�H�[�}�b�g
	swapChainDesc.SampleDesc.Count = 1;								// �s�N�Z��������̃}���`�T���v���̐��B
	swapChainDesc.SampleDesc.Quality = 0;							// �N�I���e�B�͍Œ�
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1000;			// �L�����̐擪��\�������Ȃ������l�B
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			// �L�����̒��\�������Ȃ������l�B
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �T�[�t�F�X�܂��̓��\�[�X���o�̓����_�[ �^�[�Q�b�g�Ƃ��Ďg�p���܂��B
	swapChainDesc.BufferCount = 1;									// �o�b�N�o�b�t�@�[�\�����[�h
	swapChainDesc.OutputWindow = hWnd;								// Application��HWND�n���h��
	swapChainDesc.Windowed = fullscreen ? FALSE : TRUE;				// �o�͂��E�B���h�E ���[�h
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPU�ŕ`��
		D3D_DRIVER_TYPE_WARP,		// �����x(�ᑬ)
		D3D_DRIVER_TYPE_REFERENCE,	// CPU�ŕ`��
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,		// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,		// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1		// Direct9.1�Ή�GPU���x��
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,							// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�iNULL�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
			D3D_DRIVER_TYPE_HARDWARE,		// �f�o�C�X�h���C�o�̃^�C�v
			NULL,							// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
			createDeviceFlags,				// �f�o�C�X�t���O
			featureLevels,					// �@�\���x��
			numFeatureLevels,				// �@�\���x����
			D3D11_SDK_VERSION,				// SDK�̃o�[�W����
			&swapChainDesc,					// �X���b�v�`�F�C���̐ݒ�
			&m_SwapChain,					// IDXGIDwapChain�C���^�t�F�[�X	
			&m_Device,						// ID3D11Device�C���^�t�F�[�X
			&featureLevel,					// �T�|�[�g����Ă���@�\���x��
			&m_DeviceContext);				// �f�o�C�X�R���e�L�X�g
		if (SUCCEEDED(hr)) {
			break;
		}
	}
	if (FAILED(hr)) {
		return hr;
	}

	//-----------------------------------------------------------------------------
	// FillMode					: �����_�����O���Ɏg�p����h��Ԃ����[�h
	// CullMode					: �w�肵�������Ɍ����Ă���O�p�`���`�惂�[�h
	// FrontCounterClockwise	: �O�p�`�����ʌ������w�ʌ�������ݒ�
	// DepthClipEnable			: �����Ɋ�Â��ăN���b�s���O��L���ɂ��邩
	// MultisampleEnable		: �}���`�T���v�� �A���`�G�C���A�V���O (MSAA) �����_�[ �^�[�Q�b�g�Ŏl�p�`�܂��̓A���t�@���̃A���`�G�C���A�V���O �A���S���Y�����g�p���邩
	//-----------------------------------------------------------------------------
	// �J�����O�ݒ�
	D3D11_RASTERIZER_DESC rasterizer = {};
	D3D11_CULL_MODE cull[] = {
		D3D11_CULL_NONE,
		D3D11_CULL_FRONT,
		D3D11_CULL_BACK
	};
	rasterizer.FillMode = D3D11_FILL_SOLID;		// ���_�ɂ���Č`�����ꂽ�O�p�`��h��Ԃ��i�\���b�h
	rasterizer.FrontCounterClockwise = true;	// ���ʌ���
	rasterizer.DepthClipEnable = TRUE;			// �N���b�s���O��L��
	rasterizer.MultisampleEnable = FALSE;		// �A���`�G�C���A�V���O �A���S���Y�����g�p���Ȃ�
	for (int i = 0; i < 3; ++i)
	{
		rasterizer.CullMode = cull[i];
		hr = m_Device->CreateRasterizerState(&rasterizer, &g_pRasterizerState[i]);
	}
	SetCullingMode(D3D11_CULL_NONE);

	//--- �[�x�e�X�g
	// -----------------------------------------------------------------------------
	// DepthEnable					: �[�x�e�X�g��L���ɂ��邩�ǂ���
	// DepthFunc					: �[�x�f�[�^�Ɗ����̐[�x�f�[�^���r����
	// DepthWriteMask				: ���[�x�f�[�^�ƕύX�ł���[�x�X�e���V���o�b�t�@�̈ꕔ�̎���
	// StencilEnable				: �X�e���V���X�e�[�g��L���ɂ��邩�ǂ��� 
	// StencilReadMask				: �X�e���V�� �f�[�^��ǂݎ�邽�߂̐[�x�X�e���V���o�b�t�@�[�̈ꕔ������
	// StencilWriteMask				: �X�e���V�� �f�[�^���������ނ��߂̐[�x�X�e���V���o�b�t�@�[�̈ꕔ������
	// FrontFace					: �[�x�e�X�g�̌��ʂƃT�[�t�F�X�̖@�����J�����Ɍ����Ă���s�N�Z���̃X�e���V���e�X�g�̎g�p���@�����
	// FrontFace.StencilFailOp		: �X�e���V���e�X�g�����s�����Ƃ��Ɏ��s����X�e���V������
	// FrontFace.StencilDepthFailOp	: �X�e���V���e�X�g���������Đ[�x�e�X�g�����s�����Ƃ��Ɏ��s����X�e���V������
	// FrontFace.StencilPassOp		: �X�e���V���e�X�g�Ɛ[�x�e�X�g�̗��������������ꍇ�̎��s����X�e���V������
	// FrontFace.StencilFunc		: �X�e���V�� �f�[�^�Ɗ����̃X�e���V�� �f�[�^���r����
	// BackFace						: �[�x�e�X�g�̌��ʂƃT�[�t�F�X�̖@�����J�������痣��Ă���s�N�Z���̃X�e���V���e�X�g�̎g�p���@�����
	// https://tositeru.github.io/ImasaraDX11/part/ZBuffer-and-depth-stencil
	// -----------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	// �[�x�e�X�g���s���[�x�X�e���V���X�e�[�g�̍쐬�ƃX�e���V���e�X�g���s���[�x�X�e���V���X�e�[�g�̍쐬
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
	dsDesc.BackFace = dsDesc.FrontFace;
	bool enablePattern[] = { true, true, false };
	D3D11_DEPTH_WRITE_MASK maskPattern[] = {
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_DEPTH_WRITE_MASK_ZERO,
	};
	for (int i = 0; i < DEPTH_MAX; ++i)
	{
		dsDesc.DepthEnable = dsDesc.StencilEnable = enablePattern[i];
		dsDesc.DepthWriteMask = maskPattern[i];
		hr = m_Device->CreateDepthStencilState(&dsDesc, &g_pDepthStencilState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetDepthTest(DEPTH_ENABLE_WRITE_TEST);

	// �A���t�@�u�����f�B���O
	// -----------------------------------------------------------------------------
	// AlphaToCoverageEnable				: �s�N�Z���������_�[ �^�[�Q�b�g�ɐݒ肷��Ƃ��ɁA�}���`�T���v�����O��@�Ƃ��ăA���t�@����J�o���b�W���g�p���邩�ǂ���
	// IndependentBlendEnable				: ���������_�[ �^�[�Q�b�g�œƗ������u�����h��L���ɂ��邩�ǂ���
	// RenderTarget[8]						: �����_�[ �^�[�Q�b�g�̃u�����h���
	// RenderTarget.BlendEnable				: �u�����h��L�� (�܂��͖���) 
	// RenderTarget.BlendOp					: SrcBlend ����� DestBlend �����g�ݍ��킹����@
	// RenderTarget.BlendOpAlpha			: SrcBlendAlpha ����� DestBlendAlpha �����g�ݍ��킹����@
	// RenderTarget.SrcBlend				: �`�挳�̃s�N�Z�� �V�F�[�_�[���o�͂��� RGB �l�ɑ΂��Ď��s���鑀����w�� 
	// RenderTarget.SrcBlendAlpha			: �`�挳�̃s�N�Z�� �V�F�[�_�[���o�͂���A���t�@�l�ɑ΂��Ď��s���鑀����w��
	// RenderTarget.DestBlend				: �`���̃����_�[ �^�[�Q�b�g�̌��݂� RGB �l�ɑ΂��Ď��s���鑀����w��
	// RenderTarget.DestBlendAlpha			: �`���̃����_�[ �^�[�Q�b�g�̌��݂̃A���t�@�l�ɑ΂��Ď��s���鑀����w��
	// RenderTarget.RenderTargetWriteMask	: �������݃}�X�N
	// -----------------------------------------------------------------------------
	// https://pgming-ctrl.com/directx11/blend/
	// PMX��`�悷��Ƃ���RenderTexture���̃A���t�@�u�����h���s�������ƁA
	// ���ۂɃt���[���o�b�t�@�ɕ`���A�ēx�v�Z����鎞�Ƀ��l�̃u�����h�W�������������Ȃ�A�{�������Ȃ����f���̃e�N�X�`���������Ă��܂��̂�
	// �`���̃A���t�@�l�ɑ΂���D3D11_BLEND_ONE�i�f�ނ̐F�j��D3D11_BLEND_INV_SRC_ALPHA�i���l�̔��]�F�j��ݒ肷��
	// DestBlendAlpha�����l�̃u�����h�W���Ȃ̂�DestBlend�����킹��K�v������
	// https://github.com/benikabocha/saba/issues/15 �Q�l����
	// SrcBlend��DestBlend�̐ݒ�͂̂��ɐ؂�ւ����悤�Ɏ���
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;						// �J�o���b�W���g�p���Ȃ�
	blendDesc.IndependentBlendEnable = FALSE;						// �Ɨ��u�����h����
	blendDesc.RenderTarget[0].BlendEnable = TRUE;					// �L��
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			// ���Z
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	// ���Z
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		// �`�挳�̃��l�͑f�ނ̌��̐F
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;		// �`���̃��l�͑f�ނ̌��̐F		��D3D11_BLEND_INV_SRC_ALPHA�ł��悢
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	// �S�F�Ή�
	// �u�����h�̌W���g�ݍ��킹
	// D3D11_BLEND_ZERO				0��Z : �Ȃ�ł��[���ɂȂ�
	// D3D11_BLEND_ONE				1��Z : �f�ނ̌��̐F�ƂȂ�
	// D3D11_BLEND_SRC_COLOR		�`�挳�̐F�Ə�Z����
	// D3D11_BLEND_INV_SRC_COLOR	�`�挳�̔��]�F�Ə�Z����
	// D3D11_BLEND_SRC_ALPHA		�`�挳�̃��l�Ə�Z����
	// D3D11_BLEND_INV_SRC_ALPHA	�`�挳�̃��l�̔��]�F�Ə�Z����
	// D3D11_BLEND_DEST_ALPHA		�`���̃��l�Ə�Z����
	// D3D11_BLEND_INV_DEST_ALPHA	�`���̃��l�̔��]�F�Ə�Z����
	// D3D11_BLEND_DEST_COLOR		�`���̐F�Ə�Z����
	// D3D11_BLEND_INV_DEST_COLOR	�`���̔��]�F�Ə�Z����
	// D3D11_BLEND_SRC_ALPHA_SAT	����1-���̏������ق����W���Ƃ��č̗p
	// D3D11_BLEND_BLEND_FACTOR		OMSetBlendFactor()�֐�����^������W��
	// D3D11_BLEND_INV_BLEND_FACTOR	OMSetBlendFactor()�֐�����^������W���̔��]�F
	// D3D11_BLEND_SRC1_COLOR		target1�̐F���W���Ƃ���
	// D3D11_BLEND_INV_SRC1_COLOR	target1�̐F�̔��]�l���W���Ƃ���
	// D3D11_BLEND_SRC1_ALPHA		target1�̃��l���W���Ƃ���
	// D3D11_BLEND_INV_SRC1_ALPHA	target1�̃��l�̔��]�F���W���Ƃ���
	D3D11_BLEND blend[BLEND_MAX][2] = {
		{D3D11_BLEND_ONE, D3D11_BLEND_ZERO},					// 1��Z + 0��
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},		// �`�挳�̃��l + �`�挳�̃��l�̔��]�F
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ZERO},		// �`�挳�̃��l + �`�挳�̃��l�̔��]�F
		{D3D11_BLEND_ONE, D3D11_BLEND_ONE},						// 1��Z + 1��Z
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE},				// �`�挳�̃��l + 1��Z
		{D3D11_BLEND_ZERO, D3D11_BLEND_INV_SRC_COLOR},			// 0�� + �`�挳�̔��]�F
		{D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE},			// �`�挳�̔��]�F + 1��Z
	};
	for (int i = 0; i < BLEND_MAX; ++i)
	{
		blendDesc.RenderTarget[0].SrcBlend = blend[i][0];
		blendDesc.RenderTarget[0].DestBlend = blend[i][1];
		hr = m_Device->CreateBlendState(&blendDesc, &g_pBlendState[i]);
		if (FAILED(hr)) { return hr; }
	}
	// SrcBlend��DestBlend�ݒ�
	// 	BLEND_NONE	�u�����h�����Ȃ�
	//	BLEND_ALPHA	�s�N�Z���V�F�[�_�[����n�����J���[�f�[�^�ɏ]���ău�����h
	//	BLEND_NONEALPHA �s�N�Z���V�F�[�_�[����n�����J���[�f�[�^��1�]�킸�u�����h
	//	BLEND_ADD	���Z����
	//	BLEND_ADDALPHA  ���l�����Z��������
	//	BLEND_SUB		���Z����
	//	BLEND_SCREEN    �X�N���[���u�����h
	//	BLEND_TEST
	//	BLEND_MAX	�ő吔count�p
	SetBlendMode(BLEND_ALPHA);

	// �T���v���[
	D3D11_SAMPLER_DESC samplerDesc = {};
	D3D11_FILTER filter[] = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_FILTER_MIN_MAG_MIP_POINT,
	};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	for (int i = 0; i < SAMPLER_MAX; ++i)
	{
		samplerDesc.Filter = filter[i];
		hr = m_Device->CreateSamplerState(&samplerDesc, &g_pSamplerState[i]);
		if (FAILED(hr)) { return hr; }
	}
	SetSamplerState(SAMPLER_LINEAR);

	return S_OK;
}
//-----------------------------------------------------------------------------
//      ��������ł�.
//-----------------------------------------------------------------------------
void Renderer::Uninit()
{
	for (int i = 0; i < SAMPLER_MAX; ++i)
		g_pSamplerState[i]->Release();
	for (int i = 0; i < BLEND_MAX; ++i)
		g_pBlendState[i]->Release();
	for (int i = 0; i < 3; ++i)
		g_pRasterizerState[i]->Release();

	m_DeviceContext->ClearState();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->QueryInterface(&m_Device);
	m_Device->Release();
}
//-----------------------------------------------------------------------------
//      �`��폜�����ł�.
//-----------------------------------------------------------------------------
void Renderer::SwapEnd()
{
	m_SwapChain->Present(0, 0);		// ���f���L���[�𔽓]������
}

//-----------------------------------------------------------------------------
//      �J�����O�ݒ�@�w�肵�������Ɍ����Ă���O�p�`���`�惂�[�h��ݒ肷��.
//-----------------------------------------------------------------------------
void Renderer::SetCullingMode(D3D11_CULL_MODE cull)
{
	switch (cull)
	{
	case D3D11_CULL_NONE: m_DeviceContext->RSSetState(g_pRasterizerState[0]); break;
	case D3D11_CULL_FRONT: m_DeviceContext->RSSetState(g_pRasterizerState[1]); break;
	case D3D11_CULL_BACK: m_DeviceContext->RSSetState(g_pRasterizerState[2]); break;
	}
}

//-----------------------------------------------------------------------------
//      �u�����h�ݒ�.
//-----------------------------------------------------------------------------
void Renderer::SetBlendMode(BlendMode blend)
{
	if (blend < 0 || blend >= BLEND_MAX) return;
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };	// clear
	m_DeviceContext->OMSetBlendState(g_pBlendState[blend], blendFactor, 0xffffffff);
}

//-----------------------------------------------------------------------------
//      �[�x�e�X�g�ݒ�.
//-----------------------------------------------------------------------------
void Renderer::SetDepthTest(DepthState state)
{
	m_DeviceContext->OMSetDepthStencilState(g_pDepthStencilState[state], 0);
}

//-----------------------------------------------------------------------------
//      �T���v���[�ݒ�.
//-----------------------------------------------------------------------------
void Renderer::SetSamplerState(SamplerState state)
{
	if (state < 0 || state >= SAMPLER_MAX) return;
	m_DeviceContext->VSSetSamplers(0, 1, &g_pSamplerState[state]);	// ���_�V�F�[�_�[
	m_DeviceContext->GSSetSamplers(0, 1, &g_pSamplerState[state]);	// �W�I���g���V�F�[�_�[
	m_DeviceContext->HSSetSamplers(0, 1, &g_pSamplerState[state]);	// �n���V�F�[�_�[
	m_DeviceContext->DSSetSamplers(0, 1, &g_pSamplerState[state]);	// �h���C���V�F�[�_�[
	m_DeviceContext->PSSetSamplers(0, 1, &g_pSamplerState[state]);	// �s�N�Z���V�F�[�_�[
}

//-----------------------------------------------------------------------------
//      �����_�����O�ݒ�.
//-----------------------------------------------------------------------------
void Renderer::SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView)
{
	static ID3D11RenderTargetView* rtvs[4];

	if (num > 4) num = 4;
	for (UINT i = 0; i < num; ++i)
		rtvs[i] = ppViews[i]->GetView();
	m_DeviceContext->OMSetRenderTargets(num, rtvs, pView ? pView->GetView() : nullptr);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (float)ppViews[0]->GetWidth();
	vp.Height = (float)ppViews[0]->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_DeviceContext->RSSetViewports(1, &vp);
}
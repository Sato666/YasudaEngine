//-------------------------------------------------------------------------------------
// Class : Renderer
// 
// �`��֌W�̋@�\�̂��Ƃ����N���X
// �V�F�[�_�ɏ���n���@�\������
// DirectX11�̋@�\��Ԃ��@�\������
// 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<d3d11.h>
#include	<SimpleMath.h>
#include	<string>
#include	"Texture.h"


// �����N���ׂ��O�����C�u����
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

//-----------------------------------------------------------------------------
// �}�e���A��
struct MATERIAL
{
	DirectX::SimpleMath::Color	Ambient;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Color	Specular;
	DirectX::SimpleMath::Color	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2]{};
};
//-----------------------------------------------------------------------------
// ���s����
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	DirectX::SimpleMath::Vector3	Direction;		// ���C�g�̈ʒu
	DirectX::SimpleMath::Color		Diffuse;		// ���C�g�̊g�U
	DirectX::SimpleMath::Color		Ambient;		// ���C�g�̐F
};
//-----------------------------------------------------------------------------
// ���b�V���i�}�e���A�����ɃT�u�Z�b�g�����݂���j
struct SUBSET {
	std::string		Name;							//�T�u�Z�b�g��
	unsigned int	IndexNum = 0;					// �C���f�b�N�X��
	unsigned int	VertexNum = 0;					// ���_��
	unsigned int	IndexBase = 0;					// �J�n�C���f�b�N�X
	unsigned int	VertexBase = 0;					// ���_�x�[�X
	unsigned int	MaterialIdx = 0;				// �}�e���A���C���f�b�N�X
};

// �S�������悤
#define SAFE_RELEASE(p) do{if(p){p->Release(); p = nullptr;}}while(0)

// �O���錾
class RenderTarget;		// �����_�[�^�[�Q�b�g
class DepthStencil;		// �[�x�e�N�X�`��

// �u�����h���[�h��`
enum BlendMode
{
	BLEND_NONE,
	BLEND_ALPHA,
	BLEND_NONEALPHA,
	BLEND_ADD,
	BLEND_ADDALPHA,
	BLEND_SUB,
	BLEND_SCREEN,
	BLEND_MAX
};

// �T���v���[�X�e�[�g��`
enum SamplerState
{
	SAMPLER_LINEAR,
	SAMPLER_POINT,
	SAMPLER_MAX
};

enum DepthState
{
	DEPTH_ENABLE_WRITE_TEST,
	DEPTH_ENABLE_TEST,
	DEPTH_DISABLE,
	DEPTH_MAX
};

//-----------------------------------------------------------------------------
// �����_��
class Renderer
{
private:
	//-----------------------------------------------------------------------------
	// �h���C�o�n
	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RasterizerState* g_pRasterizerState[3];
	static ID3D11BlendState* g_pBlendState[BLEND_MAX];
	static ID3D11SamplerState* g_pSamplerState[SAMPLER_MAX];
	static ID3D11DepthStencilState* g_pDepthStencilState[DEPTH_MAX];

public:
	// ����������
	static HRESULT Init(HWND hWnd, UINT width, UINT height, bool fullscreen);
	// �������
	static void Uninit();
	// �`��I������
	static void SwapEnd();

	/// <summary>
	/// <para>�w�肵�������Ɍ����Ă���O�p�`�̕`�惂�[�h��ݒ肷��</para>
	/// <para>D3D11_CULL_NONE  : ���ׂĂ̎O�p�`��`��</para>
	/// <para>D3D11_CULL_FRONT : �\�ʂ̂ݕ`��</para>
	/// <para>D3D11_CULL_BACK  : ���ʂ̂ݕ`��</para>
	/// </summary>
	/// <param name="cull">D3D11_CULL_MODE</param>
	static void SetCullingMode(D3D11_CULL_MODE cull);

	/// <summary>
	/// <para>�u�����h</para>
	/// </summary>
	/// <param name="blend"></param>
	static void SetBlendMode(BlendMode blend);
	static void SetDepthTest(DepthState state);

	static void SetSamplerState(SamplerState state);
	static void SetRenderTargets(UINT num, RenderTarget** ppViews, DepthStencil* pView);

	/// <summary>
	/// �h���C�u�擾
	/// </summary>
	/// <returns>ID3D11Device</returns>
	static ID3D11Device* GetDevice(void) { return m_Device; }
	/// <summary>
	/// �R���e�L�X�g�擾
	/// </summary>
	/// <returns>ID3D11DeviceContext</returns>
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }
	/// <summary>
	/// �X���b�v�`�F�[���擾
	/// </summary>
	/// <returns>IDXGISwapChain</returns>
	static IDXGISwapChain* GetSwapChain(void) { return m_SwapChain; }
};

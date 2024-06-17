//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Shader.h"
#include <d3dcompiler.h>
#include <stdio.h>

#pragma comment(lib, "d3dcompiler.lib")

/// <summary>
/// Shader�̊�{�N���X�R���X�g���N�^
/// </summary>
/// <param name="kind"></param>
Shader::Shader(Kind kind)
	: m_kind(kind)
{
}
/// <summary>
///  �f�X�g���N�^
/// </summary>
Shader::~Shader()
{
}
/// <summary>
/// .cso�t�@�C�������[�h����
/// </summary>
/// <param name="pFileName">�p�X��</param>
/// <returns>�ǂݍ��߂����ǂ���</returns>
HRESULT Shader::Load(const char* pFileName)
{
	HRESULT hr = E_FAIL;

	// �t�@�C����ǂݍ���
	FILE* fp;
	fopen_s(&fp, pFileName, "rb");
	if (!fp) { return hr; }

	// �t�@�C���̃T�C�Y�𒲂ׂ�
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);

	// �������ɓǂݍ���
	fseek(fp, 0, SEEK_SET);
	char* pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	// �V�F�[�_�[�쐬
	hr = Make(pData, fileSize);

	// �I������
	if (pData) { delete[] pData; }
	return hr;
}

/// <summary>
/// text����V�F�[�_�[���쐬����
/// </summary>
/// <param name="pCode">�V�F�[�_�[�̃\�[�X�R�[�h</param>
/// <returns>�쐬�ł������ǂ���</returns>
HRESULT Shader::Compile(const char* pCode)
{
	static const char* pTargetList[] =
	{
		"vs_5_0",
		"gs_5_0",
		"hs_5_0",
		"ds_5_0",
		"ps_5_0"
	};

	HRESULT hr;
	ID3DBlob* pBlob;
	ID3DBlob* error;
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	// HLSL�R�[�h���R���p�C��
	hr = D3DCompile(pCode, strlen(pCode), nullptr, nullptr, nullptr,
		"main", pTargetList[m_kind], compileFlag, 0, &pBlob, &error);
	// ���s������G���[��Ԃ�
	if (FAILED(hr)) { return hr; }

	// �V�F�[�_�쐬
	hr = Make(pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());
	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(error);
	return hr;
}

/// <summary>
/// �萔�o�b�t�@�ɏ��𑗂�
/// </summary>
/// <param name="slot">�X���b�g�ԍ�</param>
/// <param name="pData">�f�[�^</param>
void Shader::WriteBuffer(UINT slot, void* pData)
{
	if (slot < m_pBuffers.size())
		Renderer::GetDeviceContext()->UpdateSubresource(m_pBuffers[slot], 0, nullptr, pData, 0, 0);
}
/// <summary>
/// �e�N�X�`������n��
/// </summary>
/// <param name="slot">�X���b�g�ԍ�</param>
/// <param name="tex">�e�N�X�`��RTV</param>
void Shader::SetTexture(UINT slot, Texture* tex)
{
	if (slot >= m_pTextures.size()) { return; }
	ID3D11ShaderResourceView* pTex = tex ? tex->GetResource() : nullptr;
	m_pTextures[slot] = pTex;
	switch (m_kind)
	{
	case Vertex:	Renderer::GetDeviceContext()->VSSetShaderResources(slot, 1, &pTex); break;		// ���_�V�F�[�_�[�Ƀe�N�X�`����n��
	case Geometory:	Renderer::GetDeviceContext()->GSSetShaderResources(slot, 1, &pTex); break;		// �W�I���g���V�F�[�_�[�Ƀe�N�X�`����n��
	case Hull:		Renderer::GetDeviceContext()->HSSetShaderResources(slot, 1, &pTex); break;		// �n���V�F�[�_�[�Ƀe�N�X�`����n��
	case Domain:	Renderer::GetDeviceContext()->DSSetShaderResources(slot, 1, &pTex); break;		// �h���C���V�F�[�_�[�Ƀe�N�X�`����n��
	case Pixel:		Renderer::GetDeviceContext()->PSSetShaderResources(slot, 1, &pTex); break;		// �s�N�Z���V�F�[�_�[�ɃV�F�[�_�[�Ƀe�N�X�`����n��
	}
}

/// <summary>
/// �V�F�[�_�[�쐬
/// </summary>
/// <param name="pData">�f�[�^</param>
/// <param name="size">�f�[�^�T�C�Y</param>
/// <returns>�f�[�^���쐬�����̂�MakeShader()�̏������s��</returns>
HRESULT Shader::Make(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = Renderer::GetDevice();

	// ��͗p�̃��t���N�V�����쐬
	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return hr; }

	// �萔�o�b�t�@�쐬
	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);
	m_pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		// �V�F�[�_�[�̒萔�o�b�t�@�̏����擾
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;
		ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);
		cbuf->GetDesc(&shaderBufDesc);

		// �쐬����o�b�t�@�̏��
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = shaderBufDesc.Size;
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		// �o�b�t�@�̍쐬
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffers[i]);
		if (FAILED(hr)) { return hr; }
	}
	// �e�N�X�`���̈�쐬
	m_pTextures.resize(shaderDesc.BoundResources, nullptr);

	return MakeShader(pData, size);
}

/// <summary>
/// ���_�V�F�[�_�[
/// </summary>
VertexShader::VertexShader()
	: Shader(Shader::Vertex)
	, m_pVS(nullptr)
	, m_pInputLayout(nullptr)
{
}

/// <summary>
/// ���_�V�F�[�_�[�f�X�g���N�^
/// </summary>
VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pVS);
}

/// <summary>
/// ���_�V�F�[�_�[��`��
/// </summary>
/// <param name="">void</param>
void VertexShader::Bind(void)
{
	ID3D11DeviceContext* pContext = Renderer::GetDeviceContext();
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->IASetInputLayout(m_pInputLayout);
	for (int i = 0; i < m_pBuffers.size(); ++i)
		pContext->VSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	for (int i = 0; i < m_pTextures.size(); ++i)
		pContext->VSSetShaderResources(i, 1, &m_pTextures[i]);
}

void VertexShader::Unbind(void)
{
	Renderer::GetDeviceContext()->VSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// ���_�V�F�[�_�[���쐬
/// </summary>
/// <param name="pData">�f�[�^</param>
/// <param name="size">�f�[�^�T�C�Y</param>
/// <returns>�����������ǂ���</returns>
HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = Renderer::GetDevice();

	// �V�F�[�_�[�쐬
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVS);
	if (FAILED(hr)) { return hr; }

	/*
	�V�F�[�_�쐬���ɃV�F�[�_���t���N�V������ʂ��ăC���v�b�g���C�A�E�g���擾
	�Z�}���e�B�N�X�̔z�u�Ȃǂ��环�ʎq���쐬
	���ʎq���o�^�ρ��ė��p�A�Ȃ���ΐV�K�쐬
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/

	ID3D11ShaderReflection* pReflection;
	D3D11_SHADER_DESC shaderDesc;
	D3D11_INPUT_ELEMENT_DESC* pInputDesc;
	D3D11_SIGNATURE_PARAMETER_DESC sigDesc;

	DXGI_FORMAT formats[][4] =
	{
		{
			DXGI_FORMAT_R32_UINT,
			DXGI_FORMAT_R32G32_UINT,
			DXGI_FORMAT_R32G32B32_UINT,
			DXGI_FORMAT_R32G32B32A32_UINT,
		}, {
			DXGI_FORMAT_R32_SINT,
			DXGI_FORMAT_R32G32_SINT,
			DXGI_FORMAT_R32G32B32_SINT,
			DXGI_FORMAT_R32G32B32A32_SINT,
		}, {
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
		},
	};

	// ���t���N�V���� �I�u�W�F�N�g���쐬
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	// ���s������G���[��Ԃ�
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];	// ���̓X���b�g
	// �C���X�^���X�����[�v
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		pReflection->GetInputParameterDesc(i, &sigDesc);
		pInputDesc[i].SemanticName = sigDesc.SemanticName;
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;

		// http://marupeke296.com/TIPS_No17_Bit.html
		BYTE elementCount = sigDesc.Mask;
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);

		switch (sigDesc.ComponentType)
		{
		case D3D_REGISTER_COMPONENT_UINT32:
			pInputDesc[i].Format = formats[0][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_SINT32:
			pInputDesc[i].Format = formats[1][elementCount - 1];
			break;
		case D3D_REGISTER_COMPONENT_FLOAT32:
			pInputDesc[i].Format = formats[2][elementCount - 1];
			break;
		}
		pInputDesc[i].InputSlot = 0;
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		pInputDesc[i].InstanceDataStepRate = 0;
	}

	hr = pDevice->CreateInputLayout(
		pInputDesc, shaderDesc.InputParameters,
		pData, size, &m_pInputLayout
	);

	delete[] pInputDesc;
	return hr;
}

/// <summary>
/// �s�N�Z���V�F�[�_�[
/// </summary>
PixelShader::PixelShader()
	: Shader(Shader::Pixel)
	, m_pPS(nullptr)
{
}
/// <summary>
/// �s�N�Z���V�F�[�_�[�f�X�g���N�^
/// </summary>
PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPS);
}
/// <summary>
/// �s�N�Z���V�F�[�_�[��`��
/// </summary>
/// <param name=""></param>
void PixelShader::Bind(void)
{
	ID3D11DeviceContext* pContext = Renderer::GetDeviceContext();
	pContext->PSSetShader(m_pPS, nullptr, 0);
	for (int i = 0; i < m_pBuffers.size(); ++i)
		pContext->PSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	for (int i = 0; i < m_pTextures.size(); ++i)
		pContext->PSSetShaderResources(i, 1, &m_pTextures[i]);
}
void PixelShader::Unbind(void)
{
	Renderer::GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
}
/// <summary>
/// �s�N�Z���V�F�[�_�[�쐬
/// </summary>
/// <param name="pData">�f�[�^</param>
/// <param name="size">�f�[�^�T�C�Y</param>
/// <returns>�s�N�Z���V�F�[�_�[���쐬</returns>
HRESULT PixelShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreatePixelShader(pData, size, NULL, &m_pPS);
}

/// <summary>
/// �n���V�F�[�_�[
/// </summary>
HullShader::HullShader()
	: Shader(Shader::Hull)
	, m_pHS(nullptr)
{
}
/// <summary>
/// �n���V�F�[�_�[�f�X�g���N�^
/// </summary>
HullShader::~HullShader()
{
	SAFE_RELEASE(m_pHS);
}
/// <summary>
/// �n���V�F�[�_�[��`��
/// </summary>
/// <param name="">void</param>
void HullShader::Bind(void)
{
	ID3D11DeviceContext* pContext = Renderer::GetDeviceContext();
	pContext->HSSetShader(m_pHS, nullptr, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	for (int i = 0; i < m_pBuffers.size(); ++i)
		pContext->HSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	for (int i = 0; i < m_pTextures.size(); ++i)
		pContext->HSSetShaderResources(i, 1, &m_pTextures[i]);
}
/// <summary>
/// �n���V�F�[�_�[�̃f�[�^����ɂ���
/// </summary>
/// <param name="">void</param>
void HullShader::Unbind(void)
{
	Renderer::GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
}
/// <summary>
/// �n���V�F�[�_�[�쐬
/// </summary>
/// <param name="pData">�f�[�^</param>
/// <param name="size">�f�[�^�T�C�Y</param>
/// <returns>�n���V�F�[�_�[���쐬</returns>
HRESULT HullShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreateHullShader(pData, size, NULL, &m_pHS);
}

/// <summary>
/// �h���C���V�F�[�_�[
/// </summary>
DomainShader::DomainShader()
	: Shader(Shader::Domain)
	, m_pDS(nullptr)
{
}
/// <summary>
/// �h���C���V�F�[�_�[�f�X�g���N�^
/// </summary>
DomainShader::~DomainShader()
{
	SAFE_RELEASE(m_pDS);
}
/// <summary>
/// �h���C���V�F�[�_�[��`��
/// </summary>
/// <param name=""></param>
void DomainShader::Bind(void)
{
	ID3D11DeviceContext* pContext = Renderer::GetDeviceContext();
	pContext->DSSetShader(m_pDS, nullptr, 0);
	for (int i = 0; i < m_pBuffers.size(); ++i)
		pContext->DSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	for (int i = 0; i < m_pTextures.size(); ++i)
		pContext->DSSetShaderResources(i, 1, &m_pTextures[i]);
}
/// <summary>
/// �h���C���V�F�[�_�[�̃f�[�^����ɂ���
/// </summary>
/// <param name="">void</param>
void DomainShader::Unbind(void)
{
	Renderer::GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
}
/// <summary>
/// �h���C���V�F�[�_�[�쐬
/// </summary>
/// <param name="pData">�f�[�^</param>
/// <param name="size">�f�[�^�T�C�Y</param>
/// <returns>�h���C���V�F�[�_�[�쐬</returns>
HRESULT DomainShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreateDomainShader(pData, size, NULL, &m_pDS);
}

/// <summary>
/// �W�I���g���V�F�[�_�[
/// </summary>
GeometoryShader::GeometoryShader()
	: Shader(Shader::Geometory)
	, m_pGS(nullptr)
{
}
/// <summary>
/// �W�I���g���V�F�[�_�[�f�X�g���N�^
/// </summary>
GeometoryShader::~GeometoryShader()
{
	SAFE_RELEASE(m_pGS);
}
/// <summary>
/// �W�I���g���V�F�[�_�[�̕`��
/// </summary>
/// <param name="">void</param>
void GeometoryShader::Bind(void)
{
	ID3D11DeviceContext* pContext = Renderer::GetDeviceContext();
	pContext->GSSetShader(m_pGS, nullptr, 0);
	for (int i = 0; i < m_pBuffers.size(); ++i)
		pContext->GSSetConstantBuffers(i, 1, &m_pBuffers[i]);
	for (int i = 0; i < m_pTextures.size(); ++i)
		pContext->GSSetShaderResources(i, 1, &m_pTextures[i]);
}
/// <summary>
/// �W�I���g���V�F�[�_�[�̃f�[�^����ɂ���
/// </summary>
/// <param name="">void</param>
void GeometoryShader::Unbind(void)
{
	Renderer::GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}
/// <summary>
/// �W�I���g���V�F�[�_�[�쐬
/// </summary>
/// <param name="pData">�f�[�^</param>
/// <param name="size">�f�[�^�T�C�Y</param>
/// <returns>�W�I���g���V�F�[�_�[�쐬</returns>
HRESULT GeometoryShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreateGeometryShader(pData, size, NULL, &m_pGS);
}

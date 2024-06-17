//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Shader.h"
#include <d3dcompiler.h>
#include <stdio.h>

#pragma comment(lib, "d3dcompiler.lib")

/// <summary>
/// Shaderの基本クラスコンストラクタ
/// </summary>
/// <param name="kind"></param>
Shader::Shader(Kind kind)
	: m_kind(kind)
{
}
/// <summary>
///  デストラクタ
/// </summary>
Shader::~Shader()
{
}
/// <summary>
/// .csoファイルをロードする
/// </summary>
/// <param name="pFileName">パス名</param>
/// <returns>読み込めたかどうか</returns>
HRESULT Shader::Load(const char* pFileName)
{
	HRESULT hr = E_FAIL;

	// ファイルを読み込む
	FILE* fp;
	fopen_s(&fp, pFileName, "rb");
	if (!fp) { return hr; }

	// ファイルのサイズを調べる
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);

	// メモリに読み込み
	fseek(fp, 0, SEEK_SET);
	char* pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	// シェーダー作成
	hr = Make(pData, fileSize);

	// 終了処理
	if (pData) { delete[] pData; }
	return hr;
}

/// <summary>
/// textからシェーダーを作成する
/// </summary>
/// <param name="pCode">シェーダーのソースコード</param>
/// <returns>作成できたかどうか</returns>
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
	// HLSLコードをコンパイル
	hr = D3DCompile(pCode, strlen(pCode), nullptr, nullptr, nullptr,
		"main", pTargetList[m_kind], compileFlag, 0, &pBlob, &error);
	// 失敗したらエラーを返す
	if (FAILED(hr)) { return hr; }

	// シェーダ作成
	hr = Make(pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());
	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(error);
	return hr;
}

/// <summary>
/// 定数バッファに情報を送る
/// </summary>
/// <param name="slot">スロット番号</param>
/// <param name="pData">データ</param>
void Shader::WriteBuffer(UINT slot, void* pData)
{
	if (slot < m_pBuffers.size())
		Renderer::GetDeviceContext()->UpdateSubresource(m_pBuffers[slot], 0, nullptr, pData, 0, 0);
}
/// <summary>
/// テクスチャ情報を渡す
/// </summary>
/// <param name="slot">スロット番号</param>
/// <param name="tex">テクスチャRTV</param>
void Shader::SetTexture(UINT slot, Texture* tex)
{
	if (slot >= m_pTextures.size()) { return; }
	ID3D11ShaderResourceView* pTex = tex ? tex->GetResource() : nullptr;
	m_pTextures[slot] = pTex;
	switch (m_kind)
	{
	case Vertex:	Renderer::GetDeviceContext()->VSSetShaderResources(slot, 1, &pTex); break;		// 頂点シェーダーにテクスチャを渡す
	case Geometory:	Renderer::GetDeviceContext()->GSSetShaderResources(slot, 1, &pTex); break;		// ジオメトリシェーダーにテクスチャを渡す
	case Hull:		Renderer::GetDeviceContext()->HSSetShaderResources(slot, 1, &pTex); break;		// ハルシェーダーにテクスチャを渡す
	case Domain:	Renderer::GetDeviceContext()->DSSetShaderResources(slot, 1, &pTex); break;		// ドメインシェーダーにテクスチャを渡す
	case Pixel:		Renderer::GetDeviceContext()->PSSetShaderResources(slot, 1, &pTex); break;		// ピクセルシェーダーにシェーダーにテクスチャを渡す
	}
}

/// <summary>
/// シェーダー作成
/// </summary>
/// <param name="pData">データ</param>
/// <param name="size">データサイズ</param>
/// <returns>データを作成したのちMakeShader()の処理を行う</returns>
HRESULT Shader::Make(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = Renderer::GetDevice();

	// 解析用のリフレクション作成
	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	if (FAILED(hr)) { return hr; }

	// 定数バッファ作成
	D3D11_SHADER_DESC shaderDesc;
	pReflection->GetDesc(&shaderDesc);
	m_pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		// シェーダーの定数バッファの情報を取得
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;
		ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);
		cbuf->GetDesc(&shaderBufDesc);

		// 作成するバッファの情報
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = shaderBufDesc.Size;
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		// バッファの作成
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffers[i]);
		if (FAILED(hr)) { return hr; }
	}
	// テクスチャ領域作成
	m_pTextures.resize(shaderDesc.BoundResources, nullptr);

	return MakeShader(pData, size);
}

/// <summary>
/// 頂点シェーダー
/// </summary>
VertexShader::VertexShader()
	: Shader(Shader::Vertex)
	, m_pVS(nullptr)
	, m_pInputLayout(nullptr)
{
}

/// <summary>
/// 頂点シェーダーデストラクタ
/// </summary>
VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pVS);
}

/// <summary>
/// 頂点シェーダーを描画
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
/// 頂点シェーダーを作成
/// </summary>
/// <param name="pData">データ</param>
/// <param name="size">データサイズ</param>
/// <returns>成功したかどうか</returns>
HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = Renderer::GetDevice();

	// シェーダー作成
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVS);
	if (FAILED(hr)) { return hr; }

	/*
	シェーダ作成時にシェーダリフレクションを通してインプットレイアウトを取得
	セマンティクスの配置などから識別子を作成
	識別子が登録済→再利用、なければ新規作成
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

	// リフレクション オブジェクトを作成
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));
	// 失敗したらエラーを返す
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];	// 入力スロット
	// インスタンス数ループ
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
/// ピクセルシェーダー
/// </summary>
PixelShader::PixelShader()
	: Shader(Shader::Pixel)
	, m_pPS(nullptr)
{
}
/// <summary>
/// ピクセルシェーダーデストラクタ
/// </summary>
PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPS);
}
/// <summary>
/// ピクセルシェーダーを描画
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
/// ピクセルシェーダー作成
/// </summary>
/// <param name="pData">データ</param>
/// <param name="size">データサイズ</param>
/// <returns>ピクセルシェーダーを作成</returns>
HRESULT PixelShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreatePixelShader(pData, size, NULL, &m_pPS);
}

/// <summary>
/// ハルシェーダー
/// </summary>
HullShader::HullShader()
	: Shader(Shader::Hull)
	, m_pHS(nullptr)
{
}
/// <summary>
/// ハルシェーダーデストラクタ
/// </summary>
HullShader::~HullShader()
{
	SAFE_RELEASE(m_pHS);
}
/// <summary>
/// ハルシェーダーを描画
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
/// ハルシェーダーのデータを空にする
/// </summary>
/// <param name="">void</param>
void HullShader::Unbind(void)
{
	Renderer::GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
}
/// <summary>
/// ハルシェーダー作成
/// </summary>
/// <param name="pData">データ</param>
/// <param name="size">データサイズ</param>
/// <returns>ハルシェーダーを作成</returns>
HRESULT HullShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreateHullShader(pData, size, NULL, &m_pHS);
}

/// <summary>
/// ドメインシェーダー
/// </summary>
DomainShader::DomainShader()
	: Shader(Shader::Domain)
	, m_pDS(nullptr)
{
}
/// <summary>
/// ドメインシェーダーデストラクタ
/// </summary>
DomainShader::~DomainShader()
{
	SAFE_RELEASE(m_pDS);
}
/// <summary>
/// ドメインシェーダーを描画
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
/// ドメインシェーダーのデータを空にする
/// </summary>
/// <param name="">void</param>
void DomainShader::Unbind(void)
{
	Renderer::GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);
}
/// <summary>
/// ドメインシェーダー作成
/// </summary>
/// <param name="pData">データ</param>
/// <param name="size">データサイズ</param>
/// <returns>ドメインシェーダー作成</returns>
HRESULT DomainShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreateDomainShader(pData, size, NULL, &m_pDS);
}

/// <summary>
/// ジオメトリシェーダー
/// </summary>
GeometoryShader::GeometoryShader()
	: Shader(Shader::Geometory)
	, m_pGS(nullptr)
{
}
/// <summary>
/// ジオメトリシェーダーデストラクタ
/// </summary>
GeometoryShader::~GeometoryShader()
{
	SAFE_RELEASE(m_pGS);
}
/// <summary>
/// ジオメトリシェーダーの描画
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
/// ジオメトリシェーダーのデータを空にする
/// </summary>
/// <param name="">void</param>
void GeometoryShader::Unbind(void)
{
	Renderer::GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}
/// <summary>
/// ジオメトリシェーダー作成
/// </summary>
/// <param name="pData">データ</param>
/// <param name="size">データサイズ</param>
/// <returns>ジオメトリシェーダー作成</returns>
HRESULT GeometoryShader::MakeShader(void* pData, UINT size)
{
	return Renderer::GetDevice()->CreateGeometryShader(pData, size, NULL, &m_pGS);
}

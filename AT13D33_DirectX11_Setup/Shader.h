//-------------------------------------------------------------------------------------
// Class : Shader
// 
// Shader管理するクラス
// 
// Yasuda.Aoto  2023.11.12
//-------------------------------------------------------------------------------------
#ifndef __SHADER_H__
#define __SHADER_H__

#include "renderer.h"
#include "Texture.h"
#include <string>
#include <map>
#include <vector>

// シェーダーの基本クラス
class Shader
{
protected:
	enum Kind
	{
		Vertex,
		Geometory,
		Hull,
		Domain,
		Pixel,
	};
protected:
	Shader(Kind kind);
public:
	virtual ~Shader();
public:
	HRESULT Load(const char* pFileName);		// シェーダーファイル(*.cso)を読み込む処理
	HRESULT Compile(const char* pCode);			// 文字列からシェーダをコンパイル
	void WriteBuffer(UINT slot, void* pData);	// 定数の書き込み
	void SetTexture(UINT slot, Texture* tex);	// テクスチャの設定
	virtual void Bind(void) = 0;				// シェーダーを描画に使用
private:
	HRESULT Make(void* pData, UINT size);		// シェーダーの作成に使用
protected:
	// シェーダーファイルを読み込んだ後、シェーダーの種類別に処理を行う
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

private:
	Kind m_kind;
protected:
	std::vector<ID3D11Buffer*> m_pBuffers;
	std::vector<ID3D11ShaderResourceView*> m_pTextures;
};

//-------------------------------------------------------------------------------------
// Class : VertexShader
// 
// VertexShader管理するクラス
// 
// Yasuda.Aoto  2023.11.12
//-------------------------------------------------------------------------------------

// 頂点シェーダ
class VertexShader : public Shader
{
public:
	VertexShader();
	~VertexShader();
	void Bind(void);
	static void Unbind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11VertexShader* m_pVS;
	ID3D11InputLayout* m_pInputLayout;
};

//-------------------------------------------------------------------------------------
// Class : PixelShader
// 
// PixelShader管理するクラス
// 
// Yasuda.Aoto  2023.11.12
//-------------------------------------------------------------------------------------

// ピクセルシェーダ
class PixelShader : public Shader
{
public:
	PixelShader();
	~PixelShader();
	void Bind(void);
	static void Unbind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11PixelShader* m_pPS;
};

//-------------------------------------------------------------------------------------
// Class : HullShader
// 
// HullShader管理するクラス
// 
// Yasuda.Aoto  2023.02.07
//-------------------------------------------------------------------------------------

//----------
// ハルシェーダー
class HullShader : public Shader
{
public:
	HullShader();
	~HullShader();
	void Bind(void);
	static void Unbind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11HullShader* m_pHS;
};

//-------------------------------------------------------------------------------------
// Class : DomainShader
// 
// DomainShader管理するクラス
// 
// Yasuda.Aoto  2023.02.07
//-------------------------------------------------------------------------------------

//----------
// ドメインシェーダー
class DomainShader : public Shader
{
public:
	DomainShader();
	~DomainShader();
	void Bind(void);
	static void Unbind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11DomainShader* m_pDS;
};

//-------------------------------------------------------------------------------------
// Class : GeometoryShader
// 
// GeometoryShader管理するクラス
// 
// Yasuda.Aoto  2023.02.07
//-------------------------------------------------------------------------------------

//----------
// ジオメトリシェーダー
class GeometoryShader : public Shader
{
public:
	GeometoryShader();
	~GeometoryShader();
	void Bind(void);
	static void Unbind();
protected:
	HRESULT MakeShader(void* pData, UINT size);
private:
	ID3D11GeometryShader* m_pGS;
};


#endif
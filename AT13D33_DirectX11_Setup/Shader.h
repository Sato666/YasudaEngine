//-------------------------------------------------------------------------------------
// Class : Shader
// 
// Shader�Ǘ�����N���X
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

// �V�F�[�_�[�̊�{�N���X
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
	HRESULT Load(const char* pFileName);		// �V�F�[�_�[�t�@�C��(*.cso)��ǂݍ��ޏ���
	HRESULT Compile(const char* pCode);			// �����񂩂�V�F�[�_���R���p�C��
	void WriteBuffer(UINT slot, void* pData);	// �萔�̏�������
	void SetTexture(UINT slot, Texture* tex);	// �e�N�X�`���̐ݒ�
	virtual void Bind(void) = 0;				// �V�F�[�_�[��`��Ɏg�p
private:
	HRESULT Make(void* pData, UINT size);		// �V�F�[�_�[�̍쐬�Ɏg�p
protected:
	// �V�F�[�_�[�t�@�C����ǂݍ��񂾌�A�V�F�[�_�[�̎�ޕʂɏ������s��
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
// VertexShader�Ǘ�����N���X
// 
// Yasuda.Aoto  2023.11.12
//-------------------------------------------------------------------------------------

// ���_�V�F�[�_
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
// PixelShader�Ǘ�����N���X
// 
// Yasuda.Aoto  2023.11.12
//-------------------------------------------------------------------------------------

// �s�N�Z���V�F�[�_
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
// HullShader�Ǘ�����N���X
// 
// Yasuda.Aoto  2023.02.07
//-------------------------------------------------------------------------------------

//----------
// �n���V�F�[�_�[
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
// DomainShader�Ǘ�����N���X
// 
// Yasuda.Aoto  2023.02.07
//-------------------------------------------------------------------------------------

//----------
// �h���C���V�F�[�_�[
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
// GeometoryShader�Ǘ�����N���X
// 
// Yasuda.Aoto  2023.02.07
//-------------------------------------------------------------------------------------

//----------
// �W�I���g���V�F�[�_�[
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
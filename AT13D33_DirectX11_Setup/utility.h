//-------------------------------------------------------------------------------------
// 
// �֗��ȋ@�\���܂Ƃ߂�.h
// 
// Yasuda.Aoto  2023.09.27
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include	<iostream>
#include	<filesystem>
#include	<functional>
#include	<thread>
#include    <d3d11.h>
#include    <string>
#include    <locale.h>
#include    <d3dcompiler.h>
//-----------------------------------------------------------------------------
//  Assimp
#include    <assimp\Importer.hpp>
#include    <assimp\scene.h>
#include    <assimp\postprocess.h>
#include    <assimp/cimport.h>
#include	<thread>
//-----------------------------------------------------------------------------
//  Texture
#include	"WICTextureLoader.h"
#include	"DDSTextureLoader.h"

// �w�莞�Ԍ�ɃX���b�h���N��
void Invoke(std::function<void()> Function, int Time);

// �t�@�C���p�X����t�@�C�����������擾����
std::string GetFileName(std::string filepath);
// �t�@�C���p�X����t�@�C�����������擾����
std::wstring GetFileNameWide(std::string filepath);
// �t�@�C���p�X����t�@�C���g���q���擾����
std::string GetFileExt_s(std::string filepath);
// �t�@�C���p�X����e�f�B���N�g�����擾����
std::wstring GetParentDirectoryWide(std::string filepath);

//--------------------------------------------------------------------------------------
// �V�F�[�_�[�t�@�C����ǂݍ���
//--------------------------------------------------------------------------------------
bool readShader(const char* csoName, std::vector<unsigned char>& byteArray);
//--------------------------------------------------------------------------------------
// �V�F�[�_�[���t�@�C���g���q�ɍ��킹�ăR���p�C��
//--------------------------------------------------------------------------------------
HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ShaderObject, size_t& ShaderObjectSize, ID3DBlob** ppBlobOut);
//--------------------------------------------------------------------------------------
// �t�@�C�������擾����
//--------------------------------------------------------------------------------------
std::string ExtractFileName(std::string fullpath, char split);
//--------------------------------------------------------------------------------------
// �t�@�C���̊g���q���擾����
//--------------------------------------------------------------------------------------
std::string GetFileExt(const char* filename);
//--------------------------------------------------------------------------------------
// �V�F�[�_�[���R���p�C��
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(
    const char* szFileName,
    LPCSTR szEntryPoint,
    LPCSTR szShaderModel,
    ID3DBlob** ppBlobOut);

/*----------------------------
�R���X�^���g�o�b�t�@���쐬(
------------------------------*/
bool CreateConstantBuffer(
    ID3D11Device* device,
    unsigned int bytesize,
    ID3D11Buffer** pConstantBuffer			// �R���X�^���g�o�b�t�@
);
/*----------------------------
�R���X�^���g�o�b�t�@���쐬(MAP�ŏ��������\)
------------------------------*/
bool CreateConstantBufferWrite(
    ID3D11Device* device,					// �f�o�C�X�I�u�W�F�N�g
    unsigned int bytesize,					// �R���X�^���g�o�b�t�@�T�C�Y
    ID3D11Buffer** pConstantBuffer			// �R���X�^���g�o�b�t�@
);
/*----------------------------
�C���f�b�N�X�o�b�t�@���쐬
------------------------------*/
bool CreateIndexBuffer(
    ID3D11Device* device,
    unsigned int indexnum,	// ���_��
    void* indexdata,							// �C���f�b�N�X�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pIndexBuffer);
/*------------------------
���_�o�b�t�@���쐬
--------------------------*/
bool CreateVertexBuffer(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
);
/*------------------------
���_�o�b�t�@���쐬(�b�o�t�������݉\)
--------------------------*/
bool CreateVertexBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
);
/*------------------------
���_�o�b�t�@(UAV)���쐬
--------------------------*/
bool CreateVertexBufferUAV(
    ID3D11Device* device,
    unsigned int stride,				// �P���_������o�C�g��
    unsigned int vertexnum,				// ���_��
    void* vertexdata,					// ���_�f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pVertexBuffer		// ���_�o�b�t�@
);
/*------------------------
 Structured�o�b�t�@���쐬
--------------------------*/
bool CreateStructuredBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// �X�g���C�h�o�C�g��
    unsigned int num,					// ��
    void* data,							// �f�[�^�i�[�������擪�A�h���X
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
);
/*---------------------------------
STAGING�o�b�t�@���쐬���R�s�[����
----------------------------------*/
ID3D11Buffer* CreateAndCopyToBuffer(
    ID3D11Device* device,
    ID3D11DeviceContext* devicecontext,
    ID3D11Buffer* pBuffer	// RWStructuredBuffer
);
/*------------------------
    ShaderResourceView���쐬
--------------------------*/
bool CreateShaderResourceView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	// Buffer
    ID3D11ShaderResourceView** ppSRV);
/*------------------------
UnOrderedAccessView���쐬
--------------------------*/
bool CreateUnOrderAccessView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	                    // Buffer
    ID3D11UnorderedAccessView** ppUAV);
/*------------------------
TGA�t�@�C����ǂݍ��݃V�F�[�_�[���\�[�X�r���[���쐬
--------------------------*/
bool CreateSRVfromTGAFile(const char* filename,					// TGA�t�@�C������V�F�[�_�[���\�[�X�r���[���쐬����
    ID3D11Device* device,
    ID3D11ShaderResourceView** srv);
/*------------------------
WIC�t�@�C����ǂݍ��݃V�F�[�_�[���\�[�X�r���[���쐬
--------------------------*/
bool CreateSRVfromWICFile(const char* filename,					// WIC�t�@�C������V�F�[�_�[���\�[�X�r���[���쐬����
    ID3D11Device* device,
    ID3D11DeviceContext* device11Context,
    ID3D11ShaderResourceView** srv);
/*------------------------
���_�V�F�[�_�[�I�u�W�F�N�g���쐬
--------------------------*/
bool CreateVertexShader(ID3D11Device* device,						// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
    const char* szFileName,
    LPCSTR szEntryPoint,
    LPCSTR szShaderModel,
    D3D11_INPUT_ELEMENT_DESC* layout,
    unsigned int numElements,
    ID3D11VertexShader** ppVertexShader,
    ID3D11InputLayout** ppVertexLayout);
/*------------------------
�s�N�Z���V�F�[�_�[�I�u�W�F�N�g���쐬
--------------------------*/
bool CreatePixelShader(ID3D11Device* device,						// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
    const char* szFileName,
    LPCSTR szEntryPoint,
    LPCSTR szShaderModel,
    ID3D11PixelShader** ppPixelShader);
/*------------------------
�t�@�C����ǂݍ��݃V�F�[�_�[���\�[�X�r���[���쐬
--------------------------*/
bool CreateSRVfromFile(
    const char* filename,
    ID3D11Device* device,
    ID3D11DeviceContext* device11Context,
    ID3D11ShaderResourceView** srv);

void Invoke(std::function<void()> Function, int Time);
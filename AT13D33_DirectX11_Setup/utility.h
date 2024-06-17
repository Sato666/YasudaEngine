//-------------------------------------------------------------------------------------
// 
// 便利な機能をまとめた.h
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

// 指定時間後にスレッドを起動
void Invoke(std::function<void()> Function, int Time);

// ファイルパスからファイル名だけを取得する
std::string GetFileName(std::string filepath);
// ファイルパスからファイル名だけを取得する
std::wstring GetFileNameWide(std::string filepath);
// ファイルパスからファイル拡張子を取得する
std::string GetFileExt_s(std::string filepath);
// ファイルパスから親ディレクトリを取得する
std::wstring GetParentDirectoryWide(std::string filepath);

//--------------------------------------------------------------------------------------
// シェーダーファイルを読み込む
//--------------------------------------------------------------------------------------
bool readShader(const char* csoName, std::vector<unsigned char>& byteArray);
//--------------------------------------------------------------------------------------
// シェーダーをファイル拡張子に合わせてコンパイル
//--------------------------------------------------------------------------------------
HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, void** ShaderObject, size_t& ShaderObjectSize, ID3DBlob** ppBlobOut);
//--------------------------------------------------------------------------------------
// ファイル名を取得する
//--------------------------------------------------------------------------------------
std::string ExtractFileName(std::string fullpath, char split);
//--------------------------------------------------------------------------------------
// ファイルの拡張子を取得する
//--------------------------------------------------------------------------------------
std::string GetFileExt(const char* filename);
//--------------------------------------------------------------------------------------
// シェーダーをコンパイル
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(
    const char* szFileName,
    LPCSTR szEntryPoint,
    LPCSTR szShaderModel,
    ID3DBlob** ppBlobOut);

/*----------------------------
コンスタントバッファを作成(
------------------------------*/
bool CreateConstantBuffer(
    ID3D11Device* device,
    unsigned int bytesize,
    ID3D11Buffer** pConstantBuffer			// コンスタントバッファ
);
/*----------------------------
コンスタントバッファを作成(MAPで書き換え可能)
------------------------------*/
bool CreateConstantBufferWrite(
    ID3D11Device* device,					// デバイスオブジェクト
    unsigned int bytesize,					// コンスタントバッファサイズ
    ID3D11Buffer** pConstantBuffer			// コンスタントバッファ
);
/*----------------------------
インデックスバッファを作成
------------------------------*/
bool CreateIndexBuffer(
    ID3D11Device* device,
    unsigned int indexnum,	// 頂点数
    void* indexdata,							// インデックスデータ格納メモリ先頭アドレス
    ID3D11Buffer** pIndexBuffer);
/*------------------------
頂点バッファを作成
--------------------------*/
bool CreateVertexBuffer(
    ID3D11Device* device,
    unsigned int stride,				// １頂点当たりバイト数
    unsigned int vertexnum,				// 頂点数
    void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
    ID3D11Buffer** pVertexBuffer		// 頂点バッファ
);
/*------------------------
頂点バッファを作成(ＣＰＵ書き込み可能)
--------------------------*/
bool CreateVertexBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// １頂点当たりバイト数
    unsigned int vertexnum,				// 頂点数
    void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
    ID3D11Buffer** pVertexBuffer		// 頂点バッファ
);
/*------------------------
頂点バッファ(UAV)を作成
--------------------------*/
bool CreateVertexBufferUAV(
    ID3D11Device* device,
    unsigned int stride,				// １頂点当たりバイト数
    unsigned int vertexnum,				// 頂点数
    void* vertexdata,					// 頂点データ格納メモリ先頭アドレス
    ID3D11Buffer** pVertexBuffer		// 頂点バッファ
);
/*------------------------
 Structuredバッファを作成
--------------------------*/
bool CreateStructuredBufferWrite(
    ID3D11Device* device,
    unsigned int stride,				// ストライドバイト数
    unsigned int num,					// 個数
    void* data,							// データ格納メモリ先頭アドレス
    ID3D11Buffer** pStructuredBuffer	// RWStructuredBuffer
);
/*---------------------------------
STAGINGバッファを作成しコピーする
----------------------------------*/
ID3D11Buffer* CreateAndCopyToBuffer(
    ID3D11Device* device,
    ID3D11DeviceContext* devicecontext,
    ID3D11Buffer* pBuffer	// RWStructuredBuffer
);
/*------------------------
    ShaderResourceViewを作成
--------------------------*/
bool CreateShaderResourceView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	// Buffer
    ID3D11ShaderResourceView** ppSRV);
/*------------------------
UnOrderedAccessViewを作成
--------------------------*/
bool CreateUnOrderAccessView(
    ID3D11Device* device,
    ID3D11Buffer* pBuffer,	                    // Buffer
    ID3D11UnorderedAccessView** ppUAV);
/*------------------------
TGAファイルを読み込みシェーダーリソースビューを作成
--------------------------*/
bool CreateSRVfromTGAFile(const char* filename,					// TGAファイルからシェーダーリソースビューを作成する
    ID3D11Device* device,
    ID3D11ShaderResourceView** srv);
/*------------------------
WICファイルを読み込みシェーダーリソースビューを作成
--------------------------*/
bool CreateSRVfromWICFile(const char* filename,					// WICファイルからシェーダーリソースビューを作成する
    ID3D11Device* device,
    ID3D11DeviceContext* device11Context,
    ID3D11ShaderResourceView** srv);
/*------------------------
頂点シェーダーオブジェクトを作成
--------------------------*/
bool CreateVertexShader(ID3D11Device* device,						// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
    const char* szFileName,
    LPCSTR szEntryPoint,
    LPCSTR szShaderModel,
    D3D11_INPUT_ELEMENT_DESC* layout,
    unsigned int numElements,
    ID3D11VertexShader** ppVertexShader,
    ID3D11InputLayout** ppVertexLayout);
/*------------------------
ピクセルシェーダーオブジェクトを作成
--------------------------*/
bool CreatePixelShader(ID3D11Device* device,						// ピクセルシェーダーオブジェクトを生成
    const char* szFileName,
    LPCSTR szEntryPoint,
    LPCSTR szShaderModel,
    ID3D11PixelShader** ppPixelShader);
/*------------------------
ファイルを読み込みシェーダーリソースビューを作成
--------------------------*/
bool CreateSRVfromFile(
    const char* filename,
    ID3D11Device* device,
    ID3D11DeviceContext* device11Context,
    ID3D11ShaderResourceView** srv);

void Invoke(std::function<void()> Function, int Time);
//-------------------------------------------------------------------------------------
// Class : ShaderComponent
// 親Class   :   Component
// 
// シェーダーのコンポーネントクラス
// シェーダーの情報を与える
// 
// Yasuda.Aoto  2023.07.14
//-------------------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "component.h"
#include <d3d11.h>
#include <memory>
#include "Shader.h"

class ShaderComponent : public Component
{
private:
	static std::shared_ptr<VertexShader> m_defVS;
	static std::shared_ptr<PixelShader> m_defPS;

	VertexShader* m_pVS;	// 現在の頂点シェーダー
	PixelShader* m_pPS;		// 現在のピクセルシェーダー

	VertexShader* m_pOldVS;	// 保存用頂点シェーダー
	PixelShader* m_pOldPS;	// 保存用ピクセルシェーダー
public:
	using Component::Component;

	void Load();
	void Draw() override;
	void Uninit() override;
	void MakeDefaultShader();

	/// <summary>
	/// VSシェーダーをセットする
	/// </summary>
	/// <param name="vs">セットしたいVSシェーダー</param>
	void SetVSShader(Shader* _vs);
	/// <summary>
	/// PSシェーダーをセットする
	/// </summary>
	/// <param name="ps">セットしたいPSシェーダー</param>
	void SetPSShader(Shader* _ps);

	/// <summary>
	/// VSシェーダーを取得する
	/// </summary>
	/// <returns>VertexShader</returns>
	Shader* GetVSShader() { return m_pVS; }

	/// <summary>
	/// PSシェーダーを取得する
	/// </summary>
	/// <returns>PixelShader</returns>
	Shader* GetPSShader() { return m_pPS; }

	/// <summary>
	/// 前のVSシェーダーを保存しておく
	/// </summary>
	/// <param name="_oldps">保存したいVSシェーダー</param>
	void SetOldVSShader(Shader* _oldvs)
	{
		VertexShader* cast = reinterpret_cast<VertexShader*>(_oldvs);
		if (cast)
			m_pOldVS = cast;
	}
	/// <summary>
	/// 前の保存されたVSシェーダーを取得する
	/// </summary>
	/// <returns>VertexShader</returns>
	Shader* GetOldVSShader() { return m_pOldVS; }

	/// <summary>
	/// 前のPSシェーダーを保存しておく
	/// </summary>
	/// <param name="_oldps">保存したいPSシェーダー</param>
	void SetOldPSShader(Shader* _oldps)
	{
		PixelShader* cast = reinterpret_cast<PixelShader*>(_oldps);
		if (cast)
			m_pOldPS = cast;
	}
	/// <summary>
	/// 前の保存されたPSシェーダーを取得する
	/// </summary>
	/// <returns>PixelShader</returns>
	Shader* GetOldPSShader() { return m_pOldPS; }

	/// <summary>
	/// セットされているPSシェーダーが一緒かを返す
	/// </summary>
	/// <param name="_nowps">比較するPSシェーダー</param>
	/// <returns>bool型</returns>
	bool ComparisonPSShader(Shader* _nowps) {

		PixelShader* cast = reinterpret_cast<PixelShader*>(_nowps);
		if (m_pPS == cast)
		{
			return true;
		}

		return false;
	}
};
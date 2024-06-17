//-------------------------------------------------------------------------------------
// Class : SceneEdit
// 親Class   :   Scene
// 
// シーンEditに関する処理が詰まっている
// 
// Yasuda.Aoto  2023.12.18
//-------------------------------------------------------------------------------------
#ifndef __SCENE_VISUAL_H__
#define __SCENE_VISUAL_H__

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Scene.hpp"
#include "main.h"		// ウィンドウサイズ取得用
// 名前関係
#include "FileName.h"

class SceneEdit : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update(float tick) override;
	void Draw() override;
	void ImGui() override;
private:
	// Guide
	void GuideLine();

	// Render
	void LastRender();
	void DefaultRender(bool p_active);
	void OutlineRender(bool p_active);
	void GBufferRender(bool p_active, std::string p_rtv, std::string p_dsv);
	void AlwaysGammaRender(bool p_active);
	void ShadowRender(bool p_active, std::string p_rtv);
	void DepthOfFieldRender(bool p_active);
	void PostEffectRender(bool p_active);

	// Material
	void ModelMaterialSet(int p_number, bool p_set);

	// ImGui
	void ImGui_UI_SceneEdit(bool* p_open);
	void ImGui_UI_UserGuide(bool* p_open);
	void ImGui_UI_AddGameObejcts(bool* p_open);
	void ImGui_UI_GameObjectList(bool* p_open);
	void ImGui_UI_Instance(bool* p_open);
	void ImGui_UI_Screen(bool* p_open);

	// Image
	void ImGui_RTVImageRender(std::string p_rtvname);
	void ImGui_DSVImageRender(std::string p_dsvname);

	// Setting
	void ImGui_ShaderSetting();					// シェーダーの設定
	void ImGui_CommonObject();					// このシーンの共通設定
	void ImGui_ObjectSetting();					// モデルの編集
	void ImGui_ObjectSetting_Shader();			// モデルのシェーダー変更
	void ImGui_ObjectSetting_ShaderSetting();	// モデルの個別シェーダーの詳細設定
	void ImGui_ObjectSetting_Transfrom();		// モデルの位置・角度・サイズ設定
	void ImGui_ObjectSetting_Material();		// モデルのマテリアル設定
	void ImGui_ObjectComponentSetting();		// オブジェクトのコンポーネント設定
	// Model
	void ImGui_ObjectAddr();															// モデルの生成
	void ImGui_ObjModelLoad(std::string _filename[], int _filenum, bool _isshadow);		// モデルのロード
	void ImGui_ObjLists();																// モデルのリスト

	// その他
	void ImGui_UserGuide();			// 操作方法

private:
	int m_index;		// PSシェーダーの番号保管用
	int m_Count = 0;	// 名前被り防止用カウント変数
	int m_Number = 0;	// 取得番号

	float m_AlphaLen = 0.75f;					// アルファディザ適応距離
	float m_OutLineVal = 0.001f;				// 輪郭線の幅
	float m_GammaValue = 1.5f;					// ガンマ値
	float m_Time = 0.0f;						// 時間
	float m_ModelOutLineVal;					// モデル個別の輪郭線の幅
	float m_ViewClipping[2] = { 0.2f,2.0f };	// 被写界深度の距離設定用
	float m_blurStrength = 0.25f;

	bool m_IsClick = false;		// クリック判定
	bool m_Outline = true;		// 輪郭線の描画
	bool m_GBuffer = true;		// Gバッファの描画
	bool m_AlwaysGamma = false;	// 常にガンマ補正適応描画フラグ
	bool m_IsShadow = true;		// 影の描画フラグ
	bool m_blur = true;			// ブラーフラグ
	bool m_DoF = true;			// 被写界深度フラグ

	bool m_ModelListOriginalSetting = false;	// モデル個々のシェーダー設定
	bool m_ModelOutline;						// モデル個別の輪郭線の描画
	bool m_FrustumCulling = true;				// フラスタムカリング
	bool m_isMulti = false;						// 画面塗りつぶしフラグ

	ImVec4 m_ModelOutLineColor;								// モデル個別の輪郭線の色変数
	ImVec4 m_LightColor;									// Lightの色変数
	ImVec4 m_OutLineColor = { 0.0f,0.0f,0.0f,1.0f };		// 輪郭線の色変数
	
	// 影のクオリティ
	enum ShadowQuality
	{
		HIGH,
		MEDIUM,
		LOW
	};
	int m_ShadowQualityLevel;	// 影の品質
private:
	// ImGui
	bool m_ShowSceneEditWindow = true;			// このシーンの共通設定GUI表示フラグ
	bool m_ShowUserGuideWindow = true;			// このシーンの操作説明GUI表示フラグ
	bool m_ShowAddGameObejctsWindow = true;		// ゲームオブジェクトを生成するGUI表示フラグ
	bool m_ShowGameObjectListWindow = true;		// 生成したゲームオブジェクトの一覧GUI表示フラグ
	bool m_ShowInstanceWindow = true;			// 指定したオブジェクトの詳細設定GUI表示フラグ
	bool m_ShowScreenWindow = true;				// レンダーターゲットの画像GUI表示フラグ
};

#endif // __SCENE_VISUAL_H__
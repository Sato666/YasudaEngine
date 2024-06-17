//-------------------------------------------------------------------------------------
// Class : ModelSetting
// 親Class   :   Component
// 
// Modelの設定flagをまとめたコンポーネントクラス
// 
// Yasuda.Aoto  2023.12.21
//-------------------------------------------------------------------------------------
#pragma once
#include "component.h"

#include "imgui.h"

class ModelSetting :
    public Component
{
private:
	using Component::Component;

	bool m_OriginalSetting;	// オリジナル設定にするかどうか
	bool m_OutLineDraw;		// アウトラインを描画するかどうか

	bool m_CharacterModel;

	float m_OutLineVal;		// 輪郭線の幅

	ImVec4 m_OutLineColor;	// 輪郭線の色変数
public:
	void Init() override;

	void SetOriginalSetting(bool _originalsetting) { m_OriginalSetting = _originalsetting; }
	void SetOutLineDraw(bool _outlinedraw) { m_OutLineDraw = _outlinedraw; }

	void SetCharacterFlag(bool _shadowdraw) { m_CharacterModel = _shadowdraw; }

	void SetOutLineVal(float _outlineval) { m_OutLineVal = _outlineval; }
	void SetOutLineColor(ImVec4 _outlinecolor) { m_OutLineColor = _outlinecolor; }

	bool GetOriginalSetting() { return m_OriginalSetting; }
	bool GetOutLineDraw() { return m_OutLineDraw; }

	bool GetCharacterFlag() { return m_CharacterModel; }

	float GetOutLineVal() { return m_OutLineVal; }
	ImVec4 GetOutLineColor() { return m_OutLineColor; }
};


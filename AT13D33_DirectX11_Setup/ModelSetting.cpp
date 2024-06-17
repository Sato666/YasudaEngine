#include "ModelSetting.h"

void ModelSetting::Init()
{
	// 初期値
	m_OriginalSetting = false;	// シーンの共通値に従うかどうか
	m_OutLineDraw = true;		// アウトラインを描画するかどうか

	m_OutLineVal = 0.001f;	// 輪郭線の幅
	m_OutLineColor = { 0.0f,0.0f,0.0f,1.0f };		// 輪郭線の色変数
}

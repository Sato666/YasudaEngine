//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SceneEdit.h"

// 文字列関係
#include <sstream>
#include <iostream>
#include "strconv2.h"

// 文字列変換関係
#include "utility.h"

// このシーンで使うGameObjectインクルード
#include "field.h"		// フィールド
#include "Box.h"		// ボックス
#include "Cylinder.h"	// シリンダー
#include "field.h"		// フィールド
#include "MoveLight.h"	// ライト

void SceneEdit::ImGui()
{
	// バーを描画
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("SceneEdit")) {
			if (ImGui::BeginMenu(u8"Active ImGuiWindow")) {
				ImGui::Checkbox(u8"SceneEdit Window", &m_ShowSceneEditWindow);			// このシーンの共通設定ウィンドウ
				ImGui::Checkbox(u8"UserGuide Window", &m_ShowUserGuideWindow);			// 操作説明ウィンドウ
				ImGui::Checkbox(u8"CreateObject Window", &m_ShowAddGameObejctsWindow);	// オブジェクトの生成ウィンドウ
				ImGui::Checkbox(u8"ObjectList Window", &m_ShowGameObjectListWindow);	// 生成したオブジェクトの一覧ウィンドウ
				ImGui::Checkbox(u8"Instance Window", &m_ShowInstanceWindow);			// 生成したオブジェクトの詳細設定ウィンドウ
				ImGui::Checkbox(u8"SceneScreen Window", &m_ShowScreenWindow);			// スクリーンウィンドウ
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// 各ImGuiウィンドウ描画
	if (m_ShowSceneEditWindow)
		ImGui_UI_SceneEdit(&m_ShowSceneEditWindow);				// このシーンの共通設定ウィンドウ処理
	if (m_ShowUserGuideWindow)
		ImGui_UI_UserGuide(&m_ShowUserGuideWindow);				// 操作説明ウィンドウ処理
	if (m_ShowAddGameObejctsWindow)
		ImGui_UI_AddGameObejcts(&m_ShowAddGameObejctsWindow);	// オブジェクトの生成ウィンドウ処理
	if (m_ShowGameObjectListWindow)
		ImGui_UI_GameObjectList(&m_ShowGameObjectListWindow);	// 生成したオブジェクトの一覧ウィンドウ処理
	if (m_ShowInstanceWindow)
		ImGui_UI_Instance(&m_ShowInstanceWindow);				// 生成したオブジェクトの詳細設定ウィンドウ処理
	if (m_ShowScreenWindow)
		ImGui_UI_Screen(&m_ShowScreenWindow);					// スクリーンウィンドウ処理
}


/// <summary>
/// このシーンの共通設定
/// </summary>
/// <param name="p_open">ウィンドウを表示するか</param>
void SceneEdit::ImGui_UI_SceneEdit(bool* p_open)
{
	if (ImGui::Begin("SceneEdit", p_open)) {
		// 現在のポストプロセスをコンボボックスで表示
		static const char* current_item = g_posteffectname[m_index].c_str();	// 初期名格納

		ImGuiStyle& style = ImGui::GetStyle();
		float w = ImGui::CalcItemWidth();
		float spacing = style.ItemInnerSpacing.x;
		float button_sz = ImGui::GetFrameHeight();
		ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
		if (ImGui::BeginCombo("##combo", current_item)) {
			for (int n = 0; n < IM_ARRAYSIZE(g_posteffectname); n++) {
				bool is_selected = (current_item == g_posteffectname[n]);		 // 名前が一致しているか確認
				if (ImGui::Selectable(g_posteffectname[n].c_str(), is_selected)) // 項目追加
				{
					m_index = n;								// コンボボックスのn番目に適応
					current_item = g_posteffectname[n].c_str();
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::Text(u8"現在のポストプロセス");
		current_item = g_posteffectname[m_index].c_str();		// m_indexに適応

		// 共有設定の一覧
		if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None)) {
			ImGui_ShaderSetting();
			ImGui_CommonObject();
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

/// <summary>
/// 操作説明
/// </summary>
/// <param name="p_open">ウィンドウを表示するか</param>
void SceneEdit::ImGui_UI_UserGuide(bool* p_open)
{
	ImGui::Begin(u8"操作方法", p_open);

	ImGui::Separator();	// 線を引く

	if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None))
		ImGui_UserGuide();

	ImGui::End();
}

/// <summary>
/// オブジェクトの生成
/// </summary>
/// <param name="p_open">ウィンドウを表示するか</param>
void SceneEdit::ImGui_UI_AddGameObejcts(bool* p_open)
{
	// オブジェクト生成関連
	if (ImGui::Begin(u8"Create", p_open)) {
		ImGui::Separator();	// 線を引く

		if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None)) {
			ImGui_ObjectAddr();	// オブジェクト生成

			ImGui::EndTabBar();
		}
		ImGui::Separator();
	}
	ImGui::End();
}

/// <summary>
/// 生成したオブジェクトの一覧
/// </summary>
/// <param name="p_open">ウィンドウを表示するか</param>
void SceneEdit::ImGui_UI_GameObjectList(bool* p_open)
{
	// 生成したオブジェクトの一覧
	if (ImGui::Begin(u8"ObjectList", p_open))
		ImGui_ObjLists();	// リスト描画

	ImGui::End();
}

/// <summary>
/// 生成したオブジェクトのImstance
/// </summary>
/// <param name="p_open">ウィンドウを表示するか</param>
void SceneEdit::ImGui_UI_Instance(bool* p_open)
{
	// 生成したオブジェクトのInstance
	if (ImGui::Begin(u8"Instance", p_open))
		ImGui_ObjectSetting();

	ImGui::End();
}

/// <summary>
/// スクリーン表示
/// </summary>
/// <param name="p_open">ウィンドウを表示するか</param>
void SceneEdit::ImGui_UI_Screen(bool* p_open)
{
	if (ImGui::Begin(u8"ブラー", p_open))
		ImGui_RTVImageRender("BlurV");
	// ImGuiUI終了
	ImGui::End();

	if (ImGui::Begin(u8"アルベド（色）出力", p_open))
		ImGui_RTVImageRender("albedoRTV");
	// ImGuiUI終了
	ImGui::End();

	if (ImGui::Begin(u8"深度値", p_open))
		ImGui_RTVImageRender("DepthRTV");
	// ImGuiUI終了
	ImGui::End();

	if (ImGui::Begin(u8"法線", p_open))
		ImGui_RTVImageRender("NomalRTV");
	// ImGuiUI終了
	ImGui::End();

	if (ImGui::Begin(u8"Gbuffer", p_open)) {
		if (m_GBuffer) {
			switch (m_ShadowQualityLevel) {
			default:
			case HIGH:		ImGui_DSVImageRender("HighDepthWriteDSV");	break;
			case MEDIUM:	ImGui_DSVImageRender("DepthWriteDSV");		break;
			case LOW:		ImGui_DSVImageRender("LowDepthWriteDSV");	break;
			}
		}
	}

	if (ImGui::Begin(u8"Edit画面", p_open))
		ImGui_RTVImageRender("LastRTV");
	// ImGuiUI終了
	ImGui::End();

	// ImGuiUI終了
	ImGui::End();
}

/// <summary>
/// RTV画像を表示するImGui関数
/// </summary>
/// <param name="p_rtvname">表示したい生成したRTVの名前</param>
void SceneEdit::ImGui_RTVImageRender(std::string p_rtvname)
{
	// レンダーターゲットのテクスチャ取得
	RenderTarget* pImGuiRTV = GetObj<RenderTarget>(p_rtvname.c_str());

	// テクスチャ表示
	ImGui::Image(pImGuiRTV->GetResource(),
		ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
	);
}

/// <summary>
/// DSV画像を表示するImGui関数
/// </summary>
/// <param name="p_dsvname">表示したい生成したDSVの名前</param>
void SceneEdit::ImGui_DSVImageRender(std::string p_dsvname)
{
	// 深度テクスチャ取得
	DepthStencil* pImGuiDSV = GetObj<DepthStencil>(p_dsvname.c_str());

	// テクスチャ表示
	ImGui::Image(pImGuiDSV->GetResource(),
		ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
	);
}

/// <summary>
/// 共通設定のShader設定
/// </summary>
void SceneEdit::ImGui_ShaderSetting()
{
	if (ImGui::BeginTabItem(u8"共通設定"))		// Tab
	{
		if (ImGui::TreeNode(u8"フラスタムカリング")) {
			ImGui::Checkbox(u8"有効", &m_FrustumCulling);
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(u8"シェーダー設定"))		// Tab
	{
		if (ImGui::TreeNode(u8"シャドウ")) {
			// Gバッファ表示するかどうか
			ImGui::Checkbox(u8"影用Gバッファ表示", &m_GBuffer);

			// Gバッファが表示されていたら影の表示非表示チェックボックスを表示
			if (m_GBuffer) {
				ImGui::Checkbox(u8"影の表示", &m_IsShadow);
			}

			// 影の品質変更コンボボックス
			const char* shadowquality[] = { "High", "Medium", "Low" };
			static const char* current_item = shadowquality[m_ShadowQualityLevel];
			ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

			ImGuiStyle& style = ImGui::GetStyle();
			float w = ImGui::CalcItemWidth();
			float spacing = style.ItemInnerSpacing.x;
			float button_sz = ImGui::GetFrameHeight();
			ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
			if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton)) {
				for (int n = 0; n < IM_ARRAYSIZE(shadowquality); n++) {
					bool is_selected = (current_item == shadowquality[n]);
					if (ImGui::Selectable(shadowquality[n], is_selected))
						current_item = shadowquality[n];
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine(0, style.ItemInnerSpacing.x);
			ImGui::Text(u8"影の品質");

			// 影の品質切り替え
			if (current_item == "High")
				m_ShadowQualityLevel = ShadowQuality::HIGH;
			if (current_item == "Medium")
				m_ShadowQualityLevel = ShadowQuality::MEDIUM;
			if (current_item == "Low")
				m_ShadowQualityLevel = ShadowQuality::LOW;

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"輪郭線"))
		{
			ImGui::Checkbox(u8"輪郭線表示", &m_Outline);
			if (m_Outline) {
				ImGui::InputFloat(u8"輪郭線の太さ", &m_OutLineVal, 0.001f);

				ImGui::ColorEdit3(u8"OutLineColor", (float*)&m_OutLineColor);	// 輪郭線の色
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"アルファディザ"))
		{
			ImGui::InputFloat(u8"アルファディザ適応距離", &m_AlphaLen, 0.05f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"ガンマ補正"))
		{
			ImGui::Checkbox(u8"常にガンマ補正", &m_AlwaysGamma);

			ImGui::InputFloat(u8"ガンマ値", &m_GammaValue, 0.01f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"被写界深度"))
		{
			ImGui::Checkbox(u8"被写界深度適応", &m_DoF);
			if (m_DoF) {
				m_blur = true;	// ブラー効果オン
				ImGui::InputFloat(u8"ブラーの強さ", &m_blurStrength, 0.01f);
				ImGui::InputFloat(u8"手前適応値", &m_ViewClipping[0], 0.01f);
				ImGui::InputFloat(u8"奥適応値", &m_ViewClipping[1], 0.01f);
			}
			else {
				m_blur = false;	// ブラー効果オフ
			}
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}
}

/// <summary>
/// 共通設定の共通オブジェクトの設定
/// </summary>
void SceneEdit::ImGui_CommonObject()
{
	if (ImGui::BeginTabItem(u8"共通オブジェクト設定"))		// Tab
	{
		if (ImGui::TreeNode(u8"Camera")) {
			Camera* pCamera = GetObj<Camera>("Camera");

			static float cameramoveval = pCamera->GetMoveVal();	// カメラ移動倍率

			ImGui::InputFloat(u8"移動倍率", &cameramoveval, 1.0f);
			ImGui::SameLine();	// 横に表示
			ImGui::Text(u8"+Shift");

			pCamera->SetMoveVal(cameramoveval);		// カメラの移動倍率をセット

			ImGui::TreePop();
		}

		if (ImGui::TreeNode(u8"Light")) {
			MoveLight* pLight = GetObj<MoveLight>("Light");

			if (ImGui::ColorEdit3(u8"Light color", (float*)&m_LightColor)) {	// 色変更
				// ライトの色をセット
				pLight->GetComponent<Light>()->SetAmbient(
					DirectX::SimpleMath::Color(m_LightColor.x / 5, m_LightColor.y / 5, m_LightColor.z / 5, m_LightColor.w));	// ALL 1.0f = (0.2f,0.2f,0.2f,1.0f)基準
				pLight->GetComponent<Light>()->SetDiffuse(
					DirectX::SimpleMath::Color(m_LightColor.x, m_LightColor.y, m_LightColor.z, m_LightColor.w));
			}
			else {
				// ライトの色を取得
				m_LightColor.x = pLight->GetComponent<Light>()->GetAmbient().x * 5;
				m_LightColor.y = pLight->GetComponent<Light>()->GetAmbient().y * 5;
				m_LightColor.z = pLight->GetComponent<Light>()->GetAmbient().z * 5;
				m_LightColor.w = pLight->GetComponent<Light>()->GetAmbient().w * 5;
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

/// <summary>
/// オブジェクトの生成
/// </summary>
void SceneEdit::ImGui_ObjectAddr()
{
	//-----------------------------------------------------------------------------
	if (ImGui::BeginTabItem(u8"建物系"))
	{
		// スクロールリスト
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiChildFlags_Border);

		// assimpの一覧から生成
		ImGui_ObjModelLoad(g_objfilename, g_objfilenum, false);

		// レンダーターゲットのテクスチャ取得
		Texture* pImGuiTex = GetObj<Texture>("IconTex");
		// スクロールリスト
		ImGui::BeginChild(ImGui::GetID((void*)10), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(u8"ボックス生成", pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (名前被り防止)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;
			Box* pBox = CreateObj<Box>(ModelName.str().c_str());
			pBox->SetID(ModelName.str().c_str());
			pBox->SetName("BoxObject");
			pBox->Init();
			pBox->GetComponent<ModelSetting>()->SetCharacterFlag(false);
			pBox->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));		// Vsシェーダーセット
			pBox->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));		// 保存Vsシェーダーセット
			pBox->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));		// Psシェーダーセット
			pBox->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// 保存Psシェーダーセット
			// ボックスオブジェクト追加
			ObjeList.push_back(pBox);
		}
		ImGui::Text(u8"Box");
		ImGui::EndChild();
		ImGui::SameLine();	// 横に表示
		ImGui::BeginChild(ImGui::GetID((void*)11), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(u8"シリンダー生成", pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (名前被り防止)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;
			Cylinder* pCylinder = CreateObj<Cylinder>(ModelName.str().c_str());
			pCylinder->SetID(ModelName.str().c_str());
			pCylinder->SetName("CylinderObject");
			pCylinder->Init();
			pCylinder->GetComponent<ModelSetting>()->SetCharacterFlag(false);
			pCylinder->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));	// Vsシェーダーセット
			pCylinder->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// 保存Vsシェーダーセット
			pCylinder->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));	// Psシェーダーセット
			pCylinder->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// 保存Psシェーダーセット
			// シリンダーオブジェクト追加
			ObjeList.push_back(pCylinder);
		}
		ImGui::Text(u8"シリンダー");
		ImGui::EndChild();
		ImGui::SameLine();	// 横に表示
		ImGui::BeginChild(ImGui::GetID((void*)12), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(u8"フィールド生成", pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (名前被り防止)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;
			Field* pField = CreateObj<Field>(ModelName.str().c_str());
			pField->SetID(ModelName.str().c_str());
			pField->SetName("FieldObject");
			pField->Init();
			pField->GetComponent<ModelSetting>()->SetCharacterFlag(false);
			pField->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));	// Vsシェーダーセット
			pField->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));	// 保存Vsシェーダーセット
			pField->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_TexColor"));	// Psシェーダーセット
			pField->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));	// 保存Psシェーダーセット
			// フィールドオブジェクト追加
			ObjeList.push_back(pField);
		}
		ImGui::Text(u8"フィールド");
		ImGui::EndChild();
		ImGui::EndChild();
		ImGui::EndTabItem();
	}
	//-----------------------------------------------------------------------------
	if (ImGui::BeginTabItem(u8"キャラクター系"))
	{
		// スクロールリスト
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiChildFlags_Border);

		// assimpの一覧から生成
		ImGui_ObjModelLoad(g_modelfilename, g_modelfilenum, true);

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}

/// <summary>
/// Assimpでモデル読み込み一覧
/// </summary>
void SceneEdit::ImGui_ObjModelLoad(std::string _filename[], int _filenum, bool _isshadow)
{
	// レンダーターゲットのテクスチャ取得
	Texture* pImGuiTex = GetObj<Texture>("IconTex");
	bool isfirstnumber = true;		// 条件式回避用番号
	for (int i = 0; i < _filenum; i++)
	{
		// スクロールリスト
		ImGui::BeginChild(ImGui::GetID((void*)i), ImVec2(120, 120), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		if (ImGui::ImageButton(_filename[i].c_str(), pImGuiTex->GetResource(), ImVec2(64, 64))) {
			m_Count++;
			// (名前被り防止)
			std::stringstream ModelName;
			ModelName << "Model" << m_Count;

			// u8エンコードでpathをコンストラクト
			std::filesystem::path p = std::filesystem::u8path(_filename[i]);
			// Model読み込み
			Model* pCharObject = CreateObj<Model>(ModelName.str().c_str());
			pCharObject->SetID(_filename[i]);																		// IDセット
			pCharObject->SetName(p.stem().u8string());																// 名前セット
			pCharObject->Init();
			pCharObject->GetComponent<ModelSetting>()->SetCharacterFlag(_isshadow);									// 影キャラフラグセット
			pCharObject->GetComponent<ShaderComponent>()->SetVSShader(GetObj<Shader>("VS_Object"));					// Vsシェーダーセット
			pCharObject->GetComponent<ShaderComponent>()->SetOldVSShader(GetObj<Shader>("VS_Object"));				// 保存Vsシェーダーセット
			pCharObject->GetComponent<ShaderComponent>()->SetPSShader(GetObj<Shader>("PS_Material"));				// Psシェーダーセット
			pCharObject->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Material"));			// 保存Psシェーダーセット
			pCharObject->GetComponent<ModelMeshLoad>()->Load(_filename[i].c_str());									// ModelLoad
			pCharObject->GetComponent<ModelMeshRenderer>()->Init(*pCharObject->GetComponent<ModelMeshLoad>());		// ModelRenderer
			pCharObject->SetScale(DirectX::SimpleMath::Vector3(0.05f, 0.05f, 0.05f));								// サイズ
			// プレイヤーオブジェクト追加
			ObjeList.push_back(pCharObject);
		}
		// u8エンコードでpathをコンストラクト
		std::filesystem::path ModelName = std::filesystem::u8path(_filename[i]);
		ImGui::Text(ModelName.stem().u8string().c_str());
		ImGui::EndChild();

		if (isfirstnumber)			// 初回の0で条件式に入ってしまうため回避処理
		{
			if (i == 0)
			{
				ImGui::SameLine();	// 横に表示
				isfirstnumber = false;
			}
		}
		else if ((i + 1) % 7 != 0)	// 7の倍数じゃなければ横に表示(初回の0考慮済み)
			ImGui::SameLine();	// 横に表示
	}
}

/// <summary>
/// オブジェクトリスト表示処理
/// </summary>
void SceneEdit::ImGui_ObjLists()
{
	// 生成したオブジェクト分Loopする
	for (int i = 0; i < ObjeList.size(); i++)
	{
		if (i != 0) {
			// 名前表示(名前被り防止)
			std::stringstream objname;
			objname << i << ": " << ObjeList[i]->GetName().c_str();
			if (ImGui::Button(objname.str().c_str()))
			{
				m_Number = i;	// instanceの参照オブジェクトをセットする
			}

			// 左クリック処理(名前被り防止)
			std::stringstream clickright;
			clickright << i << ": isSet";
			if (ImGui::BeginPopup(clickright.str().c_str())) {
				if (ImGui::Button(u8"削除")) {
					m_Number = 0;	// instanceの参照オブジェクトをリセットする

					// i番目のオブジェクトのコンポーネントなどを消す
					ObjeList[i]->Uninit();

					// オブジェクトのIDを消す
					DestroyObj(ObjeList[i]->GetID().c_str());

					ObjeList.erase(ObjeList.begin() + i);
					i--;
				}

				ImGui::EndPopup();
			}
			// 左クリック判定処理
			ImGui::OpenPopupOnItemClick(clickright.str().c_str(), ImGuiPopupFlags_MouseButtonRight);
		}
	}
}

/// <summary>
/// 選択したオブジェクトの詳細設定
/// </summary>
void SceneEdit::ImGui_ObjectSetting()
{
	// nullチェック
	if (m_Number == 0) 		return;

	// オブジェクトの名前変数
	static char objectname[256]{};
	strcpy(objectname, ObjeList[m_Number]->GetName().c_str());	// オブジェクトの名前を取得する
	ImGui::InputText("ObjectName", objectname, 256);			// 名前を指定する

	ObjeList[m_Number]->SetName(objectname);					// 常に名前を更新する

	ImGui_ObjectSetting_Shader();			// モデルのシェーダー変更
	ImGui_ObjectSetting_ShaderSetting();	// モデルの個別シェーダーの詳細設定
	ImGui_ObjectSetting_Transfrom();		// モデルの位置・角度・サイズ設定
	ImGui_ObjectSetting_Material();			// モデルのマテリアル設定
	//ImGui_ObjectComponentSetting();
}

/// <summary>
/// モデルのシェーダー変更
/// </summary>
void SceneEdit::ImGui_ObjectSetting_Shader()
{
	if (ImGui::BeginTabBar("#tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(u8"デフォルト")) {
			// スクロールリスト
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);
			if (ImGui::Button(u8"デフォルト色")) {
				// assimpで読み込んだオブジェクトか判断する
				if (ObjeList[m_Number]->GetComponent<ModelMeshRenderer>() != nullptr) {
					ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Material"));
				}
				else {
					ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_TexColor"));
				}

			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"見た目系")) {
			// スクロールリスト
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);

			if (ImGui::Button(u8"トゥーン"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_ModelToon"));

			if (ImGui::Button(u8"モノクロ"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Monochrome"));

			if (ImGui::Button(u8"セピア"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Sepia"));

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"ディザ系")) {
			// スクロールリスト
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);
			if (ImGui::Button(u8"アルファディザ"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_AlphaDither"));

			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"ノイズ系")) {
			// スクロールリスト
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);
			if (ImGui::Button(u8"ポスタリゼーション"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_Posterization"));
			if (ImGui::Button(u8"色収差"))
				ObjeList[m_Number]->GetComponent<ShaderComponent>()->SetOldPSShader(GetObj<Shader>("PS_RGBSplit"));
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

/// <summary>
/// モデルの個別シェーダーの詳細設定
/// </summary>
void SceneEdit::ImGui_ObjectSetting_ShaderSetting()
{
	// モデルセッティングコンポーネントが存在していたら処理する
	if (ObjeList[m_Number]->GetComponent<ModelSetting>() != nullptr)
	{
		// モデル個々の設定状況をとってくる
		m_ModelListOriginalSetting = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOriginalSetting();

		if (ImGui::BeginTabBar("#tabs1", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem(u8"個別設定")) {
				// スクロールリスト
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 150), ImGuiWindowFlags_NoTitleBar);

				// モデル個々の設定を適応するかどうか
				ImGui::Checkbox(u8"個別にシェーダー設定", &m_ModelListOriginalSetting);
				ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOriginalSetting(m_ModelListOriginalSetting);	// セット

				ImGui::EndChild();
				ImGui::EndTabItem();
			}

			// オリジナル設定する場合
			if (m_ModelListOriginalSetting) {
				if (ImGui::BeginTabItem(u8"シェーダー設定")) {
					// スクロールリスト
					ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 150), ImGuiWindowFlags_NoTitleBar);

					if (ImGui::TreeNode(u8"輪郭線の描画")) {
						// アウトラインを描画するかどうか取得
						m_ModelOutline = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineDraw();
						ImGui::Checkbox(u8"輪郭線の描画をするかどうか", &m_ModelOutline);
						ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOutLineDraw(m_ModelOutline);		// セット

						// 輪郭線を描画するなら輪郭線の設定項目を表示する
						if (m_ModelOutline = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineDraw()) {
							// アウトラインの太さを取得
							m_ModelOutLineVal = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineVal();
							ImGui::InputFloat(u8"輪郭線の太さ", &m_ModelOutLineVal, 0.001f);
							ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOutLineVal(m_ModelOutLineVal);		// セット

							// アウトラインの色を取得
							m_ModelOutLineColor = ObjeList[m_Number]->GetComponent<ModelSetting>()->GetOutLineColor();
							ImGui::ColorEdit3(u8"OutLineColor", (float*)&m_ModelOutLineColor); // Edit 3 floats representing a color
							ObjeList[m_Number]->GetComponent<ModelSetting>()->SetOutLineColor(m_ModelOutLineColor);		// セット
						}
						ImGui::TreePop();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
	}
}

/// <summary>
/// モデルの位置・角度・サイズ設定
/// </summary>
void SceneEdit::ImGui_ObjectSetting_Transfrom()
{
	//-----------------------------------------------------------------------------
	// ローカル宣言
	//-----------------------------------------------------------------------------
	// 生成した位置保管変数にオブジェクトの位置を入れる
	DirectX::SimpleMath::Vector3 pos = (ObjeList[m_Number]->GetPosition());
	// 生成した角度保管変数にオブジェクトの角度を入れる
	DirectX::SimpleMath::Vector3 rot = (ObjeList[m_Number]->GetRotation());
	// 生成したサイズ保管変数にオブジェクトのサイズを入れる
	DirectX::SimpleMath::Vector3 size = (ObjeList[m_Number]->GetScale());

	//-----------------------------------------------------------------------------
	//	位置
	//-----------------------------------------------------------------------------
	// 文字列の〇番目を繋げる
	if (ImGui::TreeNode(u8"Position"))
	{
		// 文字列の〇番目を繋げる
		std::stringstream stream;
		stream << "X";
		ImGui::InputFloat(stream.str().c_str(), &pos.x, 0.5f);		// X座標
		stream.str("");		// 文字列初期化
		stream << "Y";
		ImGui::InputFloat(stream.str().c_str(), &pos.y, 0.5f);		// Y座標
		stream.str("");
		stream << "Z";
		ImGui::InputFloat(stream.str().c_str(), &pos.z, 0.5f);		// Z座標
		stream.str("");
		ObjeList[m_Number]->SetPosition(pos);						// 設定した位置をセットする
		ImGui::TreePop();
	}
	//-----------------------------------------------------------------------------
	//	角度
	//-----------------------------------------------------------------------------
	if (ImGui::TreeNode(u8"Rotation"))
	{
		// 文字列の〇番目を繋げる
		std::stringstream stream;
		stream << "X";
		ImGui::InputFloat(stream.str().c_str(), &rot.x, 0.5f);		// X座標
		stream.str("");		// 文字列初期化
		stream << "Y";
		ImGui::InputFloat(stream.str().c_str(), &rot.y, 0.5f);		// Y座標
		stream.str("");
		stream << "Z";
		ImGui::InputFloat(stream.str().c_str(), &rot.z, 0.5f);		// Z座標
		stream.str("");
		ObjeList[m_Number]->SetRotation(rot);						// 設定した角度をセットする
		ImGui::TreePop();
	}
	//-----------------------------------------------------------------------------
	//	サイズ
	//-----------------------------------------------------------------------------
	if (ImGui::TreeNode(u8"Size"))
	{
		// 文字列の〇番目を繋げる
		std::stringstream stream;
		stream << "X";
		ImGui::InputFloat(stream.str().c_str(), &size.x, 0.05f);		// X座標
		stream.str("");		// 文字列初期化
		stream << "Y";
		ImGui::InputFloat(stream.str().c_str(), &size.y, 0.05f);		// Y座標
		stream.str("");
		stream << "Z";
		ImGui::InputFloat(stream.str().c_str(), &size.z, 0.05f);		// Z座標
		stream.str("");
		ObjeList[m_Number]->SetScale(size);							// 設定した位置をセットする
		ImGui::TreePop();
	}
}

/// <summary>
/// モデルのマテリアル設定
/// </summary>
void SceneEdit::ImGui_ObjectSetting_Material()
{
	// ModelMeshRendererがなければ以降の処理をしない
	if (ObjeList[m_Number]->GetComponent<ModelMeshRenderer>() == nullptr)	return;

	std::vector<ID3D11ShaderResourceView*>	m_DiffuseSRVtable
		= ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetDiffuseRTV();	// ディフューズテクスチャSRV
	std::vector<SUBSET>	m_Subsets
		= ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetSubSet();		// サブセット情報
	std::vector<CMaterial*> m_Materials
		= ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetMaterial();		// マテリアル群

	ImVec4 m_DiffuseColor;
	ImVec4 m_AmbientColor;

	int count = 0;				// 画像3つ横に並べるようcount

	if (ImGui::TreeNode(u8"ModelMaterial"))
	{
		if (ImGui::TreeNode(u8"Texture")) {
			for (auto& subset : m_Subsets) {
				// テクスチャ表示
				if (count == 0) {
					// テクスチャ表示
					ImGui::Image(m_DiffuseSRVtable[subset.MaterialIdx],
						ImVec2(100, 100)
					);
					count++;			// countを進める
				}
				else if (count < 3) {
					count++;			// countを進める

					ImGui::SameLine();	// 横に表示
					// テクスチャ表示
					ImGui::Image(m_DiffuseSRVtable[subset.MaterialIdx],
						ImVec2(100, 100)
					);
				}
				else {
					count = 1;			// カウントリセット
					// テクスチャ表示
					ImGui::Image(m_DiffuseSRVtable[subset.MaterialIdx],
						ImVec2(100, 100)
					);
				}
			}
			ImGui::TreePop();
		}
		// マテリアル情報一覧表示
		if (ImGui::TreeNode(u8"Diffuse")) {
			// サブセットの数分ループ（サブセット数はマテリアルの数）
			for (auto& subset : m_Subsets) {
				m_DiffuseColor.x = m_Materials[subset.MaterialIdx]->GetDiffuse().x;
				m_DiffuseColor.y = m_Materials[subset.MaterialIdx]->GetDiffuse().y;
				m_DiffuseColor.z = m_Materials[subset.MaterialIdx]->GetDiffuse().z;
				m_DiffuseColor.w = m_Materials[subset.MaterialIdx]->GetDiffuse().w;

				// 名前表示(名前被り防止)
				std::stringstream materialval;
				materialval << subset.MaterialIdx << "Color";
				if (ImGui::ColorEdit4(materialval.str().c_str(), (float*)&m_DiffuseColor))
				{
					ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetMaterial()[subset.MaterialIdx]->SetDiffuse(
						DirectX::SimpleMath::Color(m_DiffuseColor.x, m_DiffuseColor.y, m_DiffuseColor.z, m_DiffuseColor.w));
				}
			}
			ImGui::TreePop();
		}
		// マテリアル情報一覧表示
		if (ImGui::TreeNode(u8"Ambient")) {
			// サブセットの数分ループ（サブセット数はマテリアルの数）
			for (auto& subset : m_Subsets) {
				m_AmbientColor.x = m_Materials[subset.MaterialIdx]->GetAmbient().x;
				m_AmbientColor.y = m_Materials[subset.MaterialIdx]->GetAmbient().y;
				m_AmbientColor.z = m_Materials[subset.MaterialIdx]->GetAmbient().z;
				m_AmbientColor.w = m_Materials[subset.MaterialIdx]->GetAmbient().w;

				// 名前表示(名前被り防止)
				std::stringstream materialval;
				materialval << subset.MaterialIdx << "Color";
				if (ImGui::ColorEdit4(materialval.str().c_str(), (float*)&m_AmbientColor))
				{
					ObjeList[m_Number]->GetComponent<ModelMeshRenderer>()->GetMaterial()[subset.MaterialIdx]->SetAmbient(
						DirectX::SimpleMath::Color(m_AmbientColor.x, m_AmbientColor.y, m_AmbientColor.z, m_AmbientColor.w));
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

/// <summary>
/// オブジェクトのコンポーネント設定
/// </summary>
void SceneEdit::ImGui_ObjectComponentSetting()
{
	if (ImGui::TreeNode(u8"Component"))
	{

		ImGui::TreePop();
	}

	static bool isactive = false;
	if (ImGui::Button(u8"AddComponent")) {
		isactive = !isactive;
	}

	if (ImGui::Begin(u8"ComponentList", &isactive))
	{
		// スクロールリスト
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(300, 0), ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button(u8"un")) {

		}
		ImGui::EndChild();
	}
	ImGui::End();
}

/// <summary>
/// ImGui操作説明
/// </summary>
void SceneEdit::ImGui_UserGuide()
{
	if (ImGui::BeginTabItem(u8"カメラ操作"))
	{
		// 初期化位置の詳細情報
		ImGui::Text(u8"ALT + SHIFT(位置リセット)");
		// 自由移動の詳細情報
		if (ImGui::TreeNode(u8"左クリック(自由移動)"))
		{
			ImGui::Text(u8"+Shift		: カメラ倍率速度適応");
			ImGui::Text(u8"WS			: カメラ前進後進");
			ImGui::Text(u8"AD			: カメラ右移動左移動");
			ImGui::Text(u8"QE			: カメラ真上移動真下移動");
			ImGui::TreePop();
		}
		// 回転制御の詳細情報
		ImGui::Text(u8"ALT + 右クリック(回転制御)");
		// 平行移動制御の詳細情報
		ImGui::Text(u8"ALT + 中クリック(平行移動制御)");
		// ズーム制御の詳細情報
		ImGui::Text(u8"ALT + 左クリック(ズーム制御)");

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem(u8"light操作"))
	{
		// light移動の詳細情報
		if (ImGui::TreeNode(u8"light移動"))
		{
			ImGui::Text(u8"[Lキー]を押しながら、以下のキーで各操作を行う");
			ImGui::Text(u8"[ADキー] 原点を中心にY軸回転");
			ImGui::Text(u8"[WSキー] 原点を中心に縦回転");
			ImGui::TreePop();
		}
		// light変色の詳細情報
		if (ImGui::TreeNode(u8"light変色"))
		{
			ImGui::Text(u8"[Rキー]を押しながら、以下のキーで各操作を行う");
			ImGui::Text(u8"[Rキー] lightの色に戻す");
			ImGui::Text(u8"[左右矢印キー] 色彩の変更(HSVのHを変更)");
			ImGui::Text(u8"[上下矢印キー] 彩度と明度を変更(1.0(彩度0/明度1)～0.5f(彩度1/明度1)～0.0f(彩度1/明度0))");
			ImGui::TreePop();
		}
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem(u8"オブジェクト操作"))
	{
		if (ImGui::TreeNode(u8"Instance説明"))
		{
			// 初期化位置の詳細情報
			ImGui::Text(u8"各種シェーダー項目で見た目を変更できる");
			ImGui::Text(u8"角度・位置・サイズのXYZを変更できる");
			ImGui::Text(u8"+ボタン　加算 : -ボタン　減算");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(u8"ObjectList説明"))
		{
			ImGui::Text(u8"オブジェクト名左クリックでInstanceに詳細設定可能");
			ImGui::Text(u8"オブジェクト名右クリックで削除ボタン出現");
			ImGui::TreePop();
		}

		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

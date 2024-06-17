//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SceneMain.h"
#include <stdio.h>

// シーン
#include "SceneEdit.h"

// ジオメトリ
#include "Geometory.h"

// 算術ライブラリ
#include <DirectXMath.h>

/// <summary>
/// サブシーンの構造体
/// </summary>
enum SceneKind
{
	SCENE_VISUAL,		// defaultサブシーン
	SCENE_MAX			// 最大数
};

/// <summary>
/// シーンの変更
/// </summary>
void SceneMain::ChangeScene()
{
	switch (m_index)
	{
	default:
	case SCENE_VISUAL:	AddSubScene<SceneEdit>(); break;
	}
}
/// <summary>
/// 次回起動時にカメラ情報とライト情報と現在のサブシーンを引き続く
/// </summary>
struct ViewSetting
{
	DirectX::XMFLOAT3 camPos;		// カメラ位置
	DirectX::XMFLOAT3 camLook;		// カメラ視点
	DirectX::XMFLOAT3 camUp;		// カメラベクトル
	float lightRadXZ;				// ライトのXZ位置
	float lightRadY;				// ライトのY位置
	float lightH;					// ライトの色相
	float lightSV;					// ライトの彩度・明度
	DirectX::XMFLOAT4 color;		// ライトの色(RGB)
	int index;						// サブシーン番号
};
// ファイルパスを指定
const char* SettingFileName = "asset/Setting/setting.Didyouthinktext";

/// <summary>
/// 初期化処理(ファイルからカメラ位置を読み出し)
/// </summary>
void SceneMain::Init()
{
	// 保存データの読み込み
	ViewSetting setting =
	{
		DirectX::XMFLOAT3(0.0f, 1.0f, -5.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
		0.0f, DirectX::XM_PIDIV4,
		0.0f, 1.0f,
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		SCENE_VISUAL
	};
	FILE* fp;
	fopen_s(&fp, SettingFileName, "rb");
	if (fp)
	{
		fread(&setting, sizeof(ViewSetting), 1, fp);	// 書き込み
		fclose(fp);	// 閉じる
	}

	// カメラの作成
	Camera* pCamera = CreateObj<Camera>("Camera");
	pCamera->GameObjInit();
	pCamera->GetComponent<DXCamera>()->SetPos(setting.camPos);
	pCamera->GetComponent<DXCamera>()->SetLook(setting.camLook);
	pCamera->GetComponent<DXCamera>()->SetUp(setting.camUp);

	// ライトの作成
	MoveLight* pLight = CreateObj<MoveLight>("Light");
	pLight->GameObjInit();
	pLight->GetComponent<Light>()->SetRot(setting.lightRadXZ, setting.lightRadY);
	pLight->GetComponent<Light>()->SetHSV(setting.lightH, setting.lightSV);
	pLight->GetComponent<Light>()->SetAmbient(DirectX::XMFLOAT4(setting.color.x, setting.color.y, setting.color.z, setting.color.w));
	pLight->GetComponent<Light>()->UpdateParam();

	// マウスのトラッキング開始
	Input::Get()->EnableMouseTracking();
	// マウスを中央に戻さない
	Input::Get()->SetMouseCenter(false);

	// シーンの作成
	m_index = setting.index;
	ChangeScene();
}

/// <summary>
/// 解放処理(現在のカメラ位置をファイルに保存)
/// </summary>
void SceneMain::Uninit()
{
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// 位置をセーブする
	ViewSetting setting =
	{
		pCamera->GetComponent<DXCamera>()->GetPos(),
		pCamera->GetComponent<DXCamera>()->GetLook(),
		pCamera->GetComponent<DXCamera>()->GetUp(),
		pLight->GetComponent<Light>()->GetRotXZ(), pLight->GetComponent<Light>()->GetRotY(),
		pLight->GetComponent<Light>()->GetH(), pLight->GetComponent<Light>()->GetSV(),
		pLight->GetComponent<Light>()->GetAmbient(),
		m_index
	};
	FILE* fp;
	fopen_s(&fp, SettingFileName, "wb");
	if (fp)
	{
		fwrite(&setting, sizeof(ViewSetting), 1, fp);
		fclose(fp);
	}

	pCamera->Uninit();
	pLight->Uninit();

	// リストの解放
	ObjeList.clear();
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="tick">時間</param>
void SceneMain::Update(float tick)
{
	// 登録されているカメラとライトをとってくる
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// マウスを中央に戻すか戻さないか
	if (Input::Get()->GetKeyPress(VK_RBUTTON))
	{
		// マウスを中央に戻す
		Input::Get()->SetMouseCenter(true);

		// マウスカーソルの非表示
		do
		{
			i_duplicationMouse = ShowCursor(0);

		} while (i_duplicationMouse >= 0);
	}
	else
	{
		// マウスを中央に戻さない
		Input::Get()->SetMouseCenter(false);

		// マウスカーソルの表示
		do
		{
			i_duplicationMouse = ShowCursor(1);

		} while (i_duplicationMouse < 0);
	}

	// shiftキーを押していなければ更新して返す
	if (!Input::GetKeyTrigger(VK_SHIFT))
	{
		pCamera->Update();
		pLight->Update();
		return;
	}

	// SHIFTキーが押されてれば、シーンの切り替え処理
	int idx = m_index;
	if (Input::GetKeyTrigger(VK_LEFT)) --idx;
	if (Input::GetKeyTrigger(VK_RIGHT)) ++idx;

	// 最大数を超えないように
	if (idx < 0) idx = SCENE_MAX - 1;
	if (idx >= SCENE_MAX) idx = 0;

	if (idx != m_index)
	{
		m_index = idx;
		RemoveSubScene();
		ChangeScene();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void SceneMain::Draw()
{
	// 登録されているカメラとライトをとってくる
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// ジオメトリに行列を渡す
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, DirectX::XMMatrixIdentity());
	Geometory::SetWorld(fmat);
	Geometory::SetView(pCamera->GetComponent<DXCamera>()->GetView());
	Geometory::SetProjection(pCamera->GetComponent<DXCamera>()->GetProj());

	// オブジェクト描画
	pCamera->DefaultDraw();		// カメラオブジェクト
	pLight->DefaultDraw();		// 照明オブジェクト
}
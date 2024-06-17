//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "DXCamera.h"
//-------------------------------------------------------
// オプション関係
#include "main.h"
#include "renderer.h"
//-------------------------------------------------------
// 入力情報関係
#include "input.h"
//-------------------------------------------------------
// シーン関係
#include "manager.h"	// 現在のシーンを取得するため
#include "camera.h"
//-----------------------------------------------------------------------------
//      ステート
//-----------------------------------------------------------------------------
enum CameraDCCKind
{
	CAM_DCC_NONE,		// カメラ制御なし
	CAM_DCC_ORBIT,		// 回転制御
	CAM_DCC_TRACK,		// 平行移動制御	
	CAM_DCC_DOLLY,		// ズーム制御
	CAM_DCC_FLIGHT,		// 自由移動制御
};
//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
void DXCamera::Init()
{
	m_pos = (DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));									// 位置
	m_look = (DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));										// 視点
	m_up = (DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));										// ベクトル
	m_fovy = (DirectX::XMConvertToRadians(60.0f));										// 視野
	m_aspect = (static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT));	// アスペクト比
	m_near = (0.01f);																	// ニアグリップ
	m_far = (1000.0f);																	// ファーグリップ

	m_state = (CAM_DCC_NONE);	// 現在の状態
	m_oldPos = { 0, 0 };		// マウスの前回座標
}
//-----------------------------------------------------------------------------
//      描画処理です.
//-----------------------------------------------------------------------------
void DXCamera::Draw()
{
	DirectX::SimpleMath::Matrix mat;
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_look.x, m_look.y, m_look.z);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f);
	mat = S * T;
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, DirectX::XMMatrixTranspose(mat));		// 転置

	//プロジェクション行列の生成
	DirectX::SimpleMath::Matrix projectionMatrix;
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_near, m_far);	// 左手系にした　20230511 by suzuki.tomoki
}
//-----------------------------------------------------------------------------
//      更新処理です.
//-----------------------------------------------------------------------------
void DXCamera::Update()
{
	UpdateState();

	if (m_state == CAM_DCC_NONE) return;

	Argument arg;
	// マウス移動量を取得する
	POINT cursorPos;
	cursorPos = Input::Get()->GetMouseMove();
	arg.mouseMove = DirectX::XMFLOAT2((float)cursorPos.x, (float)cursorPos.y);
	// カメラ情報
	arg.vCamPos = DirectX::XMLoadFloat3(&m_pos);
	arg.vCamLook = DirectX::XMLoadFloat3(&m_look);
	DirectX::XMVECTOR vCamUp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_up));
	DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(arg.vCamLook, arg.vCamPos);
	// カメラ姿勢
	arg.vCamFront = DirectX::XMVector3Normalize(vFront);
	arg.vCamSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vCamUp, arg.vCamFront));
	arg.vCamUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(arg.vCamFront, arg.vCamSide));
	// フォーカス距離
	DirectX::XMStoreFloat(&arg.focus, DirectX::XMVector3Length(vFront));
	//-----------------------------------------------------------------------------
	// それぞれのステートの更新処理をする
	switch (m_state)
	{
	case CAM_DCC_ORBIT:		UpdateOrbit(arg);	break;
	case CAM_DCC_TRACK:		UpdateTrack(arg);	break;
	case CAM_DCC_DOLLY:		UpdateDolly(arg);	break;
	case CAM_DCC_FLIGHT:	UpdateFlight(arg);	break;
	}

	//-----------------------------------------------------------------------------
	// 最後に計算したそれぞれの変数を変換してビューマトリックスに値を与える
	// 
	// XMFLOAT3→XMVECTORへ変換
	DirectX::SimpleMath::Vector3 eye = DirectX::XMLoadFloat3(&m_pos);			// 位置
	DirectX::SimpleMath::Vector3 focus = DirectX::XMLoadFloat3(&m_look);		// 視点
	DirectX::SimpleMath::Vector3 up = DirectX::XMLoadFloat3(&m_up);				// ベクトル

	// ビュー変換後列作成
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);
}
//-----------------------------------------------------------------------------
//      ビューを返す
//-----------------------------------------------------------------------------
DirectX::XMFLOAT4X4 DXCamera::GetView(bool transpose)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&m_pos), DirectX::XMLoadFloat3(&m_look), DirectX::XMLoadFloat3(&m_up)
	);
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, mat);
	return fmat;
}
//-----------------------------------------------------------------------------
//      プロジェクションを返す
//-----------------------------------------------------------------------------
DirectX::XMFLOAT4X4 DXCamera::GetProj(bool transpose)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_near, m_far);
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, mat);
	return fmat;
}
//-----------------------------------------------------------------------------
//      位置を返す
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 DXCamera::GetPos()
{
	return m_pos;
}
//-----------------------------------------------------------------------------
//      視点を返す
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 DXCamera::GetLook()
{
	return m_look;
}
//-----------------------------------------------------------------------------
//      ベクトルを返す
//-----------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 DXCamera::GetUp()
{
	return m_up;
}
//-----------------------------------------------------------------------------
//      位置をセットする
//-----------------------------------------------------------------------------
void DXCamera::SetPos(DirectX::SimpleMath::Vector3 pos)
{
	m_pos = pos;
}
//-----------------------------------------------------------------------------
//      視点をセットする
//-----------------------------------------------------------------------------
void DXCamera::SetLook(DirectX::SimpleMath::Vector3 look)
{
	m_look = look;
}
//-----------------------------------------------------------------------------
//      ベクトルをセットする
//-----------------------------------------------------------------------------
void DXCamera::SetUp(DirectX::SimpleMath::Vector3 up)
{
	m_up = up;
}
//-----------------------------------------------------------------------------
//      ステートごとの更新判定
//-----------------------------------------------------------------------------
void DXCamera::UpdateState()
{
	// 現在のステート
	CameraDCCKind prev = (CameraDCCKind)m_state;

	// ALT + ○○
	if (Input::GetKeyPress(VK_MENU))
	{
		m_state = CAM_DCC_NONE;

		// 初期化する
		if (Input::GetKeyPress(VK_SHIFT))
		{
			// 初期化する
			m_pos = (DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));									// 位置
			m_look = (DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));										// 視点
			m_up = (DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));										// ベクトル
		}

		if (Input::GetKeyPress(VK_LBUTTON)) m_state = CAM_DCC_ORBIT;		// 回転制御
		if (Input::GetKeyPress(VK_MBUTTON)) m_state = CAM_DCC_TRACK;		// 平行移動制御
		if (Input::GetKeyPress(VK_RBUTTON)) m_state = CAM_DCC_DOLLY;		// ズーム制御
	}
	else if (Input::GetKeyPress(VK_RBUTTON))
	{
		// 左クリックが押されたとき
		m_state = CAM_DCC_FLIGHT;										// 自由移動
	}
	else
	{
		m_state = CAM_DCC_NONE;
	}
	if (prev != m_state)
	{
		// マウスの移動を記憶する
		GetCursorPos(&m_oldPos);
	}
}
//-----------------------------------------------------------------------------
//      回転制御更新処理
//-----------------------------------------------------------------------------
void DXCamera::UpdateOrbit(Argument& arg)
{
	// マウスの移動量 / 画面サイズ の比率から、画面全体でどれだけ回転するか指定する。
	float angleX = 360.0f * arg.mouseMove.x / SCREEN_WIDTH;
	float angleY = 180.0f * arg.mouseMove.y / SCREEN_HEIGHT;

	// Y軸回転でカメラの横軸を回転
	DirectX::XMMATRIX matRotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matRotY));

	// 回転後の軸で縦回転
	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationAxis(vAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(arg.vCamFront, arg.focus);
	vRelative = DirectX::XMVector3TransformCoord(vRelative, matRotY * matRotSide);

	// カメラ座標更新
	DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorSubtract(arg.vCamLook, vRelative));

	// アップベクトル更新
	DirectX::XMStoreFloat3(&m_up, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vRelative), vAxis)));
}
//-----------------------------------------------------------------------------
//      平行制御更新処理
//-----------------------------------------------------------------------------
void DXCamera::UpdateTrack(Argument& arg)
{
	// 高さA、底辺Bとする三角形について tanΘ = A / Bが成り立つ
	// 上記式をもとに割り出した遠景の高さを、移動量 / 画面サイズ の比率から、移動量として求める
	float farScreenHeight = tanf(m_fovy * 0.5f) * m_far;
	float screenRateW = arg.mouseMove.x / 640.0f;
	float screenRateH = arg.mouseMove.y / 360.0f;
	float farMoveX = -farScreenHeight * screenRateW * m_aspect;
	float farMoveY = farScreenHeight * screenRateH;

	// カメラの姿勢をもとに移動
	float rate = arg.focus / m_far;
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamSide, farMoveX * rate));
	vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamUp, farMoveY * rate));
	DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(arg.vCamPos, vCamMove));
	DirectX::XMStoreFloat3(&m_look, DirectX::XMVectorAdd(arg.vCamLook, vCamMove));
}
//-----------------------------------------------------------------------------
//      ズーム制御更新処理
//-----------------------------------------------------------------------------
void DXCamera::UpdateDolly(Argument& arg)
{
	float rate = arg.focus / m_far;

	DirectX::XMVECTOR vMove = DirectX::XMVectorScale(arg.vCamFront, m_far * 0.01f * rate * (arg.mouseMove.x + arg.mouseMove.y));
	DirectX::XMStoreFloat3(&m_pos, DirectX::XMVectorAdd(arg.vCamPos, vMove));

	//printf("%f \n", arg.focus);
}
//-----------------------------------------------------------------------------
//      自由移動更新処理
//-----------------------------------------------------------------------------
void DXCamera::UpdateFlight(Argument& arg)
{
	// マウスの移動量 / 画面サイズ の比率から、画面全体でどれだけ回転するか指定する。
	float angleX = 360.0f * arg.mouseMove.x / SCREEN_WIDTH;
	float angleY = 180.0f * arg.mouseMove.y / SCREEN_HEIGHT;

	// 横回転
	DirectX::XMMATRIX matUpRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
	DirectX::XMVECTOR vSideAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matUpRot));

	// 縦回転
	DirectX::XMMATRIX matSideRot = DirectX::XMMatrixRotationAxis(vSideAxis, DirectX::XMConvertToRadians(angleY));
	DirectX::XMVECTOR vFrontAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamFront, matUpRot * matSideRot));

	// キー入力で移動
	DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
	if (Input::GetKeyPress('W')) vCamMove = DirectX::XMVectorAdd(vCamMove, vFrontAxis);
	if (Input::GetKeyPress('S')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vFrontAxis);
	if (Input::GetKeyPress('A')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vSideAxis);
	if (Input::GetKeyPress('D')) vCamMove = DirectX::XMVectorAdd(vCamMove, vSideAxis);
	if (Input::GetKeyPress('Q')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	if (Input::GetKeyPress('E')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));

	// 初期化する
	if (Input::GetKeyPress(VK_SHIFT))
	{
		vCamMove = DirectX::XMVectorScale(vCamMove, 0.1f * m_MoveVal);		// 移動 * 倍率
	}
	else
	{
		vCamMove = DirectX::XMVectorScale(vCamMove, 0.1f);					// 通常移動
	}

	// 更新
	DirectX::XMVECTOR vCamPos = DirectX::XMVectorAdd(arg.vCamPos, vCamMove);
	DirectX::XMStoreFloat3(&m_pos, vCamPos);
	DirectX::XMStoreFloat3(&m_look, DirectX::XMVectorAdd(vCamPos, DirectX::XMVectorScale(vFrontAxis, arg.focus)));
	DirectX::XMStoreFloat3(&m_up, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFrontAxis, vSideAxis)));
}
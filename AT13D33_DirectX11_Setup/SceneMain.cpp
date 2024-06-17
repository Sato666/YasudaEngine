//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SceneMain.h"
#include <stdio.h>

// �V�[��
#include "SceneEdit.h"

// �W�I���g��
#include "Geometory.h"

// �Z�p���C�u����
#include <DirectXMath.h>

/// <summary>
/// �T�u�V�[���̍\����
/// </summary>
enum SceneKind
{
	SCENE_VISUAL,		// default�T�u�V�[��
	SCENE_MAX			// �ő吔
};

/// <summary>
/// �V�[���̕ύX
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
/// ����N�����ɃJ�������ƃ��C�g���ƌ��݂̃T�u�V�[������������
/// </summary>
struct ViewSetting
{
	DirectX::XMFLOAT3 camPos;		// �J�����ʒu
	DirectX::XMFLOAT3 camLook;		// �J�������_
	DirectX::XMFLOAT3 camUp;		// �J�����x�N�g��
	float lightRadXZ;				// ���C�g��XZ�ʒu
	float lightRadY;				// ���C�g��Y�ʒu
	float lightH;					// ���C�g�̐F��
	float lightSV;					// ���C�g�̍ʓx�E���x
	DirectX::XMFLOAT4 color;		// ���C�g�̐F(RGB)
	int index;						// �T�u�V�[���ԍ�
};
// �t�@�C���p�X���w��
const char* SettingFileName = "asset/Setting/setting.Didyouthinktext";

/// <summary>
/// ����������(�t�@�C������J�����ʒu��ǂݏo��)
/// </summary>
void SceneMain::Init()
{
	// �ۑ��f�[�^�̓ǂݍ���
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
		fread(&setting, sizeof(ViewSetting), 1, fp);	// ��������
		fclose(fp);	// ����
	}

	// �J�����̍쐬
	Camera* pCamera = CreateObj<Camera>("Camera");
	pCamera->GameObjInit();
	pCamera->GetComponent<DXCamera>()->SetPos(setting.camPos);
	pCamera->GetComponent<DXCamera>()->SetLook(setting.camLook);
	pCamera->GetComponent<DXCamera>()->SetUp(setting.camUp);

	// ���C�g�̍쐬
	MoveLight* pLight = CreateObj<MoveLight>("Light");
	pLight->GameObjInit();
	pLight->GetComponent<Light>()->SetRot(setting.lightRadXZ, setting.lightRadY);
	pLight->GetComponent<Light>()->SetHSV(setting.lightH, setting.lightSV);
	pLight->GetComponent<Light>()->SetAmbient(DirectX::XMFLOAT4(setting.color.x, setting.color.y, setting.color.z, setting.color.w));
	pLight->GetComponent<Light>()->UpdateParam();

	// �}�E�X�̃g���b�L���O�J�n
	Input::Get()->EnableMouseTracking();
	// �}�E�X�𒆉��ɖ߂��Ȃ�
	Input::Get()->SetMouseCenter(false);

	// �V�[���̍쐬
	m_index = setting.index;
	ChangeScene();
}

/// <summary>
/// �������(���݂̃J�����ʒu���t�@�C���ɕۑ�)
/// </summary>
void SceneMain::Uninit()
{
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// �ʒu���Z�[�u����
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

	// ���X�g�̉��
	ObjeList.clear();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="tick">����</param>
void SceneMain::Update(float tick)
{
	// �o�^����Ă���J�����ƃ��C�g���Ƃ��Ă���
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// �}�E�X�𒆉��ɖ߂����߂��Ȃ���
	if (Input::Get()->GetKeyPress(VK_RBUTTON))
	{
		// �}�E�X�𒆉��ɖ߂�
		Input::Get()->SetMouseCenter(true);

		// �}�E�X�J�[�\���̔�\��
		do
		{
			i_duplicationMouse = ShowCursor(0);

		} while (i_duplicationMouse >= 0);
	}
	else
	{
		// �}�E�X�𒆉��ɖ߂��Ȃ�
		Input::Get()->SetMouseCenter(false);

		// �}�E�X�J�[�\���̕\��
		do
		{
			i_duplicationMouse = ShowCursor(1);

		} while (i_duplicationMouse < 0);
	}

	// shift�L�[�������Ă��Ȃ���΍X�V���ĕԂ�
	if (!Input::GetKeyTrigger(VK_SHIFT))
	{
		pCamera->Update();
		pLight->Update();
		return;
	}

	// SHIFT�L�[��������Ă�΁A�V�[���̐؂�ւ�����
	int idx = m_index;
	if (Input::GetKeyTrigger(VK_LEFT)) --idx;
	if (Input::GetKeyTrigger(VK_RIGHT)) ++idx;

	// �ő吔�𒴂��Ȃ��悤��
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
/// �`�揈��
/// </summary>
void SceneMain::Draw()
{
	// �o�^����Ă���J�����ƃ��C�g���Ƃ��Ă���
	Camera* pCamera = GetObj<Camera>("Camera");
	MoveLight* pLight = GetObj<MoveLight>("Light");

	// �W�I���g���ɍs���n��
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, DirectX::XMMatrixIdentity());
	Geometory::SetWorld(fmat);
	Geometory::SetView(pCamera->GetComponent<DXCamera>()->GetView());
	Geometory::SetProjection(pCamera->GetComponent<DXCamera>()->GetProj());

	// �I�u�W�F�N�g�`��
	pCamera->DefaultDraw();		// �J�����I�u�W�F�N�g
	pLight->DefaultDraw();		// �Ɩ��I�u�W�F�N�g
}
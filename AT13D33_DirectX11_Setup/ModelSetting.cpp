#include "ModelSetting.h"

void ModelSetting::Init()
{
	// �����l
	m_OriginalSetting = false;	// �V�[���̋��ʒl�ɏ]�����ǂ���
	m_OutLineDraw = true;		// �A�E�g���C����`�悷�邩�ǂ���

	m_OutLineVal = 0.001f;	// �֊s���̕�
	m_OutLineColor = { 0.0f,0.0f,0.0f,1.0f };		// �֊s���̐F�ϐ�
}

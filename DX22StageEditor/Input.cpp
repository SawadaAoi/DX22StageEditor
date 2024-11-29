/* ========================================
	DX22Base/
	------------------------------------
	�L�[&�R���g���[���[���͗pcpp
	------------------------------------
	Input.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "Input.h"
#include "TimeManager.h"



// =============== �萔��` =======================
const float INITIAL_REPEAT_DELAY	= 0.5f; // �������s�[�g�x��
const float REPEAT_INTERVAL			= 0.1f; // ���s�[�g�Ԋu


/* ========================================
	�������֐�
	-------------------------------------
	���e�F����������
=========================================== */
void Input::Init()
{
	// �L�[�{�[�h�̓��͏����擾
	if (!GetKeyboardState(m_InputKeysNow))
	{
		MessageBox(nullptr, TEXT("�L�[�{�[�h�̏������Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
	}

	// m_aInputKeys_Old��m_aInputKeys_Now�̓��e���R�s�[
	memcpy_s(m_InputKeysOld, sizeof(m_InputKeysOld), m_InputKeysNow, sizeof(m_InputKeysNow));

	// �L�[���s�[�g�^�C�}�[�̏�����
	m_fKeyRepeatTime.fill(0.0f);
	
}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
=========================================== */
void Input::Update()
{
	// m_aInputKeys_Old��m_aInputKeys_Now�̓��e���R�s�[
	memcpy_s(m_InputKeysOld, sizeof(m_InputKeysOld), m_InputKeysNow, sizeof(m_InputKeysNow));

	// �L�[�{�[�h�̓��͏����擾
	if (!GetKeyboardState(m_InputKeysNow))
	{
		MessageBox(nullptr, TEXT("�L�[�{�[�h�̍X�V�Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
	}

	RepeatUpdate();	// �L�[���s�[�g����
}


/* ========================================
	�L�[����������֐�
	-------------------------------------
	���e�F�L�[�������Ă���ԏ������s��
=========================================== */
bool Input::IsKeyPress(BYTE key)
{
	return m_InputKeysNow[key] & 0x80;	// 0x80��10000000��16�i���\�L(1���ł�1�������true)
}

/* ========================================
	�L�[�����J�n����֐�
	-------------------------------------
	���e�F�L�[�������ꂽ�Ƃ��ɏ������s��
=========================================== */
bool Input::IsKeyTrigger(BYTE key)
{
	// ���̃L�[�ƑO�̃L�[�̔r���I�_���a�����A���̃L�[�̏�Ԃ��擾
	// ���̃L�[�̏�Ԃ�1�ŁA�O�̃L�[�̏�Ԃ�0�̏ꍇ�A�g���K�[�Ƃ���true��Ԃ�
	return (m_InputKeysNow[key] ^ m_InputKeysOld[key]) & m_InputKeysNow[key] & 0x80;
}

/* ========================================
	�L�[�����I������֐�
	-------------------------------------
	���e�F�L�[���������㗣�����Ƃ��ɍs������
=========================================== */
bool Input::IsKeyRelease(BYTE key)
{
	// ���̃L�[�ƑO�̃L�[�̔r���I�_���a�����A�O�̃L�[�̏�Ԃ��擾
	// ���̃L�[�̏�Ԃ�0�ŁA�O�̃L�[�̏�Ԃ�1�̏ꍇ�A�����[�X�Ƃ���true��Ԃ�
	return (m_InputKeysNow[key] ^ m_InputKeysOld[key]) & m_InputKeysOld[key] & 0x80;
}

/* ========================================
	�L�[��������t���O
	-------------------------------------
	���e�F�����L�[�����������Ă���ԍs������
=========================================== */
bool Input::IsKeyRepeat(BYTE key)
{
	// �L�[��������Ă����Ԃ����s�[�g�x�����Ԃ�0�ȉ��̏ꍇ
	// ���s�[�g�x�����Ԃ�0�ȉ��̏ꍇ�́A���s�[�g�Ԋu���Ԃ��o�߂����Ƃ���true��Ԃ�
	return IsKeyPress(key) && m_fKeyRepeatTime[key] <= 0.0f;
}

/* ========================================
	���s�[�g�����֐�
	-------------------------------------
	���e�F�L�[�̃��s�[�g�������s��
=========================================== */
void Input::RepeatUpdate()
{
	// �S�ẴL�[�̃^�C�}�[���X�V
	for (int key = 0; key < KEY_MAX; key++)
	{
		// �L�[��������Ă���ꍇ
		if (IsKeyPress(key))
		{
			// �V���ɃL�[�������ꂽ�ꍇ
			if (IsKeyTrigger(key))
			{
				m_fKeyRepeatTime[key] = INITIAL_REPEAT_DELAY;	// ���s�[�g�x�����Ԃ��Z�b�g(������Ă���ŏ��Ƀ��s�[�g�����܂ł̎���)
			}
			// �����ꑱ���Ă���ꍇ(��莞�Ԃ��ƂɃ��s�[�g�����)
			else
			{
				// ���s�[�g�x�����Ԃ����炷
				m_fKeyRepeatTime[key] -= DELTA_TIME;

				// ���s�[�g�x�����Ԃ�0�ȉ��ɂȂ����ꍇ
				if (m_fKeyRepeatTime[key] <= 0.0f)
				{
					// ���s�[�g�Ԋu���Ԃ��Z�b�g
					m_fKeyRepeatTime[key] = REPEAT_INTERVAL;
				}
			}
		}
		// �L�[�������ꂽ�ꍇ
		else
		{
			// �L�[�������ꂽ�ꍇ�A���s�[�g�x�����Ԃ�0�ɂ���
			m_fKeyRepeatTime[key] = 0.0f;
		}
	}
}



/* ========================================
	DX22Base/
	------------------------------------
	�L�[&�R���g���[���[���͗p�w�b�_
	------------------------------------
	�����F�L�[���͂ƃR���g���[���[���͂��Ǘ�����
	------------------------------------
	Input.h
========================================== */
#pragma once


// =============== �C���N���[�h ===================
#include <Windows.h>
#include "Vector3.h"
#include <array>


const int	KEY_MAX = 256;	// �L�[�̍ő吔

class Input
{
public:
	static void Init();
	static void Update();

	static bool IsKeyPress(BYTE key);
	static bool IsKeyTrigger(BYTE key);
	static bool IsKeyRelease(BYTE key);
	static bool IsKeyRepeat(BYTE key);

private:
	static void RepeatUpdate();

private:
	static inline BYTE m_InputKeysNow[KEY_MAX];		// �L�[�{�[�h�̓��͏��
	static inline BYTE m_InputKeysOld[KEY_MAX];		// �L�[�{�[�h�̓��͏��(�O��)
	static inline  std::array<float, KEY_MAX> m_fKeyRepeatTime;	// �L�[���s�[�g����
};
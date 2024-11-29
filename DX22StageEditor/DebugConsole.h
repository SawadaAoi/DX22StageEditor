/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�R���\�[���\���p�w�b�_
	------------------------------------
	�����F�ʃR���\�[���E�B���h�E��\�����A
		�@������\������
	------------------------------------
	DebugConsole.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include <string>		// std::string

class DebugConsole
{
public:
	static void Init();		// ������

	static void Update();

	static void Uninit();	// �I��

	static void Print(const std::string message);	// ������\��
	static void Printf(const char* format, ...);	// �����t���\��
};




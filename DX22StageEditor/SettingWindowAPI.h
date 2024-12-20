/* ========================================
	DX22Base/
	------------------------------------
	�E�B���h�E�ݒ�p�w�b�_
	------------------------------------
	����:�E�B���h�E�̐ݒ�萔���L�q
	
	------------------------------------
	SettingWindowAPI.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include <Windows.h>
#include <string>

namespace WindowConfig
{
	// ��ʃT�C�Y
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	// ���t���b�V�����[�g
	const int REFRESH_RATE = 120;

	// �E�B���h�E�^�C�g��
	const std::string APP_TITLE = "�X�e�[�W�G�f�B�^�[";
	const LPCSTR	WINDOW_NAME = "�X�e�[�W�G�f�B�^�[";

	// �A�X�y�N�g��
	const float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
}
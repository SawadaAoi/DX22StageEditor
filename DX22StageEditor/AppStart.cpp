/* ========================================
	DX22Base/
	------------------------------------
	�A�v���G���g���[�pcpp
	------------------------------------
	�����F��ԍŏ��ɌĂяo�����֐�
	------------------------------------
	AppStart.cpp
========================================== */


// =============== �C���N���[�h ===================
#include <windows.h>
#include "WindowAPI.h"
#include "AppController.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


/* ========================================
	�G���g���[�|�C���g�֐�
	-------------------------------------
	���e�F���s���ɍŏ��ɌĂяo�����֐�
	-------------------------------------
	����1�F�C���X�^���X�n���h��
	����2�F�ȑO�̃C���X�^���X�̃n���h��(�g�p���Ȃ�)
	����3�F�R�}���h���C������(�g�p���Ȃ�)
	����4�F�ŏ��ɃE�B���h�E���\�������ۂ̕\���I�v�V����(�g�p���Ȃ�)
=========================================== */
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ���������[�N���o��L����
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	// �E�B���h�E�̏������A�쐬�A�\��
	if (SUCCEEDED(WINDOW_INST.InitWindow(hInstance)))
	{
		WINDOW_INST.Run();	// ���C�����[�v

	}

	WINDOW_INST.Cleanup();	// �폜


	return 0;
}


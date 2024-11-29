/* ========================================
	DX22Base/
	------------------------------------
	�E�B���h�E�ݒ�p�w�b�_
	------------------------------------
	����:�E�B���h�E�̊e��ݒ���s��
	------------------------------------
	WindowAPI.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#pragma comment(lib, "winmm.lib")	// �E�B���h�E���b�Z�[�W�����p
#include <Windows.h>				// WindowsAPI
#include "AppController.h"

// =============== �萔��` =======================
#define WINDOW_INST WindowAPI::GetInstance()	// �E�B���h�E�N���X�̃C���X�^���X�擾


// =============== �N���X��` =====================
class WindowAPI
{
public:
	typedef struct
	{
		UINT nWidth;
		UINT nHeight;
	}SCREEN_SIZE;
public:
	static WindowAPI& GetInstance();	// �C���X�^���X�擾

	HRESULT InitWindow(HINSTANCE hInstance);	// �E�B���h�E������
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);			// ���b�Z�[�W����
	void Run();																		// ���C�����[�v
	void Cleanup();																	// �I������

	// �Q�b�^�[
	HWND GetHwnd();					// �E�B���h�E�n���h���擾
	SCREEN_SIZE GetScreenSize();	// ��ʃT�C�Y�擾
	UINT GetRefreshRate();			// ���t���b�V�����[�g�擾
	UINT GetOneFrame();				// 1�t���[���̎��Ԏ擾
	float GetAspectRatio();			// �A�X�y�N�g��擾
	bool GetActive();				// �A�N�e�B�u�t���O�擾

	// �Z�b�^�[
	void SetActive(bool bActive);	// �A�N�e�B�u�t���O�ݒ�

private:
	void InitWndClass(HINSTANCE hInstance);	// �E�B���h�E�N���X�̍\�����̏�����
	bool IsWindowMoving();					// �E�B���h�E���ړ������ǂ���
	bool IsWindowFocus();					// �E�B���h�E���t�H�[�J�X�����ǂ���

	WNDCLASSEX m_Wcex;	// �E�B���h�E�N���X�̍\�����
	HWND m_hWnd;		// �E�B���h�E�n���h��

	AppController m_AppController;	// �A�v������p
	float m_fRefreshRate;			// ���t���b�V�����[�g

	bool m_bActive;		// �A�N�e�B�u�t���O
};
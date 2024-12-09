/* ========================================
	DX11Base/
	------------------------------------
	�Q�[���A�v���J�n�pcpp
	------------------------------------
	StartWinMain.cpp
========================================== */


// =============== �C���N���[�h ===================
#include "WindowAPI.h"
#include "SettingWindowAPI.h"

#include <stdio.h>
#include <crtdbg.h>

#include "Input.h"

#include <ImGui/imgui_impl_win32.h>
#include "DebugMenu.h"

// �E�B���h�E�v���V�[�W���֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


/* ========================================
	�E�B���h�E�v���V�[�W���֐�
=========================================== */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return WINDOW_INST.MsgProc(hWnd, message, wParam, lParam);	// �����̓N���X�����Ă���̂ŁA�N���X�̃����o�֐����Ăяo��
}



/* ========================================
	�C���X�^���X�擾�֐�
	-------------------------------------
	���e�F�ÓI�ȃC���X�^���X���擾����
	-------------------------------------
	�ߒl�FWindowAPI�N���X�̃C���X�^���X
=========================================== */
WindowAPI& WindowAPI::GetInstance()
{
	static WindowAPI instance;
	return instance;
}

/* ========================================
	�E�B���h�E�������֐�
	-------------------------------------
	���e�F�E�B���h�E�̏������A�쐬�A�\�����s��
	-------------------------------------
	����1�F�C���X�^���X�n���h��
	����2�F��ʂ̉���
	����3�F��ʂ̏c��
	-------------------------------------
	�ߒl�FHRESULT�^�̖߂�l
=========================================== */
HRESULT WindowAPI::InitWindow(HINSTANCE hInstance)
{
	Input::Init();		// ���͏�����

	m_bActive = true;	// �A�N�e�B�u�t���O��true�ɐݒ�
	m_fRefreshRate = WindowConfig::REFRESH_RATE;	// ���t���b�V�����[�g��ݒ�

	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �E�B���h�E�N���X�̍\���w��
	InitWndClass(hInstance);

	// �E�B���h�E�N���X���̓o�^
	if (!RegisterClassEx(&m_Wcex))
	{
		MessageBox(NULL, "�E�B���h�E�N���X���̓o�^�Ɏ��s���܂���", "Error", MB_OK);
		return -1;
	}


	RECT	rect = { 0, 0, WindowConfig::SCREEN_WIDTH, WindowConfig::SCREEN_HEIGHT };	// �E�B���h�E�̃T�C�Y��ݒ�
	DWORD	style = WS_CAPTION | WS_SYSMENU | WS_OVERLAPPEDWINDOW;					// �E�B���h�E�X�^�C��(�^�C�g���o�[�ƃV�X�e�����j���[�̂�)
	DWORD	exStyle = WS_EX_OVERLAPPEDWINDOW;					// �g���E�B���h�E�X�^�C��

	AdjustWindowRectEx(&rect, style, false, exStyle);	// �E�B���h�E�̃T�C�Y�𒲐�(�E�B���h�E�̘g���܂߂��T�C�Y�ɕϊ�)

	m_hWnd = CreateWindowEx(
		exStyle,							// �g���E�B���h�E�X�^�C��
		m_Wcex.lpszClassName,				// �E�B���h�E�N���X��
		WindowConfig::APP_TITLE.c_str(),	// �E�B���h�E�^�C�g��	
		style,								// �E�B���h�E�X�^�C��	
		CW_USEDEFAULT,						// �E�B���h�E��X���W
		CW_USEDEFAULT,						// �E�B���h�E��Y���W
		rect.right - rect.left,				// �E�B���h�E�̕�
		rect.bottom - rect.top,				// �E�B���h�E�̍���
		HWND_DESKTOP,						// �e�E�B���h�E�̃n���h��
		NULL,								// ���j���[�n���h��
		hInstance, 							// �C���X�^���X�n���h��
		NULL								// ���̑��̃p�����[�^
	);



	ShowWindow(m_hWnd, SW_SHOW);	// �E�B���h�E�̕\��
	UpdateWindow(m_hWnd);			// �E�B���h�E�̍X�V

	// ����������
	if (FAILED(m_AppController.Init()))
	{
		m_AppController.Uninit();
		UnregisterClass(m_Wcex.lpszClassName, hInstance);
		return 0;
	}

	return 0;

}

/* ========================================
	�E�B���h�E�v���V�[�W���֐�
	-------------------------------------
	���e�FWindows�A�v���P�[�V�����ŃE�B���h�E��
		�@���b�Z�[�W�������s�����߂̃R�[���o�b�N�֐�
	-------------------------------------
	����1�F���b�Z�[�W���󂯎�����E�B���h�E�̃n���h��
	����2�F�������郁�b�Z�[�W�̎�ނ����������l
	����3�F���b�Z�[�W�Ɋւ���ǉ������i�[���� 32 �r�b�g�̃f�[�^(�����ꂽ�L�[�̃R�[�h��)
	����4�F���b�Z�[�W�Ɋ֘A����ǉ������i�[���� 32 �r�b�g�܂��� 64 �r�b�g�̃f�[�^(�}�E�X�C�x���g�ł���΁A�}�E�X�J�[�\���̍��W��)
=========================================== */
LRESULT WindowAPI::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	// ImGui�̃}�E�X���͓���L���ɂ���
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
#endif
	switch (message)
	{
		// �E�B���h�E���j�����ꂽ�Ƃ�
	case WM_DESTROY:
		PostQuitMessage(0);	// ���b�Z�[�W�L���[�ɏI�����b�Z�[�W�𑗂�
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);	// �f�t�H���g�̃E�B���h�E�v���V�[�W�����Ăяo��
}

/* ========================================
	�E�B���h�E���s�֐�
	-------------------------------------
	���e�F�Q�[���������s�����C�����[�v
=========================================== */
void WindowAPI::Run()
{
	MSG message;	// ���b�Z�[�W�\����

	//--- FPS����
	timeBeginPeriod(1);						// ����\��1ms�ɐݒ�(�^�C�~���O�֐�����萳�m�ɓ��삷��)
	DWORD countStartTime = timeGetTime();	// �J�E���^�̊J�n����
	DWORD preExecTime = countStartTime;		// �O��̎��s����

	// ���b�Z�[�W���[�v
	while (1)
	{
		// ���b�Z�[�W�����邩
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&message, NULL, 0, 0))	// �I�����b�Z�[�W�����邩
			{
				break;
			}
			else
			{
				TranslateMessage(&message);	// �L�[���̓��b�Z�[�W�̏���
				DispatchMessage(&message);	// ���b�Z�[�W�̏���
			}
		}
		// ���b�Z�[�W���Ȃ��ꍇ
		else
		{
			DWORD nowTime = timeGetTime();	// ���݂̎��Ԃ��擾
			float diff = static_cast<float>(nowTime - preExecTime);	// �O��̎��s���Ԃ���̍������擾
			// �t���[�����[�g����
			if (diff >= 1000.0f / m_fRefreshRate) {

				Input::Update();		// ���͍X�V

#ifdef _DEBUG	// �f�o�b�O���j���[�͕K���������ׂ�m_AppController�̊O�ɋL�q
				DebugUI::Menu::Update();

				m_bActive		= !WIN_BASIC_SETTING["Draw"]["Pause"].GetBool();			// �|�[�Y�t���O���擾
				m_fRefreshRate	= WIN_BASIC_SETTING["Draw"]["MaxFPS"].GetFloat();		// ���t���b�V�����[�g���擾
				WIN_BASIC_SETTING["Draw"]["FPS"].SetText(std::to_string(1000.0f / diff));	// FPS���擾
#endif

				// �|�[�Y������Ȃ����E�B���h�E���ړ�������Ȃ����E�B���h�E���t�H�[�J�X����Ă���ꍇ
				if (m_bActive && !IsWindowMoving() && IsWindowFocus())
				{
					m_AppController.Update(diff * 0.001f);	// 0.001f�̓~���b��b�ɕϊ����邽��

				}
				m_AppController.Draw();

				preExecTime = nowTime;	// �O��̎��s���Ԃ��X�V
			}

		}
	}

}

/* ========================================
	�I�������֐�
	-------------------------------------
	���e�F�I���������s��
=========================================== */
void WindowAPI::Cleanup()
{
	timeEndPeriod(1);	// ����\�����ɖ߂�

	m_AppController.Uninit();	// DX11Base�̏I������

	UnregisterClass(m_Wcex.lpszClassName, m_Wcex.hInstance);	// �E�B���h�E�N���X�̓o�^����

}


/* ========================================
	�E�B���h�N���X���ݒ�֐�
	-------------------------------------
	���e�F�E�B���h�E�N���X�\������ݒ肷��
	-------------------------------------
	����1�F�C���X�^���X�n���h��
=========================================== */
void WindowAPI::InitWndClass(HINSTANCE hInstance)
{
	ZeroMemory(&m_Wcex, sizeof(m_Wcex));							// �\���̂̏�����		
	m_Wcex.cbSize			= sizeof(WNDCLASSEX);					// �^�̑傫�����w��	
	m_Wcex.style			= CS_CLASSDC | CS_DBLCLKS;				// �E�B���h�E�X�^�C��
	m_Wcex.lpfnWndProc		= WndProc;								// �E�B���h�E�v���V�[�W�����w��
	m_Wcex.cbClsExtra		= 0;									// �g���p�̃������̈�1
	m_Wcex.cbWndExtra		= 0;									// �g���p�̃������̈�2
	m_Wcex.hInstance		= hInstance;							// �C���X�^���X�n���h��
	m_Wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);		// �A�C�R���̎w��
	m_Wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);			// �J�[�\���̎w��
	m_Wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	// �w�i�F�̎w��
	m_Wcex.lpszMenuName		= NULL;									// ���j���[��(��:�t�@�C�� (F)��)
	m_Wcex.lpszClassName	= WindowConfig::WINDOW_NAME;			// �E�B���h�E�N���X��
	m_Wcex.hIconSm			= m_Wcex.hIcon;							// �A�C�R���̎w��
}

bool WindowAPI::IsWindowMoving()
{
	static RECT lastRect = { 0 };
	RECT currentRect;

	// ���݂̃E�B���h�E�ʒu���擾
	if (GetWindowRect(m_hWnd, &currentRect)) {
		// �O��̈ʒu�ƈقȂ邩�ǂ������`�F�b�N
		if (currentRect.left != lastRect.left || currentRect.top != lastRect.top) {
			lastRect = currentRect;
			return true; // �E�B���h�E���ړ����Ă���
		}
	}
	return false; // �ړ����Ă��Ȃ�
}

bool WindowAPI::IsWindowFocus()
{
	// �t�H�[�J�X����Ă���E�B���h�E�̃n���h�����擾
	HWND foregroundWindow = GetForegroundWindow();

	// �t�H�[�J�X����Ă���E�B���h�E�������̃E�B���h�E���ǂ������`�F�b�N
	return (foregroundWindow == m_hWnd);
}



/* ========================================
	�E�B���h�E�n���h���擾�֐�
	-------------------------------------
	���e�F�E�B���h�E�n���h�����擾����
	-------------------------------------
	�ߒl�FHWND�^�̃E�B���h�E�n���h��
=========================================== */
HWND WindowAPI::GetHwnd()
{
	return m_hWnd;
}

/* ========================================
	��ʃT�C�Y�擾�֐�
	-------------------------------------
	���e�F��ʂ̃T�C�Y���擾����
	-------------------------------------
	�ߒl�FSCREEN_SIZE�^�̉�ʃT�C�Y
=========================================== */
WindowAPI::SCREEN_SIZE WindowAPI::GetScreenSize()
{
	SCREEN_SIZE reSize = {
		WindowConfig::SCREEN_WIDTH ,
		WindowConfig::SCREEN_HEIGHT
	};

	return reSize;
}

/* ========================================
	���t���b�V�����[�g�擾�֐�
	-------------------------------------
	���e�F���t���b�V�����[�g���擾����
	-------------------------------------
	�ߒl�FUINT�^�̃��t���b�V�����[�g
=========================================== */
UINT WindowAPI::GetRefreshRate()
{
	return WindowConfig::REFRESH_RATE;
}

/* ========================================
	1�t���[���̎��Ԏ擾�֐�
	-------------------------------------
	���e�F1�t���[���̎��Ԃ��擾����
	-------------------------------------
	�ߒl�FUINT�^��1�t���[���̎���
=========================================== */
UINT WindowAPI::GetOneFrame()
{
	return 1.0f / WindowConfig::REFRESH_RATE;
}

/* ========================================
	�A�X�y�N�g��擾�֐�
	-------------------------------------
	���e�F��ʃA�X�y�N�g����擾����
	-------------------------------------
	�ߒl�Ffloat�^�̃A�X�y�N�g��
=========================================== */
float WindowAPI::GetAspectRatio()
{
	return WindowConfig::ASPECT_RATIO;
}

/* ========================================
	�Q�b�^�[(�A�N�e�B�u�t���O�擾)
	-------------------------------------
	�ߒl�F�A�N�e�B�u�t���O
=========================================== */
bool WindowAPI::GetActive()
{
	return m_bActive;
}

/* ========================================
	�Z�b�^�[(�A�N�e�B�u�t���O�ݒ�)
	-------------------------------------
	�����F�A�N�e�B�u�t���O
=========================================== */
void WindowAPI::SetActive(bool bActive)
{
	m_bActive = bActive;
}

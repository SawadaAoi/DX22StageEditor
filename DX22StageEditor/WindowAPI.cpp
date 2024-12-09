/* ========================================
	DX11Base/
	------------------------------------
	ゲームアプリ開始用cpp
	------------------------------------
	StartWinMain.cpp
========================================== */


// =============== インクルード ===================
#include "WindowAPI.h"
#include "SettingWindowAPI.h"

#include <stdio.h>
#include <crtdbg.h>

#include "Input.h"

#include <ImGui/imgui_impl_win32.h>
#include "DebugMenu.h"

// ウィンドウプロシージャ関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


/* ========================================
	ウィンドウプロシージャ関数
=========================================== */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return WINDOW_INST.MsgProc(hWnd, message, wParam, lParam);	// 処理はクラス化しているので、クラスのメンバ関数を呼び出す
}



/* ========================================
	インスタンス取得関数
	-------------------------------------
	内容：静的なインスタンスを取得する
	-------------------------------------
	戻値：WindowAPIクラスのインスタンス
=========================================== */
WindowAPI& WindowAPI::GetInstance()
{
	static WindowAPI instance;
	return instance;
}

/* ========================================
	ウィンドウ初期化関数
	-------------------------------------
	内容：ウィンドウの初期化、作成、表示を行う
	-------------------------------------
	引数1：インスタンスハンドル
	引数2：画面の横幅
	引数3：画面の縦幅
	-------------------------------------
	戻値：HRESULT型の戻り値
=========================================== */
HRESULT WindowAPI::InitWindow(HINSTANCE hInstance)
{
	Input::Init();		// 入力初期化

	m_bActive = true;	// アクティブフラグをtrueに設定
	m_fRefreshRate = WindowConfig::REFRESH_RATE;	// リフレッシュレートを設定

	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ウィンドウクラスの構成指定
	InitWndClass(hInstance);

	// ウィンドウクラス情報の登録
	if (!RegisterClassEx(&m_Wcex))
	{
		MessageBox(NULL, "ウィンドウクラス情報の登録に失敗しました", "Error", MB_OK);
		return -1;
	}


	RECT	rect = { 0, 0, WindowConfig::SCREEN_WIDTH, WindowConfig::SCREEN_HEIGHT };	// ウィンドウのサイズを設定
	DWORD	style = WS_CAPTION | WS_SYSMENU | WS_OVERLAPPEDWINDOW;					// ウィンドウスタイル(タイトルバーとシステムメニューのみ)
	DWORD	exStyle = WS_EX_OVERLAPPEDWINDOW;					// 拡張ウィンドウスタイル

	AdjustWindowRectEx(&rect, style, false, exStyle);	// ウィンドウのサイズを調整(ウィンドウの枠を含めたサイズに変換)

	m_hWnd = CreateWindowEx(
		exStyle,							// 拡張ウィンドウスタイル
		m_Wcex.lpszClassName,				// ウィンドウクラス名
		WindowConfig::APP_TITLE.c_str(),	// ウィンドウタイトル	
		style,								// ウィンドウスタイル	
		CW_USEDEFAULT,						// ウィンドウのX座標
		CW_USEDEFAULT,						// ウィンドウのY座標
		rect.right - rect.left,				// ウィンドウの幅
		rect.bottom - rect.top,				// ウィンドウの高さ
		HWND_DESKTOP,						// 親ウィンドウのハンドル
		NULL,								// メニューハンドル
		hInstance, 							// インスタンスハンドル
		NULL								// その他のパラメータ
	);



	ShowWindow(m_hWnd, SW_SHOW);	// ウィンドウの表示
	UpdateWindow(m_hWnd);			// ウィンドウの更新

	// 初期化処理
	if (FAILED(m_AppController.Init()))
	{
		m_AppController.Uninit();
		UnregisterClass(m_Wcex.lpszClassName, hInstance);
		return 0;
	}

	return 0;

}

/* ========================================
	ウィンドウプロシージャ関数
	-------------------------------------
	内容：Windowsアプリケーションでウィンドウの
		　メッセージ処理を行うためのコールバック関数
	-------------------------------------
	引数1：メッセージを受け取ったウィンドウのハンドル
	引数2：処理するメッセージの種類を示す整数値
	引数3：メッセージに関する追加情報を格納する 32 ビットのデータ(押されたキーのコード等)
	引数4：メッセージに関連する追加情報を格納する 32 ビットまたは 64 ビットのデータ(マウスイベントであれば、マウスカーソルの座標等)
=========================================== */
LRESULT WindowAPI::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	// ImGuiのマウス入力等を有効にする
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
#endif
	switch (message)
	{
		// ウィンドウが破棄されたとき
	case WM_DESTROY:
		PostQuitMessage(0);	// メッセージキューに終了メッセージを送る
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);	// デフォルトのウィンドウプロシージャを呼び出す
}

/* ========================================
	ウィンドウ実行関数
	-------------------------------------
	内容：ゲーム処理を行うメインループ
=========================================== */
void WindowAPI::Run()
{
	MSG message;	// メッセージ構造体

	//--- FPS制御
	timeBeginPeriod(1);						// 分解能を1msに設定(タイミング関数がより正確に動作する)
	DWORD countStartTime = timeGetTime();	// カウンタの開始時間
	DWORD preExecTime = countStartTime;		// 前回の実行時間

	// メッセージループ
	while (1)
	{
		// メッセージがあるか
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&message, NULL, 0, 0))	// 終了メッセージがあるか
			{
				break;
			}
			else
			{
				TranslateMessage(&message);	// キー入力メッセージの処理
				DispatchMessage(&message);	// メッセージの処理
			}
		}
		// メッセージがない場合
		else
		{
			DWORD nowTime = timeGetTime();	// 現在の時間を取得
			float diff = static_cast<float>(nowTime - preExecTime);	// 前回の実行時間からの差分を取得
			// フレームレート制御
			if (diff >= 1000.0f / m_fRefreshRate) {

				Input::Update();		// 入力更新

#ifdef _DEBUG	// デバッグメニューは必ず動かす為にm_AppControllerの外に記述
				DebugUI::Menu::Update();

				m_bActive		= !WIN_BASIC_SETTING["Draw"]["Pause"].GetBool();			// ポーズフラグを取得
				m_fRefreshRate	= WIN_BASIC_SETTING["Draw"]["MaxFPS"].GetFloat();		// リフレッシュレートを取得
				WIN_BASIC_SETTING["Draw"]["FPS"].SetText(std::to_string(1000.0f / diff));	// FPSを取得
#endif

				// ポーズ中じゃない＆ウィンドウが移動中じゃない＆ウィンドウがフォーカスされている場合
				if (m_bActive && !IsWindowMoving() && IsWindowFocus())
				{
					m_AppController.Update(diff * 0.001f);	// 0.001fはミリ秒を秒に変換するため

				}
				m_AppController.Draw();

				preExecTime = nowTime;	// 前回の実行時間を更新
			}

		}
	}

}

/* ========================================
	終了処理関数
	-------------------------------------
	内容：終了処理を行う
=========================================== */
void WindowAPI::Cleanup()
{
	timeEndPeriod(1);	// 分解能を元に戻す

	m_AppController.Uninit();	// DX11Baseの終了処理

	UnregisterClass(m_Wcex.lpszClassName, m_Wcex.hInstance);	// ウィンドウクラスの登録解除

}


/* ========================================
	ウィンドクラス情報設定関数
	-------------------------------------
	内容：ウィンドウクラス構成情報を設定する
	-------------------------------------
	引数1：インスタンスハンドル
=========================================== */
void WindowAPI::InitWndClass(HINSTANCE hInstance)
{
	ZeroMemory(&m_Wcex, sizeof(m_Wcex));							// 構造体の初期化		
	m_Wcex.cbSize			= sizeof(WNDCLASSEX);					// 型の大きさを指定	
	m_Wcex.style			= CS_CLASSDC | CS_DBLCLKS;				// ウィンドウスタイル
	m_Wcex.lpfnWndProc		= WndProc;								// ウィンドウプロシージャを指定
	m_Wcex.cbClsExtra		= 0;									// 拡張用のメモリ領域1
	m_Wcex.cbWndExtra		= 0;									// 拡張用のメモリ領域2
	m_Wcex.hInstance		= hInstance;							// インスタンスハンドル
	m_Wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);		// アイコンの指定
	m_Wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);			// カーソルの指定
	m_Wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);	// 背景色の指定
	m_Wcex.lpszMenuName		= NULL;									// メニュー名(例:ファイル (F)等)
	m_Wcex.lpszClassName	= WindowConfig::WINDOW_NAME;			// ウィンドウクラス名
	m_Wcex.hIconSm			= m_Wcex.hIcon;							// アイコンの指定
}

bool WindowAPI::IsWindowMoving()
{
	static RECT lastRect = { 0 };
	RECT currentRect;

	// 現在のウィンドウ位置を取得
	if (GetWindowRect(m_hWnd, &currentRect)) {
		// 前回の位置と異なるかどうかをチェック
		if (currentRect.left != lastRect.left || currentRect.top != lastRect.top) {
			lastRect = currentRect;
			return true; // ウィンドウが移動している
		}
	}
	return false; // 移動していない
}

bool WindowAPI::IsWindowFocus()
{
	// フォーカスされているウィンドウのハンドルを取得
	HWND foregroundWindow = GetForegroundWindow();

	// フォーカスされているウィンドウが自分のウィンドウかどうかをチェック
	return (foregroundWindow == m_hWnd);
}



/* ========================================
	ウィンドウハンドル取得関数
	-------------------------------------
	内容：ウィンドウハンドルを取得する
	-------------------------------------
	戻値：HWND型のウィンドウハンドル
=========================================== */
HWND WindowAPI::GetHwnd()
{
	return m_hWnd;
}

/* ========================================
	画面サイズ取得関数
	-------------------------------------
	内容：画面のサイズを取得する
	-------------------------------------
	戻値：SCREEN_SIZE型の画面サイズ
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
	リフレッシュレート取得関数
	-------------------------------------
	内容：リフレッシュレートを取得する
	-------------------------------------
	戻値：UINT型のリフレッシュレート
=========================================== */
UINT WindowAPI::GetRefreshRate()
{
	return WindowConfig::REFRESH_RATE;
}

/* ========================================
	1フレームの時間取得関数
	-------------------------------------
	内容：1フレームの時間を取得する
	-------------------------------------
	戻値：UINT型の1フレームの時間
=========================================== */
UINT WindowAPI::GetOneFrame()
{
	return 1.0f / WindowConfig::REFRESH_RATE;
}

/* ========================================
	アスペクト比取得関数
	-------------------------------------
	内容：画面アスペクト比を取得する
	-------------------------------------
	戻値：float型のアスペクト比
=========================================== */
float WindowAPI::GetAspectRatio()
{
	return WindowConfig::ASPECT_RATIO;
}

/* ========================================
	ゲッター(アクティブフラグ取得)
	-------------------------------------
	戻値：アクティブフラグ
=========================================== */
bool WindowAPI::GetActive()
{
	return m_bActive;
}

/* ========================================
	セッター(アクティブフラグ設定)
	-------------------------------------
	引数：アクティブフラグ
=========================================== */
void WindowAPI::SetActive(bool bActive)
{
	m_bActive = bActive;
}

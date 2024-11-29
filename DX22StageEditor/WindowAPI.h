/* ========================================
	DX22Base/
	------------------------------------
	ウィンドウ設定用ヘッダ
	------------------------------------
	説明:ウィンドウの各種設定を行う
	------------------------------------
	WindowAPI.h
========================================== */
#pragma once

// =============== インクルード ===================
#pragma comment(lib, "winmm.lib")	// ウィンドウメッセージ処理用
#include <Windows.h>				// WindowsAPI
#include "AppController.h"

// =============== 定数定義 =======================
#define WINDOW_INST WindowAPI::GetInstance()	// ウィンドウクラスのインスタンス取得


// =============== クラス定義 =====================
class WindowAPI
{
public:
	typedef struct
	{
		UINT nWidth;
		UINT nHeight;
	}SCREEN_SIZE;
public:
	static WindowAPI& GetInstance();	// インスタンス取得

	HRESULT InitWindow(HINSTANCE hInstance);	// ウィンドウ初期化
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);			// メッセージ処理
	void Run();																		// メインループ
	void Cleanup();																	// 終了処理

	// ゲッター
	HWND GetHwnd();					// ウィンドウハンドル取得
	SCREEN_SIZE GetScreenSize();	// 画面サイズ取得
	UINT GetRefreshRate();			// リフレッシュレート取得
	UINT GetOneFrame();				// 1フレームの時間取得
	float GetAspectRatio();			// アスペクト比取得
	bool GetActive();				// アクティブフラグ取得

	// セッター
	void SetActive(bool bActive);	// アクティブフラグ設定

private:
	void InitWndClass(HINSTANCE hInstance);	// ウィンドウクラスの構成情報の初期化
	bool IsWindowMoving();					// ウィンドウが移動中かどうか
	bool IsWindowFocus();					// ウィンドウがフォーカス中かどうか

	WNDCLASSEX m_Wcex;	// ウィンドウクラスの構成情報
	HWND m_hWnd;		// ウィンドウハンドル

	AppController m_AppController;	// アプリ動作用
	float m_fRefreshRate;			// リフレッシュレート

	bool m_bActive;		// アクティブフラグ
};
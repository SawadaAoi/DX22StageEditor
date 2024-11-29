/* ========================================
	DX22Base/
	------------------------------------
	アプリエントリー用cpp
	------------------------------------
	説明：一番最初に呼び出される関数
	------------------------------------
	AppStart.cpp
========================================== */


// =============== インクルード ===================
#include <windows.h>
#include "WindowAPI.h"
#include "AppController.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


/* ========================================
	エントリーポイント関数
	-------------------------------------
	内容：実行時に最初に呼び出される関数
	-------------------------------------
	引数1：インスタンスハンドル
	引数2：以前のインスタンスのハンドル(使用しない)
	引数3：コマンドライン引数(使用しない)
	引数4：最初にウィンドウが表示される際の表示オプション(使用しない)
=========================================== */
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// メモリリーク検出を有効化
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	// ウィンドウの初期化、作成、表示
	if (SUCCEEDED(WINDOW_INST.InitWindow(hInstance)))
	{
		WINDOW_INST.Run();	// メインループ

	}

	WINDOW_INST.Cleanup();	// 削除


	return 0;
}


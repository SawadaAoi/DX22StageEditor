/* ========================================
	DX22Base/
	------------------------------------
	ウィンドウ設定用ヘッダ
	------------------------------------
	説明:ウィンドウの設定定数を記述
	
	------------------------------------
	SettingWindowAPI.h
========================================== */
#pragma once

// =============== インクルード =====================
#include <Windows.h>
#include <string>

namespace WindowConfig
{
	// 画面サイズ
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	// リフレッシュレート
	const int REFRESH_RATE = 120;

	// ウィンドウタイトル
	const std::string APP_TITLE = "ステージエディター";
	const LPCSTR	WINDOW_NAME = "ステージエディター";

	// アスペクト比
	const float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
}
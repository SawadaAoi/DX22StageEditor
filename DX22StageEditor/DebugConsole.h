/* ========================================
	DX22Base/
	------------------------------------
	デバッグコンソール表示用ヘッダ
	------------------------------------
	説明：別コンソールウィンドウを表示し、
		　文字を表示する
	------------------------------------
	DebugConsole.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <string>		// std::string

class DebugConsole
{
public:
	static void Init();		// 初期化

	static void Update();

	static void Uninit();	// 終了

	static void Print(const std::string message);	// 文字列表示
	static void Printf(const char* format, ...);	// 書式付き表示
};




/* ========================================
	DX22Base/
	------------------------------------
	キー&コントローラー入力用ヘッダ
	------------------------------------
	説明：キー入力とコントローラー入力を管理する
	------------------------------------
	Input.h
========================================== */
#pragma once


// =============== インクルード ===================
#include <Windows.h>
#include "Vector3.h"
#include <array>


const int	KEY_MAX = 256;	// キーの最大数

class Input
{
public:
	static void Init();
	static void Update();

	static bool IsKeyPress(BYTE key);
	static bool IsKeyTrigger(BYTE key);
	static bool IsKeyRelease(BYTE key);
	static bool IsKeyRepeat(BYTE key);

private:
	static void RepeatUpdate();

private:
	static inline BYTE m_InputKeysNow[KEY_MAX];		// キーボードの入力情報
	static inline BYTE m_InputKeysOld[KEY_MAX];		// キーボードの入力情報(前回)
	static inline  std::array<float, KEY_MAX> m_fKeyRepeatTime;	// キーリピート時間
};
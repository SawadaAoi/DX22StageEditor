/* ========================================
	DX22Base/
	------------------------------------
	ゲーム内時間管理用ヘッダ
	------------------------------------
	説明：前フレームからの経過時間等を管理する
	------------------------------------
	TimeManager.h
========================================== */
#pragma once

// =============== 定数定義 =======================
#define DELTA_TIME TimeManager::GetDeltaTime()	// 前フレームからの経過時間取得

// =============== クラス定義 =====================
class TimeManager
{
public:
	static void Init();	// 初期化
	static void Update(float tick);	// 更新

	static float GetDeltaTime();	// 前フレームからの経過時間取得
	static float GetTotalTime();	// ゲーム開始からの経過時間取得
private:
	static inline float m_fDeltaTime;	// 前フレームからの経過時間
	static inline float m_fTotalTime;	// ゲーム開始からの経過時間
};


/* ========================================
	DX22Base/
	------------------------------------
	キー&コントローラー入力用cpp
	------------------------------------
	Input.cpp
========================================== */

// =============== インクルード ===================
#include "Input.h"
#include "TimeManager.h"



// =============== 定数定義 =======================
const float INITIAL_REPEAT_DELAY	= 0.5f; // 初期リピート遅延
const float REPEAT_INTERVAL			= 0.1f; // リピート間隔


/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
=========================================== */
void Input::Init()
{
	// キーボードの入力情報を取得
	if (!GetKeyboardState(m_InputKeysNow))
	{
		MessageBox(nullptr, TEXT("キーボードの初期化に失敗しました"), TEXT("エラー"), MB_OK);
	}

	// m_aInputKeys_Oldにm_aInputKeys_Nowの内容をコピー
	memcpy_s(m_InputKeysOld, sizeof(m_InputKeysOld), m_InputKeysNow, sizeof(m_InputKeysNow));

	// キーリピートタイマーの初期化
	m_fKeyRepeatTime.fill(0.0f);
	
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
=========================================== */
void Input::Update()
{
	// m_aInputKeys_Oldにm_aInputKeys_Nowの内容をコピー
	memcpy_s(m_InputKeysOld, sizeof(m_InputKeysOld), m_InputKeysNow, sizeof(m_InputKeysNow));

	// キーボードの入力情報を取得
	if (!GetKeyboardState(m_InputKeysNow))
	{
		MessageBox(nullptr, TEXT("キーボードの更新に失敗しました"), TEXT("エラー"), MB_OK);
	}

	RepeatUpdate();	// キーリピート処理
}


/* ========================================
	キー押下中判定関数
	-------------------------------------
	内容：キーを押している間処理を行う
=========================================== */
bool Input::IsKeyPress(BYTE key)
{
	return m_InputKeysNow[key] & 0x80;	// 0x80は10000000の16進数表記(1桁でも1があればtrue)
}

/* ========================================
	キー押下開始判定関数
	-------------------------------------
	内容：キーが押されたときに処理を行う
=========================================== */
bool Input::IsKeyTrigger(BYTE key)
{
	// 今のキーと前のキーの排他的論理和を取り、今のキーの状態を取得
	// 今のキーの状態が1で、前のキーの状態が0の場合、トリガーとしてtrueを返す
	return (m_InputKeysNow[key] ^ m_InputKeysOld[key]) & m_InputKeysNow[key] & 0x80;
}

/* ========================================
	キー押下終了判定関数
	-------------------------------------
	内容：キーを押した後離したときに行う処理
=========================================== */
bool Input::IsKeyRelease(BYTE key)
{
	// 今のキーと前のキーの排他的論理和を取り、前のキーの状態を取得
	// 今のキーの状態が0で、前のキーの状態が1の場合、リリースとしてtrueを返す
	return (m_InputKeysNow[key] ^ m_InputKeysOld[key]) & m_InputKeysOld[key] & 0x80;
}

/* ========================================
	キー押下判定フラグ
	-------------------------------------
	内容：同じキーを押し続けている間行う処理
=========================================== */
bool Input::IsKeyRepeat(BYTE key)
{
	// キーが押されている状態かつリピート遅延時間が0以下の場合
	// リピート遅延時間が0以下の場合は、リピート間隔時間が経過したときにtrueを返す
	return IsKeyPress(key) && m_fKeyRepeatTime[key] <= 0.0f;
}

/* ========================================
	リピート処理関数
	-------------------------------------
	内容：キーのリピート処理を行う
=========================================== */
void Input::RepeatUpdate()
{
	// 全てのキーのタイマーを更新
	for (int key = 0; key < KEY_MAX; key++)
	{
		// キーが押されている場合
		if (IsKeyPress(key))
		{
			// 新たにキーが押された場合
			if (IsKeyTrigger(key))
			{
				m_fKeyRepeatTime[key] = INITIAL_REPEAT_DELAY;	// リピート遅延時間をセット(押されてから最初にリピートされるまでの時間)
			}
			// 押され続けている場合(一定時間ごとにリピートされる)
			else
			{
				// リピート遅延時間を減らす
				m_fKeyRepeatTime[key] -= DELTA_TIME;

				// リピート遅延時間が0以下になった場合
				if (m_fKeyRepeatTime[key] <= 0.0f)
				{
					// リピート間隔時間をセット
					m_fKeyRepeatTime[key] = REPEAT_INTERVAL;
				}
			}
		}
		// キーが離された場合
		else
		{
			// キーが離された場合、リピート遅延時間を0にする
			m_fKeyRepeatTime[key] = 0.0f;
		}
	}
}



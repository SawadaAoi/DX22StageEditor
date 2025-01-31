/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(ビルボード(アニメーション有り))用ヘッダ
	------------------------------------
	説明：アニメーション付きビルボードのコンポーネント
	------------------------------------
	ComponentBillboardAnime.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBillboard.h"

// =============== クラスの定義 ===================
class ComponentBillboardAnime :
    public ComponentBillboard
{
public:
	ComponentBillboardAnime(ObjectBase* pOwner);
	virtual void Update() override;
	virtual void Draw() override;

	void Play(int frameMax, Vector2<int> frameColRow, float animeSpeed = 1.0f, bool animeLoop = false);
	void Play(int frameCnt = -1);
	void Stop();

	// ゲッター
	Vector2<int> GetFrameColRow();
	int GetFrameCnt();
	int GetFrameMax();
	float GetAnimeSpeed();
	bool GetAnimeLoop();
	bool GetAnimeEnd();

	// セッター
	void SetFrameColRow(Vector2<int> frameColRow);
	void SetFrameCnt(int frameCnt);
	void SetFrameMax(int frameMax);
	void SetAnimeSpeed(float animeSpeed);
	void SetAnimeLoop(bool animeLoop);
	void SetAnimeEnd(bool animeEnd);

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void SetFrameSwitchTimeMax();
private:
	Vector2<int> m_vFrameColRow;	// 縦横のコマ数
	int m_nFrameCnt;				// 現在のコマ数
	int m_nFrameMax;				// 最大コマ数
	float m_fAnimeSpeed;			// アニメーション速度(1ループにかかる時間)
	float m_fFrameSwitchCnt;		// コマ切替加算カウンタ
	float m_fFrameSwitchTimeMax;	// 1コマにかかる時間
	bool m_bAnimeLoop;				// アニメーションループフラグ(true:ループする false:ループしない)
	bool m_bAnimeEnd;				// アニメーション終了フラグ(true:終了 false:終了していない)
};


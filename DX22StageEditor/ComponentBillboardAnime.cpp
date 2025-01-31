/* ========================================
	CatRobotGame/
	------------------------------------
	コンポーネント(ビルボード(アニメーション有り))用cpp
	------------------------------------
	ComponentBillboardAnime.cpp
========================================== */

// =============== インクルード =====================
#include "ComponentBillboardAnime.h"
#include "ComponentTransform.h"

#include "ObjectBase.h"

#include "TextureManager.h"
#include "ShaderManager.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所持オブジェクト
========================================== */
ComponentBillboardAnime::ComponentBillboardAnime(ObjectBase* pOwner)
	: ComponentBillboard(pOwner)
	, m_vFrameColRow(1, 1)
	, m_nFrameCnt(0)
	, m_nFrameMax(1)
	, m_fAnimeSpeed(1.0f)
	, m_fFrameSwitchCnt(0.0f)
	, m_fFrameSwitchTimeMax(0.0f)
	, m_bAnimeLoop(false)
	, m_bAnimeEnd(false)
{
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ComponentBillboardAnime::Update()
{
	if (m_bAnimeEnd) return;	// アニメーション終了していたら更新しない

	m_fFrameSwitchCnt += DELTA_TIME;	// コマ切替加算
	if (m_fFrameSwitchCnt >= m_fFrameSwitchTimeMax)
	{
		m_fFrameSwitchCnt = 0.0f;	// カウンタリセット

		// 次のコマへ
		m_nFrameCnt++;
		if (m_nFrameCnt >= m_nFrameMax)
		{
			if (m_bAnimeLoop)
			{
				m_nFrameCnt = 0;	// ループする
			}
			else
			{
				m_nFrameCnt = m_nFrameMax - 1;	// ループしない
				m_bAnimeEnd = true;	// アニメーション終了
			}
		}
	}
}

/* ========================================
	描画関数
	-------------------------------------
	内容：描画処理
========================================== */
void ComponentBillboardAnime::Draw()
{
	float fUvPosX = (m_nFrameCnt % m_vFrameColRow.x) * m_vUvScale.x;	// UV座標X
	float fUvPosY = (m_nFrameCnt / m_vFrameColRow.x) * m_vUvScale.y;	// UV座標Y

	SetUvPos(Vector2<float>(fUvPosX, fUvPosY));	// UV座標を再設定

	ComponentBillboard::Draw();
}

/* ========================================
	再生関数
	-------------------------------------
	内容：アニメーション再生
	-------------------------------------
	引数1：frameMax		最大コマ数
	引数2：frameColRow	縦横コマ数
	引数3：animeSpeed	アニメーション速度
	引数4：animeLoop	アニメーションループフラグ
========================================== */
void ComponentBillboardAnime::Play(int frameMax, Vector2<int> frameColRow, float animeSpeed, bool animeLoop)
{
	SetFrameColRow(frameColRow);	// 縦横コマ数を設定
	SetFrameMax(frameMax);			// 最大コマ数を設定
	SetAnimeSpeed(animeSpeed);		// アニメーション速度を設定
	SetAnimeLoop(animeLoop);		// アニメーションループフラグを設定

	m_bAnimeEnd = false;	// アニメーション終了フラグをリセット
}

/* ========================================
	再生関数
	-------------------------------------
	内容：アニメーション再生
	-------------------------------------
	引数1：frameCnt	再生コマ数
========================================== */
void ComponentBillboardAnime::Play(int frameCnt)
{
	m_bAnimeEnd = false;	// アニメーション終了フラグをリセット

	// コマ数指定がある場合
	if (frameCnt >= 0)
	{
		m_nFrameCnt = frameCnt;	// 指定コマから再生

		// 最大コマ数を超えていたら最大コマ数に設定
		if (m_nFrameCnt >= m_nFrameMax)
			m_nFrameCnt = m_nFrameMax - 1;
	}

	// コマ数指定がない場合は現在のコマから再生
}

/* ========================================
	停止関数
	-------------------------------------
	内容：アニメーション停止
========================================== */
void ComponentBillboardAnime::Stop()
{
	m_bAnimeEnd = true;	// アニメーション終了フラグを立てる
}

/* ========================================
	フレーム切替時間設定関数
	-------------------------------------
	内容：1コマにかかる時間を設定
========================================== */
void ComponentBillboardAnime::SetFrameSwitchTimeMax()
{
	m_fFrameSwitchTimeMax = m_fAnimeSpeed / m_nFrameMax;	// 1ループにかかる時間 / 最大フレーム数
}

/* ========================================
	ゲッター(縦横コマ数)関数
	-------------------------------------
	戻値：Vector2<int>	縦横コマ数
=========================================== */
Vector2<int> ComponentBillboardAnime::GetFrameColRow()
{
	return m_vFrameColRow;
}

/* ========================================
	ゲッター(現在コマ数)関数
	-------------------------------------
	戻値：int	現在コマ数
=========================================== */
int ComponentBillboardAnime::GetFrameCnt()
{
	return m_nFrameCnt;
}

/* ========================================
	ゲッター(最大コマ数)関数
	-------------------------------------
	戻値：int	最大コマ数
=========================================== */
int ComponentBillboardAnime::GetFrameMax()
{
	return m_nFrameMax;
}

/* ========================================
	ゲッター(アニメーション速度)関数
	-------------------------------------
	戻値：float	アニメーション速度
=========================================== */
float ComponentBillboardAnime::GetAnimeSpeed()
{
	return m_fAnimeSpeed;
}

/* ========================================
	ゲッター(アニメーションループフラグ)関数
	-------------------------------------
	戻値：bool	アニメーションループフラグ
=========================================== */
bool ComponentBillboardAnime::GetAnimeLoop()
{
	return m_bAnimeLoop;
}


/* ========================================
	ゲッター(アニメーション終了フラグ)関数
	-------------------------------------
	戻値：bool	アニメーション終了フラグ
=========================================== */
bool ComponentBillboardAnime::GetAnimeEnd()
{
	return m_bAnimeEnd;
}

/* ========================================
	セッター(縦横コマ数)関数
	-------------------------------------
	引数1：Vector2<int>	縦横コマ数
=========================================== */
void ComponentBillboardAnime::SetFrameColRow(Vector2<int> frameColRow)
{
	m_vFrameColRow = frameColRow;

	m_nFrameMax = m_vFrameColRow.x * m_vFrameColRow.y;	// 最大フレーム数を計算

	float fUvWidth	= 1.0f / m_vFrameColRow.x;	// 1コマの幅
	float fUvHeight = 1.0f / m_vFrameColRow.y;	// 1コマの高さ
	SetUvScale(Vector2<float>(fUvWidth, fUvHeight));	// UVスケールを再設定
}

/* ========================================
	セッター(現在コマ数)関数
	-------------------------------------
	引数1：int	現在コマ数
=========================================== */
void ComponentBillboardAnime::SetFrameCnt(int frameCnt)
{
	m_nFrameCnt = frameCnt;
}

/* ========================================
	セッター(最大コマ数)関数
	-------------------------------------
	引数1：int	最大コマ数
=========================================== */
void ComponentBillboardAnime::SetFrameMax(int frameMax)
{
	m_nFrameMax = frameMax;
	SetFrameSwitchTimeMax();	// 1コマにかかる時間を再設定
}

/* ========================================
	セッター(アニメーション速度)関数
	-------------------------------------
	引数1：float	アニメーション速度
=========================================== */
void ComponentBillboardAnime::SetAnimeSpeed(float animeSpeed)
{
	m_fAnimeSpeed = animeSpeed;
	SetFrameSwitchTimeMax();	// 1コマにかかる時間を再設定
}

/* ========================================
	セッター(アニメーションループフラグ)関数
	-------------------------------------
	引数1：bool	アニメーションループフラグ
=========================================== */
void ComponentBillboardAnime::SetAnimeLoop(bool animeLoop)
{
	m_bAnimeLoop = animeLoop;
}

/* ========================================
	セッター(アニメーション終了フラグ)関数
	-------------------------------------
	引数1：bool	アニメーション終了フラグ
=========================================== */
void ComponentBillboardAnime::SetAnimeEnd(bool animeEnd)
{
	m_bAnimeEnd = animeEnd;
}


#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentBillboardAnime::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupBillboardAnime = Item::CreateGroup("BillboardAnime");

	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("PosOffset", Item::Kind::Vector, &m_vPosOffset));	// 色
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("UVPos", Item::Kind::Vector2, &m_vUvPos));	// UV座標
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("UVScale", Item::Kind::Vector2, &m_vUvScale));	// UVスケール
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("Color", Item::Kind::Vector, &m_vColor));	// 色
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("Alpha", Item::Kind::Float, &m_fAlpha));	// 透過度

	pGroupBillboardAnime->AddGroupItem(InitTextureList());	// テクスチャリスト

	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("FrameColRow", Item::Kind::Vector2, &m_vFrameColRow));	// 縦横コマ数
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("FrameCnt", Item::Kind::Int, &m_nFrameCnt));	// 現在コマ数
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("FrameMax", Item::Kind::Int, &m_nFrameMax));	// 最大コマ数
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("AnimeSpeed", Item::Kind::Float, &m_fAnimeSpeed));	// アニメーション速度
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("AnimeLoop", Item::Kind::Bool, &m_bAnimeLoop));	// アニメーションループフラグ
	pGroupBillboardAnime->AddGroupItem(Item::CreateBind("AnimeEnd", Item::Kind::Bool, &m_bAnimeEnd));	// アニメーション終了フラグ

	window.AddItem(pGroupBillboardAnime);

}


#endif // _DEBUG
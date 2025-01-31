/* ========================================
	CatRobotGame/
	------------------------------------
	オブジェクト(爆発エフェクト)用cpp
	------------------------------------
	ObjectExplosion.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectExplosion.h"
#include "ComponentBillboardAnime.h"
#include "ComponentTransform.h"
#include "TextureManager.h"

// =============== 定数定義 =======================
const Vector3<float>	DEFAULT_SCALE	= Vector3<float>(2.0f, 2.0f, 2.0f);	// デフォルトスケール
const int				ANIME_FRAME_MAX = 12;								// アニメーションフレーム数
const Vector2<int>		ANIME_COL_ROW	= Vector2<int>(4, 3);				// アニメーションの列と行
const float				ANIME_SPEED		= 0.5f;								// アニメーション速度

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーンポインタ
========================================== */
ObjectExplosion::ObjectExplosion(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompBillboardAnime(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
========================================== */
void ObjectExplosion::InitLocal()
{
	m_pCompBillboardAnime = AddComponent<ComponentBillboardAnime>();
	m_pCompBillboardAnime->SetTexture(GET_TEXTURE_DATA(TEX_KEY::EFFECT_EXPLOSION));

	// アニメーション再生
	m_pCompBillboardAnime->Play(ANIME_FRAME_MAX, ANIME_COL_ROW, ANIME_SPEED, false);
	// スケール設定
	m_pCompTransform->SetScale(DEFAULT_SCALE);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================== */
void ObjectExplosion::UpdateLocal()
{
	// アニメーション終了時
	if (m_pCompBillboardAnime->GetAnimeEnd())
	{
		SetState(E_State::STATE_DEAD);
	}
}

/* ========================================
	ゲッター(ビルボード(アニメーション有り)コンポーネント)関数
	-------------------------------------
	戻値：ComponentBillboardAnime*　ビルボード(アニメーション有り)コンポーネント
=========================================== */
ComponentBillboardAnime* ObjectExplosion::GetCompBillboardAnime()
{
	return nullptr;
}

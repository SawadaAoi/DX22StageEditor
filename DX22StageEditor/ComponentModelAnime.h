/* ========================================
	DX22Base/
	------------------------------------
	コンポーネント(アニメーションモデル)用ヘッダ
	------------------------------------
	説明：アニメーションモデルの描画を行う
	------------------------------------
	ComponentModelAnime.h
========================================== */

#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"
#include "ModelAnimeManager.h"

// =============== 前方宣言 =======================
class ComponentTransform;	// トランスフォームコンポーネント
class ModelAnime;			// アニメーションモデル

// =============== クラス定義 =====================
class ComponentModelAnime :
    public ComponentBase
{
public:
	ComponentModelAnime(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;


	void PlayAnime(int animeNo, bool loop = false, float speed = 1.0f);
	void PlayAnimeBlend(int animeNo, float blendTime = 1.0f, bool loop = false, float speed = 1.0f);
	void PlayAnimeParametric(int animeNo1, int animeNo2, float blendRate = 0.5f);
	void PlayAnimeParametricBlend(int animeNo1, int animeNo2, float blendRate, float blendTime, bool loop = false, float speed = 1.0f);

	// ゲッター
	bool GetIsPlayAnime(int animeNo);
	float GetAnimeNowTime(int animeNo);
	float GetAnimeTotalTime(int animeNo);
	float GetAnimePlaybackRatio(int animeNo);
	ModelAnime* GetModel();
	bool GetIsVisible();
	Vector3<float> GetPosOffset();

	// セッター
	void SetAnimeTimeRatio(int animeNo, float rate);
	void SetModel(ModelAnime* pModel);
	void SetIsVisible(bool bIsVisible);
	void SetPosOffset(Vector3<float> offset);

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitModelList();
	void ClickModelList(std::string sListName);
	void InitAnimeList();
	void ClickAnimeList(std::string sListName);
#endif // _DEBUG
private:
	ComponentTransform* m_pCompTran;	// トランスフォームコンポーネント
	ModelAnime*			m_pModel;		// モデル
	
	bool				m_bIsVisible;	// 表示フラグ
	Vector3<float>		m_vPosOffset;	// 位置オフセット

#ifdef _DEBUG
	bool								m_bDispBone;	// ボーン表示フラグ
	ModelAnimeManager::E_ANIME_BASE_KEY m_nModelKey;	// モデルキー
	int									m_nAnimeKey;	// アニメーションキー

	bool	m_bAnimeLoop;
	float	m_fAnimeSpeed;

	ModelAnime* m_pDelModel;
#endif // _DEBUG
};


/* ========================================
	DX22Base/
	------------------------------------
	�R���|�[�l���g(�A�j���[�V�������f��)�p�w�b�_
	------------------------------------
	�����F�A�j���[�V�������f���̕`����s��
	------------------------------------
	ComponentModelAnime.h
========================================== */

#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"
#include "ModelAnimeManager.h"

// =============== �O���錾 =======================
class ComponentTransform;	// �g�����X�t�H�[���R���|�[�l���g
class ModelAnime;			// �A�j���[�V�������f��

// =============== �N���X��` =====================
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

	// �Q�b�^�[
	bool GetIsPlayAnime(int animeNo);
	float GetAnimeNowTime(int animeNo);
	float GetAnimeTotalTime(int animeNo);
	float GetAnimePlaybackRatio(int animeNo);
	ModelAnime* GetModel();
	bool GetIsVisible();
	Vector3<float> GetPosOffset();

	// �Z�b�^�[
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
	ComponentTransform* m_pCompTran;	// �g�����X�t�H�[���R���|�[�l���g
	ModelAnime*			m_pModel;		// ���f��
	
	bool				m_bIsVisible;	// �\���t���O
	Vector3<float>		m_vPosOffset;	// �ʒu�I�t�Z�b�g

#ifdef _DEBUG
	bool								m_bDispBone;	// �{�[���\���t���O
	ModelAnimeManager::E_ANIME_BASE_KEY m_nModelKey;	// ���f���L�[
	int									m_nAnimeKey;	// �A�j���[�V�����L�[

	bool	m_bAnimeLoop;
	float	m_fAnimeSpeed;

	ModelAnime* m_pDelModel;
#endif // _DEBUG
};


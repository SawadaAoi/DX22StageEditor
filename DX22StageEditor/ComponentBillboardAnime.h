/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�r���{�[�h(�A�j���[�V�����L��))�p�w�b�_
	------------------------------------
	�����F�A�j���[�V�����t���r���{�[�h�̃R���|�[�l���g
	------------------------------------
	ComponentBillboardAnime.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBillboard.h"

// =============== �N���X�̒�` ===================
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

	// �Q�b�^�[
	Vector2<int> GetFrameColRow();
	int GetFrameCnt();
	int GetFrameMax();
	float GetAnimeSpeed();
	bool GetAnimeLoop();
	bool GetAnimeEnd();

	// �Z�b�^�[
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
	Vector2<int> m_vFrameColRow;	// �c���̃R�}��
	int m_nFrameCnt;				// ���݂̃R�}��
	int m_nFrameMax;				// �ő�R�}��
	float m_fAnimeSpeed;			// �A�j���[�V�������x(1���[�v�ɂ����鎞��)
	float m_fFrameSwitchCnt;		// �R�}�ؑ։��Z�J�E���^
	float m_fFrameSwitchTimeMax;	// 1�R�}�ɂ����鎞��
	bool m_bAnimeLoop;				// �A�j���[�V�������[�v�t���O(true:���[�v���� false:���[�v���Ȃ�)
	bool m_bAnimeEnd;				// �A�j���[�V�����I���t���O(true:�I�� false:�I�����Ă��Ȃ�)
};


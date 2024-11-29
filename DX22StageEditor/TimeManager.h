/* ========================================
	DX22Base/
	------------------------------------
	�Q�[�������ԊǗ��p�w�b�_
	------------------------------------
	�����F�O�t���[������̌o�ߎ��ԓ����Ǘ�����
	------------------------------------
	TimeManager.h
========================================== */
#pragma once

// =============== �萔��` =======================
#define DELTA_TIME TimeManager::GetDeltaTime()	// �O�t���[������̌o�ߎ��Ԏ擾

// =============== �N���X��` =====================
class TimeManager
{
public:
	static void Init();	// ������
	static void Update(float tick);	// �X�V

	static float GetDeltaTime();	// �O�t���[������̌o�ߎ��Ԏ擾
	static float GetTotalTime();	// �Q�[���J�n����̌o�ߎ��Ԏ擾
private:
	static inline float m_fDeltaTime;	// �O�t���[������̌o�ߎ���
	static inline float m_fTotalTime;	// �Q�[���J�n����̌o�ߎ���
};


/* ========================================
	DX22Base/
	------------------------------------
	�I�u�W�F�N�g(�j��\�u���b�N)�p�w�b�_
	------------------------------------
	�����F�e�ɓ�����Ɖ���u���b�N
		�@AABB�A�n�ʔ���L��
	------------------------------------
	ObjectBlockBreakable.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBlock.h"

// =============== �N���X��` ===================
class ObjectBlockBreakable :
    public ObjectBlock
{
public:
	struct S_SaveData
	{
		int nHp;	// HP
	};
public:
	ObjectBlockBreakable(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	// �Z�b�^�[
	void SetHp(int nHp);

	DEFINE_OBJECT_TYPE(ObjectBlockBreakable)
#ifdef _DEBUG
	void DebugLocal(DebugUI::Window& window);
#endif // DEBUG
private:
	int m_nHp;		// HP
	int m_nHpOld;	// HP(�O��)
};


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
	ObjectBlockBreakable(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;

	DEFINE_OBJECT_TYPE(ObjectBlockBreakable)
private:
	int m_nHp;		// HP
	int m_nHpOld;	// HP(�O��)
};


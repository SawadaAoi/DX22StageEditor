/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�ړ���)�p�w�b�_
	------------------------------------
	�����F�ړ����鏰�̃I�u�W�F�N�g
		�@(�v���C���[����邱�Ƃ��ł���)
	------------------------------------
	ObjectMoveFloor.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"
#include "ObjectBlock.h"


// =============== �O���錾 =======================
class ComponentGimmickMoveLinear;
class ComponentRigidbody;
class ComponentGroundBox;

// =============== �N���X��` =====================
class ObjectMoveFloor :
    public ObjectBlock
{
public:
	// �Z�[�u�f�[�^
	struct S_SaveData
	{
		float fMoveSpeed;	// �ړ����x
		int nWayPointNum;	// �ړ����W��
		bool bIsReverse;	// �t���t���O
	};
public:
	ObjectMoveFloor(SceneBase* pScene);
	void InitLocal() override;

	void OnCollisionEnter(ObjectBase* pHit) override;
	void OnCollisionExit(ObjectBase* pHit) override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectMoveFloor)
private:
	ComponentGimmickMoveLinear* m_pCompMoveLinear;
	ComponentRigidbody* m_pCompRigidbody;
	ComponentGroundBox* m_pCompGroundBox;
};


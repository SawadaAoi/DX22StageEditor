/* ========================================
	CatRobotGame/
	------------------------------------
	�R���|�[�l���g(�X�e�[�W�M�~�b�N�����ړ�)�p�w�b�_
	------------------------------------
	�����F�X�e�[�W�M�~�b�N�̒����ړ����s���R���|�[�l���g
		�@�G�L�����̃R���|�[�l���g�Ƃ̈Ⴂ�́A
		  �����ƁA�ړ������W�w��ł��邱��
	------------------------------------
	ComponentGimmickMoveLinear.h
========================================== */
#pragma once
#include "ComponentBase.h"

// =============== �C���N���[�h =====================
#include <vector>
#include <memory>

// =============== �O���錾 =======================
class ShapeLine;
class ComponentTransform;
class ComponentRigidbody;

// =============== �N���X��` =====================
class ComponentGimmickMoveLinear :
    public ComponentBase
{
public:
	ComponentGimmickMoveLinear(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	void AddWayPoint(const Vector3<float>& vWayPoint);
	void InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex);
	void RemoveWayPoint(int nIndex);

	// �Q�b�^�[
	float GetMoveSpeed();
	std::vector<Vector3<float>>& GetWayPoints();
	bool GetIsReverse();

	// �Z�b�^�[
	void SetMoveSpeed(float fSpeed);
	void SetIsReverse(bool bIsReverse);	

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	void InitDebugWayPointList();
	void FuncWayCurrent(bool isWrite, void* arg);
	void FuncWayPoint(bool isWrite, void* arg);
#endif // _DEBUG
private:
	void Move();
	void ReverseMove();
private:
	ComponentTransform* m_pCompTransform;
	float				m_fMoveSpeed;

	std::vector<Vector3<float>> m_vtWayPoints;	// �ړ����W�z��
	int m_nCurrentWayPoint;						// ���݂̍��W�ԍ�

	bool m_bIsReverse;							// �t���t���O

	bool m_bDispMoveLine;						// �ړ����C���\���t���O
	std::unique_ptr<ShapeLine> m_pMoveLine;		// �ړ����C���`��p

#ifdef _DEBUG
	int m_nSelectWayPointIdx; 					// �I�𒆂̍��W�ԍ�
#endif // _DEBUG
};


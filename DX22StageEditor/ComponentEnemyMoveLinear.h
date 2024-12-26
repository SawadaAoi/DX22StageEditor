/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(����)�p�w�b�_
	------------------------------------
	�����F�G�L�����̒����ړ������N���X
		�@�w�肵�����W�����ԂɈړ�����
	------------------------------------
	ComponentEnemyMoveLinear.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentEnemyMoveBase.h"
#include <vector>
#include <memory>

// =============== �O���錾 =======================
class ShapeLine;


// =============== �N���X��` =====================
class ComponentEnemyMoveLinear :
    public ComponentEnemyMoveBase
{
public:
	ComponentEnemyMoveLinear(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	void AddWayPoint(const Vector3<float>& vWayPoint);
	void InsertWayPoint(const Vector3<float>& vWayPoint, int nIndex);
	void RemoveWayPoint(int nIndex);

	// �Q�b�^�[
	std::vector<Vector3<float>>& GetWayPoints();
	bool GetIsReverse();

	// �Z�b�^�[
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
	std::vector<Vector3<float>> m_vtWayPoints;	// �ړ����W�z��
	int m_nCurrentWayPoint;						// ���݂̍��W�ԍ�

	bool m_bIsReverse;							// �t���t���O

	bool m_bDispMoveLine;						// �ړ����C���\���t���O
	std::unique_ptr<ShapeLine> m_pMoveLine;		// �ړ����C���`��p

#ifdef _DEBUG
	int m_nSelectWayPointIdx; 					// �I�𒆂̍��W�ԍ�
#endif // _DEBUG
};


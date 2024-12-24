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

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Move();
	void ReverseMove();
private:
	std::vector<Vector3<float>> m_vtWayPoints;	// �ړ����W
	int m_nCurrentWayPoint;						// ���݂̍��W�ԍ�

	bool m_bIsReverse;							// �t���t���O

	bool m_bDispMoveLine;						// �ړ����C���\���t���O
	std::unique_ptr<ShapeLine> m_pMoveLine;	// �ړ����C���`��p
};


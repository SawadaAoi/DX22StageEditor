/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(�����_��)�p�w�b�_
	------------------------------------
	�����F�G�L�����̃����_���ړ������N���X
		�@���Ԋu�Ń����_���ȍ��W�𐶐����A�ړ�����
	------------------------------------
	ComponentEnemyMoveRandom.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentEnemyMoveBase.h"
#include <vector>
#include <memory>

// =============== �O���錾 =======================
class ShapeLine;
class ShapeSphere;

// =============== �N���X��` =====================
class ComponentEnemyMoveRandom :
    public ComponentEnemyMoveBase
{
public:
	ComponentEnemyMoveRandom(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	// �Q�b�^�[
	Vector3<float> GetTargetPos() const;
	float GetTargetSwitchTime() const;
	std::vector<Vector3<float>>& GetMoveLimitRect();

	// �Z�b�^�[
	void SetTargetSwitchTime(float fTime);
	void SetMoveLimitRect(const std::vector<Vector3<float>>& vtRect);	// �ړ��͈͂�ݒ肷��
	void SetMoveLimitRectSquare(float fSize);				// �ړ��͈͂𐳕��`�ɐݒ肷��ꍇ�̈�ӂ̒���
	void SetMoveLimitRectXZ(const Vector2<float>& vSize);	// �ړ��͈͂�XZ���ʂŐݒ肷��ꍇ�̃T�C�Y
	void SetMoveLimitRectPosY(float fPosY);					// �ړ��͈͍��WY��ݒ肷��
	void SetDispMoveLimitRect(bool bDisp);					// �ړ��͈͕\���t���O��ݒ肷��

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Move();
	void SwitchTargetPos();

	Vector3<float> GetRandomPoint(
		const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const Vector3<float>& v4);
	Vector3<float> GetRandomPointInTriangle(
		const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3);

	float GetTriangleArea(const Vector3<float>& P1, const Vector3<float>& P2, const Vector3<float>& P3);
private:
	Vector3<float>	m_vTargetPos;			// �ړI���W
	float			m_fTargetSwitchTime;	// �ړI���W�؂�ւ�����
	float			m_fTargetSwitchCnt;		// �ړI���W�؂�ւ��J�E���g

	std::vector<Vector3<float>> m_vtMoveLimitRect;	// �ړ��͈͍��W(�l�p�`)
	float m_fMoveLimitRectPosY;						// �ړ��͈͍��WY

	bool m_bDispMoveLimitRect;						// �ړ��͈͕\���t���O
	std::unique_ptr<ShapeLine>	m_pRectLine;		// �ړ��͈͕`��p
	std::unique_ptr<ShapeSphere> m_pTargetSphere;	// �ړI���W�`��p
};


/* ========================================
	CatRobotGame/
	------------------------------------
	�G�L�����ړ��R���|�[�l���g(�ǐ�)�p�w�b�_
	------------------------------------
	�����F�G�L�����̒ǐՈړ������N���X
		�@�w��͈͓��ɋ߂Â�����v���C���[��ǐՂ���
	------------------------------------
	ComponentEnemyMoveChase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentEnemyMoveBase.h"
#include <memory>
#include "Quaternion.h"

// =============== �O���錾 =======================
class ShapeLine;

// =============== �N���X��` =====================
class ComponentEnemyMoveChase :
    public ComponentEnemyMoveBase
{
public:
	ComponentEnemyMoveChase(ObjectBase* pOwner);
	void Init() override;
	void Update() override;
	void Draw() override;

	// �Q�b�^�[
	float GetChaseStartDist() const;		// �ǐՊJ�n�������擾����
	bool GetDispDistLine() const;			// �������C���\���t���O���擾����
	float GetLimitDistSq() const;			// �ǐՌ��E�������擾����

	// �Z�b�^�[
	void SetChaseStartDist(float fDist);	// �ǐՊJ�n������ݒ肷��
	void SetDispDistLine(bool bDisp);		// �������C���\���t���O��ݒ肷��
	void SetLimitDistSq(float fDist);		// �ǐՌ��E������ݒ肷��

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void Move();
	void ChaseTarget();	// �v���C���[��ǐՂ���
	void BackToStartPos();	// �ړ��J�n���W�ɖ߂�

	// �v���C���[�Ƃ̋������C����`�悷��
	void DrawPlayerDistLine();
private:
	ObjectBase* m_pTargetObj;	// �ǐՑΏۃI�u�W�F�N�g

	float m_fChaseStartDist;	// �ǐՊJ�n����
	float m_fLimitDistSq;		// �ǐՌ��E����

	bool m_bDispDistLine;					// �������C���\���t���O
	std::unique_ptr<ShapeLine> m_pDistLine;	// �������C���`��p

	Vector3<float>	m_vStartPos;	// �ړ��J�n���W
	Quaternion		m_qStartRot;	// �ړ��J�n�p�x
};


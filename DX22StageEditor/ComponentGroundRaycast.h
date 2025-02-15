/* ========================================
	CatRobotGame/
	------------------------------------
	�n�ʐڐG����R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�������Ƀ��C���΂��A�n�ʂƂ̐ڐG������s��
	------------------------------------
	ComponentGroundRaycast.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentBase.h"
#include "Vector3.h"
#include <memory>
#include <vector>
#include "ShapeLine.h"


// =============== �O���錾 =======================
class ComponentTransform;
class ComponentGround;

// =============== �N���X��` =====================
class ComponentGroundRaycast :
	public ComponentBase
{
public:
	ComponentGroundRaycast(ObjectBase* pOwner);
	~ComponentGroundRaycast() {};

	void Init() override;
	void Update() override;
	void Draw() override;

	// �Q�b�^�[
	Vector3<float> GetHitPos();				// ���C�n�ʐڐG���W
	bool GetHitFlg();						// ���C�n�ʐڐG����t���O
	std::string GetHitObjName();			// �ڐG�I�u�W�F�N�g��

	// �Z�b�^�[
	void SetRayLength(float fLength);						// ���C�̒�����ݒ�
	void SetRayDirection(const Vector3<float>& vDir);		// ���C�̕�����ݒ�
	void SetStartPosOffset(const Vector3<float>& vOffset);	// ���C�̎n�_�̃I�t�Z�b�g��ݒ�
	void SetDrawLine(bool bDraw);							// ���C�`��t���O��ݒ�

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	void CheckGround();									// �n�ʂƂ̐ڐG����
	bool CheckHit(ComponentGround* pPlaneGround);		// ���C���n�ʂɓ������Ă��邩�ǂ����𔻒�
	bool CheckOnGround(ComponentGround* pPlaneGround);	// �ڐG���W���n�ʂ̏�ɂ��邩�ǂ����𔻒�

	Vector3<float> GetGroundCenterPos(ComponentGround* pPlaneGround);	// �n�ʂ̍��W���擾
private:
	Vector3<float>		m_vStartPos;		// ���C�̎n�_
	Vector3<float>		m_vStartPosOffset;	// ���C�̎n�_�̃I�t�Z�b�g
	Vector3<float>		m_vDirection;		// ���C�̕���
	float				m_fRayLength;		// ���C�̒���

	bool									m_bIsDispRayLine;			// ���C�`��t���O
	std::unique_ptr<ShapeLine>				m_pRayLine;					// ���C�`���
	bool									m_bIsDispTriPointToHitPos;	// �O�p���_���n�ʐڐG���W�܂ł̐��\���t���O
	std::vector<std::unique_ptr<ShapeLine>> m_pLines_TriPointToHitPos;	// �n�ʎO�p���_����n�ʐڐG���W�܂ł̐�

	Vector3<float>	m_vHitPos;		// ���C�n�ʐڐG���W
	bool			m_bIsHit;		// ���C�n�ʐڐG����t���O

	std::string m_sHitObjName;	// �ڐG�I�u�W�F�N�g��

#ifdef _DEBUG
	std::string		m_sDispIsHit;	// �n�ʐڐG�\��
#endif // _DEBUG

};


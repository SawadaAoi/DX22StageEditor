/* ========================================
	CatRobotGame/
	------------------------------------
	�I�u�W�F�N�g(�t�@�C���[�o�[)�p�w�b�_
	------------------------------------
	�����F�v���C���[�Ƀ_���[�W��^����I�u�W�F�N�g
	------------------------------------
	ObjectFireBar.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"


// =============== �萔��` =======================
class ObjectBlockGroundAABB;

// =============== �N���X��` ===================
class ObjectFireBar :
    public ObjectBase
{
public:
	struct S_SaveData
	{
		int		nFireBallNum;	// �΂̋ʂ̐�
		float	fRotateSpeed;	// ��]���x
	};
public:
	ObjectFireBar(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	void CopyLocal(ObjectBase* pObject) override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	// �Z�b�^�[
	void SetFireBallNum(int nFireBallNum);
	void SetRotateSpeed(float fRotateSpeed);
	

	DEFINE_OBJECT_TYPE(ObjectFireBar)
private:
	void InitCenterBlock();
	void InitFireBall();
private:
	ObjectBlockGroundAABB*		m_pCenterBlock;		// ���S�u���b�N
	ObjectBase*					m_pFireBallParent;	// �΂̋ʐe(��]�p)
	std::vector<ObjectBase*>	m_pFireBall;		// �΂̋�

	int m_nFireBallNum;	// �΂̋ʂ̐�
	float m_fRotateSpeed;	// ��]���x
};


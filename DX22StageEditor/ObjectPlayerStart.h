/* ========================================
	CatRobotGame/
	------------------------------------
	�v���C���[�J�n�n�_�p�w�b�_
	------------------------------------
	�����F�X�e�[�W�̃v���C���[�J�n�n�_
		�@���G�f�B�^�[�Ńv���C���[�̏����ʒu��ݒ肵�₷�����邽��
	------------------------------------
	ObjectPlayerStart.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ObjectBase.h"

// =============== �N���X��` ===================
class ObjectPlayerStart :
    public ObjectBase
{
public:
	ObjectPlayerStart(SceneBase* pScene);

	void InitLocal() override;
	void UpdateLocal() override;

	// �Q�b�^�[
	bool GetIsPlayerStart() const;

	// �Z�b�^�[
	void SetIsPlayerStart(bool bIsPlayerStart);

	DEFINE_OBJECT_TYPE(ObjectPlayerStart)

#ifdef _DEBUG
		void DebugLocal(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	bool m_bIsPlayerStart; // true = �X�^�[�g / false = ���X�^�[�g
};


/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃI�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F�v���C���[�����n�ʃI�u�W�F�N�g
	------------------------------------
	ObjectGround.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"
#include "Vector3.h"
#include <memory>
#include "ShapeLine.h"

// =============== �N���X��` =====================
class ObjectGround :
    public ObjectBase
{
public:
	ObjectGround(SceneBase* pScene);
	~ObjectGround() {};

	void InitLocal() override;
	void UninitLocal() override;
	void UpdateLocal() override;
	void DrawLocal() override;

	DEFINE_OBJECT_TYPE(ObjectGround)	// �I�u�W�F�N�g�̎��ID�擾�֐�

private:
	class ComponentTransform* m_pTransform;	// ���W�A��]�A�X�P�[��
	class ComponentGeometry* m_pGeometry;	// �`��
};


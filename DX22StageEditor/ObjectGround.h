/* ========================================
	DX22Base/
	------------------------------------
	�n�ʃI�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F���C��������n�ʃI�u�W�F�N�g
	------------------------------------
	ObjectGround.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"

// =============== �O���錾 =======================
class ComponentGeometry;
class ComponentGround;


// =============== �N���X��` =====================
class ObjectGround :
	public ObjectBase
{
public:
	ObjectGround(SceneBase* pScene);
	void InitLocal() override;

	DEFINE_OBJECT_TYPE(ObjectGround)	// �I�u�W�F�N�g�̎��ID�擾�֐�

protected:
	ComponentGeometry* m_pCompGeometry;	// �`��
	ComponentGround* m_pCompGround;		// �n�ʃR���|�[�l���g
};


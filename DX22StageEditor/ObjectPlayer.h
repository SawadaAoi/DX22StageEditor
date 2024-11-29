/* ========================================
	DX22Base/
	------------------------------------
	�v���C���[�I�u�W�F�N�g�p�w�b�_
	------------------------------------
	�����F�v���C���[�I�u�W�F�N�g�̏������Ǘ�����
	------------------------------------
	ObjectPlayer.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ObjectBase.h"
#include "ComponentTransform.h"
#include <memory>

// =============== �N���X��` =====================
class ObjectPlayer :
    public ObjectBase
{
public:
	ObjectPlayer(SceneBase* pScene);
	void InitLocal();
	void UpdateLocal();
	void DrawLocal();

	// �Q�b�^�[
	DEFINE_OBJECT_TYPE	// �I�u�W�F�N�g�̎��ID�擾�֐�

private:
	ComponentTransform* m_pTransform;
	class ComponentGeometry* m_pGeometry;
	std::unique_ptr<class ShapeLine> m_pLine;
	class ComponentGroundRaycast* m_pGroundRaycast;

};


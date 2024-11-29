/* ========================================
	DX22Base/
	------------------------------------
	�f�o�b�O�J�����p�w�b�_
	------------------------------------
	�����F���R�Ɉړ��A��]�ł���J����
	------------------------------------
	ComponentCameraDebug.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"


// =============== �N���X��` =====================
class ComponentCameraDebug :
    public ComponentBase
{
public:
	ComponentCameraDebug(class ObjectBase* pObject);

	void Init() override;
	void Update() override;

	DEFINE_COMPONENT_TYPE;
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	void Move();
	void Rotate();
private:

	class ComponentCameraBase* m_pCameraBase;	// �J�����̊�{�@�\
	float m_fMoveSpeed;		// �ړ����x
	float m_fRotSpeed;	// ��]���x
};


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
#include "Vector2.h"
#include "Input.h"

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
	void RotateKey();
	void RotateMouse();
private:

	class ComponentCameraBase* m_pCameraBase;	// �J�����̊�{�@�\
	float m_fMoveSpeed;		// �ړ����x
	float m_fRotSpeed;	// ��]���x

	Vector2<float> m_vMouseMove;	// �}�E�X�ړ���
	POINT m_CursorPosOld;			// 1�t���[���O�̃J�[�\�����W

	bool m_bRotateMouse;	// ��]����
};


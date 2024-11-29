/* ========================================
	DX22Base/
	------------------------------------
	�}�`�\���R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�w�肳�ꂽ�}�`��\������
	------------------------------------
	ComponentGeometry.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"	// �R���|�[�l���g���N���X
#include "ShapeBase.h"		// �}�`���N���X
#include <memory>

// =============== �N���X��` ===================
class ComponentGeometry :
	public ComponentBase
{
public:
	enum E_ShapeType
	{
		TYPE_NONE = 0,		// �Ȃ�
		TYPE_BOX,			// ������
		TYPE_SPHERE,		// ��
		TYPE_PLANE,			// ����

		TYPE_MAX,			// �}�`�̍ő吔
	};
public:
	ComponentGeometry(ObjectBase* pOwner);

	void Init() override;
	void Update() override;
	void Draw() override;

	// �Q�b�^�[
	void SetShapeType(E_ShapeType eType);	// �}�`�̎�ނ�ݒ�	

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitShapeTypeList();
	DebugUI::Item* InitTextureList();
#endif // _DEBUG
private:
	void CreateShape();	// �}�`�I�u�W�F�N�g�𐶐�

	std::shared_ptr<ShapeBase> m_pShape;	// �}�`�I�u�W�F�N�g
	class ComponentTransform* m_pTransform;	// ���W�R���|�[�l���g

	Texture*	m_pTexture;	// �e�N�X�`��
	bool		m_bIsTex;	// �e�N�X�`���g�p�t���O

	E_ShapeType m_eType;	// �}�`�̎��

};


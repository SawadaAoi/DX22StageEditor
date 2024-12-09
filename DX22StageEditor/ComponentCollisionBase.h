/* ========================================
	DX22Base/
	------------------------------------
	�Փ˔���R���|�[�l���g���p�w�b�_
	------------------------------------
	�����F�e�`��̏Փ˔���̊��N���X
	------------------------------------
	ComponentCollisionBase.h
========================================== */
#pragma once

// =============== �C���N���[�h =====================
#include "ComponentBase.h"
#include <memory>
#include "Vector3.h"
#include "Quaternion.h"
#include <unordered_map>

#include "ShapeBase.h"
#include "ShapeLine.h"

#include "ColorVec3.h"
#include <memory>

// =============== �O���錾 =======================
class ComponentCollisionAABB;
class ComponentCollisionOBB;
class ComponentCollisionSphere;

// =============== �N���X��` =====================
class ComponentCollisionBase :
	public ComponentBase
{
public:

	// �f�o�b�O�\���Փ˔��胏�C���[�t���[���̐F
	const Vector3<float> WIRE_COLOR_NORMAL	= ColorVec3::GREEN;	// �ʏ펞
	const Vector3<float> WIRE_COLOR_HIT		= ColorVec3::RED;	// �Փˎ�

	// �R���W�����̎��
	enum E_ColType
	{
		COL_NONE,		// �R���W�����Ȃ�
		COL_AABB,		// �{�b�N�X�R���W����(�����s�{�b�N�X)
		COL_OBB,		// �{�b�N�X�R���W����(�C�ӎ��{�b�N�X)
		COL_SPHERE,		// �X�t�B�A�R���W����
	};

public:
	ComponentCollisionBase(ObjectBase* pOwner);
	~ComponentCollisionBase(); \
		void Update() override;
	void Draw() override;

	void UpdateCollision(ComponentCollisionBase* otherCol);			// �Փ˔���X�V�֐�

	// �Q�b�^�[
	Vector3<float> GetPosition();
	Vector3<float> GetPositionOffset();
	Vector3<float> GetScale();
	Quaternion GetRotation();
	bool GetRefOwnerTransform();
	E_ColType GetColType();
	bool GetDynamic();
	bool GetEnable();
	bool GetTrigger();

	// �Z�b�^�[
	void SetPosition(Vector3<float> vPos);
	void SetPositionOffset(Vector3<float> vOffset);
	void SetScale(Vector3<float> vScale);
	void SetRotation(Quaternion qRot);
	void SetRefOwnerTransform(bool bRef);
	void SetColType(E_ColType eType);
	void SetDynamic(bool bDynamic);
	void SetEnable(bool bEnable);
	void SetTrigger(bool bTrigger);

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
		void DebugColBase(DebugUI::Item* pGroupItem);
#endif // _DEBUG
protected:
	virtual bool CheckCollision(ComponentCollisionBase* otherCol) = 0;		// �Փ˔���
	bool CheckCollisionAABBToSphere(ComponentCollisionAABB* colAABB, ComponentCollisionSphere* colSphere);							// AABB�Ƌ��̏Փ˔���
	bool CheckCollisionAABBToOBB(ComponentCollisionAABB* colAABB, ComponentCollisionOBB* colOBB);									// AABB��OBB�̏Փ˔���
	bool CheckCollisionOBBToSphere(ComponentCollisionOBB* colOBB, ComponentCollisionSphere* colSphere, bool bSetMtv = false);		// OBB�Ƌ��̏Փ˔���
protected:
	class ComponentTransform*	m_pOwnerTransform;		// �����I�u�W�F�N�g�̃g�����X�t�H�[��
	Vector3<float>				m_vPosition;			// �R���W�����̒��S���W
	Vector3<float>				m_vPositionOffset;		// �R���W�����̒��S���W�I�t�Z�b�g
	Vector3<float>				m_vScale;				// �R���W�����̃X�P�[��
	Quaternion					m_qRotation;			// �R���W�����̉�]
	bool						m_bRefOwnerTransform;	// �����I�u�W�F�N�g�̃g�����X�t�H�[�����Q�Ƃ��邩�ǂ���

	bool								m_bIsDispCol;	// �R���W�����\���t���O
	E_ColType							m_eColType;		// �R���W�����̎��
	std::unique_ptr<ShapeBase>			m_pShape;		// �R���W�����̌`��(�f�o�b�O�\���p)

	bool								m_bIsDynamic;		// (true:���I�R���W�����Afalse:�ÓI�R���W����)
	bool								m_bIsEnabled;		// (true:�R���W�����L���Afalse:�R���W��������)
	bool								m_bIsTrigger;		// �g���K�[�R���W�����t���O(���蔲����R���W����)

	// �e�I�u�W�F�N�g�Ƃ̏Փˏ�Ԃ�ێ�����}�b�v(�L�[�F�I�u�W�F�N�g�A�l�F�Փˏ��(true:�Փ˒��Afalse:��Փ�)))
	std::unordered_map<ObjectBase*, bool> m_bColStatesMap;	

	static  inline bool m_bIsDispColAll = false;	// �S�R���W�����\���t���O

#ifdef _DEBUG
	DebugUI::Item* m_pColObjList;	// �Փ˃I�u�W�F�N�g���X�g(�f�o�b�O�\���p)
#endif // _DEBUG
};


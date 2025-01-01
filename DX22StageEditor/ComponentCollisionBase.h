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


	// �ŏ��ړ��x�N�g���\����(�߂荞�ݖ߂��p)
	struct T_MTV
	{
		Vector3<float>	vAxis;		// �Փˎ�
		float			fOverlap;	// �d�Ȃ��
		bool			bIsCol;		// �Փ˂��Ă��邩
		bool			bIsTrigger;	// true:�g���K�[����p(���蔲������)
		std::string		sName;		// �Փˑ���I�u�W�F�N�g��

		T_MTV()
		{
			vAxis = Vector3<float>(0.0f, 0.0f, 0.0f);
			fOverlap = 10000.0f;
			bIsCol = false;
			bIsTrigger = false;
			sName = "None";
		}
	};

public:
	ComponentCollisionBase(ObjectBase* pOwner);
	~ComponentCollisionBase(); 
	void Update() override;
	void Draw() override;

	void UpdateCollision(ComponentCollisionBase* otherCol);			// �Փ˔���X�V�֐�
	void UpdateCollisionMap(ComponentCollisionBase* otherCol);										// �Փˏ�ԃ}�b�v�X�V�֐�

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
	std::vector<T_MTV> GetMtvs();
	T_MTV GetMtv(std::string sName);

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
	void SetMtvs(std::vector<T_MTV> mtvs);

	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
	void DebugColBase(DebugUI::Item* pGroupItem, std::string sCompName);
	void UpdateDebugMTVList();
#endif // _DEBUG
private:
	void UpdateMTVList();	// �ŏ��ړ��x�N�g�����X�g�X�V�֐�

protected:
	virtual bool CheckCollision(ComponentCollisionBase* otherCol) = 0;		// �Փ˔���
	void SetColObjMTV(T_MTV tMtv, std::string sName);						// �ŏ��ړ��x�N�g���X�V�֐�

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

	// �ŏ��ړ��x�N�g���\���̔z��
	// ���Փˎ��̂߂荞�݉����Ɏg�p
	// ���Փˊm�F����I�u�W�F�N�g����
	std::vector<T_MTV>	m_tMtvs;

#ifdef _DEBUG
	DebugUI::Item*	m_pColObjList;	// �Փ˃I�u�W�F�N�g���X�g(�f�o�b�O�\���p)
	std::string		m_sColCompName;	// �R���|�[�l���g��(�f�o�b�O���j���[�\���m�F�p)
	int				m_nSelectMTV;	// �I�𒆂̍ŏ��ړ��x�N�g��
#endif // _DEBUG
};


/* ========================================
	DX22Base/
	------------------------------------
	���C�g��{�R���|�[�l���g�pcpp
	------------------------------------
	ComponentLightBase.cpp
========================================== */

// =============== �C���N���[�h ===================
#include "ComponentLightBase.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "ShapeLine.h"
#include "ShapeSphere.h"
#include "ColorVec3.h"
#include "MathUtils.h"

/* ========================================
	�R���X�g���N�^�֐�
	-------------------------------------
	���e�F������
	-------------------------------------
	����1�F���L�҃I�u�W�F�N�g
========================================== */
ComponentLightBase::ComponentLightBase(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderLightBase)
	, m_eLightType(E_LightType::NONE)
	, m_vColor(Vector3<float>(1.0f, 1.0f, 1.0f))
	, m_fRange(0.0f)
	, m_fAngle(0.0f)
	, m_bDispLine(false)
	, m_pDirLine(nullptr)
{
}

/* ========================================
	�������֐�
	-------------------------------------
	���e�F�R���|�[�l���g�̏�����
========================================== */
void ComponentLightBase::Init()
{
	Vector3<float> vStart = m_pOwnerObj->GetTransform()->GetPosition();
	Vector3<float> vEnd = vStart + m_pOwnerObj->GetTransform()->GetForwardVector();
	m_pDirLine = std::make_unique<ShapeLine>(vStart, vEnd, ColorVec3::RED);

	m_pSphere = std::make_unique<ShapeSphere>();
	m_pSphere->SetPosition(vStart);
	m_pSphere->SetScale(Vector3<float>(m_fRange, m_fRange, m_fRange));
	m_pSphere->SetDrawMode(ShapeBase::E_DrawMode::DRAW_WIRE_FRAME);

}

/* ========================================
	�X�V�֐�
	-------------------------------------
	���e�F�X�V����
========================================== */
void ComponentLightBase::Update()
{
	m_pDirLine->SetPos(m_pOwnerObj->GetTransform()->GetPosition());
	m_pDirLine->SetRotation(m_pOwnerObj->GetTransform()->GetRotation());

	switch (m_eLightType)
	{
	case ComponentLightBase::POINT:
		m_pSphere->SetPosition(m_pOwnerObj->GetTransform()->GetPosition());
		m_pSphere->SetScale(Vector3<float>(m_fRange, m_fRange, m_fRange));
		break;
	case ComponentLightBase::SPOT:
		m_pDirLine->SetScale(Vector3<float>(m_fRange, m_fRange, m_fRange));
		break;
	}


#ifdef _DEBUG
	// LightBase����\�����Ă��邩�ǂ���
	if (&WIN_OBJ_INFO["LightBase"] != DebugUI::Item::dummy 
		&& WIN_OBJ_INFO["ObjectName"].GetText() == m_pOwnerObj->GetName())
	{
		DirectX::XMFLOAT4 color = WIN_OBJ_INFO["LightBase"]["Color"].GetColor();
		m_vColor			= Vector3<float>(color.x, color.y, color.z);
	}
	
#endif // _DEBUG

}

/* ========================================
	�`��֐�
	-------------------------------------
	���e�F�`�揈��
========================================== */
void ComponentLightBase::Draw()
{
	if (!m_bDispLine) return;
	
	if (m_eLightType == E_LightType::POINT)
	{
 		m_pSphere->Draw();
	}
	else
	{
		m_pDirLine->Draw();
	}

}

/* ========================================
	�Q�b�^�[(���C�g���)�֐�
	-------------------------------------
	�ߒl�FE_LightType	���C�g���
=========================================== */
ComponentLightBase::E_LightType ComponentLightBase::GetLightType() const
{
	return m_eLightType;
}

/* ========================================
	�Q�b�^�[(�f�B�t���[�Y�J���[)�֐�
	-------------------------------------
	�ߒl�FVector3<float>	�f�B�t���[�Y�J���[
=========================================== */
Vector3<float> ComponentLightBase::GetColor() const
{
	return m_vColor;
}

/* ========================================
	�Q�b�^�[(���C�g�L���͈�)�֐�
	-------------------------------------
	�ߒl�Ffloat	���C�g�L���͈�
=========================================== */
float ComponentLightBase::GetRange() const
{
	return m_fRange;
}

/* ========================================
	�Q�b�^�[(�X�|�b�g���C�g�p�x)�֐�
	-------------------------------------
	�ߒl�Ffloat	�X�|�b�g���C�g�p�x
=========================================== */
float ComponentLightBase::GetAngle() const
{
	return m_fAngle;
}

/* ========================================
	�Z�b�^�[(���C�g���)�֐�
	-------------------------------------
	����1�Ftype	���C�g���
=========================================== */
void ComponentLightBase::SetLightType(E_LightType type)
{
	m_eLightType = type;
}

/* ========================================
	�Z�b�^�[(�f�B�t���[�Y�J���[)�֐�
	-------------------------------------
	����1�Fcolor	�f�B�t���[�Y�J���[
=========================================== */
void ComponentLightBase::SetColor(const Vector3<float>& color)
{
	m_vColor = color;
}

/* ========================================
	�Z�b�^�[(���C�g�L���͈�)�֐�
	-------------------------------------
	����1�Frange	���C�g�L���͈�
=========================================== */
void ComponentLightBase::SetRange(float range)
{
	m_fRange = range;
}

/* ========================================
	�Z�b�^�[(�X�|�b�g���C�g�p�x)�֐�
	-------------------------------------
	����1�Fangle	�X�|�b�g���C�g�p�x
=========================================== */
void ComponentLightBase::SetAngle(float angle)
{
	m_fAngle = angle;
}


#ifdef _DEBUG
/* ========================================
	�f�o�b�O�֐�
	-------------------------------------
	���e�F�f�o�b�O�p�̏���
	-------------------------------------
	����1�F�E�B���h�E	DebugUI::Window&
=========================================== */
void ComponentLightBase::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupLight = Item::CreateGroup("LightBase");
	pGroupLight->AddGroupItem(Item::CreateValue("LightType", Item::Kind::Text));

	pGroupLight->AddGroupItem(Item::CreateValue("Color",	Item::Kind::Color));
	pGroupLight->AddGroupItem(Item::CreateBind("DispLine",	Item::Kind::Bool, &m_bDispLine));
	pGroupLight->AddGroupItem(Item::CreateBind("Range",		Item::Kind::Float, &m_fRange));
	pGroupLight->AddGroupItem(Item::CreateCallBack("Angle", Item::Kind::Float, [this](bool isWrite, void* arg) 
	{
		float* pAngle = static_cast<float*>(arg);
		if (isWrite)
			m_fAngle = MathUtils::ToRadian(*pAngle);
		else
			*pAngle = MathUtils::ToDegree(m_fAngle);

	}));

	window.AddItem(pGroupLight);

	// �����l�ݒ�
	// �F
	WIN_OBJ_INFO["LightBase"]["Color"].SetColor(DirectX::XMFLOAT4(m_vColor.x, m_vColor.y, m_vColor.z, 1.0f));

	// ���C�g���
	std::string sLightType;
	switch (m_eLightType)
	{
	case ComponentLightBase::NONE:			sLightType = "NONE";		break;
	case ComponentLightBase::DIRECTIONAL:	sLightType = "DIRECTIONAL"; break;
	case ComponentLightBase::POINT:			sLightType = "POINT";		break;
	case ComponentLightBase::SPOT:			sLightType = "SPOT";		break;
	}
	WIN_OBJ_INFO["LightBase"]["LightType"].SetText(sLightType.c_str());
}

#endif // _DEBUG

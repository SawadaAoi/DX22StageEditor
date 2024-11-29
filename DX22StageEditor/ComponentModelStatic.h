/* ========================================
	DX22Base/
	------------------------------------
	�R���|�[�l���g(�ÓI���f��)�p�w�b�_
	------------------------------------
	�����F�ÓI���f���̕`����s��
	------------------------------------
	ComponentModelStatic.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"


// =============== �O���錾 =======================
class ComponentTransform;	// �g�����X�t�H�[���R���|�[�l���g
class ModelStatic;			// �ÓI���f��


// =============== �N���X��` =====================
class ComponentModelStatic :
    public ComponentBase
{
public:
	ComponentModelStatic(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Draw() override;


	// �Q�b�^�[
	ModelStatic* GetModel();
	bool GetIsVisible();
	Vector3<float> GetPosOffset();

	// �Z�b�^�[
	void SetModel(ModelStatic* pModel);
	void SetIsVisible(bool bIsVisible);
	void SetPosOffset(Vector3<float> offset);

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitModelList();	
	void ClickModelList(std::string sListName);
#endif // _DEBUG
private:
	ComponentTransform* m_pCompTran;	// �g�����X�t�H�[���R���|�[�l���g
	ModelStatic*		m_pModel;		// ���f��

	bool				m_bIsVisible;	// �\���t���O
	Vector3<float>		m_vPosOffset;	// �ʒu�I�t�Z�b�g

};


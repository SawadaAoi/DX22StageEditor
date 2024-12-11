/* ========================================
	DX22Base/
	------------------------------------
	���C�g��{�R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F���C�g�̊�{�@�\��񋟂���
	------------------------------------
	ComponentLightBase.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"
#include <memory>

// =============== �O���錾 =======================
class ComponentTransform;
class ShapeLine;
class ShapeSphere;

// =============== �N���X��` =====================
class ComponentLightBase :
    public ComponentBase
{
public:
	enum E_LightType
	{
		NONE,			// �Ȃ�
		DIRECTIONAL,	// �f�B���N�V���i�����C�g
		POINT,			// �|�C���g���C�g
		SPOT,			// �X�|�b�g���C�g
	};
public:
    ComponentLightBase(ObjectBase* pOwner);

    virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

	// �Q�b�^�[
	E_LightType GetLightType() const;
	Vector3<float> GetColor() const;
	float GetRange() const;
	float GetAngle() const;

	// �Z�b�^�[
	void SetLightType(E_LightType type);
	void SetColor(const Vector3<float>& color);
	void SetRange(float range);
	void SetAngle(float angle);

	DEFINE_COMPONENT_TYPE
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	ComponentTransform* m_pCompTransform;	// �g�����X�t�H�[���R���|�[�l���g
	E_LightType			m_eLightType;		// ���C�g�̎��
	Vector3<float>		m_vColor;			// �f�B�t���[�Y�J���[
	float				m_fRange;			// ���C�g�̗L���͈�
	float				m_fAngle;			// �X�|�b�g���C�g�̊p�x

	bool m_bDispLine;						// ���C���̕\���t���O
	std::unique_ptr<ShapeLine>	m_pDirLine;	// ���C�g�̌����̕`��p
	std::unique_ptr<ShapeSphere> m_pSphere;	// ���C�g�̗L���͈͂̕`��p
};


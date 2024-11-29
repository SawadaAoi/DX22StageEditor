/* ========================================
	DX22Base/
	------------------------------------
	�J������{�R���|�[�l���g�p�w�b�_
	------------------------------------
	�����F�J�����̊�{�@�\��񋟂���
	------------------------------------
	ComponentCameraBase.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"
#include "Vector3.h"		// 3�����x�N�g���N���X
#include "Vector2.h"
#include "ShapeLine.h"
#include <memory>

// =============== �N���X��` =====================
class ComponentCameraBase :
    public ComponentBase
{
public:
	ComponentCameraBase(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Move(Vector3<float> vMove);							// �J�����ړ��֐�
	void Rotate(float pitch, float yaw, float roll);			// �J������]�֐�
	void RotateLimit(float pitch, float yaw, float LimitPitch);	// �J������]�֐�(�����t��)
	void ZoomInOut(float fZoom);								// �Y�[���C���A�E�g�֐�
	void RotateX(float fAngle, bool world);						// X����]�֐�
	void RotateY(float fAngle, bool world);						// Y����]�֐�


	DirectX::XMFLOAT4X4 GetViewMatrix();		// �r���[�s��擾�֐�
	DirectX::XMFLOAT4X4 GetViewMatrixNotTransposed();		// �r���[�s��擾�֐�
	DirectX::XMMATRIX GetInvViewMatrix();		// �r���[�s��̋t�s��擾�֐�

	DirectX::XMFLOAT4X4 GetProjectionMatrix();	// �v���W�F�N�V�����s��擾�֐�

	DirectX::XMFLOAT4X4 GetProjectionMatrixUI();		// 2D�p�r���[�s��擾�֐�

	// �Q�b�^�[
	Vector3<float> GetForward();	// �J�����̑O�����擾�֐�
	Vector3<float> GetRight();		// �J�����̉E�����擾�֐�
	Vector3<float> GetUp();			// �J�����̏�����擾�֐�


	DEFINE_COMPONENT_TYPE	// �R���|�[�l���g�̎��ID�擾�֐�

#ifdef _DEBUG
		void Debug(DebugUI::Window& window) override;
#endif // _DEBUG
private:
	void SetNearFarClipPos();	// �j�A�N���b�v�ʂƃt�@�[�N���b�v�ʂ̃��C���`��֐�
	void InitNearFarClipLine();		// �j�A�N���b�v�ʂƃt�@�[�N���b�v�ʂ̃��C���`��֐�
	void UpdateNearFarClipLine();	// �j�A�N���b�v�ʂƃt�@�[�N���b�v�ʂ̃��C���`��֐�
private:

	class ComponentTransform* m_pTransform;	// ���̃R���|�[�l���g����������I�u�W�F�N�g�̃g�����X�t�H�[��

	// �r���[�s����쐬����l
	Vector3<float> m_vLook;		// �J�����̒����_
	Vector3<float> m_vUp;		// �J�����̏����

	// �v���W�F�N�V�����s����쐬����l
	float m_fFovY;		// �c�����̉�p
	float m_fAspect;	// �A�X�y�N�g��
	float m_fNear;		// �J�����Ƃ̍ŏ�����
	float m_fFar;		// �J�����Ƃ̍ő勗��

	float m_fPitch;		// X����]�p�x

	std::unique_ptr<ShapeLine> m_pFrustumLine;	// �J�����`��͈͂̎����䃉�C��
	bool m_bDispLine;							// ���C���\���t���O

	Vector2<float> m_vNearHW;	// �j�A�N���b�v�ʂ̕��ƍ���
	Vector2<float> m_vFarHW;	// �t�@�[�N���b�v�ʂ̕��ƍ���

	Vector3<float> m_CornersNear[4];	// �j�A�N���b�v�ʂ�4���̍��W
	Vector3<float> m_CornersFar[4];	// �t�@�[�N���b�v�ʂ�4���̍��W

};


/* ========================================
	DX22Base/
	------------------------------------
	�r���{�[�h�p�w�b�_
	------------------------------------
	�����F��ɃJ�����̕����������摜�����[���h��ԏ�ɕ`�悷��
	------------------------------------
	ComponentBillboard.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "ComponentBase.h"
#include "Vector2.h"
#include "Vector3.h"
#include <memory>

// =============== �O���錾 =======================
class ComponentTransform;	// �g�����X�t�H�[���R���|�[�l���g
class Texture;				// �e�N�X�`��
class VertexShader;			// ���_�V�F�[�_
class PixelShader;			// �s�N�Z���V�F�[�_
class MeshBuffer;			// ���b�V���o�b�t�@


// =============== �N���X =======================
class ComponentBillboard :
    public ComponentBase
{
public:
	ComponentBillboard(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Draw() override;

	// �Q�b�^�[
	Texture* GetTexture();
	Vector3<float> GetPosOffset();
	Vector2<float> GetUvPos();
	Vector2<float> GetUvScale();
	Vector3<float> GetColor();
	float GetAlpha();

	// �Z�b�^�[
	void SetTexture(Texture* pTexture);
	void SetPosOffset(Vector3<float> offset);
	void SetUvPos(Vector2<float> uvPos);
	void SetUvScale(Vector2<float> uvScale);
	void SetColor(Vector3<float> color);
	void SetAlpha(float alpha);

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitTextureList();
#endif // _DEBUG
protected:
	void InitMesh();
	DirectX::XMFLOAT4X4 CreateWorldMatrix();
protected:
	ComponentTransform*			m_pCompTran;	// �g�����X�t�H�[���R���|�[�l���g
	Texture*					m_pTexture;		// �e�N�X�`��
	std::shared_ptr<MeshBuffer> m_pMesh;		// ���b�V���o�b�t�@

	VertexShader*		m_pVS;		// ���_�V�F�[�_
	PixelShader*		m_pPS;		// �s�N�Z���V�F�[�_

	Vector3<float>		m_vPosOffset;	// �I�t�Z�b�g


	Vector2<float>		m_vUvPos;	// UV���W
	Vector2<float>		m_vUvScale;	// UV�X�P�[��
	Vector3<float>		m_vColor;	// �F
	float				m_fAlpha;	// ���ߓx
};


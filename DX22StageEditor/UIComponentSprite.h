/* ========================================
	DX22Base/
	------------------------------------
	UI�摜�p�w�b�_
	------------------------------------
	�����FUI�摜��`�悷�邽�߂̃R���|�[�l���g
	------------------------------------
	UIComponentSprite.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "UIComponentBase.h"
#include <memory>

#include "Texture.h"

#include "Vector2.h"
#include "Vector3.h"


// =============== �O���錾 =======================
class UIComponentTransform;
class VertexShader;
class PixelShader;
class MeshBuffer;

// =============== �N���X =======================
class UIComponentSprite :
    public UIComponentBase
{
public:
	UIComponentSprite(UIObjectBase* owner);

	void Init();
	void Draw();

	// �Q�b�^�[
	Texture* GetTexture();
	bool GetIsVisible();
	Vector2<float> GetOffset();
	Vector2<float> GetUvPos();
	Vector2<float> GetUvScale();
	Vector3<float> GetColor();
	float GetAlpha();

	// �Z�b�^�[
	void SetTexture(Texture* pTexture);
	void SetIsVisible(bool visible);
	void SetOffset(Vector2<float> offset);
	void SetUvPos(Vector2<float> uvPos);
	void SetUvScale(Vector2<float> uvScale);
	void SetColor(Vector3<float> color);
	void SetAlpha(float alpha);

	DEFINE_UI_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitTextureList();
#endif // _DEBUG
private:
	void InitMesh();
	DirectX::XMFLOAT4X4 CreateWorldMatrix();
private:
	UIComponentTransform* m_pCompTran;

	std::shared_ptr<MeshBuffer> m_pMesh;	// ���b�V���o�b�t�@
	Texture* m_pTexture;	// �e�N�X�`��

	VertexShader*	m_pVS;
	PixelShader*	m_pPS;

	bool				m_bIsVisible;	// �\���t���O

	Vector2<float>		m_vOffset;	// �I�t�Z�b�g
	Vector2<float>		m_vUvPos;	// UV���W
	Vector2<float>		m_vUvScale;	// UV�X�P�[��
	Vector3<float>		m_vColor;	// �F
	float				m_fAlpha;	// ���ߓx

};


/* ========================================
	DX22Base/
	------------------------------------
	�����_�[�^�[�Q�b�g�r���[�p�w�b�_
	------------------------------------
	�����F�����_�[�^�[�Q�b�g�������N���X
		�@(�`�挋�ʂ�ۑ����邽�߂̃o�b�t�@)
	------------------------------------
	RenderTarget.h
========================================== */
#pragma once

// =============== �C���N���[�h ===================
#include "Texture.h"	// �e�N���X

// =============== �N���X��` =====================
class RenderTarget : public Texture
{
public:
	RenderTarget();
	~RenderTarget();
	void Clear();
	void Clear(const float* color);
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height);
	HRESULT CreateFromScreen();
	ID3D11RenderTargetView* GetView() const;

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ID3D11RenderTargetView* m_pRTV; // �����_�[�^�[�Q�b�g�r���[
};


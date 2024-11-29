/* ========================================
	DX22Base/
	------------------------------------
	�f�v�X�X�e���V���r���[�p�w�b�_
	------------------------------------
	�����F�f�v�X�X�e���V���r���[�������N���X
		�@(�[�x�e�X�g��X�e���V���e�X�g���s�����߂ɕK�v�ȃr���[)
		�@�[�x�e�X�g: �߂��I�u�W�F�N�g��D�悵�ĕ\�����A�B�ꂽ�������B�����ƂŁA��胊�A���ȕ`����������܂��B
		�@�X�e���V���e�X�g: ����̗̈�ɑ΂��Ă̂ݕ`����s���ȂǁA��荂�x�ȕ`�搧����\�ɂ��܂��B
	------------------------------------
	DepthStencil.h
========================================== */
#pragma once


// =============== �C���N���[�h ===================
#include "Texture.h"


// =============== �N���X��` =====================
class DepthStencil : public Texture
{
public:
	DepthStencil();
	~DepthStencil();
	void Clear();
	HRESULT Create(UINT width, UINT height, bool useStencil);
	ID3D11DepthStencilView* GetView() const;

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);

private:
	ID3D11DepthStencilView* m_pDSV;
};


/* ========================================
	DX22Base/
	------------------------------------
	ピクセルシェーダー用ヘッダ
	------------------------------------
	説明：ピクセルシェーダーのクラス
	------------------------------------
	PixelShader.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Shader.h"


// =============== クラス定義 =====================
class PixelShader : public Shader
{
public:
	PixelShader();
	~PixelShader();
	void Bind(void);
protected:
	HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11PixelShader* m_pPS;	// ピクセルシェーダ
};

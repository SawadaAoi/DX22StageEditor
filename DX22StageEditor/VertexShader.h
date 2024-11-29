/* ========================================
	DX22Base/
	------------------------------------
	頂点シェーダー用ヘッダ
	------------------------------------
	説明：頂点シェーダーのクラス
	------------------------------------
	VertexShader.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Shader.h"

// =============== クラス定義 =====================
class VertexShader : public Shader
{
public:
	VertexShader();
	~VertexShader();
	void Bind(void);	// シェーダーを描画に使用
protected:
	HRESULT MakeShader(void* pData, UINT size);

private:
	ID3D11VertexShader*		m_pVS;			// 頂点シェーダ
	ID3D11InputLayout*		m_pInputLayout;	// インプットレイアウト
};

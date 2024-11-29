/* ========================================
	DX22Base/
	------------------------------------
	シェーダー用ヘッダ
	------------------------------------
	説明:シェーダーの基本クラス
	------------------------------------
	Shader.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "DirectXManager.h"
#include "Texture.h"
#include <string>
#include <map>
#include <vector>


// =============== クラス定義 =====================
class Shader
{
protected:
	// シェーダーの種類
	enum Kind
	{
		Vertex,	// 頂点シェーダ
		Pixel,	// ピクセルシェーダ
	};
public:
	Shader(Kind kind);

	virtual ~Shader();
	HRESULT Load(const char* pFileName);	// シェーダーファイル(*.cso)を読み込む処理
	HRESULT Compile(const char* pCode);		// 文字列からシェーダをコンパイル

	void WriteBuffer(UINT slot, const void* pData);	// 定数の書き込み
	void SetTexture(UINT slot, const Texture* tex);	// テクスチャの設定
	virtual void Bind(void) = 0;				// シェーダーを描画に使用


private:
	HRESULT Make(void* pData, UINT size);

	Kind m_kind;
protected:
	// シェーダーファイルを読み込んだ後、シェーダーの種類別に処理を行う
	virtual HRESULT MakeShader(void* pData, UINT size) = 0;

	std::vector<ID3D11Buffer*>				m_pBuffers;		// 定数バッファ
	std::vector<ID3D11ShaderResourceView*>	m_pTextures;	// テクスチャリソース
};





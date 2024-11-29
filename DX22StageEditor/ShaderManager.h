/* ========================================
	DX22Base/
	------------------------------------
	シェーダー管理用ヘッダ
	------------------------------------
	説明：シェーダーをまとめて読み込んで管理する
	------------------------------------
	ShaderManagaer.h
========================================== */

// =============== インクルード ===================
#include "VertexShader.h"
#include "PixelShader.h"
#include <vector>
#include <unordered_map>
#include <memory>


// =============== 定数定義 =======================
#define VS_KEY CShaderManagaer::E_VS_KEY	// 記述省略のため
#define PS_KEY CShaderManagaer::E_PS_KEY	// 記述省略のため

#define SHADER_INST CShaderManagaer::GetInstance()			// 記述省略のため
#define GET_VS_DATA CShaderManagaer::GetInstance().GetVs	// 文章省略
#define GET_PS_DATA CShaderManagaer::GetInstance().GetPs	// 文章省略

class CShaderManagaer
{
public:
	// 頂点シェーダー種類
	enum E_VS_KEY
	{
		VS_SHAPE,				// 図形描画用
		VS_LINE,				// ライン描画用
		VS_SPRITE,				// スプライト描画用
		VS_MODEL_STATIC,	// 静的モデル用デフォルト
		VS_MODEL_ANIME,		// アニメーションモデル用デフォルト
	};

	// ピクセルシェーダー種類
	enum E_PS_KEY
	{
		PS_SHAPE,				// 図形描画用
		PS_LINE,				// ライン描画用
		PS_SPRITE,				// スプライト描画用
		PS_MODEL_STATIC,	// 静的モデル用デフォルト
		PS_MODEL_ANIME,		// アニメーションモデル用デフォルト
	};

public:
	static CShaderManagaer& GetInstance();	// インスタンス取得
	void Init();	// 初期化

	// シェーダー読み込み
	VertexShader* GetVs(CShaderManagaer::E_VS_KEY e_GetKey);
	PixelShader* GetPs(CShaderManagaer::E_PS_KEY e_GetKey);

private:
	CShaderManagaer() {};	// コンストラクタ

	// シェーダーデータを格納しておく
	std::unordered_map <E_VS_KEY, std::shared_ptr<VertexShader>> m_pVsMap;
	std::unordered_map <E_PS_KEY, std::shared_ptr<PixelShader>>	m_pPsMap;



};


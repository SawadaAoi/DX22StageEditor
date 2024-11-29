/* ========================================
	DX22Base/
	------------------------------------
	シェーダー管理用cpp
	------------------------------------
	ShaderManagaer.cpp
========================================== */

// =============== インクルード ===================
#include "ShaderManager.h"


// =============== 定数定義 =======================
// 頂点シェーダー保存先パス
const std::unordered_map < CShaderManagaer::E_VS_KEY, std::string > VS_SHADER_PATH
{
	{VS_KEY::VS_SHAPE,			"Assets/shader/VS_Shape.cso"		},
	{VS_KEY::VS_LINE,			"Assets/shader/VS_Line.cso"			},
	{VS_KEY::VS_SPRITE,			"Assets/shader/VS_Sprite.cso"		},
	{VS_KEY::VS_MODEL_STATIC,	"Assets/shader/VS_ModelStatic.cso"	},
	{VS_KEY::VS_MODEL_ANIME,	"Assets/shader/VS_ModelAnime.cso"	},
};

// ピクセルシェーダー保存先パス
const std::unordered_map < CShaderManagaer::E_PS_KEY, std::string > PS_SHADER_PATH
{
	{PS_KEY::PS_SHAPE,			"Assets/shader/PS_Shape.cso"		},
	{PS_KEY::PS_LINE,			"Assets/shader/PS_Line.cso"			},
	{PS_KEY::PS_SPRITE,			"Assets/shader/PS_Sprite.cso"		},
	{PS_KEY::PS_MODEL_STATIC,	"Assets/shader/PS_ModelStatic.cso"	},
	{PS_KEY::PS_MODEL_ANIME,	"Assets/shader/PS_ModelAnime.cso"	},
};


/* ========================================
	インスタンス取得関数
	-------------------------------------
	内容：インスタンスを取得する
	-------------------------------------
	戻値：インスタンス
=========================================== */
CShaderManagaer& CShaderManagaer::GetInstance()
{
	static CShaderManagaer instance;
	return instance;
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：シェーダーを読み込む
=========================================== */
void CShaderManagaer::Init()
{
	// シェーダーの種類分
	for (std::pair<E_VS_KEY, std::string> PathMap : VS_SHADER_PATH)
	{
		E_VS_KEY eNowKey = PathMap.first;	// 現在のキー
		std::string sNowPath = PathMap.second;	// 現在の保存位置パス

		// 頂点シェーダの読込
		VertexShader* pVs = new VertexShader;
		if (FAILED(pVs->Load(sNowPath.c_str()))) {
			MessageBox(nullptr, sNowPath.c_str(), "Error", MB_OK);
		}

		m_pVsMap.insert(std::make_pair(eNowKey, pVs));	// モデルデータ配列に追加
	}

	// シェーダーの種類分
	for (std::pair<E_PS_KEY, std::string> PathMap : PS_SHADER_PATH)
	{
		E_PS_KEY eNowKey = PathMap.first;	// 現在のキー
		std::string sNowPath = PathMap.second;	// 現在の保存位置パス

		// ピクセルシェーダーの読込
		PixelShader* pPs = new PixelShader;
		if (FAILED(pPs->Load(sNowPath.c_str()))) {
			MessageBox(nullptr, sNowPath.c_str(), "Error", MB_OK);
		}

		m_pPsMap.insert(std::make_pair(eNowKey, pPs));	// モデルデータ配列に追加
	}
}

/* ========================================
	頂点シェーダー取得関数
	-------------------------------------
	内容：キーを元に該当のシェーダーを渡す
	-------------------------------------
	引数1：キー
	-------------------------------------
	戻値：シェーダーポインタ
=========================================== */
VertexShader* CShaderManagaer::GetVs(CShaderManagaer::E_VS_KEY e_GetKey)
{
	return m_pVsMap.at(e_GetKey).get();
}

/* ========================================
	頂点シェーダー取得関数
	-------------------------------------
	内容：キーを元に該当のシェーダーを渡す
	-------------------------------------
	引数1：キー
	-------------------------------------
	戻値：シェーダーポインタ
=========================================== */
PixelShader* CShaderManagaer::GetPs(CShaderManagaer::E_PS_KEY e_GetKey)
{
	return m_pPsMap.at(e_GetKey).get();
}

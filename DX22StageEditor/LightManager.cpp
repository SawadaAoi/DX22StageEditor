/* ========================================
	DX22Base/
	------------------------------------
	ライト管理用cpp
	------------------------------------
	LightManager.cpp
========================================== */

// =============== インクルード ===================
#include "LightManager.h"
#include "SceneBase.h"		// シーン基底クラス


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
LightManager::LightManager()
	: m_pLightList()
	, m_pScene(nullptr)
{
}

/* ========================================
	インスタンス(シングルトン)取得関数
	-------------------------------------
	内容：自クラスのインスタンスを取得
	-------------------------------------
	戻値：自クラスのインスタンス
=========================================== */
LightManager& LightManager::GetInstance()
{
	static LightManager instance;
	return instance;
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：シーン開始時にライトを取得する
	-------------------------------------
	引数1：シーンクラスのポインタ
=========================================== */
void LightManager::Init(SceneBase* pScene)
{
	m_pScene = pScene;

	// ライトリストをクリア
	m_pLightList.clear();

	// シーンに登録されているライトを取得
	m_pLightList = m_pScene->GetSceneObjects<ObjectLight>();
}

/* ========================================
	ライト追加関数
	-------------------------------------
	内容：ライトを追加する
	-------------------------------------
	引数1：ライトオブジェクト
=========================================== */
void LightManager::AddLight(ObjectLight* pLight)
{
	if (GetLightNum() >= MAX_LIGHT_NUM) return;

	m_pLightList.push_back(pLight);
}

/* ========================================
	ライト削除関数
	-------------------------------------
	内容：ライトを削除する
	-------------------------------------
	引数1：ライトオブジェクト
=========================================== */
void LightManager::RemoveLight(ObjectLight* pLight)
{
	auto itr = std::find(m_pLightList.begin(), m_pLightList.end(), pLight);
	if (itr != m_pLightList.end())
	{
		m_pLightList.erase(itr);
	}
}

/* ========================================
	ゲッター(ライト)関数
	-------------------------------------
	内容：指定番号のライトを取得
	-------------------------------------
	引数1：ライト番号
	-------------------------------------
	戻値：ライトオブジェクト
=========================================== */
ObjectLight* LightManager::GetLight(int num)
{
	return m_pLightList.at(num);
}

/* ========================================
	ゲッター(ライトリスト)関数
	-------------------------------------
	戻値：std::vector<ObjectLight*> ライトリスト
=========================================== */
std::vector<ObjectLight*> LightManager::GetLightList()
{
	return m_pLightList;
}

/* ========================================
	ゲッター(ライト数)関数
	-------------------------------------
	戻値：int ライト数
=========================================== */
int LightManager::GetLightNum()
{
	return m_pLightList.size();
}

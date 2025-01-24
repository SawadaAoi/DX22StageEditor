/* ========================================
	CatRobotGame/
	------------------------------------
	ブロックオブジェクト用cpp
	------------------------------------
	ObjectBlock.cpp
========================================== */

// =============== インクルード ===================
#include "ObjectBlock.h"
#include "ComponentGeometry.h"
#include "ComponentCollisionBase.h"
#include "ComponentCollisionOBB.h"
#include "ComponentCollisionAABB.h"
#include "TextureManager.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有シーン
=========================================== */
ObjectBlock::ObjectBlock(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompGeometry(nullptr)
	, m_pCompColBase(nullptr)
	, m_eColType(E_COL_TYPE::COL_OBB)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化を行う
=========================================== */
void ObjectBlock::InitLocal()
{
	m_pCompGeometry = AddComponent<ComponentGeometry>();
	m_pCompGeometry->SetShapeType(ComponentGeometry::TYPE_BOX);
	m_pCompGeometry->SetIsTex(true);
	for (int i = 0; i < 6; i++)
		m_pCompGeometry->SetTexture(GET_TEXTURE_DATA(TextureManager::E_TEX_KEY::BLOCK_SIMPLE), i);

	// 衝突判定形状によってコンポーネントを追加
	if (m_eColType == E_COL_TYPE::COL_AABB)
		m_pCompColBase = AddComponent<ComponentCollisionAABB>();
	else if (m_eColType == E_COL_TYPE::COL_OBB)
		m_pCompColBase = AddComponent<ComponentCollisionOBB>();

}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectBlock::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// テクスチャID
	for (int i = 0; i < 6; i++)
		data.nTextureID[i] = TEXTURE_MNG_INST.GetTextureKey(m_pCompGeometry->GetTexture(i));

	// テクスチャ使用フラグ
	data.bUseTex	= m_pCompGeometry->GetIsTex();
	// 衝突判定形状
	data.eColType	= m_eColType;

	// テクスチャスケール
	for (int i = 0; i < 3; i++)
		data.vUvScale[i] = m_pCompGeometry->GetUvScale(i);
	// テクスチャオフセット
	for (int i = 0; i < 3; i++)
		data.vUvOffset[i] = m_pCompGeometry->GetUvOffset(i);

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectBlock::InputLocalData(std::ifstream& file)
{
	S_SaveData data;

	// ファイルから読み込む
	file.read((char*)&data, sizeof(S_SaveData));

	// テクスチャ設定
	for (int i = 0; i < 6; i++)
		m_pCompGeometry->SetTexture(GET_TEXTURE_DATA((TextureManager::E_TEX_KEY)data.nTextureID[i]), i);
	// テクスチャ使用フラグ
	m_pCompGeometry->SetIsTex(data.bUseTex);
	// 衝突判定形状
	m_eColType = data.eColType;
	// テクスチャスケール
	for (int i = 0; i < 3; i++)
		m_pCompGeometry->SetUvScale(data.vUvScale[i], i);
	// テクスチャオフセット
	for (int i = 0; i < 3; i++)
		m_pCompGeometry->SetUvOffset(data.vUvOffset[i], i);
}

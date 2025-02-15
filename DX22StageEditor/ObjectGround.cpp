/* ========================================
	DX22Base/
	------------------------------------
	地面オブジェクト用cpp
	------------------------------------
	ObjectGround.cpp
========================================== */

// =============== インクルード =====================
#include "ObjectGround.h"
#include "ComponentGeometry.h"
#include "ComponentGround.h"
#include "TextureManager.h"


/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：所有シーン
========================================== */
ObjectGround::ObjectGround(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompGeometry(nullptr)
	, m_pCompGround(nullptr)
{
	m_eTag = E_ObjectTag::Ground;
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================== */
void ObjectGround::InitLocal()
{
	m_pCompGeometry = AddComponent<ComponentGeometry>();
	m_pCompGround = AddComponent<ComponentGround>();

	m_pCompGeometry->SetShapeType(ComponentGeometry::TYPE_PLANE);
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
========================================== */
void ObjectGround::OutPutLocalData(std::ofstream& file)
{
	S_SaveData data;

	// テクスチャID
	data.nTextureID = TEXTURE_MNG_INST.GetTextureKey(m_pCompGeometry->GetTexture());
	// テクスチャ使用フラグ
	data.bUseTex = m_pCompGeometry->GetIsTex();
	// テクスチャスケール
	data.vUvScale = m_pCompGeometry->GetUvScale();
	// テクスチャオフセット
	data.vUvOffset = m_pCompGeometry->GetUvOffset();

	// ファイルに書き込む
	file.write((char*)&data, sizeof(S_SaveData));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
========================================== */
void ObjectGround::InputLocalData(std::ifstream& file)
{
	S_SaveData data;
	file.read((char*)&data, sizeof(S_SaveData));

	// テクスチャ設定
	m_pCompGeometry->SetTexture(GET_TEXTURE_DATA((TextureManager::E_TEX_KEY)data.nTextureID));
	// テクスチャ使用フラグ
	m_pCompGeometry->SetIsTex(data.bUseTex);
	// テクスチャスケール
	m_pCompGeometry->SetUvScale(data.vUvScale);
	// テクスチャオフセット
	m_pCompGeometry->SetUvOffset(data.vUvOffset);
}


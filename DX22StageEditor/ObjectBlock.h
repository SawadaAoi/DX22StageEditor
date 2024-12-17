/* ========================================
	DX22Base/
	------------------------------------
	ブロック用ヘッダ
	------------------------------------
	説明：シンプルなブロックオブジェクト
	------------------------------------
	ObjectBlock.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentGeometry;
class ComponentCollisionOBB;

// =============== クラス定義 ===================
class ObjectBlock
	: public ObjectBase
{
public:
	struct S_SaveData
	{
		int nTextureID;	// テクスチャID
		bool bUseTex;	// テクスチャ使用フラグ
	};
public:
	ObjectBlock(SceneBase* pScene);

	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectBlock)	// オブジェクトの種類ID取得関数
	
private:
	ComponentGeometry*		m_pCompGeometry;
	ComponentCollisionOBB*	m_pCompColObb;
};


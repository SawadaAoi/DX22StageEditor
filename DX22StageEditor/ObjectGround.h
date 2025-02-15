/* ========================================
	DX22Base/
	------------------------------------
	地面オブジェクト用ヘッダ
	------------------------------------
	説明：レイが当たる地面オブジェクト
	------------------------------------
	ObjectGround.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"

// =============== 前方宣言 =======================
class ComponentGeometry;
class ComponentGround;


// =============== クラス定義 =====================
class ObjectGround :
    public ObjectBase
{
public:
	struct S_SaveData
	{
		int nTextureID;	// テクスチャID
		bool bUseTex;	// テクスチャ使用フラグ
		Vector2<float> vUvScale;		// テクスチャスケール
		Vector2<float> vUvOffset;	// テクスチャオフセット
	};
public:
	ObjectGround(SceneBase* pScene);
	void InitLocal() override;

	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectGround)	// オブジェクトの種類ID取得関数

protected:
	ComponentGeometry* m_pCompGeometry;	// 形状
	ComponentGround* m_pCompGround;		// 地面コンポーネント
};


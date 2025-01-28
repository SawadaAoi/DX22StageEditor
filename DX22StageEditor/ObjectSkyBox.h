/* ========================================
	CatRobotGame/
	------------------------------------
	スカイボックス用ヘッダ
	------------------------------------
	説明：背景のスカイボックス
	------------------------------------
	ObjectSkyBox.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ObjectBase.h"

// =============== 前方宣言 =========================
class ComponentModelStatic;

// =============== クラス定義 =====================
class ObjectSkyBox :
    public ObjectBase
{
public:
	// セーブデータ
	struct S_SaveDataSkyBox
	{
		int nModelId;
	};
public:
	ObjectSkyBox(SceneBase* pScene);
	void InitLocal() override;
	void UpdateLocal() override;

	// データ保存
	void OutPutLocalData(std::ofstream& file) override;
	void InputLocalData(std::ifstream& file) override;

	DEFINE_OBJECT_TYPE(ObjectSkyBox)
private:
	ComponentModelStatic* m_pCompModelStatic;
};


/* ========================================
	CatRobotGame/
	------------------------------------
	スカイボックス用cpp
	------------------------------------
	ObjectSkyBox.cpp
========================================== */


// =============== インクルード =====================
#include "ObjectSkyBox.h"
#include "ComponentModelStatic.h"
#include "ModelStaticManager.h"
#include "ComponentTransform.h"

// =============== 定数 ===========================
const Vector3<float> DEFAULT_SCALE = Vector3<float>(150.0f, 150.0f, 150.0f);
const float ROTATE_SPEED = 1.0f;

// =============== コンストラクタ =====================
ObjectSkyBox::ObjectSkyBox(SceneBase* pScene)
	: ObjectBase(pScene)
	, m_pCompModelStatic(nullptr)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化処理
========================================= */
void ObjectSkyBox::InitLocal()
{
	m_pCompModelStatic = AddComponent<ComponentModelStatic>();
	m_pCompModelStatic->SetModel(GET_MODEL_STATIC(MODEL_KEY::SKYBOX_SUNNY));

	SetLightUse(false);

	m_pCompTransform->SetScale(DEFAULT_SCALE);
}

/* ========================================
	更新関数
	-------------------------------------
	内容：更新処理
========================================= */
void ObjectSkyBox::UpdateLocal()
{
	// ゆっくり回転
	m_pCompTransform->RotateY(ROTATE_SPEED * DELTA_TIME);
}

/* ========================================
	ローカルデータ出力関数
	-------------------------------------
	内容：オブジェクトのローカルデータをファイルに書き込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectSkyBox::OutPutLocalData(std::ofstream& file)
{
	S_SaveDataSkyBox data;

	// モデルIDを取得
	data.nModelId = MODEL_STATIC_MNG_INST.GetModelKey(m_pCompModelStatic->GetModel());

	file.write((char*)&data, sizeof(S_SaveDataSkyBox));
}

/* ========================================
	ローカルデータ入力関数
	-------------------------------------
	内容：ファイルからオブジェクトのローカルデータを読み込む
	-------------------------------------
	引数1：ファイル
=========================================== */
void ObjectSkyBox::InputLocalData(std::ifstream& file)
{
	S_SaveDataSkyBox data;

	file.read((char*)&data, sizeof(S_SaveDataSkyBox));

	m_pCompModelStatic->SetModel(GET_MODEL_STATIC((ModelStaticManager::E_MODEL_KEY)data.nModelId));
}

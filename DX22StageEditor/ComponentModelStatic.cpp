/* ========================================
	DX22Base/
	------------------------------------
	コンポーネント(静的モデル)用cpp
	------------------------------------
	ComponentModelStatic.cpp
========================================== */

// =============== インクルード ===================
#include "ComponentModelStatic.h"
#include "ComponentTransform.h"
#include "ObjectBase.h"
#include "ModelStatic.h"
#include "ModelStaticManager.h"
#include "CameraManager.h"
#include "LightManager.h"

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数：所有オブジェクト
=========================================== */
ComponentModelStatic::ComponentModelStatic(ObjectBase* pOwner)
	: ComponentBase(pOwner, OrderModelStatic)
	, m_pModel(nullptr)
	, m_pCompTran(nullptr)
	, m_vPosOffset(0.0f, 0.0f, 0.0f)
	, m_bIsVisible(true)
{
}

/* ========================================
	初期化関数
	-------------------------------------
	内容：コンポーネントの初期化
=========================================== */
void ComponentModelStatic::Init()
{
	m_pCompTran = m_pOwnerObj->GetComponent<ComponentTransform>();
	m_pModel = GET_MODEL_STATIC(MODEL_KEY::TEST_MODEL);	// モデルデータ取得
}

/* ========================================
	更新関数
	-------------------------------------
	内容：毎フレーム更新処理
=========================================== */
void ComponentModelStatic::Draw()
{
	if (!m_pModel) return;
	if (!m_bIsVisible) return;

	// ワールド行列作成
	DirectX::XMFLOAT4X4 matWVP[3];
	matWVP[0] = m_pCompTran->GetWorldMatrixOffset(m_vPosOffset);
	matWVP[1] = CAMERA_MNG_INST.GetActiveCamera()->GetViewMatrix();
	matWVP[2] = CAMERA_MNG_INST.GetActiveCamera()->GetProjectionMatrix();

	m_pModel->SetWVPMatrix(matWVP);	// ワールド行列をセット

	ObjectBase::T_LightParam lightParam = m_pOwnerObj->GetLightMaterial();
	m_pModel->SetLightMaterial(lightParam.fDiffuse, lightParam.fSpecular, lightParam.fAmbient, lightParam.bLightUse);	// ライトパラメータ設定
	m_pModel->SetLights(LIGHT_MNG_INST.GetLightList());																	// ライト設定
	m_pModel->SetCameraPos(CAMERA_MNG_INST.GetActiveCamera()->GetComponent<ComponentTransform>()->GetWorldPosition());	// カメラ位置設定

	m_pModel->Draw();

}


/* ========================================
	ゲッター(モデル)関数
	-------------------------------------
	戻値：ModelStatic*	モデルポインタ
=========================================== */
ModelStatic* ComponentModelStatic::GetModel()
{
	return m_pModel;
}

/* ========================================
	ゲッター(表示フラグ)関数
	-------------------------------------
	戻値：bool	表示フラグ
=========================================== */
bool ComponentModelStatic::GetIsVisible()
{
	return m_bIsVisible;
}

/* ========================================
	ゲッター(位置オフセット)関数
	-------------------------------------
	戻値：Vector3<float>	位置オフセット
=========================================== */
Vector3<float> ComponentModelStatic::GetPosOffset()
{
	return m_vPosOffset;
}

/* ========================================
	セッター(モデル)関数
	-------------------------------------
	引数：ModelStatic*	pModel	モデルポインタ
=========================================== */
void ComponentModelStatic::SetModel(ModelStatic* pModel)
{
	m_pModel = pModel;
}

/* ========================================
	セッター(表示フラグ)関数
	-------------------------------------
	引数：bool	bIsVisible	表示フラグ
=========================================== */
void ComponentModelStatic::SetIsVisible(bool bIsVisible)
{
	m_bIsVisible = bIsVisible;
}

/* ========================================
	セッター(位置オフセット)関数
	-------------------------------------
	引数：Vector3<float>	offset	位置オフセット
=========================================== */
void ComponentModelStatic::SetPosOffset(Vector3<float> offset)
{
	m_vPosOffset = offset;
}

#ifdef _DEBUG
/* ========================================
	デバッグ関数
	-------------------------------------
	内容：デバッグ用の処理
	-------------------------------------
	引数1：ウィンドウ	DebugUI::Window&
=========================================== */
void ComponentModelStatic::Debug(DebugUI::Window& window)
{
	using namespace DebugUI;

	Item* pGroupModel = Item::CreateGroup("ModelStatic");

	pGroupModel->AddGroupItem(Item::CreateBind("IsVisible", Item::Kind::Bool, &m_bIsVisible));
	pGroupModel->AddGroupItem(Item::CreateBind("PosOffset ", Item::Kind::Vector, &m_vPosOffset));

	pGroupModel->AddGroupItem(InitModelList());	// モデルリスト

	window.AddItem(pGroupModel);
}

/* ========================================
	モデルリスト初期化関数
	-------------------------------------
	内容：モデルリストの初期化
	-------------------------------------
	戻り値：リスト項目
=========================================== */
DebugUI::Item* ComponentModelStatic::InitModelList()
{
	using namespace DebugUI;

	DebugUI::Item* pModelList = Item::CreateList("Model", [this](const void* arg)
	{
		// テクスチャ変更
		ClickModelList(reinterpret_cast<const char*>(arg));

	}, false, true);

	// 登録されているテクスチャをリストに追加
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::string> modelPathMap = MODEL_STATIC_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		pModelList->AddListItem(modelPath.second.c_str());
	}

	// 現在のテクスチャを選択
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::shared_ptr<ModelStatic>> modelMap = MODEL_STATIC_MNG_INST.GetModelDatas();
	for (auto& model : modelMap)
	{
		if (model.second.get() == m_pModel)
		{
			pModelList->SetListNo(model.first);
			break;
		}
	}

	return pModelList;
}

/* ========================================
	モデルリストクリック関数
	-------------------------------------
	内容：モデルリストをクリックした際の処理
	-------------------------------------
	引数：リストに表示されている文字列
=========================================== */
void ComponentModelStatic::ClickModelList(std::string sListName)
{
	ModelStaticManager::E_MODEL_KEY eKey;

	// モデルキー取得
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::string> modelPathMap = MODEL_STATIC_MNG_INST.GetModelPaths();
	for (auto& modelPath : modelPathMap)
	{
		if (modelPath.second == sListName)
		{
			eKey = modelPath.first;
			break;
		}
	}

	// モデルデータ取得
	std::unordered_map<ModelStaticManager::E_MODEL_KEY, std::shared_ptr<ModelStatic>> modelMap = MODEL_STATIC_MNG_INST.GetModelDatas();
	m_pModel = modelMap.find(eKey)->second.get();

}
#endif // _DEBUG
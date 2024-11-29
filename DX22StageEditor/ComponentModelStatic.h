/* ========================================
	DX22Base/
	------------------------------------
	コンポーネント(静的モデル)用ヘッダ
	------------------------------------
	説明：静的モデルの描画を行う
	------------------------------------
	ComponentModelStatic.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"


// =============== 前方宣言 =======================
class ComponentTransform;	// トランスフォームコンポーネント
class ModelStatic;			// 静的モデル


// =============== クラス定義 =====================
class ComponentModelStatic :
    public ComponentBase
{
public:
	ComponentModelStatic(ObjectBase* pOwner);
	virtual void Init() override;
	virtual void Draw() override;


	// ゲッター
	ModelStatic* GetModel();
	bool GetIsVisible();
	Vector3<float> GetPosOffset();

	// セッター
	void SetModel(ModelStatic* pModel);
	void SetIsVisible(bool bIsVisible);
	void SetPosOffset(Vector3<float> offset);

	DEFINE_COMPONENT_TYPE

#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
	DebugUI::Item* InitModelList();	
	void ClickModelList(std::string sListName);
#endif // _DEBUG
private:
	ComponentTransform* m_pCompTran;	// トランスフォームコンポーネント
	ModelStatic*		m_pModel;		// モデル

	bool				m_bIsVisible;	// 表示フラグ
	Vector3<float>		m_vPosOffset;	// 位置オフセット

};


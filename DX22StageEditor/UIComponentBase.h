/* ========================================
	DX22Base/
	------------------------------------
	UIコンポーネント基底用ヘッダ
	------------------------------------
	説明：UI用コンポーネントの基底クラス
	------------------------------------
	main.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "DebugMenu.h"
#include "TimeManager.h"		// 経過時間をどこでも取得できるようにするため
#include "DebugMenu.h"			// デバッグメニュー出力用
#include "UIComponentOrder.h"	// UIコンポーネント更新順定義

// =============== 前方宣言 =======================
class UIObjectBase;	// UIオブジェクト基底クラス

// =============== 定数定義 =======================
// コンポーネントID取得関数の定義を省略するためのマクロ
// 使い方：クラス定義内でDEFINE_COMPONENT_TYPEを記述する
// 例：DEFINE_COMPONENT_TYPE(ComponentTransform);
// 関数のポインタをIDとして扱うため、コンポーネントの種類によって異なるIDを取得できる
#define DEFINE_UI_COMPONENT_TYPE											\
    static size_t GetStaticTypeID() {										\
		 static const size_t typeID = reinterpret_cast<size_t>(&typeID);	\
        return typeID;  													\
    }																		\
    size_t GetTypeID() const override {										\
        return GetStaticTypeID();											\
    }

// =============== クラス定義 =====================
class UIComponentBase
{
public:
	UIComponentBase() = delete;							// デフォルトコンストラクタは禁止
	UIComponentBase(UIObjectBase* pOwner, int nOrder);	// コンストラクタ(所有者オブジェクトを設定)
	virtual ~UIComponentBase() {};
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	// ゲッター
	UIObjectBase* GetOwnerObject();		// 所有者オブジェクトの取得
	int GetUpdateOrder();				// 更新順序の取得
	virtual size_t GetTypeID() const;	// コンポーネントの種類IDの取得

#ifdef _DEBUG
	// デバッグ用の処理(オブジェクト情報ウィンドウに表示)
	virtual void Debug(DebugUI::Window& window) {};
#endif // _DEBUG

protected:
	UIObjectBase*	m_pOwnerObj;	// 所有者オブジェクト
	int				m_nUpdateOrder;	// 更新順序(数値が小さいほど先に更新)
};


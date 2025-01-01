/* ========================================
	DX22Base/
	------------------------------------
	衝突判定コンポーネント基底用ヘッダ
	------------------------------------
	説明：各形状の衝突判定の基底クラス
	------------------------------------
	ComponentCollisionBase.h
========================================== */
#pragma once

// =============== インクルード =====================
#include "ComponentBase.h"
#include <memory>
#include "Vector3.h"
#include "Quaternion.h"
#include <unordered_map>

#include "ShapeBase.h"
#include "ShapeLine.h"

#include "ColorVec3.h"
#include <memory>

// =============== 前方宣言 =======================
class ComponentCollisionAABB;
class ComponentCollisionOBB;
class ComponentCollisionSphere;

// =============== クラス定義 =====================
class ComponentCollisionBase :
	public ComponentBase
{
public:

	// デバッグ表示衝突判定ワイヤーフレームの色
	const Vector3<float> WIRE_COLOR_NORMAL	= ColorVec3::GREEN;	// 通常時
	const Vector3<float> WIRE_COLOR_HIT		= ColorVec3::RED;	// 衝突時

	// コリジョンの種類
	enum E_ColType
	{
		COL_NONE,		// コリジョンなし
		COL_AABB,		// ボックスコリジョン(軸平行ボックス)
		COL_OBB,		// ボックスコリジョン(任意軸ボックス)
		COL_SPHERE,		// スフィアコリジョン
	};


	// 最小移動ベクトル構造体(めり込み戻し用)
	struct T_MTV
	{
		Vector3<float>	vAxis;		// 衝突軸
		float			fOverlap;	// 重なり量
		bool			bIsCol;		// 衝突しているか
		bool			bIsTrigger;	// true:トリガー判定用(すり抜け判定)
		std::string		sName;		// 衝突相手オブジェクト名

		T_MTV()
		{
			vAxis = Vector3<float>(0.0f, 0.0f, 0.0f);
			fOverlap = 10000.0f;
			bIsCol = false;
			bIsTrigger = false;
			sName = "None";
		}
	};

public:
	ComponentCollisionBase(ObjectBase* pOwner);
	~ComponentCollisionBase(); 
	void Update() override;
	void Draw() override;

	void UpdateCollision(ComponentCollisionBase* otherCol);			// 衝突判定更新関数
	void UpdateCollisionMap(ComponentCollisionBase* otherCol);										// 衝突状態マップ更新関数

	// ゲッター
	Vector3<float> GetPosition();
	Vector3<float> GetPositionOffset();
	Vector3<float> GetScale();
	Quaternion GetRotation();
	bool GetRefOwnerTransform();
	E_ColType GetColType();
	bool GetDynamic();
	bool GetEnable();
	bool GetTrigger();
	std::vector<T_MTV> GetMtvs();
	T_MTV GetMtv(std::string sName);

	// セッター
	void SetPosition(Vector3<float> vPos);
	void SetPositionOffset(Vector3<float> vOffset);
	void SetScale(Vector3<float> vScale);
	void SetRotation(Quaternion qRot);
	void SetRefOwnerTransform(bool bRef);
	void SetColType(E_ColType eType);
	void SetDynamic(bool bDynamic);
	void SetEnable(bool bEnable);
	void SetTrigger(bool bTrigger);
	void SetMtvs(std::vector<T_MTV> mtvs);

	DEFINE_COMPONENT_TYPE	// コンポーネントの種類ID取得関数

#ifdef _DEBUG
	void DebugColBase(DebugUI::Item* pGroupItem, std::string sCompName);
	void UpdateDebugMTVList();
#endif // _DEBUG
private:
	void UpdateMTVList();	// 最小移動ベクトルリスト更新関数

protected:
	virtual bool CheckCollision(ComponentCollisionBase* otherCol) = 0;		// 衝突判定
	void SetColObjMTV(T_MTV tMtv, std::string sName);						// 最小移動ベクトル更新関数

protected:
	class ComponentTransform*	m_pOwnerTransform;		// 所持オブジェクトのトランスフォーム
	Vector3<float>				m_vPosition;			// コリジョンの中心座標
	Vector3<float>				m_vPositionOffset;		// コリジョンの中心座標オフセット
	Vector3<float>				m_vScale;				// コリジョンのスケール
	Quaternion					m_qRotation;			// コリジョンの回転
	bool						m_bRefOwnerTransform;	// 所持オブジェクトのトランスフォームを参照するかどうか

	bool								m_bIsDispCol;	// コリジョン表示フラグ
	E_ColType							m_eColType;		// コリジョンの種類
	std::unique_ptr<ShapeBase>			m_pShape;		// コリジョンの形状(デバッグ表示用)

	bool								m_bIsDynamic;		// (true:動的コリジョン、false:静的コリジョン)
	bool								m_bIsEnabled;		// (true:コリジョン有効、false:コリジョン無効)
	bool								m_bIsTrigger;		// トリガーコリジョンフラグ(すり抜けるコリジョン)

	// 各オブジェクトとの衝突状態を保持するマップ(キー：オブジェクト、値：衝突状態(true:衝突中、false:非衝突)))
	std::unordered_map<ObjectBase*, bool> m_bColStatesMap;	

	static  inline bool m_bIsDispColAll = false;	// 全コリジョン表示フラグ

	// 最小移動ベクトル構造体配列
	// ※衝突時のめり込み解消に使用
	// ※衝突確認するオブジェクト数分
	std::vector<T_MTV>	m_tMtvs;

#ifdef _DEBUG
	DebugUI::Item*	m_pColObjList;	// 衝突オブジェクトリスト(デバッグ表示用)
	std::string		m_sColCompName;	// コンポーネント名(デバッグメニュー表示確認用)
	int				m_nSelectMTV;	// 選択中の最小移動ベクトル
#endif // _DEBUG
};


#include "SceneBase.h"
/* ========================================
	DX22Base/
	------------------------------------
	オブジェクト用inlファイル
	------------------------------------
	※inlファイルはヘッダファイルに記載されているのと同じ
	　ヘッダーファイルの冗長化を防ぐために使用
	------------------------------------
	ObjectBase.inl
========================================== */



/* ========================================
	オブジェクト追加関数
	-------------------------------------
	内容：シーンにオブジェクトを追加する
	-------------------------------------
	引数：sName	オブジェクト名
	-------------------------------------
	戻値：追加オブジェクトポインタ
========================================== */
template<typename T>
inline T* SceneBase::AddSceneObject(std::string sName)
{
	// 基底クラスがObjectBaseでない場合はエラーを出力
	if (std::is_base_of<ObjectBase, T>() == false)	return nullptr;

	// オブジェクト作成
	auto pObject = std::make_unique<T>(this);

	// 名前が重複している場合は連番を付ける
	sName = CreateUniqueName(sName);

	// 初期化処理(
	pObject->Init(sName);

	// シーンが更新中かどうかをチェックします
	if (m_bIsUpdating)
	{
		// 一時保存用の配列にユニークポインタを移動します
		m_pStandbyObjects.push_back(std::move(pObject));
	}
	else
	{
		// シーンのオブジェクト配列にユニークポインタを移動します
		m_pObjects.push_back(std::move(pObject));
	}


	// 追加したオブジェクトのポインタを返します(更新中の場合は一時保存用配列から取得)
	return static_cast<T*>((m_bIsUpdating ? m_pStandbyObjects : m_pObjects).back().get());
}




/* ========================================
	オブジェクト取得関数
	-------------------------------------
	内容：シーンに所属するオブジェクトを取得する
	-------------------------------------
	引数：sName	オブジェクト名
	-------------------------------------
	戻値：取得オブジェクトポインタ
========================================== */
template<typename T>
inline T* SceneBase::GetSceneObject(std::string sName)
{
	// 基底クラスがObjectBaseでない場合はエラーを出力
	if (std::is_base_of<ObjectBase, T>() == false)	return nullptr;

	// 指定の種類のオブジェクトを全て取得
	std::vector<T*> pObjectList = GetSceneObjects<T>();	

	// シーンに所属するオブジェクトを検索
	for (const auto& pObject : pObjectList)
	{
		// オブジェクト名が一致したらポインタを返す
		if (pObject->GetName() == sName)
		{
			return static_cast<T*>(pObject);
		}
	}

	return nullptr;	// 見つからなかった場合はnullptrを返す
}

/* ========================================
	オブジェクト取得関数
	-------------------------------------
	内容：シーンに所属するオブジェクトを取得する
		　※一番最初に見つかったオブジェクトを取得
	-------------------------------------
	引数：取得オブジェクトポインタ
========================================== */
template<typename T>
inline T* SceneBase::GetSceneObject()
{
	// 基底クラスがObjectBaseでない場合はエラーを出力
	if (std::is_base_of<ObjectBase, T>() == false)	return nullptr;

	// シーンに所属するオブジェクトを検索
	for (const auto& pObject : m_pObjects)
	{
		// オブジェクトの型が一致したらオブジェクトポインタを返す
		if (pObject->GetTypeID() == T::GetStaticTypeID())
		{
			return static_cast<T*>(pObject.get());
		}
	}

	return nullptr;	// 見つからなかった場合はnullptrを返す
}

/* ========================================
	型別オブジェクト収集関数
	-------------------------------------
	内容：シーンに所属する特定の方のオブジェクト
		　を全て収集する
	-------------------------------------
	戻値：オブジェクトポインタ配列
========================================== */
template<typename T>
inline std::vector<T*> SceneBase::GetSceneObjects()
{
	// 取得したオブジェクトを格納する配列
	std::vector<T*> objects;

	// 基底クラスがObjectBaseでない場合はエラーを出力
	if (std::is_base_of<ObjectBase, T>() == false)	return objects;

	// シーンに所属するオブジェクトを検索
	for (const auto& pObject : m_pObjects)
	{
		// オブジェクトの型が一致したら配列に追加
		if (pObject->GetTypeID() == T::GetStaticTypeID())
		{
			objects.push_back(static_cast<T*>(pObject.get()));
		}
	}

	return objects;	// 取得したオブジェクトの配列を返す
}




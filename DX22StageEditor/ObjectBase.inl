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
	コンポーネント追加関数
	-------------------------------------
	内容：自オブジェクトにコンポーネントを追加する
	-------------------------------------
	戻値：追加したコンポーネントのポインタ
=========================================== */
template<typename T>
inline T* ObjectBase::AddComponent()
{
	// 既に同じ種類のコンポーネントがある場合は追加しない
	if (GetComponent<T>() != nullptr)
	{
		return nullptr;
	}

	auto pComponent = std::make_unique<T>(this);	// コンポーネントを生成
 	int nOrder		= pComponent->GetUpdateOrder(); // 更新順序を取得


	// 挿入位置を探す(先頭から順に探す)
	auto it = m_pComponents.begin();
	for (; it != m_pComponents.end(); it++)
	{
		// 更新順序が小さい順に挿入
		if (nOrder < static_cast<ComponentBase*>((*it).get())->GetUpdateOrder())
		{
			break; // 挿入位置を見つけたらループを抜ける
		}
	}

	// コンポーネントを挿入し、その位置のイテレータを保持
	auto insertedIt = m_pComponents.insert(it, std::move(pComponent));

	// 初期化処理
	insertedIt->get()->Init();

	// 挿入されたコンポーネントのポインタを返す
	return static_cast<T*>(insertedIt->get());
}

/* ========================================
	コンポーネント取得関数
	-------------------------------------
	内容：指定した種類のコンポーネントを取得する
	-------------------------------------
	戻値：取得したコンポーネントのポインタ
=========================================== */
template<typename T>
inline T* ObjectBase::GetComponent()
{
	// 所持コンポーネント配列を探索
	for (auto& pComponent : m_pComponents)
	{
		// 指定した種類のコンポーネントが見つかった場合はそのポインタを返す
		if (pComponent->GetTypeID() == T::GetStaticTypeID())
		{
			return static_cast<T*>(pComponent.get());
		}
	}

	return nullptr; // 見つからなかった場合はnullptrを返す
}

/* ========================================
	コンポーネント削除関数
	-------------------------------------
	内容：指定した種類のコンポーネントを削除する
=========================================== */
template<typename T>
inline void ObjectBase::RemoveComponent()
{
	for (auto it = m_pComponents.begin(); it != m_pComponents.end(); ++it)
	{
		if ((*it)->GetTypeID() == T::GetStaticTypeID())
		{
			(*it)->Uninit(); // 終了処理
			m_pComponents.erase(it); // 配列からコンポーネント削除
			return;
		}
	}

}

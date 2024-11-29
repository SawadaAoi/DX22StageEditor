/* ========================================
	DX22Base/
	------------------------------------
	デバッグウィンドウ用cpp
	------------------------------------
	DebugWindow.cpp
========================================== */
#ifdef _DEBUG	// デバッグモードのみ有効

// =============== インクルード ===================
#include "DebugWindow.h"
#include "DebugMenu.h"

// =============== 名前空間 ===================
namespace DebugUI {

// 静的メンバ変数の初期化
Window	Window::dummy;

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
Window::Window()
	: enable(false), name("")
{
}

/* ========================================
	配列演算子
	-------------------------------------
	内容：指定名のアイテムを取得
	-------------------------------------
	引数：const char* name 項目名
	-------------------------------------
	戻り値：Item& アイテム
=========================================== */
Item& Window::operator[](const char* name)
{
	auto it = FindItem(name);
	if (it == items.end())
		return *Item::dummy;
	return **it;
}

/* ========================================
	アイテム追加
	-------------------------------------
	内容：ウィンドウへ表示項目を追加
	-------------------------------------
	引数：Item* item アイテム
=========================================== */
void Window::AddItem(Item* item)
{
	// 既存の項目と同名のアイテムがないか探索
	auto it = FindItem(item->GetName());
	if (it != items.end()) return;

	// 保存データの場合は、追加時に保存済みのデータで上書き
	Menu::DataRead(name + "/", item);

	// 表示項目の追加
	items.push_back(item);
}

/* ========================================
	アイテム削除
	-------------------------------------
	内容：ウィンドウから表示項目を削除
	-------------------------------------
	引数：const char* name 項目名
=========================================== */
void Window::RemoveItem(const char* name)
{
	// 既存の項目と同名のアイテムがないか探索
	auto it = FindItem(name);
	if (it == items.end()) return;

	delete* it;			// メモリ解放
	items.erase(it);	// リストから削除
}


/* ========================================
	アイテム全削除
	-------------------------------------
	内容：ウィンドウの全表示項目を削除
=========================================== */
void Window::Clear()
{
	auto it = items.begin();
	while (it != items.end())
	{
		delete* it;
		++it;
	}
	items.clear();
}

/*
* @brief アイテムの探索
*/
Items::iterator Window::FindItem(const char* name)
{
	return std::find_if(items.begin(), items.end(),
		[&name](const Item* obj) {
		return strcmp(obj->GetName(), name) == 0;
	});
}

}

#endif
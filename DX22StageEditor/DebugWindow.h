/* ========================================
	DX22Base/
	------------------------------------
	デバッグウィンドウ用ヘッダ
	------------------------------------
	説明：imguiを使用したデバッグウィンドウ
		　作成処理はDebugMenuで行う
	------------------------------------
	DebugWindow.h
========================================== */
#ifdef _DEBUG	// デバッグモードのみ有効
#pragma once

// =============== インクルード ===================
#include "DebugItem.h"
#include <string>


// =============== 名前空間 ===================
namespace DebugUI {

class Window
{
	friend class Menu;
public:
	Window();
	Item& operator[](const char* name);

	// アイテム追加
	void AddItem(Item* item);
	// アイテム削除
	void RemoveItem(const char* name);
	// 全アイテム削除
	void Clear();

private:
	// アイテムの探索
	Items::iterator FindItem(const char* name);

public:
	static Window	dummy;	// 指定ウィンドウが取得できなかったときのダミー
private:
	bool			enable;	// 表示フラグ
	std::string		name;	// ウィンドウ名
	Items			items;	// 表示項目
};
}
#endif

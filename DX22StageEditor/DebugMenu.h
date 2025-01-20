/* ========================================
	DX22Base/
	------------------------------------
	デバッグメニュー用ヘッダ
	------------------------------------
	説明：imguiを使用したデバッグメニュー
	------------------------------------
	DebugMenu.h
========================================== */

/**
 * 2024.04.08
 *	imgui導入、テキストファイル読み込み、バインド追加
 * 2024.04.09
 *	Bind処理をコールバックで対応、データの読み書きを追加、リスト追加、シーンの切り替え
 * 2024.04.10
 *	モデルの切り替え、シーンの切り替え
 * 2024.07.23
 * 　表示内容をデータから読み取るのではなく、プログラム上で設定できるように変更
 * 　GameObjectを変更して、Unity風のインスペクター表示を行うように変更
 */
#ifdef _DEBUG	// デバッグモードのみ有効

#pragma once

 // =============== インクルード ===================
#include <string>
#include <vector>

#include "DebugWindow.h"	// デバッグ用ウィンドウ
#include "DebugWindowSetting.h"

// =============== 名前空間 ===================
namespace DebugUI {


class Menu
{
	friend class Window;
public:
	// 保存データ
	struct SaveData
	{
		Item::Kind kind;	// 表示項目の種類
		std::string path;	// ファイルパス
		std::string value;	// 値
	};
private:
	Menu();
	~Menu();

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	// ウィンドウ生成
	static Window& Create(const char* name);
	// ウィンドウ取得
	static Window& Get(const char* name);
private:
	static void DrawImgui(Item* item);
	static void DataWrite(std::string& data, std::string path, Item* item);
	static void DataRead(std::string path, Item* item);

private:
	static inline std::vector<Window>		m_WindowList;		// 表示ウィンドウ一覧
	static inline std::vector<SaveData>		m_SaveDataList;		// 保存データ
};



} // namespace debug
#endif // _DEBUG

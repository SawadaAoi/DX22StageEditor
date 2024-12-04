/* ========================================
	DX22Base/
	------------------------------------
	デバッグメニュー用cpp
	------------------------------------
	DebugMenu.cpp
========================================== */
#ifdef _DEBUG


// =============== インクルード ===================
#include "DebugMenu.h"

// imgui
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

#include "Input.h"
#include "DirectXManager.h"

// =============== 定数定義 =======================
const char* SAVE_DATA_PATH = "Assets/DebugMenu.csv";


// =============== 名前空間 ===================
namespace DebugUI {



/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
Menu::Menu()
{
}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：終了処理
=========================================== */
Menu::~Menu()
{
}


/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
=========================================== */
void Menu::Init()
{
	// ImGui初期化
	IMGUI_CHECKVERSION();	// ImGuiライブラリとアプリケーションコードの間で互換性があるか確認
	ImGui::CreateContext();	// ImGuiコンテキストの作成(ウィジェットの状態、スタイル、データを保持)

	
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// キーボード入力
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// ゲームパッド入力
	ImGui::StyleColorsDark();				// 見た目の設定

	ImGui_ImplWin32_Init(GetActiveWindow());
	ImGui_ImplDX11_Init(DirectXManager::GetDevice(), DirectXManager::GetContext());

	// Dummyデータ初期化
	Item::dummy = Item::CreateValue("None", Item::Label);
	
	// 初期ウィンドウの作成
	Window window;
	window.name = WIN_NAME[0];
	m_WindowList.push_back(window);

	// ファイル読み出し
	FILE* fp;
	fopen_s(&fp, SAVE_DATA_PATH, "rt");
	if (fp) {
		const int size = 1024;	// 1行の最大文字数
		char line[size];
		m_SaveDataList.clear();

		// 1行ずつ読み込み
		while (fgets(line, size, fp) != NULL) {
			// セル内容取得
			std::vector<std::string> cells;
			*strstr(line, "\n") = ',';		// 改行文字をカンマに変換

			char* start = line;
			char* end = strstr(start, ",");	

			// カンマで区切られたセルを取得
			do {
				*end = '\0';				
				cells.push_back(start);	
				start = end + 1;
				end = strstr(start, ",");
			} while (end);

			// データ追加
			SaveData data;
			data.kind = Item::StrToKind(cells[0]);
			data.path = cells[1];
			data.value = cells[2];
			m_SaveDataList.push_back(data);
		}
		fclose(fp);
	}
}

/* ========================================
	終了処理関数
	-------------------------------------
	内容：終了処理
=========================================== */
void Menu::Uninit()
{
	// ダミーの削除
	delete Item::dummy;

	// 保存データの作成
	std::string data;

	auto it = m_WindowList.begin();
	while (it != m_WindowList.end())
	{
		// ウィンドウやグループが階層となるようにパスを作成
		std::string path = it->name + "/";

		// 書き込みが必要なデータを保存
		auto itemIt = it->items.begin();
		while (itemIt != it->items.end())
		{
			DataWrite(data, path, *itemIt);
			++itemIt;
		}

		// ウィンドウのクリア
		it->Clear();
		++it;
	}

	// 保存処理
	FILE* fp;
	fopen_s(&fp, SAVE_DATA_PATH, "wt");
	if (fp) {
		fwrite(data.c_str(), data.size(), 1, fp);
		fclose(fp);
	}

	// ImGui終了処理
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


/* ========================================
	更新処理関数
	-------------------------------------
	内容：更新
=========================================== */
void Menu::Update()
{
	auto& mainWindow = m_WindowList[0];	// メインウィンドウ

	// デバッグメニューの表示切り替え
	if (Input::IsKeyPress(VK_SHIFT) && Input::IsKeyPress(VK_SPACE))
	{
		if (Input::IsKeyTrigger(VK_RETURN))
		{
			//mainWindow.enable ^= true;
			for (int i = 1; i < m_WindowList.size(); i++)
			{
				m_WindowList[i].enable ^= true;
			}
		}

			
	}

	// Menuの各ウィンドウの表示切り替え
	//int cnt = 1;						// 0は自身のウィンドウ
	//auto it = mainWindow.items.begin();	
	//while (it != mainWindow.items.end())
	//{
	//	m_WindowList[cnt].enable = (*it)->GetBool();
	//	++it;
	//	++cnt;
	//}

}


/* ========================================
	描画関数
	-------------------------------------
	内容：描画
=========================================== */
void Menu::Draw()
{
	// メインウィドウが有効になっていなければ非表示
	/*if (!m_WindowList[0].enable)
		return;*/

	// Imguiの描画準備
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ウィンドウ内の表示項目を描画
	auto windowIt = m_WindowList.begin();
	while (windowIt != m_WindowList.end())
	{
		// ウィンドウが有効であれば描画
		if (windowIt->enable)
		{
			// ウィンドウ内の描画開始-------------------------------------
			//ImGui::Begin(windowIt->name.c_str());
			ImGui::Begin(windowIt->name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);	// 大きさ、位置固定
			// アイテムごとに描画
			auto itemIt = windowIt->items.begin();
			while (itemIt != windowIt->items.end())
			{
				DrawImgui(*itemIt);
				++itemIt;
			}

			ImGui::End();
			// ウィンドウ内の描画終了-------------------------------------

		}
		++windowIt;
	}

	// ImGuiの表示
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


/* ========================================
	ウィンドウ生成関数
	-------------------------------------
	内容：ウィンドウ生成
	-------------------------------------
	引数：const char* name		ウィンドウ名
=========================================== */
Window& Menu::Create(const char* name)
{		
	// 個別ウィンドウ設定
	Window window;
	window.name		= name;
	window.enable	= false;
	window.items.clear();
	m_WindowList.push_back(window);

	// 初期ウィンドウに追加
	Item* label = Item::CreateValue(name, Item::Bool, true);
	DataRead(m_WindowList[0].name + "/", label);			// 保存データの読み込み
	m_WindowList[0].items.push_back(label);					// メインウィンドウに追加

	return m_WindowList.back();
}

/* ========================================
	ウィンドウ取得関数
	-------------------------------------
	内容：ウィンドウ取得
	-------------------------------------
	引数：const char* name		ウィンドウ名
=========================================== */
Window& Menu::Get(const char* name)
{
	// ウィンドウの探索
	auto winIt = std::find_if(m_WindowList.begin(), m_WindowList.end(),
		[&name](const Window& obj) {
			return strcmp(obj.name.c_str(), name) == 0;
		});

	// 見つからなければダミーを返却
	if (winIt == m_WindowList.end())
		return Window::dummy;

	// 見つかったウィンドウを返却
	return *winIt;
}

/* ========================================
	各項目描画関数
	-------------------------------------
	内容：ウィンドウ内の各項目
		　(チェックボックス、ボタンなど)の描画
	-------------------------------------
	引数：Item* item		表示アイテム
=========================================== */
void Menu::DrawImgui(Item* item)
{
	// 各要素の取得
	bool isValue	= (typeid(ItemValue) == typeid(*item));		// 値項目
	bool isBind		= (typeid(ItemBind) == typeid(*item));		// 変数紐付け項目
	bool isCallback = (typeid(ItemCallback) == typeid(*item));	// コールバック項目
	ItemValue*		pValue		= static_cast<ItemValue*>(item);
	ItemBind*		pBind		= static_cast<ItemBind*>(item);
	ItemCallback*	pCallback	= static_cast<ItemCallback*>(item);

	// 一つ前の項目と横並びに表示
	if (item->m_bSameLine)	ImGui::SameLine();

	// 項目の種類
	switch (item->GetKind())
	{
	// 項目名のみの表示
	case Item::Label:
		ImGui::Text("%s", item->GetName());
		break;

	case Item::Text:
		if (isValue)
		{
			ImGui::Text("%s : %s", item->GetName(), &pValue->m_uValue.cText);
		}
		else if (isBind)
		{
			ImGui::Text("%s : %s", item->GetName(), reinterpret_cast<std::string* > (pBind->m_pBindVar)->c_str());
		}
		break;
	// チェックフラグの表示
	case Item::Bool:
		if (isValue)
		{
			ImGui::Checkbox(item->GetName(), &pValue->m_uValue.bValid);
		}
		else if (isBind)
		{
			bool* bValidPtr = reinterpret_cast<bool*>(pBind->m_pBindVar);	// 紐づき変数のポインタ
			ImGui::Checkbox(item->GetName(), bValidPtr);					// チェックボックスの表示
			item->m_uValue.bValid = *bValidPtr;								// 保存用の値を更新
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.bValid);
			if (ImGui::Checkbox(item->GetName(), &pCallback->m_uValue.bValid))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.bValid);
		}
		break;
	// 整数項目の表示
	case Item::Int:
		if (isValue)
		{
			ImGui::InputInt(item->GetName(), &pValue->m_uValue.nValue);
		}
		else if (isBind)
		{
			int* nValuePtr = reinterpret_cast<int*>(pBind->m_pBindVar);
			ImGui::InputInt(item->GetName(), nValuePtr);
			item->m_uValue.nValue = *nValuePtr;
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.nValue);
			if (ImGui::InputInt(item->GetName(), &pCallback->m_uValue.nValue))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.nValue);
		}
		break;
	// 小数項目の表示
	case Item::Float:
		if (isValue)
		{
			ImGui::InputFloat(item->GetName(), &pValue->m_uValue.fValue);
		}
		else if (isBind)
		{
			float* fValuePtr = reinterpret_cast<float*>(pBind->m_pBindVar);
			ImGui::InputFloat(item->GetName(), fValuePtr);
			item->m_uValue.fValue = *fValuePtr;
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fValue);
			if (ImGui::InputFloat(item->GetName(), &pCallback->m_uValue.fValue))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fValue);
		}
		break;
	// ベクトル項目の表示
	case Item::Vector:
		if (isValue)
		{
			ImGui::InputFloat3(item->GetName(), &pValue->m_uValue.fVector.x, "%.4f");
		}
		else if (isBind)
		{
			ImGui::InputFloat3(item->GetName(), reinterpret_cast<float*>(pBind->m_pBindVar), "%.4f");
			item->m_uValue.fVector = DirectX::XMFLOAT3(
				reinterpret_cast<float*>(pBind->m_pBindVar)[0],
				reinterpret_cast<float*>(pBind->m_pBindVar)[1],
				reinterpret_cast<float*>(pBind->m_pBindVar)[2]
			);
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fVector.x);
			if (ImGui::InputFloat3(item->GetName(), &pCallback->m_uValue.fVector.x, "%.4f"))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fVector.x);
		}
		break;

	// ベクトル2D項目の表示
	case Item::Vector2:
		if (isValue)
		{
			ImGui::InputFloat2(item->GetName(), &pValue->m_uValue.fVector2.x, "%.4f");
		}
		else if (isBind)
		{
			ImGui::InputFloat2(item->GetName(), reinterpret_cast<float*>(pBind->m_pBindVar), "%.4f");
			item->m_uValue.fVector2 = DirectX::XMFLOAT2(
				reinterpret_cast<float*>(pBind->m_pBindVar)[0],
				reinterpret_cast<float*>(pBind->m_pBindVar)[1]
			);
		}
		else if (isCallback)
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fVector2.x);
			if (ImGui::InputFloat2(item->GetName(), &pCallback->m_uValue.fVector2.x, "%.4f"))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fVector2.x);
		}
		break;

	// 色項目の表示
	case Item::Color:
		if (isValue)
		{
			ImGui::ColorEdit4(item->GetName(), &pValue->m_uValue.fColor.x);
		}
		else if (isBind)
		{
			ImGui::ColorEdit4(item->GetName(), reinterpret_cast<float*>(pBind->m_pBindVar));
			item->m_uValue.fColor = DirectX::XMFLOAT4(
				reinterpret_cast<float*>(pBind->m_pBindVar)[0],
				reinterpret_cast<float*>(pBind->m_pBindVar)[1],
				reinterpret_cast<float*>(pBind->m_pBindVar)[2],
				reinterpret_cast<float*>(pBind->m_pBindVar)[3]
			);
		}
		else if (isCallback) 
		{
			pCallback->m_CallbackFunc(false, &pCallback->m_uValue.fColor.x);
			if(ImGui::ColorEdit4(item->GetName(), &pCallback->m_uValue.fColor.x))
				pCallback->m_CallbackFunc(true, &pCallback->m_uValue.fColor.x);
		}
		break;
	// パス項目の表示
	case Item::Path:
		if (isValue) // 通常表示
		{
			ImGui::InputText(item->GetName(), static_cast<ItemValue*>(item)->m_uValue.cPath, MAX_PATH);
		}
		else if (isBind) // 紐づけ項目の表示
		{
			ImGui::InputText(item->GetName(), reinterpret_cast<char*>(static_cast<ItemBind*>(item)->m_pBindVar), MAX_PATH);
		}
		break;
	// ボタンの表示
	case Item::Kind::Command:
		// ボタンが押されたらコールバック関数を実行
		if (ImGui::Button(item->GetName()))
		{
			pCallback->m_CallbackFunc(false, nullptr);
		}
		break;
	// グループ項目の表示
	case Item::Group:
		{
			ItemGroup* ptr = reinterpret_cast<ItemGroup*>(item);
			// 表示項目がなければ表示しない
			if (ptr->m_pItemGroup.empty()) break;

			// グループが展開されてなければ表示しない
			if (!ImGui::CollapsingHeader(item->GetName(), ImGuiTreeNodeFlags_DefaultOpen)) break;

			// グループ内の項目を再帰で表示
			auto it = ptr->m_pItemGroup.begin();
			while (it != ptr->m_pItemGroup.end())
			{
				DrawImgui(*it);
				++it;
			}
		}
		break;
	// リスト項目の表示
	case Item::List:
		{
			ItemList* ptr = reinterpret_cast<ItemList*>(item);
			// 表示項目がなければ表示しない
			if (ptr->m_sItemList.empty()) break;

			// 表示項目の構築
			static const char* pList[100];
			auto it = ptr->m_sItemList.begin();
			for (int i = 0; i < ptr->m_sItemList.size() && i < 100; ++i, ++it)
				pList[i] = it->c_str();

			// リストボックス
			if (!ptr->m_bIsDropDown)
			{
				// 表示
				if (ImGui::ListBox(item->GetName(), &ptr->m_uValue.nValue, pList, static_cast<int>(ptr->m_sItemList.size())))
				{
					// 関数があれば選択時処理を実行
					if (ptr->m_CallbackFunc && ptr->m_uValue.nValue >= 0)
						ptr->m_CallbackFunc(pList[ptr->m_uValue.nValue]);
				}
			}
			// ドロップダウンリスト
			else
			{
				// 表示
				if (ImGui::Combo(item->GetName(), &ptr->m_uValue.nValue, pList, static_cast<int>(ptr->m_sItemList.size())))
				{
					// 関数があれば選択時処理を実行
					if (ptr->m_CallbackFunc && ptr->m_uValue.nValue >= 0)
						ptr->m_CallbackFunc(pList[ptr->m_uValue.nValue]);
				}
			}
		}
		break;
	}
}



/* ========================================
	データ書込関数
	-------------------------------------
	内容：データの書き込み
	-------------------------------------
	引数1：string& data	書き込みデータ
	引数2：string path	パス
	引数3：Item* item	アイテム
=========================================== */
void Menu::DataWrite(std::string& data, std::string path, Item* item)
{
	// グループアイテムであれば、パスを更新して再帰呼び出し
	if (item->GetKind() == Item::Group)
	{
		ItemGroup* groupItem= static_cast<ItemGroup*>(item);

		// パスの更新
		path = path + groupItem->GetName() + "/";

		// グループ内アイテムのデータ書き込み
		auto it = groupItem->m_pItemGroup.begin();
		while (it != groupItem->m_pItemGroup.end())
		{
			DataWrite(data, path, *it);
			++it;
		}

		// 以降の処理は行わない
		return;
	}


	// セーブしない場合は以下の処理を行わない
	if (!item->m_bSave)	return;

	// 項目の書き込み
	data += Item::KindToStr(item->GetKind()) + ",";
	// 表示名の書き込み
	data += path + item->GetName() + ",";
	// 項目別の書き込み
	switch (item->GetKind())
	{
	default:
		data += "0";
	case Item::Bool:
		data += item->m_uValue.bValid ? "1" : "0";
		break;
	case Item::Int:
		data += std::to_string(item->m_uValue.nValue);
		break;
	case Item::Float:
		data += std::to_string(item->m_uValue.fValue);
		break;
	case Item::Color:
		data += std::to_string(item->m_uValue.fColor.x) + "/";
		data += std::to_string(item->m_uValue.fColor.y) + "/";
		data += std::to_string(item->m_uValue.fColor.z) + "/";
		data += std::to_string(item->m_uValue.fColor.w);
		break;
	case Item::List:
		data += std::to_string(item->m_uValue.nValue);
		break;
	}
	data += "\n";
}

/* ========================================
	データ読込関数
	-------------------------------------
	内容：データの読み込み
	-------------------------------------
	引数1：string path	パス
	引数2：Item* item	アイテム
=========================================== */
void Menu::DataRead(std::string path, Item* item)
{
	// グループアイテムであれば、パスを更新して再帰呼び出し
	if (item->GetKind() == Item::Group)
	{
		ItemGroup* groupItem = static_cast<ItemGroup*>(item);
		// パスの更新
		path = path + groupItem->GetName() + "/";

		// グループ内アイテムのデータ読み取り
		auto it = groupItem->m_pItemGroup.begin();
		while (it != groupItem->m_pItemGroup.end())
		{
			DataRead(path, *it);
			++it;
		}

		// 以降の処理は行わない
		return;
	}

	// セーブしない場合は以下の処理を行わない
	if (!item->m_bSave)	return;

	// パスと項目名から読み取るデータ名を作成
	path += item->GetName();

	// データ名に対応するデータを探索
	auto dataIt = std::find_if(m_SaveDataList.begin(), m_SaveDataList.end(),
		[&path](const SaveData& data) {
			return data.path == path;
		});

	// なければデータの上書きを行わない
	if (dataIt == m_SaveDataList.end()) return;

	// 項目に応じてデータの読み取り
	switch (item->GetKind())
	{
	case Item::Bool:
		item->m_uValue.bValid = atoi(dataIt->value.c_str()) > 0;
		break;
	case Item::Int:
		item->m_uValue.nValue = atoi(dataIt->value.c_str());
		break;
	case Item::Float:
		item->m_uValue.fValue = strtof(dataIt->value.c_str(), nullptr);
		break;
	case Item::Color: {
		const char* top[] = {
			dataIt->value.c_str(),
			strstr(top[0], "/") + 1,
			strstr(top[1], "/") + 1,
			strstr(top[2], "/") + 1,
		};
		item->m_uValue.fColor = DirectX::XMFLOAT4(
			strtof(top[0], nullptr),
			strtof(top[1], nullptr),
			strtof(top[2], nullptr),
			strtof(top[3], nullptr)
		);
		} break;
	case Item::List:
		ItemList* pList = static_cast<ItemList*>(item);
		item->m_uValue.nValue = atoi(dataIt->value.c_str());
		if (pList->m_CallbackFunc) {
			auto it = pList->m_sItemList.begin();
			for (int i = 0; i < pList->m_uValue.nValue; ++i)
				it++;
			pList->m_CallbackFunc(it->c_str());
		}
		break;
	}
}


} // namespace debug
#endif // _DEBUG
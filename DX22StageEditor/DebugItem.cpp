/* ========================================
	DX22Base/
	------------------------------------
	デバッグ表示項目用cpp
	------------------------------------
	DebugItem.cpp
========================================== */
#ifdef _DEBUG	// デバッグモードのみ有効

// =============== インクルード ===================
#include "DebugItem.h"

// =============== 名前空間 ===================
namespace DebugUI {

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
Item::Item()
	: m_sName("None")
	, m_eKind(Kind::Label)
	, m_bSave(false)
	, m_uValue{}
	, m_bSameLine(false)
{

}

/* ========================================
	デストラクタ関数
	-------------------------------------
	内容：解放処理
=========================================== */
Item::~Item()
{
}

/* ========================================
	項目作成(単体)
	-------------------------------------
	内容：値を保持する項目を作成
	-------------------------------------
	引数：const char* name		項目名
	引数：Kind kind				項目種別
	引数：bool isSave			保存するか
	引数：bool isSameLine		同じ行に表示するか
	-------------------------------------
	戻り値：Item* 項目ポインタ
=========================================== */
Item* Item::CreateValue(const char* name, Kind kind, bool isSave, bool isSameLine)
{
	ItemValue* item		= new ItemValue;
	item->m_sName		= name;		
	item->m_eKind		= kind;
	item->m_bSave		= isSave;
	item->m_bSameLine	= isSameLine;
	return item;
}

/* ========================================
	項目作成(変数紐づけ)
	-------------------------------------
	内容：変数と紐づけた項目を作成
	-------------------------------------
	引数：const char* name		項目名
	引数：Kind kind				項目種別
	引数：void* ptr				変数ポインタ
	引数：bool isSave			保存するか
	引数：bool isSameLine		同じ行に表示するか
	-------------------------------------
	戻り値：Item* 項目ポインタ
=========================================== */
Item* Item::CreateBind(const char* name, Kind kind, void* ptr, bool isSave, bool isSameLine)
{
	ItemBind* item		= new ItemBind;
	item->m_sName		= name;
	item->m_eKind		= kind;
	item->m_pBindVar	= ptr;
	item->m_bSave		= isSave;
	item->m_bSameLine = isSameLine;
	return item;
}


/* ========================================
	項目作成(コールバック)
	-------------------------------------
	内容：コールバック関数を紐づけた項目を作成
	-------------------------------------
	引数：const char* name		項目名
	引数：Kind kind				項目種別
	引数：Callback func			コールバック関数
	引数：bool isSave			保存するか
	-------------------------------------
	戻り値：Item* 項目ポインタ
=========================================== */
Item* Item::CreateCallBack(const char* name, Kind kind, Callback func, bool isSave, bool isSameLine)
{
	ItemCallback* item	= new ItemCallback;
	item->m_sName			= name;
	item->m_eKind			= kind;
	item->m_CallbackFunc	= func;
	item->m_bSave			= isSave;
	item->m_bSameLine = isSameLine;
	return item;
}


/* ========================================
	項目作成(グループ)
	-------------------------------------
	内容：グループ項目を作成
	-------------------------------------
	引数：const char* name		項目名
	-------------------------------------
	戻り値：Item* 項目ポインタ
=========================================== */
Item* Item::CreateGroup(const char* name)
{
	ItemGroup* item = new ItemGroup;
	item->m_sName		= name;
	item->m_eKind		= Item::Group;
	return item;
}

/* ========================================
	項目作成(リスト)
	-------------------------------------
	内容：リスト項目を作成
	-------------------------------------
	引数1：const char* name		項目名
	引数2：ConstCallback func	コールバック関数
	引数3：bool isSave			保存するか
	引数4：bool isDropDown		表示形式をドロップダウンにするか
	引数5：bool isSameLine		同じ行に表示するか
	引数6：int nDispLineNum		表示行数
	-------------------------------------
	戻り値：Item* 項目ポインタ
=========================================== */
Item* Item::CreateList(const char* name, ConstCallback func, bool isSave, bool bIsDropDown, bool isSameLine, int nDispLineNum)
{
	ItemList* item			= new ItemList;
	item->m_sName			= name;
	item->m_eKind			= Item::List;
	item->m_CallbackFunc	= func;
	item->m_bSave			= isSave;
	item->m_uValue.nValue	= -1;		// 最初は何も選択していない状態
	item->m_bIsDropDown		= bIsDropDown;
	item->m_bSameLine		= isSameLine;
	item->m_nDispLineNum	= nDispLineNum;
	return item;
}

/* ========================================
	リスト項目追加関数
	-------------------------------------
	内容：リストに表示項目を追加
	-------------------------------------
	引数1：const char* name		項目名
=========================================== */
void Item::AddListItem(const char* name)
{
	if (m_eKind != List) return;

	ItemList* pItemList = reinterpret_cast<ItemList*>(this);

	// 同じ名前の項目がある場合は追加しない
	if (std::find(pItemList->m_sItemList.begin(), pItemList->m_sItemList.end(), name) != pItemList->m_sItemList.end())
		return;

	reinterpret_cast<ItemList*>(this)->m_sItemList.push_back(name);
}

/* ========================================
	リスト項目挿入関数
	-------------------------------------
	内容：リストの指定した位置に表示項目を挿入
	-------------------------------------
	引数1：const char* name		項目名
	引数2：int num				挿入位置
=========================================== */
void Item::InsertListItem(const char* name, int num)
{
	if (m_eKind != List) return;
	ItemList* pItemList = reinterpret_cast<ItemList*>(this);
	std::list<std::string>::iterator itr = pItemList->m_sItemList.begin();

	// 挿入位置まで移動
	for (int i = 0; i < num; ++i)
		++itr;

	pItemList->m_sItemList.insert(itr, name);
}

/* ========================================
	リスト項目削除関数
	-------------------------------------
	内容：リストから表示項目を削除
	-------------------------------------
	引数1：const char* name		項目名
=========================================== */
void Item::RemoveListItem(const char* name)
{
	if (m_eKind != List) return;
	reinterpret_cast<ItemList*>(this)->m_sItemList.remove(name);
}

/* ========================================
	リスト項目削除関数(指定文字列除去)
	-------------------------------------
	内容：リストから表示項目を削除
		　検索名、リスト一覧から指定の文字を取り除いた文字列で検索
		　※オブジェクト一覧に使用
	-------------------------------------
	引数1：const char* name		項目名
	引数2：std::string sExceptText	検索時に除去する文字列
=========================================== */
void Item::RemoveListItem(const char* name, std::string sExceptText)
{
	if (m_eKind != List) return;

	ItemList* pItemList = reinterpret_cast<ItemList*>(this);

	// 検索名
	std::string sSearchName = name;
	// 指定の文字列を取り除く
	if (sSearchName.find(sExceptText) != std::string::npos)
	{
		int nHeadTextCnt = sSearchName.find(sExceptText);
		sSearchName = sSearchName.substr(nHeadTextCnt + sExceptText.size());
	}

	// リスト一覧を検索
	std::list<std::string>::iterator itr = pItemList->m_sItemList.begin();
	while (itr != pItemList->m_sItemList.end())
	{
		std::string sName = *itr;

		// 指定の文字列を取り除く
		if (sName.find(sExceptText) != std::string::npos) {
			// 先頭からDebugUI::CHILD_NAME_HEDAD_TEXTまでの文字数
			int nHeadTextCnt = sName.find(sExceptText);
			sName = sName.substr(nHeadTextCnt + sExceptText.size());
		}
		// 検索名と一致したら削除
		if (sSearchName == sName)
		{
			pItemList->m_sItemList.erase(itr);
			break;
		}
		++itr;
	}
}

/* ========================================
	リスト項目全削除関数
	-------------------------------------
	内容：リストから全ての表示項目を削除
=========================================== */
void Item::RemoveListItemAll()
{
	if (m_eKind != List) return;
	reinterpret_cast<ItemList*>(this)->m_sItemList.clear();
}


/* ========================================
	グループ項目追加関数
	-------------------------------------
	内容：グループに表示項目を追加
	-------------------------------------
	引数1：Item* item		項目
=========================================== */
void Item::AddGroupItem(Item* item)
{
	if (m_eKind != Group) return;
	static_cast<ItemGroup*>(this)->m_pItemGroup.push_back(item);
}

/* ========================================
	配列演算子(グループ)
	-------------------------------------
	内容：指定名のアイテムを取得
	-------------------------------------
	引数：const char* name 項目名
	-------------------------------------
	戻り値：Item& アイテム
=========================================== */
Item& Item::operator[](const char* name)
{
	// グループ以外の取得はダミーを返却
	if (m_eKind != Group) return *dummy;

	// グループ内のアイテム一覧を取得
	auto items = static_cast<ItemGroup*>(this)->m_pItemGroup;

	// アイテム一覧から指定アイテムの探索
	auto it = std::find_if(items.begin(), items.end(),
		[&name](const Item* obj) {
		return strcmp(obj->GetName(), name) == 0;
	});

	// 見つかったアイテムの返却
	if (it != items.end())
		return **it;

	// 見つからなかったらダミーを返却
	return *dummy;
}


/* ========================================
	変換(文字列→項目種別)関数
	-------------------------------------
	内容：文字列から項目種別に変換
	-------------------------------------
	引数：std::string 文字列
	-------------------------------------
	戻り値：Kind kind 項目種別
=========================================== */
Item::Kind Item::StrToKind(std::string str)
{
	if (str == "Label")		return Item::Label;
	if (str == "Bool")		return Item::Bool;
	if (str == "Int")		return Item::Int;
	if (str == "Float")		return Item::Float;
	if (str == "Vector")	return Item::Vector;
	if (str == "Color")		return Item::Color;
	if (str == "Path")		return Item::Path;
	if (str == "Command")	return Item::Command;
	if (str == "Group")		return Item::Group;
	if (str == "List")		return Item::List;
	return Item::Label;
}

/* ========================================
	変換(項目種別→文字列)関数
	-------------------------------------
	内容：項目種別から文字列に変換
	-------------------------------------
	引数：Kind kind 項目種別
	-------------------------------------
	戻り値：std::string 文字列
=========================================== */
std::string Item::KindToStr(Kind kind)
{
	switch (kind) {
	default:
	case Item::Label:		return "Label";
	case Item::Bool:		return "Bool";
	case Item::Int:			return "Int";
	case Item::Float:		return "Float";
	case Item::Vector:		return "Vector";
	case Item::Color:		return "Color";
	case Item::Path:		return "Path";
	case Item::Command:		return "Command";
	case Item::Group:		return "Group";
	case Item::List:		return "List";
	}
}

/* ========================================
	ゲッター(項目名)関数
	-------------------------------------
	戻値：const char* 項目名
=========================================== */
const char* Item::GetName() const
{
	return m_sName.c_str();
}

/* ========================================
	ゲッター(項目種別)関数
	-------------------------------------
	戻値：Kind 項目種別
=========================================== */
Item::Kind Item::GetKind() const
{
	return m_eKind;
}

/* ========================================
	ゲッター(項目(bool))関数
	-------------------------------------
	戻値：bool 項目の値
=========================================== */
bool Item::GetBool() const
{
	if (m_eKind == Bool)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.bValid;
	return false;
}

/* ========================================
	ゲッター(項目(int))関数
	-------------------------------------
	戻値：int 項目の値
=========================================== */
int Item::GetInt() const
{
	if (m_eKind == Int|| m_eKind == List)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.nValue;
	/*else if (m_eKind == List)
		return static_cast<const ItemList*>(this)->m_nSelectNo;*/
	return 0;
}

/* ========================================
	ゲッター(項目(float))関数
	-------------------------------------
	戻値：float 項目の値
=========================================== */
float Item::GetFloat() const
{
	if (m_eKind == Float)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.fValue;
	return 0.0f;
}

/* ========================================
	ゲッター(項目(Vector))関数
	-------------------------------------
	戻値：DirectX::XMFLOAT3 項目の値
=========================================== */
DirectX::XMFLOAT3 Item::GetVector() const
{
	if (m_eKind == Vector)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.fVector;
	return DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

/* ========================================
	ゲッター(項目(Vector2))関数
	-------------------------------------
	戻値：DirectX::XMFLOAT2 項目の値
=========================================== */
DirectX::XMFLOAT2 Item::GetVector2() const
{
	if (m_eKind == Vector2)
		return reinterpret_cast<const ItemValue*>(this)->m_uValue.fVector2;
	return DirectX::XMFLOAT2(0.0f, 0.0f);
}

/* ========================================
	ゲッター(項目(Color))関数
	-------------------------------------
	戻値：DirectX::XMFLOAT4 項目の値
=========================================== */
DirectX::XMFLOAT4 Item::GetColor() const
{
	if (m_eKind == Color)
		return static_cast<const ItemValue*>(this)->m_uValue.fColor;
	return DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
}

/* ========================================
	ゲッター(項目(Path))関数
	-------------------------------------
	戻値：const char* 項目の値
=========================================== */
const char* Item::GetPath() const
{
	if (m_eKind == Path)
		return static_cast<const ItemValue*>(this)->m_uValue.cPath;
	return "";
}


/* ========================================
	リスト番号取得関数
	-------------------------------------
	内容：リストを項目名で検索し、何番目かを取得
	-------------------------------------
	引数：const char* name 項目名
	-------------------------------------
	戻り値：int 番号
=========================================== */
int Item::GetListNo(const char* name) const
{
	if (m_eKind == List)
	{
		const ItemList* pItemList = static_cast<const ItemList*>(this);
		// リストを検索
		std::list<std::string>::const_iterator itr = std::find(pItemList->m_sItemList.begin(), pItemList->m_sItemList.end(), name);
		if (itr != pItemList->m_sItemList.end())
		{
			// 見つかったら何番目かを返す
			return std::distance(pItemList->m_sItemList.begin(), itr);
		}
	}

	return -1;
}

/* ========================================
	リスト取得関数
	-------------------------------------
	戻値：std::list<std::string> リスト
=========================================== */
std::list<std::string> Item::GetList() const
{
	if (m_eKind == List)
		return static_cast<const ItemList*>(this)->m_sItemList;
	return std::list<std::string>();
}

/* ========================================
	リストテキスト取得関数
	-------------------------------------
	内容：リストの指定番目のテキストを取得
	-------------------------------------
	引数：int num 番号
	-------------------------------------
	戻り値：std::string テキスト
=========================================== */
std::string Item::GetListText(int num) const
{
	if (m_eKind == List)
	{
		const ItemList* pItemList = static_cast<const ItemList*>(this);
		if (num < pItemList->m_sItemList.size())
		{
			auto itr = pItemList->m_sItemList.begin();
			std::advance(itr, num);	// 指定番目まで進める
			return *itr;
		}
	}
	return "";
}

/* ========================================
	ゲッター(項目(Text))関数
	-------------------------------------
	戻値：const char* 項目の値
=========================================== */
std::string Item::GetText() const
{
	if (m_eKind == Text)
		return static_cast<const ItemValue*>(this)->m_uValue.cText;
}

/* ========================================
	ゲッター(リスト表示行数)関数
	-------------------------------------
	戻値：int 表示行数
=========================================== */
int Item::GetDispLineNum() const
{
	if (m_eKind == List)
	{
		const ItemList* pItemList = static_cast<const ItemList*>(this);
		return pItemList->m_nDispLineNum;
	}
	return -1;
}

/* ========================================
	セッター(項目(bool))関数
	-------------------------------------
	引数：bool value 値
=========================================== */
void Item::SetBool(bool value)
{
	if (m_eKind == Bool)
		reinterpret_cast<ItemValue*>(this)->m_uValue.bValid = value;
}

/* ========================================
	セッター(項目(int))関数
	-------------------------------------
	引数：int value 値
=========================================== */
void Item::SetInt(int value)
{
	if (m_eKind == Int)
		reinterpret_cast<ItemValue*>(this)->m_uValue.nValue = value;
}

/* ========================================
	セッター(項目(float))関数
	-------------------------------------
	引数：float value 値
=========================================== */
void Item::SetFloat(float value)
{
	if (m_eKind == Float)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fValue = value;
}

/* ========================================
	セッター(項目(Vector))関数
	-------------------------------------
	引数：DirectX::XMFLOAT3 value 値
=========================================== */
void Item::SetVector(DirectX::XMFLOAT3 value)
{
	if (m_eKind == Vector)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fVector = value;

}

/* ========================================
	セッター(項目(Vector2))関数
	-------------------------------------
	引数：DirectX::XMFLOAT2 value 値
=========================================== */
void Item::SetVector2(DirectX::XMFLOAT2 value)
{
	if (m_eKind == Vector2)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fVector2 = value;
}

/* ========================================
	セッター(項目(Color))関数
	-------------------------------------
	引数：DirectX::XMFLOAT4 value 値
=========================================== */
void Item::SetColor(DirectX::XMFLOAT4 value)
{
	if (m_eKind == Color)
		reinterpret_cast<ItemValue*>(this)->m_uValue.fColor = value;
}

/* ========================================
	セッター(項目(Path))関数
	-------------------------------------
	引数：const char* value 値
=========================================== */
void Item::SetPath(std::string value)
{
	if (m_eKind == Path)
	{
		std::strncpy(reinterpret_cast<ItemValue*>(this)->m_uValue.cPath, value.c_str(), value.size());
		reinterpret_cast<ItemValue*>(this)->m_uValue.cPath[value.size()] = '\0';	// 終端文字
	}
}

/* ========================================
	セッター(項目(Text))関数
	-------------------------------------
	引数：const char* value 値
=========================================== */
void Item::SetText(std::string value)
{
	if (m_eKind == Text)
	{
		std::strncpy(reinterpret_cast<ItemValue*>(this)->m_uValue.cPath, value.c_str(), value.size());
		reinterpret_cast<ItemValue*>(this)->m_uValue.cPath[value.size()] = '\0';	// 終端文字
	}
}

/* ========================================
	セッター(リスト番号)関数
	-------------------------------------
	内容：リストの選択番号を設定
	-------------------------------------
	引数：int value 番号
=========================================== */
void Item::SetListNo(int value)
{
	if (m_eKind == List)
		reinterpret_cast<ItemValue*>(this)->m_uValue.nValue = value;
}

/* ========================================
	セッター(表示行数)関数
	-------------------------------------
	内容：表示行数を設定
	-------------------------------------
	引数：int value 行数
=========================================== */
void Item::SetDispLineNum(int value)
{
	if (m_eKind == List)
		reinterpret_cast<ItemValue*>(this)->m_uValue.nValue = value;
}




}

#endif
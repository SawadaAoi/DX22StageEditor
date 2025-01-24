/* ========================================
	DX22Base/
	------------------------------------
	デバッグ表示項目用ヘッダ
	------------------------------------
	説明：デバッグメニューの表示項目(チェックボックス、数値入力、ボタンなど)
		　を管理するクラス
	------------------------------------
	DebugItem.h
========================================== */
#ifdef _DEBUG	// デバッグモードのみ有効
#pragma once

// =============== インクルード ===================
#include <functional>
#include <list>
#include <DirectXMath.h>
#include <string>

// =============== 名前空間 ===================
namespace DebugUI {

	const int MAX_ITEM_TEXT = 260;	// 項目名の最大文字数

// 表示項目
class Item
{
	friend class Menu;
public:
	// コールバック関数(ボタンor値変更時)
	// 引数：isWrite	書き込み時か(ボタンの場合は押下時)
	// 引数：arg		引数(書き込んだ値、ボタンの場合はnullptr)
	using Callback = std::function<void(bool isWrite, void* arg)>;

	// コールバック関数(一覧表示)
	// 引数：arg	選択中	
	using ConstCallback = std::function<void(const void* arg)>;

	// 項目が保持する値
	union Value {
		bool				bValid;
		int					nValue;
		float				fValue;
		DirectX::XMFLOAT3	fVector;
		DirectX::XMFLOAT2	fVector2;
		DirectX::XMFLOAT4	fColor;
		char				cPath[MAX_ITEM_TEXT];
		char				cText[MAX_ITEM_TEXT];


	};

	// 項目種別
	enum Kind {
		Label,		// 項目のみの表示
		Text,		// テキスト入力
		Bool,		// チェックボックス
		Int,		// 整数入力
		Float,		// 小数入力
		Vector,		// ベクター入力
		Vector2,	// ベクター入力(2D)
		Color,		// 色入力
		Path,		// ファイルパスの指定
		Command,	// ボタン
		Group,		// 表示項目をまとめる
		List,		// 一覧表示
	};
protected:
	Item();
public:
	virtual ~Item();

	// 項目生成
	static Item* CreateValue(const char* name, Kind kind, bool isSave = false, bool isSameLine = false);
	static Item* CreateBind(const char* name, Kind kind, void* ptr, bool isSave = false, bool isSameLine = false);
	static Item* CreateCallBack(const char* name, Kind kind, Callback func, bool isSave = false, bool isSameLine = false);
	static Item* CreateGroup(const char* name);
	static Item* CreateList(const char* name, ConstCallback func = nullptr, 
		bool isSave = false, bool bIsDropDown = false, bool isSameLine = false, int nDispLineNum = 5);

public:

	// グループの設定
	void AddGroupItem(Item* item);
	Item& operator[](const char* name);
	void RemoveGroupItem(const char* name);

	// 一覧表示の設定
	void AddListItem(const char* name);
	void InsertListItem(const char* name, int num);
	void RemoveListItem(const char* name);
	void RemoveListItem(const char* name, std::string exceptText);
	void RemoveListItemAll();

	// 文字列から項目の種別を取得
	static Kind StrToKind(std::string str);
	// 項目の種別から文字列を取得
	static std::string KindToStr(Kind kind);


	// ゲッター
	const char* GetName() const;
	Kind GetKind() const;

	bool GetBool() const;
	int GetInt() const;
	float GetFloat() const;
	DirectX::XMFLOAT3 GetVector() const;
	DirectX::XMFLOAT2 GetVector2() const;
	DirectX::XMFLOAT4 GetColor() const;
	const char* GetPath() const;
	int GetListNo(const char* name) const;
	std::list<std::string> GetList() const;
	std::string GetListText(int num) const;
	std::string GetText() const;
	int GetDispLineNum() const;

	// セッター
	void SetBool(bool value);
	void SetInt(int value);
	void SetFloat(float value);
	void SetVector(DirectX::XMFLOAT3 value);
	void SetVector2(DirectX::XMFLOAT2 value);
	void SetColor(DirectX::XMFLOAT4 value);
	void SetPath(std::string value);
	void SetText(std::string value);
	void SetListNo(int value);
	void SetDispLineNum(int value);

public:
	static inline Item* dummy;			// 指定データが取得できなかった場合のダミー
	bool				m_bSave;		// 保存フラグ
	bool				m_bSameLine;	// 同じ行に表示するか
private:
	std::string m_sName;	// 項目名
	Kind		m_eKind;	// 項目種別
	Value		m_uValue;	// 入力値


};
using Items = std::list<Item*>;

// 表示項目(単体)
class ItemValue : public Item
{
public:
	ItemValue() /*: m_uValue{} , m_bSave(false)*/ {}
	~ItemValue() {}
};


// 表示項目(変数紐づけ)
class ItemBind : public Item
{
public:
	ItemBind() : m_pBindVar(nullptr) {}
	~ItemBind() {}
public:
	void* m_pBindVar;	// 紐づけ変数
};


// 表示項目(コールバック)
class ItemCallback : public Item
{
public:
	ItemCallback() : /*m_uValue{},*/ m_CallbackFunc(nullptr) {}
	~ItemCallback() {}
public:
	Callback	m_CallbackFunc;	// コールバック関数
};


// 表示項目(グループ)
class ItemGroup : public Item
{
public:
	ItemGroup() {}
	~ItemGroup()
	{
		auto it = m_pItemGroup.begin();
		while (it != m_pItemGroup.end())
		{
			delete (*it);
			++it;
		}
	}
public:
	Items m_pItemGroup;	// グループ内の項目
};



// 表示項目(リスト)
class ItemList : public Item
{
public:
	ItemList() : m_sItemList(), m_bIsDropDown(false), m_CallbackFunc(nullptr), m_nDispLineNum(5) { m_sItemList.clear(); }
	~ItemList() {}
public:
	std::list<std::string>	m_sItemList;		// アイテム一覧
	ConstCallback			m_CallbackFunc;		// 項目選択時のコールバック
	bool					m_bIsDropDown;		// コンボボックスか
	int						m_nDispLineNum;		// 表示行数
};
}
#endif
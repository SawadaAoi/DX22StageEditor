/* ========================================
	DX22Base/
	------------------------------------
	ファイル処理管理用ヘッダ
	------------------------------------
	説明：ファイルの読み書き処理を行うクラス
	------------------------------------
	FileManager.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <string>
#include "Vector3.h"
#include "Quaternion.h"


// =============== 前方宣言 =======================
class ObjectBase;


// =============== 定数定義 =======================
const int MAX_TEXT_SIZE = 256;


// =============== クラス定義 ===================
class FileManager
{
public:

	struct S_SaveDataObject
	{
		char			cClassType[MAX_TEXT_SIZE];
		Vector3<float>	vPos;
		Quaternion		qRot;
		Vector3<float>	vScale;
		char			cObjectName[MAX_TEXT_SIZE];
		char			cParentName[MAX_TEXT_SIZE];
	};

public:
	FileManager() {};
	~FileManager() {};

	static void StageObjectOutput(std::string sPath);
	static void StageObjectInput(std::string sPath);
	static void StageSelectOutput(std::string sPath);
private:
	static void RegistStageSelectSample(std::ofstream& file);
	static void AdjustObjectParameter(ObjectBase* pObject);
};


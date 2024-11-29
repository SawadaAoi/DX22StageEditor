/* ========================================
	DX22Base/
	------------------------------------
	数学ユーティリティ用ヘッダ
	------------------------------------
	説明：数学関連の便利な関数を定義
	------------------------------------
	MathUtils.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <cstdlib>	// rand関数用

// =============== 名前空間 =====================
namespace MathUtils
{
	// =============== 定数 =====================
	const float PI = 3.14159265358979323846f;	// 円周率
	const float EPSILON = 1.0e-5f;	// 許容誤差

	// =============== 関数 =====================
	// 角度をラジアンに変換
	inline float ToRadian(float degree)
	{
		return degree * PI / 180.0f;
	}

	// ラジアンを角度に変換
	inline float ToDegree(float radian)
	{
		return radian * 180.0f / PI;
	}


	// ランダムな値を取得(実数版)
	inline float Random(float min, float max)
	{
		return min + static_cast<float>(rand()) / (RAND_MAX + 1) * (max - min);
	}

	// ランダムな値を取得(整数版)
	inline int Random(int min, int max)
	{
		return min + rand() % (max - min + 1);
	}
}
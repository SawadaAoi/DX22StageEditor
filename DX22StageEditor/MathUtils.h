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
#include <random>	// 乱数生成用

// =============== 名前空間 =====================
namespace MathUtils
{
	// =============== 定数 =====================
	constexpr float PI		= 3.14159265358979323846f;	// 円周率
	constexpr float EPSILON = 1.0e-5f;					// 許容誤差

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

	// 汎用ランダム値生成（テンプレート版）
	template<typename T>
	inline T Random(T min, T max)
	{
		static thread_local std::mt19937 gen(std::random_device{}());
		if constexpr (std::is_integral<T>::value) {
			std::uniform_int_distribution<T> dist(min, max);
			return dist(gen);
		}
		else {
			std::uniform_real_distribution<T> dist(min, max);
			return dist(gen);
		}
	}
}
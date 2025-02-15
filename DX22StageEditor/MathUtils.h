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
	constexpr float PI = 3.14159265358979323846f;	// 円周率
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

	// -π～πの範囲から0～2πの範囲に変換
	inline double ConvertTo0To2Pi(double value)
	{
		// 負の値の場合、2πを加えて正の範囲に移す
		while (value < 0)
			value += 2 * PI;

		// 2πを超える値の場合、2πで割った余りを取る
		return fmod(value, 2 * PI);
	}

	// DirectXの方向(時計回り0時スタート)を三角関数の方向(反時計回り3時スタート)に変換
	inline float ConvertDirectionDXToTrigonometric(float dxDirection)
	{
		dxDirection -= MathUtils::PI / 2.0f;	// 90度回転
		dxDirection *= -1.0f;				// 逆回転

		return dxDirection;
	}
}
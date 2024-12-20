/* ========================================
    DX22Base/
    ------------------------------------
    2次元ベクトル用ヘッダ
    ------------------------------------
	説明：2次元ベクトルのテンプレートクラス
    ------------------------------------
    Vector2.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <cmath>
#include <utility>
#include <DirectXMath.h>    // DirectX::XMFLOAT2

// =============== クラス定義 =====================
template <typename T>
class Vector2
{
public:
    // コンストラクタ
	Vector2();                // デフォルトコンストラクタ
    Vector2(T x, T y);    // 2次元ベクトルの初期化

    // 演算子オーバーロード =====================
    Vector2<T> operator+(const Vector2<T>& other) const;    // 加算
    Vector2<T> operator-(const Vector2<T>& other) const;    // 減算
    Vector2<T> operator*(const Vector2<T>& other) const;    // 乗算
    Vector2<T> operator*(T scalar) const;                   // スカラー乗算
    Vector2<T> operator/(const Vector2<T>& other) const;    // 除算
    Vector2<T> operator/(T scalar) const;                   // スカラー除算

    Vector2<T>& operator+=(const Vector2<T>& other);    // 加算代入
    Vector2<T>& operator-=(const Vector2<T>& other);    // 減算代入
    Vector2<T>& operator*=(const Vector2<T>& other);    // 乗算代入
    Vector2<T>& operator*=(T scalar);                  // スカラー乗算代入
    Vector2<T>& operator/=(const Vector2<T>& other);    // 除算代入
    Vector2<T>& operator/=(T scalar);                  // スカラー除算代入

    T operator[](int index) const;    // 添え字演算子

	// 単項演算子 ==============================
	Vector2<T> operator+() const;    // 単項プラス
	Vector2<T> operator-() const;    // 単項マイナス

    // 比較演算子 ==============================
    bool operator==(const Vector2<T>& other) const;    // 等価比較
    bool operator!=(const Vector2<T>& other) const;    // 非等価比較
	bool operator<(const Vector2<T>& other) const;    // ベクトルの長さで比較
	bool operator>(const Vector2<T>& other) const;    // ベクトルの長さで比較
	bool operator<=(const Vector2<T>& other) const;    // ベクトルの長さで比較
	bool operator>=(const Vector2<T>& other) const;    // ベクトルの長さで比較

    // メンバ関数 ==============================
    double  Length() const;        // ベクトルの長さ
    T       LengthSq() const;           // ベクトルの長さの2乗
    Vector2<T> Normalize() const;    // 正規化

    T Dot(const Vector2<T>& other) const;    // 内積
	T Cross(const Vector2<T>& other) const;    // 外積

    T Distance(const Vector2<T>& other) const;    // 2点間の距離
	T DistanceSq(const Vector2<T>& other) const;    // 2点間の距離の2乗

	double AngleRad(const Vector2<T>& other) const;    // ラジアン角度
	double AngleDeg(const Vector2<T>& other) const;    // 度角度
	double DirectionRad(const Vector2<T>& other) const;    // 方向(ラジアン
	double DirectionDeg(const Vector2<T>& other) const;    // 方向(度)

	Vector2<T> Lerp(const Vector2<T>& other, float t) const;    // 線形補間

	// DirectX::XMFLOAT2 へ変換
	DirectX::XMFLOAT2 ToXMFLOAT2() const;                       // Vector2 -> DirectX::XMFLOAT2
	Vector2<T> ToVector2(const DirectX::XMFLOAT2& other) const; // DirectX::XMFLOAT2 -> Vector2 


    // 静的メンバ関数 ==========================
    static Vector2<T> Zero();    // ゼロベクトル
    static Vector2<T> One();     // 1ベクトル
	static Vector2<T> Up();      // 上方向ベクトル
	static Vector2<T> Down();    // 下方向ベクトル
	static Vector2<T> Right();   // 右方向ベクトル
	static Vector2<T> Left();    // 左方向ベクトル

public:
    T x;    // X座標
    T y;    // Y座標
};

#include "Vector2.inl"    // インラインファイル

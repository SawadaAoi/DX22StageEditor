/* ========================================
	DX22Base/
	------------------------------------
	3次元ベクトル用ヘッダ
	------------------------------------
	説明：3次元ベクトルのテンプレートクラス
	------------------------------------
	Vector3.h
========================================== */
#pragma once

// =============== インクルード ===================
#include <cmath>
#include <utility>
#include <DirectXMath.h>	// DirectX::XMFLOAT3

// =============== クラス定義 =====================
template <typename T>
class Vector3
{
public:
	Vector3();	// コンストラクタ
	Vector3(T x, T y, T z);	// コンストラクタ

	// 演算子オーバーロード =====================
	Vector3<T> operator+(const Vector3<T>& other) const;		// 加算
	Vector3<T> operator-(const Vector3<T>& other) const;		// 減算
	Vector3<T> operator*(const Vector3<T>& other) const;		// 乗算
	Vector3<T> operator*(T scalar) const;						// 乗算(スカラー)
	Vector3<T> operator/(const Vector3<T>& other) const;		// 除算
	Vector3<T> operator/(T scalar) const;						// 除算(スカラー)

	Vector3<T>& operator+=(const Vector3<T>& other);		// 加算代入
	Vector3<T>& operator-=(const Vector3<T>& other);		// 減算代入
	Vector3<T>& operator*=(const Vector3<T>& other);		// 乗算代入
	Vector3<T>& operator*=(T scalar);						// 乗算代入(スカラー)
	Vector3<T>& operator/=(const Vector3<T>& other);		// 除算代入
	Vector3<T>& operator/=(T scalar);						// 除算代入(スカラー)

	T operator[](int index) const;	// 添え字演算子

	// 演算子オーバーロード(単項演算子) =====================
	Vector3<T> operator+() const;	// 単項プラス
	Vector3<T> operator-() const;	// 単項マイナス

	// 比較演算子 ==============================
	bool operator==(const Vector3<T>& other) const;
	bool operator!=(const Vector3<T>& other) const;
	bool operator<(const Vector3<T>& other) const;	// ベクトルの長さで比較
	bool operator>(const Vector3<T>& other) const;	// ベクトルの長さで比較
	bool operator<=(const Vector3<T>& other) const;	// ベクトルの長さで比較
	bool operator>=(const Vector3<T>& other) const;	// ベクトルの長さで比較

	// メンバ関数 ==============================

	double	Length() const;					// ベクトルの長さ
	T		LengthSq() const;				// ベクトルの長さの2乗
	Vector3<T> GetNormalize() const;	// 正規化
	void Normalize();


	T Dot(const Vector3<T>& other) const;				// 内積
	Vector3<T> Cross(const Vector3<T>& other) const;	// 外積

	T Distance(const Vector3<T>& other) const;		// 2点間の距離
	T DistanceSq(const Vector3<T>& other) const;	// 2点間の距離の2乗

	double AngleRad(const Vector3<T>& other) const;		// 2つのベクトルがなす角度(ラジアン)
	double AngleDeg(const Vector3<T>& other) const;		// 2つのベクトルがなす角度(度)
	double DirectionRadXZ(const Vector3<T>& other) const;	// 引数ベクトルへの方向(ラジアン)
	double DirectionDegXZ(const Vector3<T>& other) const;	// 引数ベクトルへの方向(度)


	// 線形補間
	static Vector3<T> Lerp(const Vector3<T>& start, const Vector3<T>& end, double t);	// 静的に呼び出せる
	Vector3<T> Lerp(const Vector3<T>& other, double t) const;							// メンバ関数を変更する

	// DirectX::XMFLOAT3変換
	DirectX::XMFLOAT3 ToXMFLOAT3() const;					// Vector3 -> DirectX::XMFLOAT3
	Vector3<T>& operator=(const DirectX::XMFLOAT3& other);	// DirectX::XMFLOAT3 -> Vector3

	// DirectX::XMMATRIX変換
	static Vector3<float> FromMatrix_Translation(const DirectX::XMMATRIX matrix);	// 行列から移動成分を取得
	static Vector3<float> FromMatrix_Scale(const DirectX::XMMATRIX matrix);			// 行列から拡大成分を取得
	static Vector3<float> FromMatrix_RotationEuler(const DirectX::XMMATRIX matrix);	// 行列から回転成分を取得

	// DirectX::XMVECTOR変換
	DirectX::XMVECTOR ToXMVECTOR() const;									// Vector3 -> DirectX::XMVECTOR
	static Vector3<float> FromXMVECTOR(const DirectX::XMVECTOR& vector);	// DirectX::XMVECTOR -> Vector3

	T GetMax() const;	// 最大値
	T GetMin() const;	// 最小値

	// 静的メンバ関数 ==========================
	static Vector3<T> Zero();		// ゼロベクトル
	static Vector3<T> One();		// 1ベクトル
	static Vector3<T> Up();			// 上方向ベクトル
	static Vector3<T> Down();		// 下方向ベクトル
	static Vector3<T> Right();		// 右方向ベクトル
	static Vector3<T> Left();		// 左方向ベクトル
	static Vector3<T> Forward();	// 前方向ベクトル
	static Vector3<T> Back();		// 後方向ベクトル

public:
	T x;
	T y;
	T z;
};

#include "Vector3.inl"	// インラインファイル


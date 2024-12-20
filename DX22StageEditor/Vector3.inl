/* ========================================
	DX22Base/
	------------------------------------
	3次元ベクトル用inlファイル
	------------------------------------
	※inlファイルはヘッダファイルに記載されているのと同じ
	　ヘッダーファイルの冗長化を防ぐために使用
	------------------------------------
	Vector3.inl
========================================== */

// =============== インクルード ===================
#include "Vector3.h"
#include "MathUtils.h"
#include <algorithm>	// std::max, std::min

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：宣言時に初期化
=========================================== */
template<typename T>
inline Vector3<T>::Vector3()
{
	this->x = 0;	// this->xはクラスのメンバ変数xを指す
	this->y = 0;	// this->yはクラスのメンバ変数yを指す
	this->z = 0;	// this->zはクラスのメンバ変数zを指す
}

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：宣言時に初期化
	-------------------------------------
	引数1：x	X座標
	引数2：y	Y座標
	引数3：z	Z座標
=========================================== */
template<typename T>
Vector3<T>::Vector3(T x, T y, T z)
{
	this->x = x;	// this->xはクラスのメンバ変数xを指す
	this->y = y;	// this->yはクラスのメンバ変数yを指す
	this->z = z;	// this->zはクラスのメンバ変数zを指す
}

// ======================================== 演算子オーバーロード ========================================

// std::moveは処理の最適化を行うために使用
// この関数内でのotherの値は使用後に無効化されるため、

/* ========================================
	加算演算子
	-------------------------------------
	内容：ベクトル同士の加算
	-------------------------------------
	引数：other	加算するベクトル
	-------------------------------------
	戻値：加算結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const
{
	return std::move(Vector3<T>(this->x + other.x, this->y + other.y, this->z + other.z));
}

/* ========================================
	減算演算子
	-------------------------------------
	内容：ベクトル同士の減算
	-------------------------------------
	引数：other	減算するベクトル
	-------------------------------------
	戻値：減算結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T>& other) const
{
	return std::move(Vector3<T>(this->x - other.x, this->y - other.y, this->z - other.z));
}

/* ========================================
	乗算演算子
	-------------------------------------
	内容：ベクトル同士の乗算
	-------------------------------------
	引数：other	乗算するベクトル
	-------------------------------------
	戻値：乗算結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator*(const Vector3<T>& other) const
{
	return std::move(Vector3<T>(this->x * other.x, this->y * other.y, this->z * other.z));
}

/* ========================================
	乗算演算子(スカラー)
	-------------------------------------
	内容：ベクトルとスカラーの乗算
	-------------------------------------
	引数：scalar	乗算するスカラー
	-------------------------------------
	戻値：乗算結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator*(T scalar) const
{
	return std::move(Vector3<T>(this->x * scalar, this->y * scalar, this->z * scalar));
}

/* ========================================
	除算演算子
	-------------------------------------
	内容：ベクトル同士の除算
	-------------------------------------
	引数：other	除算するベクトル
	-------------------------------------
	戻値：除算結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator/(const Vector3<T>& other) const
{
	// 0での除算を防ぐ
	return std::move(Vector3<T>(
		other.x != 0 ? this->x / other.x : 0,
		other.y != 0 ? this->y / other.y : 0,
		other.z != 0 ? this->z / other.z : 0
	));
}

/* ========================================
	除算演算子(スカラー)
	-------------------------------------
	内容：ベクトルとスカラーの除算
	-------------------------------------
	引数：scalar	除算するスカラー
	-------------------------------------
	戻値：除算結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator/(T scalar) const
{
	// 0での除算を防ぐ
	return std::move(Vector3<T>(
		scalar != 0 ? this->x / scalar : 0,
		scalar != 0 ? this->y / scalar : 0,
		scalar != 0 ? this->z / scalar : 0
	));
}

/* ========================================
	加算代入演算子
	-------------------------------------
	内容：ベクトル同士の加算代入
	-------------------------------------
	引数：other	加算するベクトル
	-------------------------------------
	戻値：加算後の自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;

	return *this;
}

/* ========================================
	減算代入演算子
	-------------------------------------
	内容：ベクトル同士の減算代入
	-------------------------------------
	引数：other	減算するベクトル
	-------------------------------------
	戻値：減算後の自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;

	return *this;
}

/* ========================================
	乗算代入演算子
	-------------------------------------
	内容：ベクトル同士の乗算代入
	-------------------------------------
	引数：other	乗算するベクトル
	-------------------------------------
	戻値：乗算後の自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& other)
{
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;

	return *this;
}

/* ========================================
	乗算代入演算子(スカラー)
	-------------------------------------
	内容：ベクトルとスカラーの乗算代入
	-------------------------------------
	引数：scalar	乗算するスカラー
	-------------------------------------
	戻値：乗算後の自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T>& Vector3<T>::operator*=(T scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;

	return *this;
}

/* ========================================
	除算代入演算子
	-------------------------------------
	内容：ベクトル同士の除算代入
	-------------------------------------
	引数：other	除算するベクトル
	-------------------------------------
	戻値：除算後の自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T>& Vector3<T>::operator/=(const Vector3<T>& other)
{
	this->x = other.x != 0 ? this->x / other.x : 0;
	this->y = other.y != 0 ? this->y / other.y : 0;
	this->z = other.z != 0 ? this->z / other.z : 0;

	return *this;
}

/* ========================================
	除算代入演算子(スカラー)
	-------------------------------------
	内容：ベクトルとスカラーの除算代入
	-------------------------------------
	引数：scalar	除算するスカラー
	-------------------------------------
	戻値：除算後の自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T>& Vector3<T>::operator/=(T scalar)
{
	// 0での除算を防ぐ
	if (scalar != 0)
	{
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;
	}
	else
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	return *this;
}

/* ========================================
	添え字演算子
	-------------------------------------
	内容：ベクトルの各要素にアクセス
		　配列のようにアクセスできる
	-------------------------------------
	引数：index	要素番号
	-------------------------------------
	戻値：要素の値
=========================================== */
template<typename T>
inline T Vector3<T>::operator[](int index) const
{
	switch (index)
	{
	case 0: return this->x;
	case 1: return this->y;
	case 2: return this->z;
	default: return 0;
	}

}

/* ========================================
	単項演算子(+)
	-------------------------------------
	内容：ベクトルの単項演算子(+)
	-------------------------------------
	戻値：自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator+() const
{
	return Vector3<T>(*this);
}

/* ========================================
	単項演算子(-)
	-------------------------------------
	内容：ベクトルの単項演算子(-)
	-------------------------------------
	戻値：自身のベクトルの符号を反転
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-this->x, -this->y, -this->z);
}

/* ========================================
	比較演算子(==)
	-------------------------------------
	内容：ベクトル同士の比較
	-------------------------------------
	引数：other	比較するベクトル
	-------------------------------------
	戻値：true	一致
		  false	不一致
=========================================== */
template<typename T>
inline bool Vector3<T>::operator==(const Vector3<T>& other) const
{
	// 一つでも異なる値があればfalseを返す
	if (this->x != other.x) return false;
	if (this->y != other.y) return false;
	if (this->z != other.z) return false;

	return true;
}

/* ========================================
	比較演算子(!=)
	-------------------------------------
	内容：ベクトル同士の比較
	-------------------------------------
	引数：other	比較するベクトル
	-------------------------------------
	戻値：true	不一致
		  false	一致
=========================================== */
template<typename T>
inline bool Vector3<T>::operator!=(const Vector3<T>& other) const
{
	// 一つでも同じ値があればfalseを返す
	if (this->x == other.x) return false;
	if (this->y == other.y) return false;
	if (this->z == other.z) return false;

	return true;
}

/* ========================================
	比較演算子(<)
	-------------------------------------
	内容：ベクトル同士の比較(ベクトルの長さ)
	-------------------------------------
	引数：other	比較するベクトル
	-------------------------------------
	戻値：true	自身の方が小さい
		  false	自身の方が大きい
=========================================== */
template<typename T>
inline bool Vector3<T>::operator<(const Vector3<T>& other) const
{
	T thisNormSquared = this->x * this->x + this->y * this->y + this->z * this->z;
	T otherNormSquared = other.x * other.x + other.y * other.y + other.z * other.z;

	return thisNormSquared < otherNormSquared;	
}

/* ========================================
	比較演算子(>)
	-------------------------------------
	内容：ベクトル同士の比較(ベクトルの長さ)
	-------------------------------------
	引数：other	比較するベクトル
	-------------------------------------
	戻値：true	自身の方が大きい
		  false	自身の方が小さい
=========================================== */
template<typename T>
inline bool Vector3<T>::operator>(const Vector3<T>& other) const
{
	T thisNormSquared = this->x * this->x + this->y * this->y + this->z * this->z;
	T otherNormSquared = other.x * other.x + other.y * other.y + other.z * other.z;

	return thisNormSquared > otherNormSquared;
}


/* ========================================
	比較演算子(<=)
	-------------------------------------
	内容：ベクトル同士の比較(ベクトルの長さ)
	-------------------------------------
	引数：other	比較するベクトル
	-------------------------------------
	戻値：true	自身の方が小さいか等しい
		  false	自身の方が大きい
=========================================== */
template<typename T>
inline bool Vector3<T>::operator<=(const Vector3<T>& other) const
{
	T thisNormSquared = this->x * this->x + this->y * this->y + this->z * this->z;
	T otherNormSquared = other.x * other.x + other.y * other.y + other.z * other.z;

	return thisNormSquared <= otherNormSquared;
}

/* ========================================
	比較演算子(>=)
	-------------------------------------
	内容：ベクトル同士の比較(ベクトルの長さ)
	-------------------------------------
	引数：other	比較するベクトル
	-------------------------------------
	戻値：true	自身の方が大きいか等しい
		  false	自身の方が小さい
=========================================== */
template<typename T>
inline bool Vector3<T>::operator>=(const Vector3<T>& other) const
{
	T thisNormSquared = this->x * this->x + this->y * this->y + this->z * this->z;
	T otherNormSquared = other.x * other.x + other.y * other.y + other.z * other.z;

	return thisNormSquared >= otherNormSquared;
}

// ======================================== メンバ関数 ========================================

/* ========================================
	ベクトル長さ取得関数
	-------------------------------------
	内容：ベクトルの長さを取得
	-------------------------------------
	戻値：ベクトルの長さ(正確性を求める為、double型で返す)
=========================================== */
template<typename T>
inline double Vector3<T>::Length() const
{
	// ベクトルの大きさは各要素の2乗の和の平方根
	return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);	
}

/* ========================================
	ベクトル長さ(2乗)取得関数
	-------------------------------------
	内容：ベクトルの長さの2乗を取得
	-------------------------------------
	戻値：ベクトルの長さの2乗
=========================================== */
template<typename T>
inline T Vector3<T>::LengthSq() const
{
	// ベクトルの大きさの2乗は各要素の2乗の和
	return this->x * this->x + this->y * this->y + this->z * this->z;	
}

/* ========================================
	ベクトル正規化取得関数
	-------------------------------------
	内容：ベクトルを正規化した値を返す
	-------------------------------------
	戻値：ベクトルの正規化値(長さ1のベクトル)
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::GetNormalize() const
{
	double length = Length();	// ベクトルの長さを取得
	if (length == 0) return Vector3<T>(0, 0, 0);	// 長さが0の場合は正規化できないので0を返す

	// 各要素を長さで割ることで正規化
	return Vector3<T>(this->x / length, this->y / length, this->z / length);
}

/* ========================================
	ベクトル正規化関数
	-------------------------------------
	内容：自ベクトルを正規化する
=========================================== */
template<typename T>
inline void Vector3<T>::Normalize()
{
	double length = Length();	// ベクトルの長さを取得
	if (length == 0)
	{
		*this = Vector3<T>(0, 0, 0);	// 長さが0の場合は正規化できないので0をセット
	}

	// 各要素を長さで割ることで正規化
	*this = Vector3<T>(this->x / length, this->y / length, this->z / length);

	return;
	
}



/* ========================================
	内積取得関数
	-------------------------------------
	内容：ベクトル同士の内積を取得
	-------------------------------------
	引数：other	内積を取得するベクトル
	-------------------------------------
	戻値：内積結果
=========================================== */
template<typename T>
inline T Vector3<T>::Dot(const Vector3<T>& other) const
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

/* ========================================
	外積取得関数
	-------------------------------------
	内容：ベクトル同士の外積を取得
	-------------------------------------
	引数：other	外積を取得するベクトル
	-------------------------------------
	戻値：外積結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& other) const
{
	return Vector3<T>(
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x
	);
}

/* ========================================
	2点間距離取得関数
	-------------------------------------
	内容：2点間の距離を取得
		　三平方の定理を使用
	-------------------------------------
	引数：other	距離を取得するベクトル
	-------------------------------------
	戻値：2点間の距離
=========================================== */
template<typename T>
inline T Vector3<T>::Distance(const Vector3<T>& other) const
{
	return std::sqrt(
		(this->x - other.x) * (this->x - other.x) +
		(this->y - other.y) * (this->y - other.y) +
		(this->z - other.z) * (this->z - other.z)
	);
}


/* ========================================
	2点間距離(2乗)取得関数
	-------------------------------------
	内容：2点間の距離の2乗を取得
		　処理が軽い
	-------------------------------------
	引数：other	距離を取得するベクトル
	-------------------------------------
	戻値：2点間の距離の2乗
=========================================== */
template<typename T>
inline T Vector3<T>::DistanceSq(const Vector3<T>& other) const
{
	return
		(this->x - other.x) * (this->x - other.x) +
		(this->y - other.y) * (this->y - other.y) +
		(this->z - other.z) * (this->z - other.z);
}


/* ========================================
	ベクトル間角度取得関数
	-------------------------------------
	内容：2つのベクトルのなす角度を取得
	-------------------------------------
	引数：other	角度を取得するベクトル
	-------------------------------------
	戻値：2つのベクトルの角度(ラジアン)
=========================================== */
template<typename T>
inline double Vector3<T>::AngleRad(const Vector3<T>& other) const
{
	// |a||b|cosθ = a・b 　→ 　cosθ = a・b / |a||b|
	cos = this.Dot(other) / (this.Length() * other.Length());	// cosθを求める

	return std::acos(cos);	// cosθからθを求める
}

/* ========================================
	ベクトル間角度取得関数(度)
	-------------------------------------
	内容：2つのベクトルのなす角度を取得
	-------------------------------------
	引数：other	角度を取得するベクトル
	-------------------------------------
	戻値：2つのベクトルの角度(度)
=========================================== */
template<typename T>
inline double Vector3<T>::AngleDeg(const Vector3<T>& other) const
{
	// ラジアンから度に変換
	return this.AngleRad(other) * (180 / DirectX::XM_PI);	// ラジアン * 1度あたりのラジアン
}

/* ========================================
	ベクトル間方向取得関数
	-------------------------------------
	内容：自分から見た相手の方向を取得
		　XZ平面の方向を取得
	-------------------------------------
	引数：other	方向を取得するベクトル
	-------------------------------------
	戻値：2つのベクトルの方向(ラジアン)
=========================================== */
template<typename T>
inline double Vector3<T>::DirectionRadXZ(const Vector3<T>& other) const
{
	// ラジアンを求める
	return std::atan2(other.z - this->z, other.x - this->x);

}

/* ========================================
	ベクトル間方向取得関数(度)
	-------------------------------------
	内容：自分から見た相手の方向を取得
		　XZ平面の方向を取得
	-------------------------------------
	引数：other	方向を取得するベクトル
	-------------------------------------
	戻値：2つのベクトルの方向(度)
=========================================== */
template<typename T>
inline double Vector3<T>::DirectionDegXZ(const Vector3<T>& other) const
{
	// ラジアンから度に変換
	return this.DirectionRad(other) * (180 / DirectX::XM_PI);	// ラジアン * 1度あたりのラジアン
}



/* ========================================
	線形補間取得関数
	-------------------------------------
	内容：2つのベクトル間を線形補間
	-------------------------------------
	引数1：other	補間するベクトル
	引数2：t		補間係数(0.0～1.0)
	-------------------------------------
	戻値：補間結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Lerp(const Vector3<T>& other, double t) const
{
	return this + (other - this) * t;	// 開始位置 + (距離 * 係数)
}

/* ========================================
	線形補間取得関数(静的)
	-------------------------------------
	内容：2つのベクトル間を線形補間
	-------------------------------------
	引数1：start	開始ベクトル
	引数2：end		終了ベクトル
	引数3：t		補間係数(0.0～1.0)
	-------------------------------------
	戻値：補間結果
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Lerp(const Vector3<T>& start, const Vector3<T>& end, double t)
{
	return start + (end - start) * t;	// 開始位置 + (距離 * 係数)
}

/* ========================================
	DirectX::XMFLOAT3変換関数
	-------------------------------------
	内容：DirectX::XMFLOAT3に変換
	-------------------------------------
	戻値：変換後のXMFLOAT3
=========================================== */
template<typename T>
inline DirectX::XMFLOAT3 Vector3<T>::ToXMFLOAT3() const
{
	return DirectX::XMFLOAT3((float)this->x, (float)this->y, (float)this->z);
}

/* ========================================
	DirectX::XMFLOAT3代入関数
	-------------------------------------
	内容：DirectX::XMFLOAT3から代入
	-------------------------------------
	引数：other	代入するXMFLOAT3
	-------------------------------------
	戻値：代入後の自身のベクトル
=========================================== */
template<typename T>
inline Vector3<T>& Vector3<T>::operator=(const DirectX::XMFLOAT3& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return *this;
}

/* ========================================
	DirectX行列取得(移動成分)関数
	-------------------------------------
	内容：DirectX行列から移動成分を取得
	-------------------------------------
	引数1：matrix	取得する行列
	-------------------------------------
	戻値：Vector3(移動成分)
=========================================== */
template<typename T>
inline Vector3<float> Vector3<T>::FromMatrix_Translation(const DirectX::XMMATRIX matrix)
{
	// 4行目のXYZ(0️,1,2列目)成分が移動成分
	return Vector3<float>(
		matrix.r[3].m128_f32[0],
		matrix.r[3].m128_f32[1],
		matrix.r[3].m128_f32[2]
	);
}

/* ========================================
	DirectX行列取得(大きさ成分)関数
	-------------------------------------
	内容：DirectX行列から大きさ成分を取得
	-------------------------------------
	引数1：matrix	取得する行列
	-------------------------------------
	戻値：Vector3(大きさ成分)
=========================================== */
template<typename T>
inline Vector3<float> Vector3<T>::FromMatrix_Scale(const DirectX::XMMATRIX matrix)
{
	// 公式(3x3行列の各行の長さ)を使用
	float fXScale = std::sqrtf(
		matrix.r[0].m128_f32[0] * matrix.r[0].m128_f32[0] + 
		matrix.r[0].m128_f32[1] * matrix.r[0].m128_f32[1] + 
		matrix.r[0].m128_f32[2] * matrix.r[0].m128_f32[2]);

	float fYScale = std::sqrtf(
		matrix.r[1].m128_f32[0] * matrix.r[1].m128_f32[0] +
		matrix.r[1].m128_f32[1] * matrix.r[1].m128_f32[1] +
		matrix.r[1].m128_f32[2] * matrix.r[1].m128_f32[2]);

	float fZScale = std::sqrtf(
		matrix.r[2].m128_f32[0] * matrix.r[2].m128_f32[0] +
		matrix.r[2].m128_f32[1] * matrix.r[2].m128_f32[1] +
		matrix.r[2].m128_f32[2] * matrix.r[2].m128_f32[2]);


	return Vector3<float>(fXScale, fYScale, fZScale);
}

/* ========================================
	DirectX行列取得(回転成分)関数
	-------------------------------------
	内容：DirectX行列から回転成分を取得
	-------------------------------------
	引数1：matrix	取得する行列
	-------------------------------------
	戻値：Vector3(回転成分(オイラー))
=========================================== */
template<typename T>
inline Vector3<float> Vector3<T>::FromMatrix_RotationEuler(DirectX::XMMATRIX matrix)
{
    // ピッチの計算
    float pitch = atan2f(matrix.r[1].m128_f32[2], matrix.r[2].m128_f32[2]);

    // ヨーの計算（分母が0でないか確認）
	// ヨーの計算
	float yaw = atan2f(-matrix.r[0].m128_f32[2], matrix.r[1].m128_f32[2]);
	if (yaw < 0) {
		yaw += 2 * MathUtils::PI; // 0から360度の範囲に調整
	}


    // ロールの計算
    float roll = atan2f(matrix.r[0].m128_f32[1], matrix.r[0].m128_f32[0]);

    return Vector3<float>(pitch, yaw, roll);


}

/* ========================================
	DirectXベクトル変換渡し関数
	-------------------------------------
	内容：DirectXベクトルに変換
	-------------------------------------
	戻値：変換後のXMVECTOR
=========================================== */
template<typename T>
inline DirectX::XMVECTOR Vector3<T>::ToXMVECTOR() const
{
	return DirectX::XMVectorSet(this->x, this->y, this->z, 0);
}

/* ========================================
	DirectXベクトル取得関数
	-------------------------------------
	内容：DirectXベクトルからVector3に変換
	-------------------------------------
	引数：vector	変換するベクトル
	-------------------------------------
	戻値：Vector3
=========================================== */
template<typename T>
inline Vector3<float> Vector3<T>::FromXMVECTOR(const DirectX::XMVECTOR& vector)
{
	return Vector3<float>(
		DirectX::XMVectorGetX(vector),
		DirectX::XMVectorGetY(vector),
		DirectX::XMVectorGetZ(vector)
	);
}

/* ========================================
	最大値取得関数
	-------------------------------------
	内容：ベクトルの最大値を取得
	-------------------------------------
	戻値：最大値
=========================================== */
template<typename T>
inline T Vector3<T>::GetMax() const
{
	if (this->x > this->y)
	{
		if (this->x > this->z) return this->x;
		else return this->z;
	}
	else
	{
		if (this->y > this->z) return this->y;
		else return this->z;
	}
}

/* ========================================
	最小値取得関数
	-------------------------------------
	内容：ベクトルの最小値を取得
	-------------------------------------
	戻値：最小値
=========================================== */
template<typename T>
inline T Vector3<T>::GetMin() const
{
	if (this->x < this->y)
	{
		if (this->x < this->z) return this->x;
		else return this->z;
	}
	else
	{
		if (this->y < this->z) return this->y;
		else return this->z;
	}
}




/* ========================================
	ゼロベクトル取得関数
	-------------------------------------
	内容：ゼロベクトルを取得
	-------------------------------------
	戻値：ゼロベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Zero()
{
	return Vector3<T>(0, 0, 0);
}


/* ========================================
	1ベクトル取得関数
	-------------------------------------
	内容：1ベクトルを取得
	-------------------------------------
	戻値：1ベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::One()
{
	return Vector3<T>(1, 1, 1);
}

/* ========================================
	上ベクトル取得関数
	-------------------------------------
	内容：上ベクトルを取得
	-------------------------------------
	戻値：上ベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Up()
{
	return Vector3<T>(0, 1, 0);
}

/* ========================================
	下ベクトル取得関数
	-------------------------------------
	内容：下ベクトルを取得
	-------------------------------------
	戻値：下ベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Down()
{
	return Vector3<T>(0, -1, 0);
}

/* ========================================
	右ベクトル取得関数
	-------------------------------------
	内容：右ベクトルを取得
	-------------------------------------
	戻値：右ベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Right()
{
	return Vector3<T>(1, 0, 0);
}

/* ========================================
	左ベクトル取得関数
	-------------------------------------
	内容：左ベクトルを取得
	-------------------------------------
	戻値：左ベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Left()
{
	return Vector3<T>(-1, 0, 0);
}

/* ========================================
	前ベクトル取得関数
	-------------------------------------
	内容：前ベクトルを取得
	-------------------------------------
	戻値：前ベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Forward()
{
	return Vector3<T>(0, 0, 1);
}

/* ========================================
	後ベクトル取得関数
	-------------------------------------
	内容：後ベクトルを取得
	-------------------------------------
	戻値：後ベクトル
=========================================== */
template<typename T>
inline Vector3<T> Vector3<T>::Back()
{
	return Vector3<T>(0, 0, -1);
}



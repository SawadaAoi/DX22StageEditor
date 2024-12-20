/* ========================================
	DX22Base/
	------------------------------------
	クォータニオン用cpp
	------------------------------------
	Quaternion.cpp
========================================== */

// =============== インクルード ===================
#include "Quaternion.h"
#include <math.h>		// std::cosf, std::sinf, std::sqrtf
#include "MathUtils.h"	// MathUtils::ToRadian, MathUtils::ToDegree
#include <algorithm>	// std::clamp

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
=========================================== */
Quaternion::Quaternion()
	: m_fAngle(1.0f)							// 回転角度は1.0f(=cos(0))で初期化
	, m_fAxis(Vector3<float>(0.0f, 0.0f, 0.0f))	// 回転軸は0で初期化
{
}

/* ========================================
	コンストラクタ関数
	-------------------------------------
	内容：初期化
	-------------------------------------
	引数1：float fAngle			回転角度
	引数2：Vector3<float> fAxis	回転軸
=========================================== */
Quaternion::Quaternion(Vector3<float> fAxis, float fAngle)
	: m_fAxis(fAxis)
	, m_fAngle(fAngle)
{
}


/* ========================================
	クォータニオン作成関数
	-------------------------------------
	内容：軸と角度からクォータニオンを作成
	-------------------------------------
	引数1：Vector3<float> fAxis	回転軸
	引数2：float fAngle			回転角度
	-------------------------------------
	戻り値：Quaternion	クォータニオン
=========================================== */
Quaternion Quaternion::FromAxisAngle(Vector3<float> fAxis, float fAngle)
{
	// 軸ベクトルを正規化（ゼロベクトルに注意）
	Vector3<float> fAxisNormalized = fAxis;
	if (fAxis.LengthSq() > 0.0f) {
		fAxisNormalized = fAxis.GetNormalize();
	}

	Quaternion q;
	float fHalfAngle = fAngle * 0.5f;
	float sinHalfAngle = std::sinf(fHalfAngle);

	q.m_fAngle = std::cosf(fHalfAngle);
	q.m_fAxis.x = fAxisNormalized.x * sinHalfAngle;
	q.m_fAxis.y = fAxisNormalized.y * sinHalfAngle;
	q.m_fAxis.z = fAxisNormalized.z * sinHalfAngle;

	return q;
}

/* ========================================
	クォータニオン作成関数(正規化)
	-------------------------------------
	内容：軸を正規化してからクォータニオンを作成
	-------------------------------------
	引数1：Vector3<float> fAxis	回転軸
	引数2：float fAngle			回転角度
	-------------------------------------
	戻り値：Quaternion	クォータニオン
=========================================== */
Quaternion Quaternion::FromAxisAngleNormalized(Vector3<float> fAxis, float fAngle)
{
	fAxis = fAxis.GetNormalize();			// 軸ベクトルを正規化
	return FromAxisAngle(fAxis, fAngle);	// クォータニオンを作成
}

/* ========================================
	クォータニオン作成関数(正規化)
	-------------------------------------
	内容：軸と角度からクォータニオンを作成(自身を変更)
	-------------------------------------
	引数1：Vector3<float> fAxis	回転軸
	引数2：float fAngle			回転角度
=========================================== */
void Quaternion::SetFromAxisAngle(Vector3<float> fAxis, float fAngle)
{
	*this =  FromAxisAngle(fAxis, fAngle);
}

/* ========================================
	クォータニオン作成関数(正規化)
	-------------------------------------
	内容：軸と角度からクォータニオンを作成(自身を変更)
	-------------------------------------
	引数1：Vector3<float> fAxis	回転軸
	引数2：float fAngle			回転角度
=========================================== */
void Quaternion::SetFromAxisAngleNormalized(Vector3<float> fAxis, float fAngle)
{
	*this = FromAxisAngleNormalized(fAxis, fAngle);
}

/* ========================================
	正規化関数
	-------------------------------------
	内容：クォータニオンを正規化する
=========================================== */
void Quaternion::Normalize()
{
	float fLength = std::sqrtf(m_fAngle * m_fAngle + m_fAxis.LengthSq());	// 長さを計算

	// 長さが0以下の場合は正規化しない(0除算を避ける)
	if (fLength <= 0.0f)	return;

	m_fAngle /= fLength;	// 実数部を長さで割る
	m_fAxis /= fLength;		// 虚数部を長さで割る
}

/* ========================================
	正規化関数(コピーを返す)
	-------------------------------------
	内容：クォータニオンを正規化する(コピーを返す)
	-------------------------------------
	戻り値：Quaternion	正規化されたクォータニオン
=========================================== */
Quaternion Quaternion::GetNormalized()
{
	Quaternion q = *this;
	q.Normalize();
	return q;
}

/* ========================================
	回転加算関数
	-------------------------------------
	内容：指定された軸と角度で回転する
	-------------------------------------
	引数1：float fAngle			回転角度
	引数2：Vector3<float> fAxis	回転軸
=========================================== */
void Quaternion::Rotate(Vector3<float> fAxis, float fAngle)
{
	Quaternion qChange = FromAxisAngleNormalized(fAxis, fAngle);	// 回転クォータニオンを作成

	this->Normalize();		// 自身を正規化
	qChange.Normalize();	// 回転クォータニオンを正規化

	*this = (*this) * qChange;	// 自身に回転クォータニオンを掛ける

}

/* ========================================
	ベクトル回転関数
	-------------------------------------
	内容：引数のベクトルを自身のクォータニオンで回転する
	-------------------------------------
	引数1：Vector3<float> fVec	回転させるベクトル
=========================================== */
Vector3<float> Quaternion::Rotate(Vector3<float> fVec)
{
	Quaternion qVec = Quaternion(fVec, 0.0f);   // ベクトルをクォータニオンに変換
	Quaternion qConjugate = this->Conjugate();   // 共役を取得

	// 回転方向を反時計周りにするために、共役*ベクトル*元のクォータニオンの順で掛ける
	Quaternion qResult = qConjugate * qVec * (*this);  
	//qResult.Normalize();  // 正規化

	return qResult.m_fAxis;  // 回転後のベクトルを返す
}



/* ========================================
	球面線形補間関数
	-------------------------------------
	内容：2つのクォータニオン間を球面線形補間する
	-------------------------------------
	引数1：const Quaternion& q1		クォータニオン1
	引数2：const Quaternion& q2		クォータニオン2
	引数3：float t					補間係数(0.0～1.0)
	-------------------------------------
	戻り値：Quaternion	補間結果のクォータニオン
=========================================== */
Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	if (q1 == q2)	return q1;	// 同じクォータニオンの場合はそのまま返す

	// 内積を計算
	float dot = q1.m_fAngle * q2.m_fAngle + q1.m_fAxis.Dot(q2.m_fAxis);
	const float THRESHOLD = 0.9995f;	// しきい値(線形補完へ切り替える)

	// ほぼ同じ向きの場合は線形補間
	if (dot > THRESHOLD) 
	{
		Quaternion result = q1 + ((q2 - q1) * t);	// 線形補間
		return result.GetNormalized();
	}

	dot = std::clamp(dot, -1.0f, 1.0f);	// -1.0～1.0の範囲に収める(誤差対策)

	float theta_0 = std::acosf(dot);	// クォータニオン間の角度
	float theta = theta_0 * t;			// 線形補間後の角度(変化していく値)

	// 補間後のクォータニオンを計算
	Quaternion q2_ = q2 - q1 * dot;	// q1を引いて内積を取り除く
	q2_ = q2_.GetNormalized();		// 正規化


	return (q1 * std::cosf(theta)) + (q2_ * std::sinf(theta));

}

/* ========================================
	線形補間関数
	-------------------------------------
	内容：2つのクォータニオン間を線形補間する
	-------------------------------------
	引数1：const Quaternion& q1		クォータニオン1
	引数2：const Quaternion& q2		クォータニオン2
	引数3：float t					補間係数(0.0～1.0)
	-------------------------------------
	戻り値：Quaternion	補間結果のクォータニオン
=========================================== */
Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	Quaternion result = q1 + ((q2 - q1) * t);	// 線形補間
	return result.GetNormalized();				// 正規化して返す
}

/* ========================================
	球面線形補間関数
	-------------------------------------
	内容：2つのクォータニオン間を球面線形補間する
	-------------------------------------
	引数1：const Quaternion& target	補間するクォータニオン
	引数2：float t					補間係数(0.0～1.0)
=========================================== */
void Quaternion::Slerp(const Quaternion& target, float t)
{
	*this = Slerp(*this, target, t);
}

/* ========================================
	線形補間関数
	-------------------------------------
	内容：2つのクォータニオン間を線形補間する
	-------------------------------------
	引数1：const Quaternion& target	補間するクォータニオン
	引数2：float t					補間係数(0.0～1.0)
=========================================== */
void Quaternion::Lerp(const Quaternion& target, float t)
{
	*this = Lerp(*this, target, t);
}


/* ========================================
	共役関数
	-------------------------------------
	内容：共役を取得する(回転軸の符号を反転)
	-------------------------------------
	戻り値：Quaternion	共役クォータニオン
=========================================== */
Quaternion Quaternion::Conjugate() const
{
	Vector3<float> fConjugateAxis = m_fAxis * -1.0f;	// 回転軸の符号を反転
	return Quaternion(fConjugateAxis, m_fAngle);		// 回転角度はそのまま、回転軸の符号を反転
}

/* ========================================
	逆クォータニオン関数
	-------------------------------------
	内容：逆クォータニオンを取得する
	-------------------------------------
	戻り値：Quaternion	逆クォータニオン
=========================================== */
Quaternion Quaternion::Inverse() const
{
	Quaternion qConjugate = Conjugate();	// 共役を取得
	float fLengthSq = m_fAngle * m_fAngle + m_fAxis.LengthSq();	// 長さの2乗

	if (fLengthSq <= 0.0f)	return Quaternion();	// 長さが0以下の場合は逆クォータニオンは存在しない

	// 共役 / 長さの2乗
	return Quaternion(qConjugate.m_fAxis / fLengthSq, qConjugate.m_fAngle / fLengthSq);
}

/* ========================================
	内積関数
	--------------------------------
	内容：クォータニオン同士の内積を取得する
	--------------------------------
	引数1：const Quaternion& other	内積を取るクォータニオン
	--------------------------------
	戻り値：float	内積
=========================================== */
float Quaternion::Dot(const Quaternion& other) const
{
	return m_fAngle * other.m_fAngle + m_fAxis.Dot(other.m_fAxis);
}

/* ========================================
	前方ベクトル取得関数
	-------------------------------------
	内容：クォータニオンから前方ベクトルを取得する
	-------------------------------------
	戻り値：Vector3<float>	前方ベクトル
=========================================== */
Vector3<float> Quaternion::GetForwardVector() const
{
	DirectX::XMMATRIX matRot = ToDirectXMatrix();	// 回転行列を取得
	DirectX::XMVECTOR vecForward = Vector3<float>::Forward().ToXMVECTOR();	// 前方ベクトルを取得

	// 回転行列を掛けて前方ベクトルを取得(単位ベクトルを回転させる)
	DirectX::XMVECTOR vecRotated = DirectX::XMVector3Transform(vecForward, matRot);

	return Vector3<float>(Vector3<float>::FromXMVECTOR(vecRotated));	// Vector3に変換して返す
}

/* ========================================
	右方ベクトル取得関数
	-------------------------------------
	内容：クォータニオンから右方ベクトルを取得する
	-------------------------------------
	戻り値：Vector3<float>	右方ベクトル
=========================================== */
Vector3<float> Quaternion::GetRightVector() const
{
	DirectX::XMMATRIX matRot = ToDirectXMatrix();	// 回転行列を取得
	DirectX::XMVECTOR vecRight = Vector3<float>::Right().ToXMVECTOR();	// 右方ベクトルを取得

	// 回転行列を掛けて右方ベクトルを取得(単位ベクトルを回転させる)
	DirectX::XMVECTOR vecRotated = DirectX::XMVector3Transform(vecRight, matRot);

	return Vector3<float>(Vector3<float>::FromXMVECTOR(vecRotated));	// Vector3に変換して返す
}

/* ========================================
	上方ベクトル取得関数
	-------------------------------------
	内容：クォータニオンから上方ベクトルを取得する
	-------------------------------------
	戻り値：Vector3<float>	上方ベクトル
=========================================== */
Vector3<float> Quaternion::GetUpVector() const
{
	DirectX::XMMATRIX matRot = ToDirectXMatrix();	// 回転行列を取得
	DirectX::XMVECTOR vecUp = Vector3<float>::Up().ToXMVECTOR();	// 上方ベクトルを取得

	// 回転行列を掛けて上方ベクトルを取得(単位ベクトルを回転させる)
	DirectX::XMVECTOR vecRotated = DirectX::XMVector3Transform(vecUp, matRot);

	return Vector3<float>(Vector3<float>::FromXMVECTOR(vecRotated));	// Vector3に変換して返す
}

/* ========================================
	DirectX行列変換渡し関数
	-------------------------------------
	内容：クォータニオンをDirectXの行列に変換する
	-------------------------------------
	戻り値：DirectX::XMMATRIX	DirectXの行列
=========================================== */
DirectX::XMMATRIX Quaternion::ToDirectXMatrix() const
{
	DirectX::XMFLOAT4 float4 = ToDirectXFloat4();	// 一時変数に保存(XMLoadFloat4で参照エラーが出るため)
	return DirectX::XMMatrixRotationQuaternion(		// クォータニオン(XMVECTOR)から回転行列を作成
		DirectX::XMLoadFloat4(&float4)	// XMFLOAT4→XMVECTOR
	);
}

/* ========================================
	DirectX::XMFLOAT4変換渡し関数
	-------------------------------------
	内容：クォータニオンをDirectX::XMFLOAT4に変換する
	-------------------------------------
	戻り値：DirectX::XMFLOAT4	DirectX::XMFLOAT4
=========================================== */
DirectX::XMFLOAT4 Quaternion::ToDirectXFloat4() const
{
	return DirectX::XMFLOAT4(m_fAxis.x, m_fAxis.y, m_fAxis.z, m_fAngle);
}

/* ========================================
	DirectX::XMVECTOR変換渡し関数
	-------------------------------------
	内容：クォータニオンをDirectX::XMVECTORに変換する
	-------------------------------------
	戻り値：DirectX::XMVECTOR	DirectX::XMVECTOR
=========================================== */
DirectX::XMVECTOR Quaternion::ToXMVECTOR() const
{
	DirectX::XMFLOAT4 float4 = ToDirectXFloat4();	// 一時変数に保存(XMLoadFloat4で参照エラーが出���ため)
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat4(&float4);	// XMFLOAT4→XMVECTOR

	return vec;
}

/* ========================================
	オイラー角変換渡し関数
	-------------------------------------
	内容：クォータニオンをオイラー角に変換する
	-------------------------------------
	戻り値：Vector3<float>	オイラー角
=========================================== */
Vector3<float> Quaternion::ToEulerAngle() const
{
	float fYaw, fPitch, fRoll;

	// ロール
	float fSinRoll = 2.0f * (m_fAngle * m_fAxis.x + m_fAxis.y * m_fAxis.z);
	float fCosRoll = 1.0f - 2.0f * (m_fAxis.x * m_fAxis.x + m_fAxis.y * m_fAxis.y);
	fRoll = std::atan2f(fSinRoll, fCosRoll);

	// ピッチ
	float fSinPitch = 2.0f * (m_fAngle * m_fAxis.y - m_fAxis.z * m_fAxis.x);

	// 絶対値が1以上の場合は90度の場合
	if (std::abs(fSinPitch) >= 1.0f)
	{
		// ピッチが90度の場合
		fPitch = std::copysignf(MathUtils::PI / 2.0f, fSinPitch);	// copysignf：符号をコピー
	}
	else
	{
		fPitch = std::asinf(fSinPitch);	// 通常の場合
	}

	// ヨー
	float fSinYaw = 2.0f * (m_fAngle * m_fAxis.z + m_fAxis.x * m_fAxis.y);
	float fCosYaw = 1.0f - 2.0f * (m_fAxis.y * m_fAxis.y + m_fAxis.z * m_fAxis.z);
	fYaw = std::atan2f(fSinYaw, fCosYaw);

	return Vector3<float>(fRoll, fPitch, fYaw);
}

/* ========================================
	DirectX行列変換取得関数
	-------------------------------------
	内容：行列からクォータニオンに変換する
	-------------------------------------
	引数1：DirectX::XMMATRIX matrix	行列
	-------------------------------------
	戻り値：Quaternion	クォータニオン
=========================================== */
Quaternion Quaternion::FromDirectXMatrix(DirectX::XMMATRIX matrix)
{
	// 行列からクォータニオン(DirectX上)に変換
	DirectX::XMVECTOR directXQuat = DirectX::XMQuaternionRotationMatrix(matrix);	

	return Quaternion(
		Vector3<float>(
			DirectX::XMVectorGetX(directXQuat),
			DirectX::XMVectorGetY(directXQuat),
			DirectX::XMVectorGetZ(directXQuat)),
		DirectX::XMVectorGetW(directXQuat)
	).GetNormalized();	// 正規化して返す

}

/* ========================================
	オイラー角変換取得関数
	-------------------------------------
	内容：オイラー角からクォータニオンに変換する
	-------------------------------------
	引数1：Vector3<float> fEulerAngle	オイラー角
	-------------------------------------
	戻り値：クォータニオンに変換した値
=========================================== */
Quaternion Quaternion::FromEulerAngle(Vector3<float> fEulerAngle)
{
	Quaternion qRoll	= Quaternion::FromAxisAngleNormalized(Vector3<float>::Forward(), fEulerAngle.z);	// ロール回転
	Quaternion qPitch	= Quaternion::FromAxisAngleNormalized(Vector3<float>::Right(), fEulerAngle.x);		// ピッチ回転
	Quaternion qYaw		= Quaternion::FromAxisAngleNormalized(Vector3<float>::Up(), fEulerAngle.y);			// ヨー回転

	Quaternion reQ = qYaw * qPitch * qRoll;// ヨー、ピッチ、ロールの順で掛ける

	return reQ;
}

/* ========================================
	XMVECTOR変換取得関数
	-------------------------------------
	内容：XMVECTORからクォータニオンに変換する
	-------------------------------------
	引数1：DirectX::XMVECTOR vec	変換するXMVECTOR
	-------------------------------------
	戻り値：Quaternion	クォータニオン
=========================================== */
Quaternion Quaternion::FromXMVECTOR(DirectX::XMVECTOR vec)
{
	DirectX::XMFLOAT4 float4;
	DirectX::XMStoreFloat4(&float4, vec);	// XMVECTOR→XMFLOAT4

	return Quaternion({ float4.x, float4.y, float4.z }, float4.w);	// XMFLOAT4からクォータニオンに変換して返す
}

/* ========================================
	加算演算子オーバーロード
	-------------------------------------
	内容：クォータニオン同士の加算
	-------------------------------------
	引数1：const Quaternion& other	加算するクォータニオン
	-------------------------------------
	戻り値：Quaternion	加算結果のクォータニオン
=========================================== */
Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return Quaternion(
		m_fAxis + other.m_fAxis,
		m_fAngle + other.m_fAngle
	);
}

/* ========================================
	減算演算子オーバーロード
	-------------------------------------
	内容：クォータニオン同士の減算
	-------------------------------------
	引数1：const Quaternion& other	減算するクォータニオン
	-------------------------------------
	戻り値：Quaternion	減算結果のクォータニオン
=========================================== */
Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return Quaternion(
		m_fAxis - other.m_fAxis,
		m_fAngle - other.m_fAngle

	);
}

/* ========================================
	乗算演算子オーバーロード
	-------------------------------------
	内容：クォータニオン同士の乗算
	-------------------------------------
	引数1：const Quaternion& other	乗算するクォータニオン
	-------------------------------------
	戻り値：Quaternion	乗算結果のクォータニオン
=========================================== */
Quaternion Quaternion::operator*(const Quaternion& other) const
{
	// 実数部の計算(w) = θ1θ2 - x1x2 - y1y2 - z1z2
	float realPart = (m_fAngle * other.m_fAngle) - (this->m_fAxis.x * other.m_fAxis.x) - 
		(this->m_fAxis.y * other.m_fAxis.y) - (this->m_fAxis.z * other.m_fAxis.z);
	
	// 虚数部の計算
	// x = θ1x2 + x1θ2 + y1z2 - z1y2
	float xComponent = (other.m_fAngle * this->m_fAxis.x) + (other.m_fAxis.x * this->m_fAngle) +
		(other.m_fAxis.y * this->m_fAxis.z) - (other.m_fAxis.z * this->m_fAxis.y);
	// y = θ1y2 - x1z2 + y1θ2 + z1x2
	float yComponent = (other.m_fAngle * this->m_fAxis.y) - (other.m_fAxis.x * this->m_fAxis.z) +
		(other.m_fAxis.y * this->m_fAngle) + (other.m_fAxis.z * this->m_fAxis.x);
	// z = θ1z2 + x1y2 - y1x2 + z1θ2
	float zComponent = (other.m_fAngle * this->m_fAxis.z) + (other.m_fAxis.x * this->m_fAxis.y) -
		(other.m_fAxis.y * this->m_fAxis.x) + (other.m_fAxis.z * this->m_fAngle);

	return Quaternion({ xComponent, yComponent, zComponent }, realPart);
}

/* ========================================
	スカラー乗算演算子オーバーロード
	-------------------------------------
	内容：クォータニオンとスカラーの乗算
	-------------------------------------
	引数1：float scalar	乗算するスカラー
	-------------------------------------
	戻り値：Quaternion	乗算結果のクォータニオン
=========================================== */
Quaternion Quaternion::operator*(float scalar) const
{
	return Quaternion(
		m_fAxis * scalar, 
		m_fAngle * scalar

	);
}

/* ========================================
	代入乗算演算子オーバーロード
	-------------------------------------
	内容：クォータニオン同士の乗算(代入)
	-------------------------------------
	引数1：const Quaternion& other	乗算するクォータニオン
	-------------------------------------
	戻り値：Quaternion&	乗算結果のクォータニオン
=========================================== */
Quaternion& Quaternion::operator*=(const Quaternion& other) 
{
	*this = (*this) * other;
	return *this;
}

/* ========================================
	等価演算子オーバーロード
	-------------------------------------
	内容：クォータニオン同士の等価比較
	-------------------------------------
	引数1：const Quaternion& other	比較するクォータニオン
	-------------------------------------
	戻り値：bool	等しい場合true
=========================================== */
bool Quaternion::operator==(const Quaternion& other) const
{
	return (m_fAngle == other.m_fAngle) && (m_fAxis == other.m_fAxis);
}

/* ========================================
	非等価演算子オーバーロード
	-------------------------------------
	内容：クォータニオン同士の非等価比較
	-------------------------------------
	引数1：const Quaternion& other	比較するクォータニオン
	-------------------------------------
	戻り値：bool	異なる場合true
=========================================== */
bool Quaternion::operator!=(const Quaternion& other) const
{
	return !(*this == other);
}

/* ========================================
	単項プラス演算子オーバーロード
	-------------------------------------
	内容：クォータニオンの符号をそのまま返す
	-------------------------------------
	戻り値：Quaternion	そのままのクォータニオン
=========================================== */
Quaternion Quaternion::operator+() const
{
	return *this;
}

/* ========================================
	単項マイナス演算子オーバーロード
	-------------------------------------
	内容：クォータニオンの符号を反転
	-------------------------------------
	戻り値：Quaternion	符号を反転したクォータニオン
=========================================== */
Quaternion Quaternion::operator-() const
{
	return Quaternion(-m_fAxis, -m_fAngle);
}

/* ========================================
	XYZ軸回転角度取得関数
	-------------------------------------
	内容：ワールド座標におけるXYZ軸の回転角度を取得する
	-------------------------------------
	戻り値：Vector3<float>	回転角度
=========================================== */
Vector3<float> Quaternion::GetAngleWorldXYZ() const
{
	// 回転行列を取得
	DirectX::XMMATRIX matRot = ToDirectXMatrix();

	return Vector3<float>::FromMatrix_RotationEuler(matRot);
}



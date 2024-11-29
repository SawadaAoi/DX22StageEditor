/* ========================================
	DX22Base/
	------------------------------------
	クォータニオン用ヘッダ
	------------------------------------
	説明：3D空間における回転を表現するためのクラスです。
		　回転軸と回転角度を基にクォータニオンを生成し、
		　回転を行うためのメソッドを提供します
	------------------------------------
	Quaternion.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Vector3.h"		// Vector3<float>
#include <DirectXMath.h>	// DirectX::XMFLOAT4, DirectX::XMMATRIX



// =============== クラス定義 =====================
class Quaternion
{
public:
	// コンストラクタ
	Quaternion();
	Quaternion(Vector3<float> fAxis, float fAngle);	// 虚数と実数から初期化

	// 軸と角度からクォータニオンを作成
	static Quaternion FromAxisAngle(Vector3<float> fAxis, float fAngle);			// 非正規化
	static Quaternion FromAxisAngleNormalized(Vector3<float> fAxis, float fAngle);	// 正規化
	void SetFromAxisAngle(Vector3<float> fAxis, float fAngle);				// 自身を変更(非正規化)
	void SetFromAxisAngleNormalized(Vector3<float> fAxis, float fAngle);	// 自身を変更(正規化)

	// 正規化関連メソッド
	void Normalize();			// 正規化
	Quaternion GetNormalized();	// 正規化(コピーを返す)

	// 回転
	void Rotate(Vector3<float> fAxis, float fAngle);	// 軸と角度から回転
	Vector3<float> Rotate(Vector3<float> fVec);		// ベクトルを回転

	// クォータニオンの補完
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);	// 球面線形補間
	static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);	// 線形補間
	void Slerp(const Quaternion& target, float t);	// 球面線形補間(自身を変更
	void Lerp(const Quaternion& target, float t);	// 線形補間(自身を変更)

	// クォータニオン関連メソッド
	Quaternion Conjugate() const;	// 共役
	Quaternion Inverse() const;		// 逆クォータニオン

	Vector3<float> GetForwardVector() const;	// 前方ベクトル取得
	Vector3<float> GetRightVector() const;		// 右方ベクトル取得
	Vector3<float> GetUpVector() const;			// 上方ベクトル取得

	// 変換関数
	DirectX::XMMATRIX ToDirectXMatrix() const;			// DirectXの行列に変換
	DirectX::XMFLOAT4 ToDirectXFloat4() const;			// DirectXのXMFLOAT4に変換
	DirectX::XMVECTOR ToXMVECTOR() const;				// DirectXのXMVECTORに変換
	Vector3<float>	  ToEulerAngle() const;				// オイラー角に変換
	static Quaternion FromDirectXMatrix(DirectX::XMMATRIX matrix);	// 行列からクォータニオンに変換
	static Quaternion FromEulerAngle(Vector3<float> fEulerAngle);	// オイラー角からクォータニオンに変換
	static Quaternion FromXMVECTOR(DirectX::XMVECTOR vec);			// XMVECTORからクォータニオンに変換

	// 演算子オーバーロード
	Quaternion operator+(const Quaternion& other) const;	// 加算
	Quaternion operator-(const Quaternion& other) const;	// 減算
	Quaternion operator*(const Quaternion& other) const;	// 乗算
	Quaternion operator*(float scalar) const;				// スカラー乗算
	Quaternion& operator*=(const Quaternion& other) ;		// 代入乗算
	bool operator==(const Quaternion& other) const;			// 等価
	bool operator!=(const Quaternion& other) const;			// 非等価


	// 指定した軸方向の角度を取得
	Vector3<float> GetAngleWorldXYZ() const;	// ワールド座標系での角度取得



public:
	Vector3<float> m_fAxis;	// 回転軸(虚数部)	※ただのベクトルが格納されているわけではない
	float m_fAngle;			// 回転角度(実数部)	※ただの回転角度が格納されているわけではない
};


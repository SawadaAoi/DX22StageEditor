/* ========================================
	DX22Base/
	------------------------------------
	線描画用ヘッダ
	------------------------------------
	説明：二つの点を結ぶ線を描画する
		　ShapeBaseを継承していないが、
		  似た機能を持つ為にShapeを名前に含めている
	------------------------------------
	ShapeLine.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Vector3.h"
#include "MeshBuffer.h"		// メッシュバッファ
#include "VertexShader.h"	// 頂点シェーダ
#include "PixelShader.h"	// ピクセルシェーダ
#include "Quaternion.h"


// =============== クラスの定義 ===================
class ShapeLine
{
public:
	// 線の頂点構造体
	struct T_LineVertex
	{
		float pos[3];	// 座標
		float color[4];	// 線の色
	};
public:
	ShapeLine();
	ShapeLine(int DrawNum);
	ShapeLine(Vector3<float> start, Vector3<float> end,
		Vector3<float> color = Vector3<float>(1.0f, 1.0f, 1.0f));
	~ShapeLine();
	ShapeLine(const ShapeLine& other);
	void Draw();

	void AddLine(Vector3<float> start, Vector3<float> end,
		Vector3<float> color = Vector3<float>(1.0f, 1.0f, 1.0f));

	void UpdateLine(int nLineNum, Vector3<float> start, Vector3<float> end,
		Vector3<float> color = Vector3<float>(1.0f, 1.0f, 1.0f));

	// セッター
	void SetPos(Vector3<float> pos);
	void SetRotation(Quaternion rot);
	void SetScale(Vector3<float> scale);

private:
	void MakeLine();
	void SetWVP();

	MeshBuffer*		m_pMeshBuffer;	// メッシュバッファ
	void*			m_pVertex;		// 頂点データ
	PixelShader*	m_pPS;			// ピクセルシェーダ
	VertexShader*	m_pVS;			// 頂点シェーダ

	int				m_nLineCnt;		// 線の数
	int				m_nMaxLineCnt;	// 最大線数

	Vector3<float>	m_vPos;			// 座標
	Quaternion		m_qRotation;		// 回転
	Vector3<float>	m_vScale;		// スケール

	DirectX::XMFLOAT4X4 m_WVP[3];	// ワールド、ビュー、プロジェクション行列


};


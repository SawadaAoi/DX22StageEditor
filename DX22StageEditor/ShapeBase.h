/* ========================================
	DX22Base/
	------------------------------------
	図形描画用ヘッダ
	------------------------------------
	説明：球、立方体、円柱クラスの基底クラス
	------------------------------------
	Shape.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "MeshBuffer.h"		// メッシュバッファ
#include "VertexShader.h"	// 頂点シェーダ
#include "PixelShader.h"	// ピクセルシェーダ
#include <DirectXMath.h>	// DirectXMath
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "ObjectLight.h"

// =============== クラスの定義 ===================
class ShapeBase
{
public:
	// 頂点構造体
	struct T_Vertex
	{
		Vector3<float> fPos;	// 座標
		Vector3<float> fNormal;	// 法線
		Vector2<float> fUv;		// テクスチャ座標
	};

	// 表示モード
	enum E_DrawMode
	{
		DRAW_NORMAL,			// 通常
		DRAW_WIRE_FRAME,		// ワイヤーフレーム
		DRAW_MAX
	};

public:
	ShapeBase();
	~ShapeBase();
	virtual void Draw();
	virtual void MakeMesh() = 0;	// 図形作成
 

	// ゲッター
	Vector3<float>	GetPosition();
	Vector3<float>	GetScale();
	Quaternion		GetRotation();

	Vector3<float>	GetColor();
	E_DrawMode		GetDrawMode();

	bool GetIsTexture();
	bool GetIsCulling();
	Vector2<float> GetUvScale(int nIndex = 0);
	Vector2<float> GetUvOffset(int nIndex = 0);
	

	// セッター
	void SetPosition(Vector3<float> fPos);
	void SetScale(Vector3<float> fSize);
	void SetRotation(Quaternion qRot);

	void SetColor(Vector3<float> fColor);
	void SetDrawMode(E_DrawMode eMode);

	void SetTexture(Texture* pTexture, int nIndex = 0);
	void SetUseTexture(int bIsTex);

	void SetLightMaterial(float fDiffuse, float fSpecular, float fAmbient, bool bUseLight);		// ライトパラメータ設定(ピクセルシェーダー用)
	void SetCameraPos(Vector3<float> fCameraPos);												// カメラ位置設定(ピクセルシェーダー用)
	void SetLights(std::vector<ObjectLight*> lights);											// ライト設定(ピクセルシェーダー用)

	void SetIsCulling(bool bCulling);	// カリング設定

	void SetUvScale(Vector2<float> fScale, int nIndex = 0);
	void SetUvOffset(Vector2<float> fOffset, int nIndex = 0);

protected:
	void SetWVPMatrix();

protected:
	MeshBuffer*		m_pMeshBuffer[DRAW_MAX];	// メッシュバッファ(頂点、インデックス)
	E_DrawMode		m_eDrawMode;				// 表示モード

	VertexShader*	m_pVS;			// 頂点シェーダ
	PixelShader*	m_pPS;			// ピクセルシェーダ

	Vector3<float>	m_vPosition;	// 座標
	Quaternion		m_qRotation;	// 回転(クォータニオン)
	Vector3<float>	m_vScale;		// サイズ


	std::vector<Texture*>	m_pTextures;	// テクスチャ配列
	int						m_bIsTex;		// テクスチャ使用フラグ
	bool					m_bIsCulling;	// カリングフラグ(カリングするかどうか)

	std::vector<Vector2<float>>	m_fUvScale;		// テクスチャ座標スケール
	std::vector<Vector2<float>>	m_fUvOffset;	// テクスチャ座標オフセット
		
	Vector3<float>			 m_fColor;		// 色	

	DirectX::XMFLOAT4X4		m_WVP[3];		// ワールド、ビュー、プロジェクション行列

};



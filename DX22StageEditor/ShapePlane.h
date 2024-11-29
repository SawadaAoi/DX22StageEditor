/* ========================================
	DX22Base/
	------------------------------------
	図形描画(板ポリゴン)用ヘッダ
	------------------------------------
	説明：板ポリゴンを描画するクラス
	------------------------------------
	ShapePlane.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ShapeBase.h"

// =============== クラス定義 ===================
class ShapePlane :
    public ShapeBase
{
public:
	ShapePlane();
	~ShapePlane() {};

	void MakeMesh() override;
	
private:
	void MakeMeshNormal();
	void MakeMeshWire();
};


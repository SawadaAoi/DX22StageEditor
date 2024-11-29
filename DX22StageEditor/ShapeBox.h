/* ========================================
	DX22Base/
	------------------------------------
	図形描画(立方体)用ヘッダ
	------------------------------------
	説明：立方体を描画するクラス
	------------------------------------
	ShapeBox.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ShapeBase.h"

// =============== クラスの定義 ===================
class ShapeBox :
    public ShapeBase
{
public:
	ShapeBox();
	ShapeBox(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode);
	~ShapeBox();
	void MakeMesh() override;

private:
	void MakeMeshNormal();
	void MakeMeshWire();
};


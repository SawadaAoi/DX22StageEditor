/* ========================================
	DX22Base/
	------------------------------------
	図形描画(球)用ヘッダ
	------------------------------------
	説明：球を描画するクラス
	------------------------------------
	ShapeSphere.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ShapeBase.h"
#include <vector>

class ShapeSphere :
    public ShapeBase
{
public:
	ShapeSphere();
	ShapeSphere(Vector3<float> fPos, Vector3<float> fSize, Vector3<float> fColor, E_DrawMode eMode);
	~ShapeSphere();
	void MakeMesh() override;
private:
	void MakeMeshNormal();
	void MakeMeshWire();

	// 頂点データ作成
	std::vector<ShapeBase::T_Vertex> CreateSphereVertices(int latitudeCount, int longitudeCount);
	// インデックスデータ作成
	std::vector<int> CreateSphereIndices(int latitudeCount, int longitudeCount);
};


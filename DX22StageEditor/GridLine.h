/* ========================================
	DX22Base/
	------------------------------------
	グリッド線描画用ヘッダ
	------------------------------------
	説明：デバッグ用にXZ方向のグリッド線
		　XYZ軸線を描画する
	------------------------------------
	GridLine.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ShapeLine.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <memory>

// =============== クラスの定義 ===================
class GridLine
{
public:
	static void Init();
	static void Draw();

private:
	static void MakeGrid();
	static void MakeGridAxis();

private:
	static inline std::unique_ptr<ShapeLine> m_pGridLine;	// グリッド線
	static inline std::unique_ptr<ShapeLine> m_pAxisLine;	// XYZ軸線
	
	static inline bool				m_bIsEnable;
	static inline float				m_fGridSize;
	static inline float				m_fGridMargin;
	static inline Vector3<float>	m_fGridColor;
	static inline bool				m_bIsAxis;
	
};


/* ========================================
	DX22Base/
	------------------------------------
	グリッド線描画用cpp
	------------------------------------
	GridLine.cpp
========================================== */

// =============== インクルード ===================
#include "GridLine.h"
#include "ColorVec3.h"		// 色定義
#include "DebugMenu.h"		// デバッグメニュー

// =============== 定数定義 =======================
const int	GRID_LINE_NUM	= 10;		// 1方向におけるグリッド線の数
const float GRID_CELL_SIZE	= 1.0f;	// グリッドの1マスの大きさ

// グリッド線の色
const Vector3<float> LINE_COLOR_NORMAL = ColorVec3::GRAY;	// グリッド線の色(灰色)
const Vector3<float> LINE_COLOR_AXIS[3] = {
	ColorVec3::RED,		// X軸の色(赤)
	ColorVec3::GREEN,	// Y軸の色(緑)
	ColorVec3::BLUE		// Z軸の色(青)
};


/* ========================================
	初期化関数
	-------------------------------------
	内容：初期化
=========================================== */
void GridLine::Init()
{
	m_pGridLine.reset();
	m_pAxisLine.reset();

	m_bIsEnable = true;
	m_fGridSize = GRID_LINE_NUM;
	m_fGridMargin = GRID_CELL_SIZE;
	m_fGridColor = LINE_COLOR_NORMAL;
	m_bIsAxis = true;

#ifdef _DEBUG
	m_bIsEnable = WIN_BASIC_SETTING["Grid"]["GridEnable"].GetBool();
	m_fGridSize = WIN_BASIC_SETTING["Grid"]["Size"].GetFloat();
	m_fGridMargin = WIN_BASIC_SETTING["Grid"]["Margin"].GetFloat();
	DirectX::XMFLOAT4 color = WIN_BASIC_SETTING["Grid"]["Color"].GetColor();
	m_fGridColor = { color.x, color.y, color.z };
	m_bIsAxis = WIN_BASIC_SETTING["Grid"]["AxisEnable"].GetBool();
#endif

	m_pGridLine = std::make_unique<ShapeLine>(m_fGridSize * m_fGridSize);

	m_pAxisLine = std::make_unique<ShapeLine>(3);	// XYZ軸のみ

	MakeGrid();		// グリッド線作成
	MakeGridAxis();	// 軸グリッド線作成
	
}


/* ========================================
	描画関数
	-------------------------------------
	内容：描画
=========================================== */
void GridLine::Draw()
{
	if (m_bIsEnable)
	{
		m_pGridLine->Draw();

		if (m_bIsAxis)
		{
			m_pAxisLine->Draw();
		}
	}

	
}

/* ========================================
	グリッド線作成関数
	-------------------------------------
	内容：グリッド線を作成
=========================================== */
void GridLine::MakeGrid()
{
	// グリッド線の長さ
	float fLineLength = m_fGridSize * m_fGridMargin / 2.0f;		// プラスとマイナスの両方に描画する為に半分にする

	// 軸以外のグリッド線を描画
	for (int i = 1; i <= (m_fGridSize / 2); i++)
	{
		float fGridLinePos = i * m_fGridMargin;

		// X方向
		// プラス側(奥)
		m_pGridLine->AddLine(
			Vector3<float>(-fLineLength, 0.0f, fGridLinePos),	// 開始点(左端)
			Vector3<float>( fLineLength, 0.0f, fGridLinePos),	// 終了点(右端)
			m_fGridColor);

		// マイナス側(手前)
		m_pGridLine->AddLine(
			Vector3<float>(-fLineLength, 0.0f, -fGridLinePos),	// 開始点(左端)
			Vector3<float>( fLineLength, 0.0f, -fGridLinePos),	// 終了点(右端)
			m_fGridColor);

		// Z方向
		// プラス側(右)
		m_pGridLine->AddLine(
			Vector3<float>(fGridLinePos, 0.0f,  fLineLength),	// 開始点(画面奥端)
			Vector3<float>(fGridLinePos, 0.0f, -fLineLength),	// 終了点(画面手前端)
			m_fGridColor);

		// マイナス側(左)
		m_pGridLine->AddLine(
			Vector3<float>(-fGridLinePos, 0.0f,  fLineLength),	// 開始点(画面奥端)
			Vector3<float>(-fGridLinePos, 0.0f, -fLineLength),	// 終了点(画面手前端)
			m_fGridColor);
	}


}

/* ========================================
	軸グリッド線作成関数
	-------------------------------------
	内容：XYZ軸のグリッド線を作成
=========================================== */
void GridLine::MakeGridAxis()
{
	// グリッド線の長さ
	float fLineLength = m_fGridSize * m_fGridMargin / 2.0f;		// プラスとマイナスの両方に描画する為に半分にする

	// 軸のグリッド線を描画
	// X軸
	m_pAxisLine->AddLine(
		Vector3<float>(-fLineLength, 0.0f, 0.0f),
		Vector3<float>( fLineLength, 0.0f, 0.0f),
		LINE_COLOR_AXIS[0]);

	// Y軸
	m_pAxisLine->AddLine(
		Vector3<float>(0.0f, -fLineLength, 0.0f),
		Vector3<float>(0.0f,  fLineLength, 0.0f),
		LINE_COLOR_AXIS[1]);

	// Z軸
	m_pAxisLine->AddLine(
		Vector3<float>(0.0f, 0.0f, -fLineLength),
		Vector3<float>(0.0f, 0.0f,  fLineLength),
		LINE_COLOR_AXIS[2]);
}

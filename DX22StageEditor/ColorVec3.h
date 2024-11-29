/* ========================================
	DX22Base/
	------------------------------------
	色定数用ヘッダ
	------------------------------------
	説明：色の定数をVector3で定義
	------------------------------------
	Vector3.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "Vector3.h"

// =============== 名前空間 =====================
namespace ColorVec3
{
	const Vector3<float> WHITE		= Vector3<float>(1.0f, 1.0f, 1.0f);		
	const Vector3<float> BLACK		= Vector3<float>(0.0f, 0.0f, 0.0f);
	const Vector3<float> RED		= Vector3<float>(1.0f, 0.0f, 0.0f);
	const Vector3<float> GREEN		= Vector3<float>(0.0f, 1.0f, 0.0f);
	const Vector3<float> BLUE		= Vector3<float>(0.0f, 0.0f, 1.0f);
	const Vector3<float> YELLOW		= Vector3<float>(1.0f, 1.0f, 0.0f);
	const Vector3<float> CYAN		= Vector3<float>(0.0f, 1.0f, 1.0f);
	const Vector3<float> MAGENTA	= Vector3<float>(1.0f, 0.0f, 1.0f);
	const Vector3<float> GRAY		= Vector3<float>(0.5f, 0.5f, 0.5f);
	const Vector3<float> LIGHTGRAY	= Vector3<float>(0.75f, 0.75f, 0.75f);
	const Vector3<float> DARKGRAY	= Vector3<float>(0.25f, 0.25f, 0.25f);
	const Vector3<float> ORANGE		= Vector3<float>(1.0f, 0.5f, 0.0f);
	const Vector3<float> PINK		= Vector3<float>(1.0f, 0.0f, 0.5f);
	const Vector3<float> PURPLE		= Vector3<float>(0.5f, 0.0f, 1.0f);
	const Vector3<float> BROWN		= Vector3<float>(0.5f, 0.25f, 0.0f);
	const Vector3<float> OLIVE		= Vector3<float>(0.5f, 0.5f, 0.0f);
	const Vector3<float> LIME		= Vector3<float>(0.0f, 0.5f, 0.0f);
	const Vector3<float> TEAL		= Vector3<float>(0.0f, 0.5f, 0.5f);
	const Vector3<float> NAVY		= Vector3<float>(0.0f, 0.0f, 0.5f);


}
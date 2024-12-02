/* ========================================
	DX22Base/
	------------------------------------
	デバッグカメラ用ヘッダ
	------------------------------------
	説明：自由に移動、回転できるカメラ
	------------------------------------
	ComponentCameraDebug.h
========================================== */
#pragma once

// =============== インクルード ===================
#include "ComponentBase.h"
#include "Vector2.h"
#include "Input.h"

// =============== クラス定義 =====================
class ComponentCameraDebug :
	public ComponentBase
{
public:
	ComponentCameraDebug(class ObjectBase* pObject);

	void Init() override;
	void Update() override;

	DEFINE_COMPONENT_TYPE;
#ifdef _DEBUG
	void Debug(DebugUI::Window& window) override;
#endif // _DEBUG

private:
	void Move();
	void RotateKey();
	void RotateMouse();
private:

	class ComponentCameraBase* m_pCameraBase;	// カメラの基本機能
	float m_fMoveSpeed;		// 移動速度
	float m_fRotSpeed;	// 回転速度

	Vector2<float> m_vMouseMove;	// マウス移動量
	POINT m_CursorPosOld;			// 1フレーム前のカーソル座標

	bool m_bRotateMouse;	// 回転入力
};


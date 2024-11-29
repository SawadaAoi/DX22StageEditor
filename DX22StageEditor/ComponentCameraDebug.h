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
	void Rotate();
private:

	class ComponentCameraBase* m_pCameraBase;	// カメラの基本機能
	float m_fMoveSpeed;		// 移動速度
	float m_fRotSpeed;	// 回転速度
};


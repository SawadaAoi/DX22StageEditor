
/* ========================================
   HEW/UniBoooom!!
   ---------------------------------------
   コントローラー用ヘッダ
   ---------------------------------------
   Controller.h
   ======================================== */
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

 // =============== インクルード ===================
#include <Windows.h>
#include <mmsystem.h>             // joyGetPosEx, JOYINFOEX, JOYSTICKID1
#include "Vector3.h"

class Controller
{
public:
	// ===プロトタイプ宣言===
	Controller();
	~Controller();
	void Update();

	Vector3<float> GetStickRight();
	Vector3<float> GetStickLeft();
	int* GetKeyState() { return m_keyState; }
	int* GetKeyStateOld() { return m_oldState; }

	bool GetUse() { return m_bUse; }
private:
	void SetKeyState();

	// ===メンバ変数宣言=====
	bool Button1();
	bool Button2();
	bool Button3();
	bool Button4();
	bool Button5();
	bool Button6();
	bool Button7();
	bool Button8();
	bool Button9();
	bool Button10();
	bool Button11();
	bool Button12();
	float LeftStickX();
	float LeftStickY();
	float RightStickX();
	float RightStickY();


	JOYINFOEX   m_joyInfoEx;    // ジョイスティックの情報を格納する変数
	int m_keyState[12];			// ボタン押下情報セット配列(現在)
	int m_oldState[12];			// ボタン押下情報セット配列(1フレーム前)

	bool m_bUse;	// コントローラの有効無効フラグ
};




#endif // !__CONTROLLER_H__


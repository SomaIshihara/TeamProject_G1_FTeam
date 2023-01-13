//==========================================================================================
//
//入力プログラムのヘッダ[input.h]
//Author:石原颯馬
//
//==========================================================================================
//Ver.2.2.2（プロジェクト制作中のもの）
//使用可能な入力:キーボード・マウス（カーソル位置・移動量）・ゲームパッド（1~4台）
//==========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//==========================================================================================
//マクロ定義
//==========================================================================================
#define STICK_MAX			(32767)	//スティック最大値
#define MOUSE_CLICK_LEFT	(0)		//マウス左クリック
#define MOUSE_CLICK_RIGHT	(1)		//マウス右クリック
#define MAX_USE_GAMEPAD		(4)		//ゲームパッド使用可能台数
#define MOUSE_BUTTON_MAX	(3)		//マウスボタンの最大数

//==========================================================================================
//構造体定義
//==========================================================================================
//キーボード
typedef struct
{
	BYTE state;		//状態（保存用およびプレス情報）
	BYTE trigger;	//トリガー情報
	BYTE release;	//リリース情報
	BYTE repeate;	//リピート情報
	DWORD currentTime;	//現在の時間
	DWORD execLastTime;	//最後にtrueにした時間
} Keyboard;

//ゲームパッド
typedef struct
{
	XINPUT_STATE state;	//状態全部（およびプレス情報）
	WORD trigger;		//トリガー情報
	WORD release;		//リリース情報
	WORD repeate;		//リピート情報
	DWORD currentTime;	//現在の時間
	DWORD execLastTime;	//最後にtrueにした時間
	bool bUse;			//使用の有無
} GamePad;

//マウスのボタン部
typedef struct
{
	BYTE trigger;		//ボタンのトリガー情報
	BYTE release;		//ボタンのリリース情報
	BYTE repeate;		//リピート情報
	DWORD currentTime;	//現在の時間（リピート使用）
	DWORD execLastTime;	//最後にtrueを返した時間（リピート使用)
} MouseButton;

//マウスすべて
typedef struct
{
	MouseButton mb[MOUSE_BUTTON_MAX];	//マウスボタン
	DIMOUSESTATE state;					//状態およびプレス情報
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 move;					//移動量
} Mouse;

//==========================================================================================
//プロトタイプ宣言
//==========================================================================================
//=========================================================
//処理に必要なもの
//=========================================================
//=============================
//キーボード
//=============================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

//=============================
//マウス
//=============================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);

//=============================
//ゲームパッド
//=============================
void InitGamePad(void);
void UninitGamePad(void);
void UpdateGamePad(void);

//=========================================================
//キーボード
//=========================================================
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeate(int nKey);

//=========================================================
//ゲームパッド
//=========================================================
//=============================
//ボタン
//=============================
bool GetGamepadPress(int nPadNum, int nButton);
bool GetGamepadTrigger(int nPadNum, int nButton);
bool GetGamepadRelease(int nPadNum, int nButton);
bool GetGamepadRepeate(int nPadNum, int nButton);

//=============================
//スティック
//=============================
SHORT GetLStickX(int nPadNum);
SHORT GetLStickY(int nPadNum);
SHORT GetRStickX(int nPadNum);
SHORT GetRStickY(int nPadNum);

//=============================
//使用の有無
//=============================
bool GetUseGamepad(int nPadNum);

//=========================================================
//マウス
//=========================================================
//=============================
//クリック
//=============================
bool GetMouseClickPress(int nButton);
bool GetMouseClickTrigger(int nButton);
bool GetMouseClickRepeate(int nButton);

//=============================
//位置
//=============================
D3DXVECTOR3 GetMousePos(void);

//=============================
//移動量
//=============================
D3DXVECTOR3 GetMouseMove(void);

#endif // !_INPUT_H_

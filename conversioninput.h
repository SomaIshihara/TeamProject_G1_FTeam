//==========================================
//
//コントローラーの入力変換プログラムのヘッダ[conversioninput.h]
//Author:石原颯馬
//
//==========================================
#ifndef _CONVERSION_INPUT_H_
#define _CONVERSION_INPUT_H_

#include "main.h"
#include "input.h"

//マクロ
typedef enum
{
	INPUTTYPE_PRESS = 0,
	INPUTTYPE_TRIGGER,
	INPUTTYPE_RELEASE,
	INPUTTYPE_REPEATE,
	INPUTTYPE_MAX
} INPUTTYPE;

//変換後入力列挙
typedef enum
{
	BUTTON_X = 0,
	BUTTON_A,
	BUTTON_PAUSE,
	BUTTON_MAX
} BUTTON;

//スティック操作
typedef enum
{
	CONVSTICK_LEFT = -1,
	CONVSTICK_NEUTRAL,
	CONVSTICK_RIGHT,
	CONVSTICK_MAX
} CONVSTICK;

//プロトタイプ宣言
void InitConvertionInput(void);
void UpdateConvertionInput(void);
void SetButton(int nPadNum, INPUTTYPE type, BUTTON button, bool stat);
bool GetButton(int nPadNum, INPUTTYPE type, BUTTON button);
void SetStick(int nPadNum, CONVSTICK stick);
CONVSTICK GetStick(int nPadNum);

#endif // !_CONVERSION_INPUT_H_

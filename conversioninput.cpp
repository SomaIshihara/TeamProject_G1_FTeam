//==========================================
//
//コントローラーの入力変換プログラム[conversioninput.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "conversioninput.h"

//変換後入力構造体定義
typedef struct
{
	bool press[BUTTON_MAX];
	bool trigger[BUTTON_MAX];
	bool release[BUTTON_MAX];
	bool repeate[BUTTON_MAX];
} ConvButton;

//グローバル
ConvButton g_aConvButton[MAX_USE_GAMEPAD] = {};
CONVSTICK g_aConvStick[MAX_USE_GAMEPAD] = {};

//キーボード対応操作
const int c_aKeyboardInput[BUTTON_MAX] = { DIK_SPACE,DIK_RETURN,DIK_P };
const int c_aGamePadInput[BUTTON_MAX] = { XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_START };

//========================
//初期化処理
//========================
void InitConvertionInput(void)
{
	//普通に初期化
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		g_aConvButton[nCntController] = {};
		g_aConvStick[nCntController] = {};
	}
}

//========================
//更新処理
//========================
void UpdateConvertionInput(void)
{
	//ゲームパッドの入力を代入（すべて）[優先度：中]
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
		{
			g_aConvButton[nCntController].press[nCntButton] = GetGamepadPress(nCntButton, c_aGamePadInput[nCntButton]);
			g_aConvButton[nCntController].trigger[nCntButton] = GetGamepadTrigger(nCntButton, c_aGamePadInput[nCntButton]);
			g_aConvButton[nCntController].release[nCntButton] = GetGamepadRelease(nCntButton, c_aGamePadInput[nCntButton]);
			g_aConvButton[nCntController].repeate[nCntButton] = GetGamepadRepeate(nCntButton, c_aGamePadInput[nCntButton]);
		}
	}

	//キーボードの入力を代入（1Pのみ）[優先度：大]
	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		g_aConvButton[0].press[nCntButton] = GetKeyboardPress(c_aKeyboardInput[nCntButton]);
		g_aConvButton[0].trigger[nCntButton] = GetKeyboardTrigger(c_aKeyboardInput[nCntButton]);
		g_aConvButton[0].release[nCntButton] = GetKeyboardRelease(c_aKeyboardInput[nCntButton]);
		g_aConvButton[0].repeate[nCntButton] = GetKeyboardRepeate(c_aKeyboardInput[nCntButton]);
	}
}

//========================
//入力設定処理
//========================
void SetButton(int nPadNum, INPUTTYPE type, BUTTON button, bool stat)
{
	switch (type)
	{
	case INPUTTYPE_PRESS:
		g_aConvButton[nPadNum].press[button] = stat;
		break;
	case INPUTTYPE_TRIGGER:
		g_aConvButton[nPadNum].trigger[button] = stat;
		break;
	case INPUTTYPE_RELEASE:
		g_aConvButton[nPadNum].release[button] = stat;
		break;
	case INPUTTYPE_REPEATE:
		g_aConvButton[nPadNum].repeate[button] = stat;
		break;
	}
}
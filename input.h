/*==========================================================================================================================================================

キーボード・コントローラー処理[input.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
/*--------------------------------------------
ここにコントローラーの入力情報を取得する関数を書きます
---------------------------------------------*/
XINPUT_STATE *GetXInputState(void);
bool GetGamePadPress(int nKey);
bool GetGamePadTrigger(int nKey);
bool GetGamePadRelease(int nKey);
 
#endif
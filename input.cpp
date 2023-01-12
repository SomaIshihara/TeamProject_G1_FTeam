/*==========================================================================================================================================================

キーボード・コントローラー処理[main.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX		(256)		//キーの最大数

//ゲームパッドの最大ボタン数をマクロ定義しましょう

//グローバル変数宣言
LPDIRECTINPUT8		 g_pInput = NULL;			//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//入力デバイス(キーボード)へのポインタ

//マウス部
LPDIRECTINPUT8 g_pInputMouse = NULL;
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;
DIMOUSESTATE g_MouseState;						//マウスの情報（プレス）
BYTE g_aMouseStateTrigger[MOUSE_BUTTON_MAX];	//ボタンのトリガー情報
BYTE g_aMouseStateRelease[MOUSE_BUTTON_MAX];	//ボタンのリリース情報
BYTE g_aMouseStateRepeate[MOUSE_BUTTON_MAX];	//リピート情報
DWORD g_aMouseCurrentTime[NUM_KEY_MAX];			//現在の時間（リピート使用）
DWORD g_aMouseExecLastTime[NUM_KEY_MAX];		//最後にtrueを返した時間（リピート使用)
D3DXVECTOR3 g_posPoint;							//マウス座標
D3DXVECTOR3 g_moveMouse;						//マウス移動量

//========================
//入力初期化処理
//========================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void* *)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の作成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を取得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//--------------------------------------------------------
//				キーボードの終了処理
//--------------------------------------------------------
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();		//キーボードへのアクセス権を破棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//--------------------------------------------------------
//				キーボードの更新処理
//--------------------------------------------------------
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	/*　こ↑こ↓にコントローラーの入力情報を一時格納する変数を作りましょう*/

									//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRelease[nCntKey] = (aKeyState[nCntKey] ^ g_aKeyState[nCntKey]) & g_aKeyState[nCntKey];//キーボードのリリース情報を保存する

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//キーボードのトリガー情報を保存する

			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保存する
		}
	}

	else
	{
		//キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}

	//ゲームパッドから情報取得
	/*----------------------------------------------------------------------------------------------------------------

	ここで何番目のコントローラーから入力されたかを判定します　おそらくここで複数のコントローラーの設定ができます
	
	------------------------------------------------------------------------------------------------------------------*/

	/*----------------------------------------------------------------------------------------------------------------

	そしてここで押された情報を保存してあげましょう　出来れば関数分けもできればGood Job!

	------------------------------------------------------------------------------------------------------------------*/
	//ゲームパッドのリリース情報を保存する
	//ゲームパッドのトリガー情報を保存する
	//ゲームパッドのプレス情報を保存する
}

//--------------------------------------------------------
//			キーボードの入力情報返答処理
//--------------------------------------------------------
//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;		//３項演算子
}

//キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//キーボードのリリース情報を取得
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------------
//			コントローラーの入力情報返答処理
//--------------------------------------------------------
/*----------------------------------------------------------------------

ここにコントローラーの「プレス」「トリガー」「リリース」の関数を書きます

------------------------------------------------------------------------*/
XINPUT_STATE *GetXInputState(void)
{
	return 0;
}

//ゲームパッドのプレス情報
bool GetGamePadPress(int nPad)
{
	return 0;
}

//ゲームパッドのトリガー情報
bool GetGamePadTrigger(int nPad)
{
	return 0;
}

//ゲームパッドのリリース情報
bool GetGamePadRelease(int nPad)
{
	return 0;
}
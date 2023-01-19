/*==========================================================================================================================================================

ゲーム画面処理[game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "time.h"

//マクロ定義
#define END_FADE	(60)		//終了余韻

//コントローラーチェックモード列挙
typedef enum
{
	CHECKMODE_DISCONPAUSE = 0,	//切断されたら強制ポーズするモード
	CHECKMODE_DISCONNOPAUSE,	//切断されたかだけ確認するモード
	CHECKMODE_REMOVE,			//使用コントローラー削除モード
	CHECKMODE_MAX
} CHECKMODE;

//プロト
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
int SetUseController(void);
bool GetUseController(int nPadNum);
int GetUseControllerNum(void);
void SetEnablePause(bool pause);
void CheckUseController(CHECKMODE mode);

#endif
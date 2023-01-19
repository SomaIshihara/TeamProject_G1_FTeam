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

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
bool GetUseController(int nPadNum);
int GetUseControllerNum(void);

#endif
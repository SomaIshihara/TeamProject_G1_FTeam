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
void InitPvPGame(void);
void UninitPvPGame(void);
void UpdatePvPGame(void);
void DrawPvPGame(void);
int SetUseController_PvP(void);
bool GetUseController_PvP(int nPadNum);
int GetUseControllerNum_PvP(void);
void SetEnablePause_PvP(bool pause);
bool CheckUseController_PvP(CHECKMODE mode);
void ChangeNumCamera_PvP(void);

#endif
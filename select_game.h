/*==========================================================================================================================================================

//ゲームメニューの選択処理[select_game.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _SELECT_GAME_H_
#define _SELECT_GAME_H_

//選択できるゲームモード
typedef enum
{
	SelectGameMode_PVP = 0,		//落とし合うゲーム
	SelectGameMode_HipDropRace,	//ヒップドロップレースゲーム
	SelectGameMode_MAX
}SelectGameMode;

//プロトタイプ宣言
void InitSelectGame(void);
void UninitSelectGame(void);
void UpdateSelectGame(void);
void DrawSelectGame(void);

void DecideGameMode(void);

#endif 
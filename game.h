/*==========================================================================================================================================================

Q[æĘ[game.cpp]
Author:―āV

============================================================================================================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "time.h"

//}Nč`
#define END_FADE	(60)		//Iđ]C

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
bool GetUseController(int nPadNum);
int GetUseControllerNum(void);

#endif
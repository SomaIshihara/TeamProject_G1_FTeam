/*==========================================================================================================================================================

�Q�[����ʏ���[game.cpp]
Author:���V����

============================================================================================================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "time.h"

//�}�N����`
#define END_FADE	(60)		//�I���]�C

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
bool GetUseController(int nPadNum);
int GetUseControllerNum(void);

#endif
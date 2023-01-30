/*==========================================================================================================================================================

スコア処理[score.h]
Author:藤原龍輝

============================================================================================================================================================*/
#ifndef _SCORE_H_

#define _SCORE_H_

#include"main.h"

//****************************//
//	    プロトタイプ宣言      //
//****************************//
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void SetScore(int nScore, int nPlayer);
void AddScore(int nValue, int nPlayer);
#endif

/*==========================================================================================================================================================

�q�b�v�h���b�v�Q�[����ʏ���[HDRgame.cpp]
Author:�������P

============================================================================================================================================================*/
#ifndef _HDR_GAME_H_
#define _HDR_GAME_H_

#include "main.h"
#include "time.h"

//�v���g
void InitHDRGame(void);
void UninitHDRGame(void);
void UpdateHDRGame(void);
void DrawHDRGame(void);
int SetUseController_HDR(void);
bool GetUseController_HDR(int nPadNum);
int GetUseControllerNum_HDR(void);
void SetEnablePause_HDR(bool pause);
bool CheckUseController_HDR(CHECKMODE mode);
void ChangeNumCamera_HDR(void);
float HipDropBuff(void);

#endif

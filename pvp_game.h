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

//�R���g���[���[�`�F�b�N���[�h��
typedef enum
{
	CHECKMODE_DISCONPAUSE = 0,	//�ؒf���ꂽ�狭���|�[�Y���郂�[�h
	CHECKMODE_DISCONNOPAUSE,	//�ؒf���ꂽ�������m�F���郂�[�h
	CHECKMODE_REMOVE,			//�g�p�R���g���[���[�폜���[�h
	CHECKMODE_MAX
} CHECKMODE;

//�v���g
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
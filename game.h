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
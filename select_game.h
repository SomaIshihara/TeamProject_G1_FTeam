/*==========================================================================================================================================================

//�Q�[�����j���[�̑I������[select_game.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _SELECT_GAME_H_
#define _SELECT_GAME_H_

//�I���ł���Q�[�����[�h
typedef enum
{
	SelectGameMode_PVP = 0,		//���Ƃ������Q�[��
	SelectGameMode_HipDropRace,	//�q�b�v�h���b�v���[�X�Q�[��
	SelectGameMode_MAX
}SelectGameMode;

//�v���g�^�C�v�錾
void InitSelectGame(void);
void UninitSelectGame(void);
void UpdateSelectGame(void);
void DrawSelectGame(void);

void DecideGameMode(void);

#endif 
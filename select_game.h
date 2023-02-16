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

//�I�����j���[
typedef enum
{
	SelectGameMenu_BG = 0,
	SelectGameMenu_PVP,
	SelectGameMenu_HipDropRace,
	SelectGameMenu_MAX
}SelectGameMenu;

//�Q�[���I�����̍\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	float		fWidth;		// ��
	float		fHeight;	// ����
	bool		bChoice;	// �I������Ă��邩�ǂ���
}SelectGame;

//�v���g�^�C�v�錾
void InitSelectGame(void);
void LoadSelectGame(void);
void SetVertexSelectGame(void);

void UninitSelectGame(void);
void UpdateSelectGame(void);
void DrawSelectGame(void);

void ChooseGameMode(void);
void DecideGameMode(void);

#endif 
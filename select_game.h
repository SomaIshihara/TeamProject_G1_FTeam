/*==========================================================================================================================================================

//�Q�[�����j���[�̑I������[select_game.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _SELECT_GAME_H_
#define _SELECT_GAME_H_

//�I���ł���Q�[�����[�h
typedef enum
{
	SelGameMode_PVP = 0,	//���Ƃ������Q�[��
	SelGameMode_HDR,		//�q�b�v�h���b�v���[�X�Q�[��
	SelGameMode_MAX
}SelGameMode;

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
SelGameMode GetSelGameMode(void);

#endif 
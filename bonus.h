/*==========================================================================================================================================================

�{�[�i�X����[bonus.h]
Author:�������P

============================================================================================================================================================*/
#ifndef _BONUS_H_
#define _BONUS_H_

typedef enum
{
	DIRECTION_ZERO = 0,
	DIRECTION_ONE,
	DIRECTION_TWO,
	DIRECTION_THREE,
	DIRECTION_MAX
}BONUS;

//****************************//
//	    �{�[�i�X�̍\����      //
//****************************//
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 move;			//�ʒu
	D3DXVECTOR3 rot;			//�p�x
	bool        buse;			//�g�p���Ă��邩�ǂ���
	BONUS		Respawn;		//�o���ʒu
	int			DespawnLimit;	//�����郊�~�b�g
	int			nLife;			//�̗�
	float		a;				//�A���t�@�l
}Bonus;

//****************************//
//	    �v���g�^�C�v�錾      //
//****************************//
void InitBonus(void);
void UninitBonus(void);
void UpdateBonus(void);
void DrawBonus(void);
void SetBonus(void);
void MoveBonus(void);
void AppearandDisAppearBonus(void);
void CollisionBonus(D3DXVECTOR3 nPlayer,int NumPlayer);
Bonus GetBonus(void);

#endif

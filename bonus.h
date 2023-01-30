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
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ʒu
	D3DXVECTOR3 rot;		//�p�x
	bool        buse;		//�g�p���Ă��邩�ǂ���
	BONUS		Respawn;	//�o���ʒu
}Bonus;

//****************************//
//	    �v���g�^�C�v�錾      //
//****************************//
void InitBonus(void);
void UninitBonus(void);
void UpdateBonus(void);
void DrawBonus(void);
void MoveBonus(void);

#endif

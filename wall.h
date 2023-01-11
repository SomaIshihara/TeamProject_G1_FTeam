/*==========================================================================================================================================================

�ǂ̏���[wall.h]
Author:��{����

============================================================================================================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//�ǂ̏��
typedef enum
{
	WALLSTATE_NORMAL = 0,
	WALLSTATE_MAX
} WALLSTATE;

//�ǂ̏��
typedef enum
{
	WALLANGLE_BACK = 0,
	WALLANGLE_FRONT,
	WALLANGLE_LEFT,
	WALLANGLE_RIGHT,
	WALLANGLE_MAX
} WALLANGLE;

//�Ǎ\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos; //�ǂ̈ʒu
	D3DXVECTOR3 rot; //�ǂ̌���
	WALLANGLE wallAngle;
	int nType; //�ǂ̎��	
	float fWidth; //�ǂ̕�
	float fHeight; //�ǂ̍���
	float fDepth; //�ǂ̉��s��
	bool bUse; //�ǂ��g�p���Ă��邩�ǂ���
} Wall;



//�v���g�^�C�v�錾
void InitWall(void); //����������
void UninitWall(void); //�I������
void UpdateWall(void); //�X�V����
void DrawWall(void); //�`�揈��
void LoadWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif
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

//�ǂ̓ǂݍ���
typedef enum
{
	WALLCASE_TYPE = 0,
	WALLCASE_WALLANGLE,
	WALLCASE_WIDTH,
	WALLCASE_HEIGHT,
	WALLCASE_DEPTH,
	WALLCASE_POSX,
	WALLCASE_POSY,
	WALLCASE_POSZ,
	WALLCASE_MAX
}	WALLCASE;

//�Ǎ\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	WALLANGLE wallAngle;	// �����̎��
	int nType;				// ���
	float fWidth;			// ��
	float fHeight;			// ����
	float fDepth;			// ���s��
	float fAlpha;			// ���J���[
	bool bSideVibe;			// ���E�ɐk�������ǂ���		false�F���ɐk���Ă���	true�F�E�ɐk���Ă���
	float fSideVibe;		// �k���̕�
	bool bCollision;		// �����������ǂ���
	bool bDisp;				// �`�悷�邩�ǂ���
} Wall;

//�v���g�^�C�v�錾
void InitWall(void);		// ����������
void UninitWall(void);		// �I������
void UpdateWall(void);		// �X�V����
void DrawWall(void);		// �`�揈��

void LoadWall(void);		// �ǂ̏��ǂݍ���
void SetWallVertex(void);	// �ǂ̒��_���ݒ菈��

bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);	//�ǂ̓����蔻��
void CollapseWall(int nCntWall);	//�ǂ̕��ꋎ�鏈��

#endif

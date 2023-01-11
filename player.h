//==========================================
//
//�v���C���[�v���O�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	float fLength;
	float fAngle;
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	int nNumModel;			//�p�[�c����
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
} Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

//==========================================
//
//�v���C���[�v���O�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "model.h"

//�v���C���[���f���̍\����
typedef struct
{
	int nIdxModel;			//�C���f�b�N�X�ԍ�
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	LPD3DXMESH pMesh;		//���b�V��
	LPD3DXBUFFER pBuffMat;	//�}�e���A���|�C���^
	DWORD dwNumMat;			//�}�e���A���̐�

}PlayModel;

//�v���C���[�\����
typedef struct
{
	//�ʒu��
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	float moveGauge;		//�ړ���(�_�b�V��)

	//�����E�p�x
	float fLength;
	float fAngle;

	//�p�����[�^��
	ANIMAL animal;			//�g�p���Ă��铮��
	int nScore;				//���_
	int nNumHitPlayer;		//�Ō�ɏՓ˂����v���C���[�i�����l-1�j

	//�`���
	Model model;			//�g�p���f��
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
	int nIdxShadow;			//�e�ԍ�
	bool bUse;				//�g�p�̗L��

	PlayModel aPlaymodel[10];

} Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

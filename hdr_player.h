//==========================================
//
//�v���C���[�v���O�����̃w�b�_[hdr_player.h]
//Author:�������P
//
//==========================================
#ifndef _HDR_PLAYER_H_
#define _HDR_PLAYER_H_

#include "main.h"
#include "model.h"
#include "input.h"

//�v���C���[���f���̍\����
typedef struct
{
	int nIdxModel;			//�C���f�b�N�X�ԍ�
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	LPD3DXMESH pMesh;		//���b�V��
	LPD3DXBUFFER pBuffMat;	//�}�e���A���|�C���^
	DWORD dwNumMat;			//�}�e���A���̐�

}PlayModel_HDR;

//�v���C���[�\����
struct Player_HDR
{
	//�ʒu��
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	int jumpTime;			//�W�����v�J�n����̎���[�t���[���P��]
	bool bJump;				//�W�����v���Ă��邩�ǂ���
	bool bHipDrop;			//�q�b�v�h���b�v���Ă��邩�ǂ���
	bool bHipDropSpin;		//�q�b�v�h���b�v�̃X�s���������ǂ���
	int nHipDropWait;		//�q�b�v�h���b�v�́u�J�n�E���n�v�d���J�E���^�[

	//�p�����[�^��
	ANIMAL animal;			//�g�p���Ă��铮��

	//�`���
	Model model;			//�g�p���f��
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
	bool bUseShadow;		//�e�g�p�̗L��
	bool bUsePlayer;		//�v���C���[�g�p�̗L��

	PlayModel_HDR aPlaymodel[10];
};

//�v���g�^�C�v�錾
void InitPlayer_HDR(void);
void UninitPlayer_HDR(void);
void UpdatePlayer_HDR(void);
void DrawPlayer_HDR(void);
Player_HDR *GetPlayer_HDR(void);

#endif
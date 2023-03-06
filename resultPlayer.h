//==========================================
//
//�v���C���[�v���O�����̃w�b�_[hdr_player.h]
//Author:���V�����@�Ό��D�n
//
//==========================================
#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "main.h"
#include "model.h"
#include "PvP_player.h"
#include "hdr_player.h"
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

}PlayModel_RESULT;

//�v���C���[�\����
struct Player_RESULT
{
	//�ʒu��
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	bool bHipDrop;			//�q�b�v�h���b�v���Ă��邩�ǂ���
	bool bHipDropSpin;		//�q�b�v�h���b�v�̃X�s���������ǂ���
	int nHipDropWait;		//�q�b�v�h���b�v�́u�J�n�E���n�v�d���J�E���^�[
	int nRank;				//���ʂ��i�[

	//�p�����[�^��
	ANIMAL animal;			//�g�p���Ă��铮��

							//�`���
	Model model;			//�g�p���f��
	AnimalInstance animalInst[MAX_PARTS];
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
	bool bUseShadow;		//�e�g�p�̗L��
	bool bUsePlayer;		//�v���C���[�g�p�̗L��

	PlayModel_RESULT aPlaymodel[10];
};

//�v���g�^�C�v�錾
void InitPlayer_RESULT(void);
void InitResultPlayerType_GAMEMODE_PVP(Player_RESULT *pResPlayer);
void InitResultPlayerType_GAMEMODE_HDR(Player_RESULT *pResPlayer);
void UninitPlayer_RESULT(void);
void UpdatePlayer_RESULT(void);
void DrawPlayer_RESULT(void);
Player_RESULT *GetPlayer_RESULT(void);

#endif
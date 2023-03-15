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
#include "ai.h"

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
	float		fHeightLastJump;//�Ō�ɃW�����v�������̍���
	D3DXVECTOR3 posOld_Coll;//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 moveV0;		//�����ړ���
	D3DXVECTOR3 rot;		//����
	int jumpTime;			//�W�����v�J�n����̎���[�t���[���P��]
	int nHipDropWait;		//�q�b�v�h���b�v�́u�J�n�E���n�v�d���J�E���^�[
	int HipDropPower;
	bool bJump;				//�W�����v���Ă��邩�ǂ���
	bool bHipDrop;			//�q�b�v�h���b�v���Ă��邩�ǂ���
	bool bHipDropSpin;		//�q�b�v�h���b�v�̃X�s���������ǂ���
	bool bGoal;				//�S�[��������
	int nRank;				//����
	float fBodySize;		//�̂̑傫���i�����蔻��Ɏg��
	int nDestroyCounter;	//�u���b�N�j���

	//�p�����[�^��
	ANIMAL animal;			//�g�p���Ă��铮��

	//AI
	AIDIFF aiDiff;			//AI��Փx
	bool bUseAI;			//AI�g�p���邩�ǂ���
	float fAIHeight;			//�W�����v��
	int nAICT;				//AI�̃N�[���^�C��

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
void SetPlayerType_HDR(int nPlayerNum, bool bUse, bool bAIUse);	//�v���C���[�g�p��AI�g�p���邩�w�肵�Ȃ��ꍇAI�g�p

//�q�b�v�h���b�v����
const AIParam_HDR c_aAIParamHDR[AIDIFF_MAX] =
{
	{ 10,25,45,30,40 },	//EASY
	{ 5,15,40,40,30 },	//NORMAL
	{ 1,4,20,75,20 },	//HARD
	{ 1,1,1,1,20 }	//VIEW
};

#endif
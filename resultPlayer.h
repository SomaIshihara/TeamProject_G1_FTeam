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
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 posOld;	// �O��̈ʒu
	D3DXVECTOR3 rot;	// ����
	float	fFallSpeed;	// �������x
	float	fAdvance;	// �O�i���x
	bool bDive;			// ��э���
	bool bHipDrop;		// �q�b�v�h���b�v���Ă��邩�ǂ���
	bool bHipDropSpin;	// �q�b�v�h���b�v�̃X�s���������ǂ���
	int nHipDropWait;	// �q�b�v�h���b�v�̊J�n�d���J�E���^�[
	int nRank;			// ���ʂ��i�[

	//�p�����[�^��		 
	ANIMAL animal;		// �g�p���Ă��铮��

						// �`���
	Model model;		// �g�p���f��
	AnimalInstance animalInst[MAX_PARTS];
	D3DXMATRIX mtxWorld;// ���[���h�}�g��
	PlayModel_RESULT aPlaymodel[10];

	bool bUsePlayer;	// �v���C���[�g�p�̗L��
};

//�v���g�^�C�v�錾
void InitPlayer_RESULT(void);											//��{��񏉊���
void InitResultPlayerType_GAMEMODE_PVP(Player_RESULT *pResPlayer);		//�Q�[�����[�h�� "PvP" �̂Ƃ��̏���������
void InitResultPlayerType_GAMEMODE_HDR(Player_RESULT *pResPlayer);		//�Q�[�����[�h�� "HDR" �̂Ƃ��̏���������
void UninitPlayer_RESULT(void);			//�I������

void UpdatePlayer_RESULT(void);									//�X�V����
void UpdatePosPlayer_RESULT(Player_RESULT *pPlayer);			//�d�͏���
void StopPlayer_RESULT(Player_RESULT *pPlayer, int nCntPlayer);	//��~����
void HipSpinResPlayer(Player_RESULT *pPlayer);					//�q�b�v�h���b�v�X�s������

void DrawPlayer_RESULT(void);		//�`�揈��

void SetDivePlayer(void);			//��э��݊J�n����

Player_RESULT *GetPlayer_RESULT(void);						//�v���C���[�̏��擾����
Player_RESULT ResetResultPlayerInfo(int nCntResetPlayer);	//�v���C���[�̏������擾����

#endif
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
#include "item.h"
#include "input.h"
#include "ai.h"

//�}�N��
#define PLAYER_POWER_MAX	(1.5f)		//�ړ��̋����̍ő�l

//�v���C���[��ԗ�
typedef enum
{
	PLAYERSTAT_WAIT = 0,
	PLAYERSTAT_CHARGE,
	PLAYERSTAT_DASH,
	PLAYERSTAT_PUSHED,
	PLAYERSTAT_JUMP,
	PLAYERSTAT_HIPDROP,
	PLAYERSTAT_LAND,
	PLAYERSTAT_FALL,
	PLAYERSTAT_MAX
} PLAYERSTAT;

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

//�v���C���[�l�Ŏg�����f���ލ\����
typedef struct
{
	D3DXVECTOR3 pos;		//���[�V�����ݒ肵���ʒu
	D3DXVECTOR3 rot;		//���[�V�����ݒ肵������
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
} AnimalInstance;

//�v���C���[�\����
struct Player
{
	//�ʒu��
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 moveV0;		//�����ړ���
	D3DXVECTOR3 rot;		//����
	float moveGauge;		//�ړ���(�_�b�V��)
	float fOldMoveGauge;	//�O�̈ړ��ʁi�A�N�V�����d���Ɏg�p�j
	int jumpTime;			//�W�����v�J�n����̎���[�t���[���P��]
	bool bJump;				//�W�����v���Ă��邩�ǂ���
	bool bHipDrop;			//�q�b�v�h���b�v���Ă��邩�ǂ���
	bool bHipDropSpin;		//�q�b�v�h���b�v�̃X�s���������ǂ���
	int nHipDropWait;		//�q�b�v�h���b�v�́u�J�n�E���n�v�d���J�E���^�[
	bool bNotMove;			//�ړ����Ă��Ȃ�
	int nRespawnPosNum;		//���������ʒu�ԍ�

	//���[�V������
	Motion motion;				//���[�V�������
	D3DXVECTOR3 motionPos, motionPosOffset;	//�ʒu
	D3DXVECTOR3 motionRot, motionRotOffset;	//����
	bool bAnimed;

	//�Փˊ֌W
	D3DXVECTOR3 faceCollider[2];	//�����蔻��
	int nFrameAtkPlayer;			//���̃t���[���ŐG�ꂽ�v���C���[

	//�p�����[�^��
	ANIMAL animal;			//�g�p���Ă��铮��
	int nScore;				//���_
	int nLastHitPlayer;		//�Ō�ɏՓ˂����v���C���[�i�����l-1�j
	PLAYERSTAT stat;		//���
	int nActionRigor;		//�A�N�V�����d������
	int nPlayerNum;			//�v���C���[�ԍ�

	//�A�C�e����
	int nATKItemTime;		//�͂��������A�C�e���̎�������
	int nDEFItemTime;		//�������������A�C�e���̎�������
	int nGhostItemTime;		//�S�[�X�g���A�C�e���̎�������
	int nInvincibleTime;	//���G�̎�������

	//AI�̑���iAIVer�ɂĒǉ��j
	ComAIBrain *pAI;		//AI�|�C���^

	//�`���
	AnimalInstance animalInst[MAX_PARTS];
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
	bool bUseShadow;		//�e�g�p�̗L��
	bool bUsePlayer;		//�v���C���[�g�p�̗L��

	PlayModel aPlaymodel[10];
};

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayerType_PvP(int nPlayerNum, bool bUse, bool bAIUse);	//�v���C���[�g�p��AI�g�p���邩�w�肵�Ȃ��ꍇAI�g�p

#endif // !_PLAYER_H_

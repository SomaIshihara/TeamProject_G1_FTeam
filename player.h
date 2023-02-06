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

//�}�N��
#define PLAYER_POWER_LEVEL	(3)			//�p���[���x��
#define PLAYER_POWER_MAX	(0.5f * PLAYER_POWER_LEVEL)		//�ړ��̋����̍ő�l

//�v���C���[��ԗ�
typedef enum
{
	PLAYERSTAT_WAIT = 0,
	PLAYERSTAT_CHARGE,
	PLAYERSTAT_DASH,
	PLAYERSTAT_PUSHED,
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
	int jumpTime;			//�W�����v�J�n����̎���[�t���[���P��]
	bool bJump;				//�W�����v���Ă��邩�ǂ���
	bool bHipDrop;			//�q�b�v�h���b�v���Ă��邩�ǂ���
	int nHipDropWait;		//�q�b�v�h���b�v�́u�J�n�E���n�v�d���J�E���^�[
	bool bNotMove;			//�ړ����Ă��Ȃ�
	int nRespawnPosNum;		//���������ʒu�ԍ�

	//�Փˊ֌W
	D3DXVECTOR3 faceCollider[2];	//�����蔻��
	int lastAtkPlayer;				//�Ō�ɐG�ꂽ�v���C���[

	//�p�����[�^��
	ANIMAL animal;			//�g�p���Ă��铮��
	int nScore;				//���_
	int nNumHitPlayer;		//�Ō�ɏՓ˂����v���C���[�i�����l-1�j
	PLAYERSTAT stat;		//���

	//�A�C�e����
	int nATKItemTime;		//�͂��������A�C�e���̎�������
	int nDEFItemTime;		//�������������A�C�e���̎�������
	int nGoastItemTime;		//�S�[�X�g���A�C�e���̎�������
	bool bMUTEKI;			//���G��Ԃ��ǂ����i�ϐ����͉��ł������ϐ����l���Ă���j
	int nMUTEKITime;		//���G�̎�������

	//�`���
	Model model;			//�g�p���f��
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

#endif // !_PLAYER_H_

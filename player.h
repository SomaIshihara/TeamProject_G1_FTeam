//==========================================
//
//�v���C���[�v���O�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//������ޗ񋓁i��U���̂����̂݁j
typedef enum
{
	ANIMAL_WILDBOAR = 0,	//���̂���
	ANIMAL_MAX
} ANIMAL;

//�v���C���[�\����
typedef struct
{
	//�ʒu��
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����

	//�����E�p�x
	float fLength;
	float fAngle;

	//�p�����[�^��
	ANIMAL animal;			//�g�p���Ă��铮��
	int nScore;				//���_
	int nNumHitPlayer;		//�Ō�ɏՓ˂����v���C���[�i�����l-1�j

	//�`���
	D3DXMATRIX mtxWorld;	//���[���h�}�g��
	int nIdxShadow;			//�e�ԍ�
	bool bUse;				//�g�p�̗L��
} Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

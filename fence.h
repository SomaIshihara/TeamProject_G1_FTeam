//==========================================
//
//�����h�~�t�F���X�v���O�����̃w�b�_[fence.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _FENCE_H_
#define _FENCE_H_

#include "main.h"

//�}�N��
#define MAX_USE_FENCE		(32)		//�t�F���X�ő�g�p��
#define FENCE_SCALE			(4.6f)		//�t�F���X�̊g��{��
#define FENCE_WIDTH			(12.0f)		//�t�F���X�̕�
#define FENCE_DEPTH			(1.0f)		//�t�F���X�̍���

//�t�F���X�p�\����
typedef struct
{
	//�ʒu�֌W
	D3DXVECTOR3 pos, posOld;	//�ʒu
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 rot;			//����
	int nFallTime;				//��������

	//�`��֌W
	D3DXMATRIX mtxWorld;		//���[���h�}�g��

	//�g�p�L��
	bool bUse;					//�g�p�L��
	bool bCollision;			//�����蔻��L��
} Fence;

//�v���g�^�C�v�錾
void InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);
void SetFence(float fLength, D3DXVECTOR3 rot);
Fence *GetFence(void);

#endif // !_FENCE_H_

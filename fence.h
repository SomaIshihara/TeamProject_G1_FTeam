//==========================================
//
//�����h�~�t�F���X�v���O�����̃w�b�_[fence.h]
//Author:�Ό��D�n  ���V����
//
//==========================================
#ifndef _FENCE_H_
#define _FENCE_H_

#include "main.h"

//�}�N��
#define MAX_USE_FENCE		(48)		//�t�F���X�ő�g�p��
#define FENCE_SCALE			(4.5f)		//�t�F���X�̊g��{��
#define FENCE_WIDTH			(12.0f)		//�t�F���X�̕�
#define FENCE_DEPTH			(1.0f)		//�t�F���X�̍���
#define FENCE_RADIUS		(650.0f)	//�t�F���X�̈ʒu

//�t�F���X�p�\����
typedef struct
{
	//�ʒu�֌W
	D3DXVECTOR3 pos, posOld;	//�ʒu�E�O��ʒu
	D3DXVECTOR3 rot;			//����
	float		fRadius;		//���_�ʒu����̋���
	float		fDropSpeed;		//�������x
	float		fFallSpeed;		//�|��鑬�x

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

void SetFencePos(D3DXVECTOR3 *pPos, float fRadius, float rotY);	//�t�F���X�̈ʒu�ݒ�
void DropFence(Fence *pFence, float FieldRadius);				//�t�F���X�̗�������
void FallFence(Fence *pFence, float FieldRadius);				//�t�F���X�̓|��鏈��
void SetFence(float fRadius, float rotY);
Fence *GetFence(void);

#endif // !_FENCE_H_

//==========================================
//
//�����h�~��v���O�����̃w�b�_[fence.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _FENCE_H_
#define _FENCE_H_

#include "main.h"

//�}�N��
#define MAX_USE_FENCE		(16)	//��ő�g�p��

//�v���g�^�C�v�錾
void InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);
void SetFence(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif // !_FENCE_H_

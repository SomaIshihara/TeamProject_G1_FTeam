//--------------------------------------------------------------------------------------------------------
//
//���C�g����[light.h]
//Author:�e�n�z�l
//
//--------------------------------------------------------------------------------------------------------
#ifndef _LIGHT_H_	//���̃}�N����`������ĂȂ�������
#define _LIGHT_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�v���g�^�C�v�錾
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
void SetLight(int nLightNum, D3DLIGHT9 light);

#endif

//--------------------------------------------------------------------------------------------------------
//
//���b�V���̒f�ʐ}[meshfault.h]
//Author:�e�n�z�l	���V����
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHFAULT_H_	//���̃}�N����`������ĂȂ�������
#define _MESHFAULT_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�}�N��
#define MESHFAULT_WIDTH				(2000.0f)	//�w�i�̍L��
#define MESHFAULT_BOTTOM			(-1000.0f)	//�w�i�̑���
#define MESHFAULT_SPLIT				(32)		//�w�i�̒��_��
#define MESHFAULT_TEX_RESOLUTION	(3.0f)		//�w�i�̉𑜓x

//�v���g�^�C�v�錾
void InitMeshFault(void);
void UninitMeshMeshFault(void);
void UpdateMeshMeshFault(void);
void DrawMeshFault(void);

#endif
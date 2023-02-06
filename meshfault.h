//--------------------------------------------------------------------------------------------------------
//
//���b�V���̒f�ʐ}[meshfault.h]
//Author:�e�n�z�l	���V����
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHFAULT_H_	//���̃}�N����`������ĂȂ�������
#define _MESHFAULT_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�}�N��
#define MESHFAULT_BOTTOM			(-600.0f)	//�w�i�̑���
#define MESHFAULT_SPLIT				(32)		//�w�i�̒��_��
#define MESHFAULT_TEX_RESOLUTION	(4.0f)		//�w�i�̉𑜓x

//�v���g�^�C�v�錾
void InitMeshFault(void);
void SetMeshFaultVertex(void);
void SetMeshFaultIndex(void);
void UninitMeshFault(void);
void UpdateMeshFault(void);
void DrawMeshFault(void);

#endif
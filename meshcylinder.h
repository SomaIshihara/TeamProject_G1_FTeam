//--------------------------------------------------------------------------------------------------------
//
//���b�V���V�����_�[�̔w�i[meshcylinder.h]
//Author:�e�n�z�l
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHCYLONDER_H_	//���̃}�N����`������ĂȂ�������
#define _MESHCYLONDER_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�}�N��
#define MESHSYLINDER_WIDTH		(500.0f)		//�w�i�̍L��
#define MESHSYLINDER_HEIGHT		(500.0f)		//�w�i�̍���
#define MESHSYLINDER_SPLIT		(32)		//�w�i�̒��_��

//�v���g�^�C�v�錾
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif

//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.h]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//���̃}�N����`������ĂȂ�������
#define _CHARGECYLINDER_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�}�N��
#define CHARGESYLINDER_WIDTH				(2000.0f)	//�w�i�̍L��
#define CHARGESYLINDER_HEIGHT				(1000.0f)	//�w�i�̍���
#define CHARGESYLINDER_SPLIT				(32)		//�w�i�̒��_��
#define CHARGESYLINDER_TEX_RESOLUTION		(3.0f)		//�w�i�̉𑜓x

//�v���g�^�C�v�錾
void InitChargeCylinder(void);
void UninitChargeCylinder(void);
void UpdateChargeCylinder(void);
void DrawChargeCylinder(void);

#endif
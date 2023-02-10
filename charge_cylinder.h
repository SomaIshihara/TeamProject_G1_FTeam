//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.h]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//���̃}�N����`������ĂȂ�������
#define _CHARGECYLINDER_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�}�N��
#define CHARGESYLINDER_WIDTH_MAX			(20.0f)	//�w�i�̍L���ő�
#define CHARGESYLINDER_WIDTH				(10.0f)	//�������J�n����̍L��
#define CHARGESYLINDER_HEIGHT				(10.0f)	//�w�i�̍���
#define CHARGESYLINDER_SPLIT				(32)		//�w�i�̒��_��
#define CHARGESYLINDER_TEX_RESOLUTION		(3.0f)		//�w�i�̉𑜓x

//�v���g�^�C�v�錾
void InitChargeCylinder(void);
void SetChargeCylinderVertex(int nCntCylinder);
void SetChargeCylinderIndex(void);
void UninitChargeCylinder(void);
void UpdateChargeCylinder(void);
void UpdateChargeCylinderSize(int nEffect);					//�G�t�F�N�g�̃T�C�Y�X�V
void DrawChargeCylinder(void);
void SetChargeCylinderPos(void);
void SetChargeCylinder(D3DXVECTOR3 pos, int nCntType);

#endif
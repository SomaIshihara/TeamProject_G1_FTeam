//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.h]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//���̃}�N����`������ĂȂ�������
#define _CHARGECYLINDER_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�}�N��
#define CHARGESYLINDER_RADIUS				(20.0f)	//�w�i�̍L��
#define CHARGESYLINDER_HEIGHT				(10.0f)	//�w�i�̍���
#define CHARGESYLINDER_SPLIT				(32)	//�w�i�̒��_��
#define CHARGESYLINDER_TEX_RESOLUTION		(3.0f)	//�w�i�̉𑜓x

//�v���g�^�C�v�錾
void InitChargeCylinder(void);					// ����������
void SetChargeCylinderVertex(void);				// ���_���̐ݒ菈��
void SetChargeCylinderIndex(void);				// �C���f�b�N�X�ԍ��̐ݒ菈��
void UninitChargeCylinder(void);				// �I������
void UpdateChargeCylinder(void);				// �X�V����
void SpreadChargeCylinder(int nCntCylinder);	// �`���[�W�V�����_�[�̍L���鏈��
void DrawChargeCylinder(void);					// �`�揈��

#endif
//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.h]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//���̃}�N����`������ĂȂ�������
#define _CHARGECYLINDER_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�v���g�^�C�v�錾
void InitChargeCylinder(void);					// ����������
void SetChargeCylinderVertex(int nCntCylinder);	// ���_���̐ݒ菈��
void SetChargeCylinderIndex(int nCntCylinder);	// �C���f�b�N�X�ԍ��̐ݒ菈��

void UninitChargeCylinder(void);			// �I������

void UpdateChargeCylinder(void);				// �X�V����
void SetChargeCylinderPos(int nCntPos);			// �v���C���[�̈ʒu�ɐݒ肷��
void SpreadChargeCylinder(int nCntSpread);		// �`���[�W�V�����_�[�̍L���鏈��
void ExtendChargeCylinder(int nCntExtend);		// �`���[�W�V�����_�[�̐L�яk�݂��鏈��

void DrawChargeCylinder(void);		// �`�揈��

void SetChargeCylinder(D3DXVECTOR3 pos, int nCntSet);		// �V�����_�[��ݒ�

#endif
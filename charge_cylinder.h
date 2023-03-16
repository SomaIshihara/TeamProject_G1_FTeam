//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.h]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//���̃}�N����`������ĂȂ�������
#define _CHARGECYLINDER_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�`���[�W�V�����_�[���̍\����
typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����
	float			fRadius;	// ���a�̑傫��
	int				nPlayerNo;	// �Ăяo�����v���C���[�ԍ����L��
	bool			bUse;		// �g���Ă��邩�ǂ���
}ChargeCylinder;

//�v���g�^�C�v�錾
void InitChargeCylinder(void);					// ����������
void SetChargeCylinderVertex(int nCntCylinder);	// ���_���̐ݒ菈��
void SetChargeCylinderIndex(int nCntCylinder);	// �C���f�b�N�X�ԍ��̐ݒ菈��

void UninitChargeCylinder(void);			// �I������

void UpdateChargeCylinder(void);						// �X�V����
void SetChargeCylinderPos(ChargeCylinder *pCylinder);	// �v���C���[�̈ʒu�ɐݒ肷��
void SpreadChargeCylinder(ChargeCylinder *pCylinder);	// �`���[�W�V�����_�[�̍L���鏈��

void DrawChargeCylinder(void);		// �`�揈��

void SetChargeCylinder(D3DXVECTOR3 pos, int nPlayerNo);		// �V�����_�[��ݒ�

#endif
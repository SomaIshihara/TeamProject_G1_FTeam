//--------------------------------------------------------------------------------------------------------
//
//���U���g�p�w�i[resultCylinder.h]
//Author:���V����
//
//--------------------------------------------------------------------------------------------------------
#ifndef _RESULTCYLINDER_H_	//���̃}�N����`������ĂȂ�������
#define _RESULTCYLINDER_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�v���g�^�C�v�錾
void InitResultCylinder(void);					// ����������
void SetResultCylinderVertex(int nCntCylinder);	// ���_���̐ݒ菈��
void SetResultCylinderIndex(int nCntCylinder);	// �C���f�b�N�X�ԍ��̐ݒ菈��

void UninitResultCylinder(void);	// �I������
void UpdateResultCylinder(void);	// �X�V����
void DrawResultCylinder(void);		// �`�揈��

#endif
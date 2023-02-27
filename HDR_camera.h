/*==========================================================================================================================================================

�J�����̏���[camera.h]
Author:��{����  ���V����  �Ό��D�n

============================================================================================================================================================*/
#ifndef _HDRCAMERA_H_
#define _HDRCAMERA_H_

#include "camera.h"
#include "main.h"

#define NUM_HDRCAMERA_HALF	(2)			//��ʕ������A�c�E���@�ǂ��炩�Q�����̏ꍇ�̃J�����̐�
#define NUM_HDRCAMERA		(4)			//�J�����̐�

//�`�悷��J�����̐� ��
typedef enum
{
	NumHDRCamera_ONLY = 0,			//�S�̘̂��Վ��_
	NumHDRCamera_HALF_SIDE,			//���ɕ������Q�̎��_
	NumHDRCamera_HALF_HIGH_row,		//�c�ɕ������Q�̎��_
	NumHDRCamera_FOUR_Separate,		//�e�v���C���[��������S�̎��_
	NumHDRCamera_MAX,
}NumHDRCamera;

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3		posV;			// ���_
	D3DXVECTOR3		posR;			// �����_
	D3DXVECTOR3		vecU;			// ������x�N�g��
	D3DXVECTOR3		rot;			// ����
	float			fLength;		// ����
	float			fMaxLength;		// �ő�`�拗��
	D3DXMATRIX		mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		mtxView;		// �r���[�}�g���b�N�X
	D3DVIEWPORT9	viewport;		// �r���[�|�[�g
	bool			bUse;			// �g�p����Ă��邩���Ȃ���
} HDRCamera;

//�v���g�^�C�v�錾
void InitSetHDRCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumHDRCamera);	//�J�����̈ʒu�ݒ�
void InitHDRCamera(NumCamera type);				//����������
void UninitHDRCamera(void);						//�I������
void UpdateHDRCamera(void);						//�X�V����
void SetHDRCamera(int nIdx);					//�ݒ菈��
void MoveHDRCamera(int nCntHDRCamera);			//�J�����̈ړ�����

void SetPosRHDRCamera(int nCntHDRCamera);		//�����_�ݒ�
void UpdatePosVHDRCamera(int nCntHDRCamera);	//���_�̈ʒu�X�V
HDRCamera *GetHDRCamera(void);					//�J�����̎擾

										//=========================================
										//�J�����̑䐔�ʁ@�ݒ菈��
										//=========================================
void Set_NumHDRCamera(NumCamera type);
void TPS_ChaseHDRCamera(int nCntHDRCamera, D3DXVECTOR3 rot);

#endif

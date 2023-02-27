/*==========================================================================================================================================================

�J�����̏���[camera.h]
Author:��{����  ���V����  �Ό��D�n

============================================================================================================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define NUM_CAMERA_HALF	(2)			//��ʕ������A�c�E���@�ǂ��炩�Q�����̏ꍇ�̃J�����̐�
#define NUM_CAMERA		(4)			//�J�����̐�

//�`�悷��J�����̐� ��
typedef enum
{
	NumCamera_ONLY = 0,			//�S�̘̂��Վ��_
	NumCamera_HALF_SIDE,		//���ɕ������Q�̎��_
	NumCamera_HALF_HIGH_row,	//�c�ɕ������Q�̎��_
	NumCamera_FOUR_Separate,	//�e�v���C���[��������S�̎��_
	NumCamera_FOUR_SIDE,
	NumCamera_MAX,
}NumCamera;

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
} Camera;

//�v���g�^�C�v�錾
void InitSetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera);	//�J�����̈ʒu�ݒ�
void InitCamera(NumCamera type);		//����������
void UninitCamera(void);				//�I������
void UpdateCamera(void);				//�X�V����
void SetCamera(int nIdx);				//�ݒ菈��
void MoveCamera(int nCntCamera);		//�J�����̈ړ�����

void SetPosRCamera(int nCntCamera);		//�����_�ݒ�
void UpdatePosVCamera(int nCntCamera);	//���_�̈ʒu�X�V
Camera *GetCamera(void);				//�J�����̎擾

//=========================================
//�J�����̑䐔�ʁ@�ݒ菈��
//=========================================
void Set_NumCamera(NumCamera type);
void TPS_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot);

void CameraForTitle(void);//�^�C�g���p�J��������

#endif
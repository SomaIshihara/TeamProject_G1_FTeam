/*==========================================================================================================================================================

�J�����̏���[camera.h]
Author:��{����

============================================================================================================================================================*/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define NUM_CAMERA		(4)			//�J�����̐�

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
} Camera;

//�v���g�^�C�v�錾
void InitCamera(void);					//����������
void UninitCamera(void);				//�I������
void UpdateCamera(void);				//�X�V����
void SetCamera(int nIdx);				//�ݒ菈��
void MoveCamera(void);					//�J�����̈ړ�����
void UpdatePosVCamera(int nCntCamera);	//���_�̈ʒu�X�V
Camera *GetCamera(void);				//�J�����̎擾
void SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera);		//�J�����̈ʒu�ݒ�

#endif
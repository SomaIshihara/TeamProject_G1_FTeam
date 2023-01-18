/*==========================================================================================================================================================

�J�����̏���[camera.h]
Author:��{����

============================================================================================================================================================*/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV; //���_
	D3DXVECTOR3 posR; //�����_
	D3DXVECTOR3 vecU; //������x�N�g��
	D3DXVECTOR3 rot; //����
	float fLength;	// ����
	D3DXMATRIX mtxProjection; //�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView; //�r���[�}�g���b�N�X
} Camera;

//�v���g�^�C�v�錾
void InitCamera(void);			//����������
void UninitCamera(void);		//�I������
void UpdateCamera(void);		//�X�V����
void SetCamera(void);			//�ݒ菈��
void MoveCamera(void);			//�J�����̈ړ�����
void UpdatePosVCamera(void);	//���_�̈ʒu�X�V
Camera *GetCamera(void);		//�J�����̎擾

#endif
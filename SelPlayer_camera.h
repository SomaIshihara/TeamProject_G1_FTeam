/*==========================================================================================================================================================

�J�����̏���[SelPlayer_camera.h]
Author:��{����  ���V����  �Ό��D�n

============================================================================================================================================================*/
#ifndef _SELPLAYER_CAMERA_H_
#define _SELPLAYER_CAMERA_H_

#include "main.h"

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
void InitSelPlayer_SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR);	//�J�����̈ʒu�ݒ�
void InitSelPlayer_Camera(void);
void UninitSelPlayer_Camera(void);				//�I������
void UpdateSelPlayer_Camera(void);				//�X�V����
void SetSelPlayer_Camera(void);				//�ݒ菈��
void MoveSelPlayer_Camera(void);		//�J�����̈ړ�����

void UpdateSelPlayer_PosVCamera(void);	//���_�̈ʒu�X�V
Camera *GetSelPlayer_Camera(void);				//�J�����̎擾

//=========================================
//�J�����̑䐔�ʁ@�ݒ菈��
//=========================================
void TPS_SelPlayer_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot);

void SelPlayer_CameraForTitle(void);//�^�C�g���p�J��������

#endif
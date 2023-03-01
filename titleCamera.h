/*==========================================================================================================================================================

���U���g�p�J��������[resultCamera.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _TITLECAMERA_H_
#define _TITLECAMERA_H_

#include "main.h"

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV;			//���_
	D3DXVECTOR3 posR;			//�����_
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXVECTOR3 rot;			//����
	float		fLength;		//���_�ƒ����_�̋���
	D3DXMATRIX	mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxview;		//�r���[�}�g���b�N�X
}TitleCamera;

void InitTitleCamera(void);
void UninitTitleCamera(void);
void UpdateTitleCamera(void);

//�J�����̈ړ�����
void MoveTitleCamera(D3DXVECTOR3 ParentPos, D3DXVECTOR3 *pChildPos, float CorRot);
void SpinTitlePosV(void);
void SetTitleCamera(void);
TitleCamera *GetTitleCamera(void);

#endif 
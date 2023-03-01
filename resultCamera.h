/*==========================================================================================================================================================

���U���g�p�J��������[resultCamera.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _RESULTCAMERA_H_
#define _RESULTCAMERA_H_

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
}ResultCamera;

void InitResultCamera(void);
void UninitResultCamera(void);
void UpdateResultCamera(void);

//�J�����̈ړ�����
void MoveResultCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot);
void MovePosV(void);
void SpinPosV(void);
void ResetResultCameraPos(void);
void FixResultCameraRot(void);
void SetResultCamera(void);
ResultCamera *GetResultCamera(void);
void LengthResultCamera(void);

#endif 
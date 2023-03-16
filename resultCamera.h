/*==========================================================================================================================================================

���U���g�p�J��������[resultCamera.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _RESULTCAMERA_H_
#define _RESULTCAMERA_H_

#include "main.h"

//�J�����ړ���WAVE
//*************************        WAVE����        *************************
// WAVE�P�F�v���C���[�P����v���C���[�S�܂ŃJ���������ړ������ăv���C���[��������
//			�S�o�������I��������	���̃E�F�[�u��
//
// WAVE�Q�F���_����O�ɉ����点�A�����_���Q�o�E�R�o�̊ԂɃX���C�h������
//			�S�o�̉E��O������ՂŌ���i�����_�̓v���C���[�Q�E�R�̊ԁj
//			���_�̈ʒu�ݒ肪�I�������S�v���C���[��O���ɃW�����v������
//			�W�����v���Ă�����x���������̂����͂�����	���̃E�F�[�u��
//
// WAVE�R�F�W�����v���Ă����莞�Ԍ�ɑS�v���C���[����ʓ��ɓ���悤�ɒǂ�������
//			����Ɉ�莞�Ԍ�ɑS�v���C���[���q�b�v�h���b�v������
//
// WAVE LAST�F�n�\�܂ŃJ�������ړ����ĕ\����ƃv���C���[�̒��n���d�Ȃ�悤�ɒ������āA�I��
//**************************************************************************
typedef enum
{
	WAVECamera_01_SideWays = 0,		//���ړ�
	WAVECamera_02_BirdEyes,			//���Վ��_	�i���Վ��_��󂷂Ɓubird's eys�v�Ƃ����炵��...
	WAVECamera_03_SkyDiving,		//��э~��̋�B
	WAVECamera_LAST_VictoryStand,	//�\����
	WAVECamera_FINISH,				//�S�E�F�[�u�I��
	WAVECamera_MAX
}WAVECamera;

//���U���g�J�����̃A�j���[�V�����\����
typedef struct
{
	int nFrameCounter;					//�ړ��o�ߎ���
	int nWholeFrame;					//�S�̃t���[��
	bool bEnd;							//�S�̃t���[���ɒB�������ǂ���
	int nWaitCount;						//�ҋ@�o�ߎ���
	int WaitTime;						//�ڕW�ʒu�ɒB������̑҂�����
	D3DXVECTOR3 StartPosV, StartPosR;	//���_�E�����_�̊J�n�ʒu
	D3DXVECTOR3 OffsetPosV, OffsetPosR;	//���_�E�����_�̖ڕW�ʒu
	D3DXVECTOR3 movePosV, movePosR;		//���_�E�����_�̈ړ���
}AnimResCamera;

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
void LoadWaveCamera(void);
void UninitResultCamera(void);
void UpdateResultCamera(void);

void WaveResultCamera(void);
void SpecialWave(void);
void SetNextWave(int nWave);

void SetResultCamera(void);
ResultCamera *GetResultCamera(void);
WAVECamera *GetWaveCamera(void);

#endif 
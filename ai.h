//==========================================
//
//AI�̃w�b�_[ai.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"
#include "input.h"

//AI�̃p�����[�^��ށi���Ƃ���Փx�j
typedef enum
{
	AIDIFF_EASY = 0,
	AIDIFF_NORMAL,
	AIDIFF_HARD,
	AIDIFF_VIEW,
	AIDIFF_MAX
} AIDIFF;

//AI�̃p�����[�^
typedef struct
{
	float fChargeAboutPlus;	//�`���[�W�̃A�o�E�g��i�v���X�j
	float fChargeAboutMinus;//�`���[�W�̃A�o�E�g��i�}�C�i�X�j
	int nHipdropRandom;		//�q�b�v�h���b�v�������m��
	int nHipdropTime;		//�W�����v���q�b�v�h���b�v�܂ł̎���
} AIParam;

//AI�iHDR�j�̃p�����[�^�[
typedef struct
{
	int nHDLevel_1;
	int nHDLevel_2;
	int nHDLevel_3;
	int nHDLevel_Max;
	int nAICT;
} AIParam_HDR;

//�R���s���[�^�[AI�̃p�����[�^�\����
typedef struct
{
	//�]�݂���
	AIDIFF difficulty;		//AI�̓�Փx

	//�`���[�W�n
	float fChargePower;		//�`���[�W��
	int nCounterWaitTime;	//�`���[�W�҂�����

	//�q�b�v�h���b�v����n
	bool bDoHipdrop;		//�q�b�v�h���b�v������邩�ǂ���
	bool bHipdropped;		//�q�b�v�h���b�v���������

} ComAIBrain;

#endif // !_AI_H_

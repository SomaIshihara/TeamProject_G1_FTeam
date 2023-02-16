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

//�R���s���[�^�[AI�̃X�e�B�b�N����
typedef enum
{
	AISTICK_LEFT = -1,
	AISTICK_NEUTRAL,
	AISTICK_RIGHT,
	AISTICK_MAX
} AISTICK;

//AI�̃p�����[�^��ށi���Ƃ���Փx�j
typedef enum
{
	AIDIFF_EASY = 0,
	AIDIFF_NORMAL,
	AIDIFF_HARD,
	AIDIFF_MAX
} AIDIFF;

//AI�̃R���g���[���[���
typedef struct
{
	bool bPushX, bPushXRele;//X�{�^��
	bool bPushA;			//A�{�^��
	AISTICK stick;			//�X�e�B�b�N
} AIControll;

//AI�̃p�����[�^
typedef struct
{
	float fChargeAboutPlus;	//�`���[�W�̃A�o�E�g��i�v���X�j
	float fChargeAboutMinus;//�`���[�W�̃A�o�E�g��i�}�C�i�X�j
	int nHipdropRandom;		//�q�b�v�h���b�v�������m��
	int nHipdropTime;		//�W�����v���q�b�v�h���b�v�܂ł̎���
} AIParam;

//�R���s���[�^�[AI�̃p�����[�^�\����
typedef struct
{
	//�R���g���[���[��
	AIControll controll;	//AI�����R���g���[���[�̏o��

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

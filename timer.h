//==========================================
//
//�^�C�}�[�v���O����[timer.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//�}�N����`
#define LIMIT_TIMER			(60)		//�Q�[������

#define TIMER_MINUS_X			(300.0f)	//�^�C�}�[�̃}�C�i�XX���W
#define TIMER_PLUS_X			(350.0f)	//�^�C�}�[�̃v���XX���W
#define TIMER_WIDTH_X			(50.0f)	    //�^�C�}�[�̕�

#define TIMER_MINUS_Y			(50.0f)		//�^�C�}�[�̃}�C�i�XY���W
#define TIMER_PLUS_Y			(150.0f)	//�^�C�}�[�̃v���XX���W

//��ʁi���[�h�j�̎��
typedef enum
{
	TIMERSTATE_NONE = 0,		//�ʏ���
	TIMERSTATE_STOP,			//��~���
	TIMERSTATE_END,				//�I�����
	TIMERSTATE_MAX
}TIMERSTATE;

//�\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;				//�^�C���̈ʒu
	int nTime;						//�^�C���̒l
	int nCounter;					//�^�C���̃J�E���^�[	
	int nDejit;						//����
	int nTemp;						//�^�C���̕ۑ��p
}TIME;

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
void SetTimerDejit(void);


#endif

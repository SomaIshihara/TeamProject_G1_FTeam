//==========================================
//
//�^�C�}�[�v���O����[timer.h]
//Author:�ѓc�F�� �Ό��D�n
//
//==========================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//�}�N����`
#define LIMIT_TIMER				(90)		//�Q�[������

#define TIMER_MINUS_X			(590.0f)						//�^�C�}�[�̃}�C�i�XX���W
#define TIMER_WIDTH_X			(50.0f)							//�^�C�}�[�̕�
#define TIMER_PLUS_X			(TIMER_MINUS_X + TIMER_WIDTH_X)	//�^�C�}�[�̃v���XX���W

#define TIMER_MINUS_Y			(20.0f)		//�^�C�}�[�̃}�C�i�XY���W
#define TIMER_PLUS_Y			(120.0f)	//�^�C�}�[�̃v���XX���W

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
	int nDigit;						//����
	int nTemp;						//�^�C���̕ۑ��p
}TIME;

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(MODE mode);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
void SetTimerDigit(void);
int GetTime(void);


#endif

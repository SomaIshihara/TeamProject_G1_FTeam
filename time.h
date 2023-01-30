#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

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
}TIME;

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);


#endif

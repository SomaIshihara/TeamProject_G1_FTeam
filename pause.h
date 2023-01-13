//==========================================
//
//�|�[�Y�̃w�b�_[pause.h]
//Author:�������P
//
//==========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//****************************//
//		�|�[�Y�̏��		  //
//****************************//
typedef enum
{
	PAUSE_CONTINUE = 0,
	PAUSE_RETRY,
	PAUSE_QUIT,
	PAUSE_MAX
}PAUSE;

//****************************//
//		�v���g�^�C�v�錾	  //
//****************************//
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(PAUSE Pause);
PAUSE *GetPause(void);

#endif 

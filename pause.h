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
	PAUSE_CONTINUE = 1,
	PAUSE_RETRY,
	PAUSE_MODESELECT,
	PAUSE_QUIT,
	PAUSE_MAX
}PAUSE;

//****************************//
//		�|�[�Y�̍\����		  //
//****************************//
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
}Pause;

//****************************//
//		�v���g�^�C�v�錾	  //
//****************************//
void InitPause(void);
void UninitPause(void);
void UpdatePause(MODE mode);
void DrawPause(void);
void SetPause(PAUSE Pause);
void SetPadPause(bool bDisconect, int GamePad = -1);
void SelectDownPause(int nPadNum);
void SelectUpPause(int nPadNum);
void UIStatePause(void);
PAUSE *GetPause(void);

#endif 

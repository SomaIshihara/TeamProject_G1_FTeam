//==========================================
//
//ポーズのヘッダ[pause.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//****************************//
//		ポーズの状態		  //
//****************************//
typedef enum
{
	PAUSE_CONTINUE = 0,
	PAUSE_RETRY,
	PAUSE_QUIT,
	PAUSE_MAX
}PAUSE;

//****************************//
//		プロトタイプ宣言	  //
//****************************//
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(PAUSE Pause);
PAUSE *GetPause(void);

#endif 

#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//画面（モード）の種類
typedef enum
{
	TIMERSTATE_NONE = 0,		//通常状態
	TIMERSTATE_STOP,			//停止状態
	TIMERSTATE_END,				//終了状態
	TIMERSTATE_MAX
}TIMERSTATE;

//構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;				//タイムの位置
}TIME;

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);


#endif

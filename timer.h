//==========================================
//
//タイマープログラム[timer.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//マクロ定義
#define LIMIT_TIMER			(60)		//ゲーム時間

#define TIMER_MINUS_X			(300.0f)	//タイマーのマイナスX座標
#define TIMER_PLUS_X			(350.0f)	//タイマーのプラスX座標
#define TIMER_WIDTH_X			(50.0f)	    //タイマーの幅

#define TIMER_MINUS_Y			(50.0f)		//タイマーのマイナスY座標
#define TIMER_PLUS_Y			(150.0f)	//タイマーのプラスX座標

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
	int nTime;						//タイムの値
	int nCounter;					//タイムのカウンター	
	int nDejit;						//桁数
	int nTemp;						//タイムの保存用
}TIME;

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
void AddTime(int nValue);
void SetTimerDejit(void);


#endif

/*==========================================================================================================================================================

ゲージの処理[gauge.h]
Author:大宮愛羅

============================================================================================================================================================*/

#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

//ゲージの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DCOLOR col;			//色	
	float fGaugeWidth;		//ゲージの幅
	float fWidth;			//幅(枠、背景)
	float fHeight;			//高さ(枠、背景)
	bool bUse;				//使用しているかどうか
} Gauge;

void InitGauge(void);		//初期化処理
void UninitGauge(void);		//終了処理
void UpdateGauge(void);		//更新処理
void DrawGauge(void);		//描画処理

#endif

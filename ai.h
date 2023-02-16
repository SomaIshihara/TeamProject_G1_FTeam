//==========================================
//
//AIのヘッダ[ai.h]
//Author:石原颯馬
//
//==========================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"
#include "input.h"

//コンピューターAIのスティック操作
typedef enum
{
	AISTICK_LEFT = -1,
	AISTICK_NEUTRAL,
	AISTICK_RIGHT,
	AISTICK_MAX
} AISTICK;

//AIのパラメータ種類（もとい難易度）
typedef enum
{
	AIDIFF_EASY = 0,
	AIDIFF_NORMAL,
	AIDIFF_HARD,
	AIDIFF_MAX
} AIDIFF;

//AIのコントローラー状態
typedef struct
{
	bool bPushX, bPushXRele;//Xボタン
	bool bPushA;			//Aボタン
	AISTICK stick;			//スティック
} AIControll;

//AIのパラメータ
typedef struct
{
	float fChargeAboutPlus;	//チャージのアバウト具合（プラス）
	float fChargeAboutMinus;//チャージのアバウト具合（マイナス）
	int nHipdropRandom;		//ヒップドロップ回避する確率
	int nHipdropTime;		//ジャンプ→ヒップドロップまでの時間
} AIParam;

//コンピューターAIのパラメータ構造体
typedef struct
{
	//コントローラー部
	AIControll controll;	//AIが持つコントローラーの出力

	//脳みそ部
	AIDIFF difficulty;		//AIの難易度

	//チャージ系
	float fChargePower;		//チャージ量
	int nCounterWaitTime;	//チャージ待ち時間

	//ヒップドロップ回避系
	bool bDoHipdrop;		//ヒップドロップ回避するかどうか
	bool bHipdropped;		//ヒップドロップ回避したか

} ComAIBrain;

#endif // !_AI_H_

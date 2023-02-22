//==========================================
//
//落下防止柵プログラムのヘッダ[fence.h]
//Author:石原颯馬
//
//==========================================
#ifndef _FENCE_H_
#define _FENCE_H_

#include "main.h"

//マクロ
#define MAX_USE_FENCE		(32)	//柵最大使用数

//フェンス用構造体
typedef struct
{
	//位置関係
	D3DXVECTOR3 pos, posOld;	//位置
	D3DXVECTOR3 rot;			//向き
	int nFallTime;				//落下時間

	//描画関係
	D3DXMATRIX mtxWorld;		//ワールドマトリ

	//使用有無
	bool bUse;					//使用有無
} Fence;

//プロトタイプ宣言
void InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);
void SetFence(float fLength, D3DXVECTOR3 rot);

#endif // !_FENCE_H_

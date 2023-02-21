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
#define MAX_USE_FENCE		(16)	//柵最大使用数

//プロトタイプ宣言
void InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);
void SetFence(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif // !_FENCE_H_

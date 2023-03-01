//==========================================
//
//ブロックプログラムのヘッダ[Block.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _RANK_H_
#define _RANK_H_

#include "main.h"

//プレイヤーの位置設定のため、ヘッダに移動しました
#define MAX_RANK			(4)		//最大数

//アイテム種類列挙型
typedef enum
{
	RANK_1ST = 0,
	RANK_2ND,
	RANK_3RD,
	RANK_4TH,
	RANK_MAX
}RANK;

//アイテムの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	RANK        rank;			//種類
	bool        buse;			//使用しているかどうか
}Rank;

//プロトタイプ宣言
void InitRank(void);
void UninitRank(void);
void UpdateRank(int nPlayer);
void DrawRank(void);

#endif

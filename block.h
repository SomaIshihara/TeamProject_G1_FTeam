//==========================================
//
//ブロックプログラムのヘッダ[Block.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//アイテム種類列挙型
typedef enum
{
	BLOCKTYPE_ATK = 0,		//はじき強化
	BLOCKTYPE_DEF,			//押し合い強化
	BLOCKTYPE_GHOST,		//ゴースト
	BLOCKTYPE_INVINCIBLE,	//無敵
	BLOCKTYPE_MAX
}BLOCKTYPE;

//アイテムの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	BLOCKTYPE	type;			//種類
	float		fWidth;			//
	int			nLife;			//
	bool        buse;			//使用しているかどうか
}Block;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(void);
void CollisionBlock(int nPlayerNum);//ブロックの当たり判定

#endif
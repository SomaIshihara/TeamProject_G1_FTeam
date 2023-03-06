//==========================================
//
//タイトルプログラムのヘッダ[SelPlayer.h]
//Author:石原颯馬
//
//==========================================
#ifndef _SELPLAYER_H_
#define _SELPLAYER_H_

#include "main.h"

//プレイヤー種類列挙
typedef enum
{
	PLAYERTYPE_PLAYER = 0,
	PLAYERTYPE_COM,
	PLAYERTYPE_NONE,
	PLAYERTYPE_MAX
} PLAYERTYPE;

//プロトタイプ宣言
void InitSelPlayer(void);
void UninitSelPlayer(void);
void UpdateSelPlayer(void);
void DrawSelPlayer(void);

#endif // !_SELPLAYER_H_

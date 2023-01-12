//==========================================
//
//プレイヤープログラムのヘッダ[player.h]
//Author:石原颯馬
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//動物種類列挙（一旦いのししのみ）
typedef enum
{
	ANIMAL_WILDBOAR = 0,	//いのしし
	ANIMAL_MAX
} ANIMAL;

//プレイヤー構造体
typedef struct
{
	//位置類
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き

	//長さ・角度
	float fLength;
	float fAngle;

	//パラメータ類
	ANIMAL animal;			//使用している動物
	int nScore;				//得点
	int nNumHitPlayer;		//最後に衝突したプレイヤー（初期値-1）

	//描画類
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	int nIdxShadow;			//影番号
	bool bUse;				//使用の有無
} Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

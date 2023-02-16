/*==========================================================================================================================================================

壁の処理[wall.h]
Author:大宮愛羅

============================================================================================================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//壁の状態
typedef enum
{
	WALLSTATE_NORMAL = 0,
	WALLSTATE_MAX
} WALLSTATE;

//壁の状態
typedef enum
{
	WALLANGLE_BACK = 0,
	WALLANGLE_FRONT,
	WALLANGLE_LEFT,
	WALLANGLE_RIGHT,
	WALLANGLE_MAX
} WALLANGLE;

//壁の読み込み
typedef enum
{
	WALLCASE_TYPE = 0,
	WALLCASE_WALLANGLE,
	WALLCASE_WIDTH,
	WALLCASE_HEIGHT,
	WALLCASE_DEPTH,
	WALLCASE_POSX,
	WALLCASE_POSY,
	WALLCASE_POSZ,
	WALLCASE_MAX
}	WALLCASE;

//壁構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//壁の位置
	D3DXVECTOR3 rot;		//壁の向き
	WALLANGLE wallAngle;	//向きの種類
	int nType;				//壁の種類	
	float fWidth;			//壁の幅
	float fHeight;			//壁の高さ
	float fDepth;			//壁の奥行き
	bool bUse;				//壁を使用しているかどうか
} Wall;

//プロトタイプ宣言
void InitWall(void); //初期化処理
void UninitWall(void); //終了処理
void UpdateWall(void); //更新処理
void DrawWall(void); //描画処理
void LoadWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

#endif

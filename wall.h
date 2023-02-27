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
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	WALLANGLE wallAngle;	// 向きの種類
	int nType;				// 種類
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fDepth;			// 奥行き
	float fAlpha;			// αカラー
	bool bSideVibe;			// 左右に震えたかどうか		false：左に震えている	true：右に震えている
	float fSideVibe;		// 震えの幅
	bool bCollision;		// 当たったかどうか
	bool bDisp;				// 描画するかどうか
} Wall;

//プロトタイプ宣言
void InitWall(void);		// 初期化処理
void UninitWall(void);		// 終了処理
void UpdateWall(void);		// 更新処理
void DrawWall(void);		// 描画処理

void LoadWall(void);		// 壁の情報読み込み
void SetWallVertex(void);	// 壁の頂点情報設定処理

bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);	//壁の当たり判定
void CollapseWall(int nCntWall);	//壁の崩れ去る処理

#endif

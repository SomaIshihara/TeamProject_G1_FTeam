//==========================================
//
//プレイヤープログラムのヘッダ[player.h]
//Author:石原颯馬
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "model.h"

//プレイヤーモデルの構造体
typedef struct
{
	int nIdxModel;			//インデックス番号
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	LPD3DXMESH pMesh;		//メッシュ
	LPD3DXBUFFER pBuffMat;	//マテリアルポインタ
	DWORD dwNumMat;			//マテリアルの数

}PlayModel;

//プレイヤー構造体
typedef struct
{
	//位置類
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	float moveGauge;		//移動量(ダッシュ)

	//長さ・角度
	float fLength;
	float fAngle;

	//パラメータ類
	ANIMAL animal;			//使用している動物
	int nScore;				//得点
	int nNumHitPlayer;		//最後に衝突したプレイヤー（初期値-1）

	//描画類
	Model model;			//使用モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	int nIdxShadow;			//影番号
	bool bUse;				//使用の有無

	PlayModel aPlaymodel[10];

} Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

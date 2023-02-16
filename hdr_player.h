//==========================================
//
//プレイヤープログラムのヘッダ[hdr_player.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _HDR_PLAYER_H_
#define _HDR_PLAYER_H_

#include "main.h"
#include "model.h"
#include "input.h"

//プレイヤーモデルの構造体
typedef struct
{
	int nIdxModel;			//インデックス番号
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	LPD3DXMESH pMesh;		//メッシュ
	LPD3DXBUFFER pBuffMat;	//マテリアルポインタ
	DWORD dwNumMat;			//マテリアルの数

}PlayModel_HDR;

//プレイヤー構造体
struct Player_HDR
{
	//位置類
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	int jumpTime;			//ジャンプ開始からの時間[フレーム単位]
	bool bJump;				//ジャンプしているかどうか
	bool bHipDrop;			//ヒップドロップしているかどうか
	bool bHipDropSpin;		//ヒップドロップのスピンしたかどうか
	int nHipDropWait;		//ヒップドロップの「開始・着地」硬直カウンター

	//パラメータ類
	ANIMAL animal;			//使用している動物

	//描画類
	Model model;			//使用モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	bool bUseShadow;		//影使用の有無
	bool bUsePlayer;		//プレイヤー使用の有無

	PlayModel_HDR aPlaymodel[10];
};

//プロトタイプ宣言
void InitPlayer_HDR(void);
void UninitPlayer_HDR(void);
void UpdatePlayer_HDR(void);
void DrawPlayer_HDR(void);
Player_HDR *GetPlayer_HDR(void);

#endif
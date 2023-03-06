//==========================================
//
//プレイヤープログラムのヘッダ[hdr_player.h]
//Author:平澤詩苑　石原颯馬
//
//==========================================
#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "main.h"
#include "model.h"
#include "PvP_player.h"
#include "hdr_player.h"
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

}PlayModel_RESULT;

//プレイヤー構造体
struct Player_RESULT
{
	//位置類
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	bool bHipDrop;			//ヒップドロップしているかどうか
	bool bHipDropSpin;		//ヒップドロップのスピンしたかどうか
	int nHipDropWait;		//ヒップドロップの「開始・着地」硬直カウンター
	int nRank;				//順位を格納

	//パラメータ類
	ANIMAL animal;			//使用している動物

							//描画類
	Model model;			//使用モデル
	AnimalInstance animalInst[MAX_PARTS];
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	bool bUseShadow;		//影使用の有無
	bool bUsePlayer;		//プレイヤー使用の有無

	PlayModel_RESULT aPlaymodel[10];
};

//プロトタイプ宣言
void InitPlayer_RESULT(void);
void InitResultPlayerType_GAMEMODE_PVP(Player_RESULT *pResPlayer);
void InitResultPlayerType_GAMEMODE_HDR(Player_RESULT *pResPlayer);
void UninitPlayer_RESULT(void);
void UpdatePlayer_RESULT(void);
void DrawPlayer_RESULT(void);
Player_RESULT *GetPlayer_RESULT(void);

#endif
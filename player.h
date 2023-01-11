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

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	float fLength;
	float fAngle;
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	Model aModel[EXITHUMAN_MODEL_NUM];		//モデル
	int nNumModel;			//パーツ総数
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	bool bIntoSafeArea;		//セーフエリアに入っているか
	bool bInfection;		//感染しているか（提出分は敵に当たったかの判定のみ）
	Motion mMotion;			//モーション
} Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

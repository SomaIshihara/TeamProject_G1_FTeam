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

//マクロ
#define PLAYER_POWER_LEVEL	(3)			//パワーレベル
#define PLAYER_POWER_MAX	(0.5f * PLAYER_POWER_LEVEL)		//移動の強さの最大値

//プレイヤー状態列挙
typedef enum
{
	PLAYERSTAT_WAIT = 0,
	PLAYERSTAT_CHARGE,
	PLAYERSTAT_DASH,
	PLAYERSTAT_PUSHED,
	PLAYERSTAT_FALL,
	PLAYERSTAT_MAX
} PLAYERSTAT;

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
struct Player
{
	//位置類
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 moveV0;		//初期移動量
	D3DXVECTOR3 rot;		//向き
	float moveGauge;		//移動量(ダッシュ)
	int jumpTime;			//ジャンプ開始からの時間[フレーム単位]
	bool bJump;				//ジャンプしているかどうか
	bool bHipDrop;			//ヒップドロップしているかどうか
	int nHipDropWait;		//ヒップドロップの「開始・着地」硬直カウンター
	bool bNotMove;			//移動していない
	int nRespawnPosNum;		//復活した位置番号

	//衝突関係
	D3DXVECTOR3 faceCollider[2];	//当たり判定
	int lastAtkPlayer;				//最後に触れたプレイヤー

	//パラメータ類
	ANIMAL animal;			//使用している動物
	int nScore;				//得点
	int nNumHitPlayer;		//最後に衝突したプレイヤー（初期値-1）
	PLAYERSTAT stat;		//状態

	//アイテム類
	int nATKItemTime;		//はじき強化アイテムの持続時間
	int nDEFItemTime;		//押し合い強化アイテムの持続時間
	int nGoastItemTime;		//ゴースト化アイテムの持続時間
	bool bMUTEKI;			//無敵状態かどうか（変数名は仮ですいい変数名考えてくれ）
	int nMUTEKITime;		//無敵の持続時間

	//描画類
	Model model;			//使用モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	bool bUseShadow;		//影使用の有無
	bool bUsePlayer;		//プレイヤー使用の有無

	PlayModel aPlaymodel[10];
};

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

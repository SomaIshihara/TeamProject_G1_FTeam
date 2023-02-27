//==========================================
//
//プレイヤー関係プログラムのヘッダ[player.h]
//Author:石原颯馬
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//マクロ
#define PLAYER_SIZE_WIDTH	(35)	//プレイヤーのポリゴンサイズ（幅）
#define PLAYER_SIZE_HEIGHT	(75)	//プレイヤーのポリゴンサイズ（高さ）

//プレイヤー型構造体定義
typedef struct
{
	D3DXVECTOR3 pos;					//現在の位置（実際の位置）
	D3DXVECTOR3 posOld;					//前回の位置（実際の位置）
	D3DXVECTOR3 posScroll;				//カメラ範囲から移動した量
	D3DXVECTOR3 move;					//移動量	
	D3DXVECTOR3 rot;					//向き
	int nCounterJumpTime;				//ジャンプボタンを押したときの時間
	int nCounterAnim;					//アニメーションカウンタ
	int nPatternAnim;					//アニメーションパターンNo.
	int nDirectionMove;					//移動方向
	int nHaveBlock;						//設置可能ブロック数
	bool bJump;							//ジャンプしているか
	bool bDisp;							//表示するか
} Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void DrawPos(void);
void DrawSpeed(void);

#endif // !_PLAYER_H_

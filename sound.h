//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 石原颯馬
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//マクロ定義
#define NUM_DUPLICATION	(1)		//重複数

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// タイトル音
	SOUND_LABEL_BGM_TUTORIAL,		// チュートリアル音
	SOUND_LABEL_BGM_GAME,			// ゲーム音
	SOUND_LABEL_BGM_RESULT,			// リザルト音
	SOUND_LABEL_SE_CONNECT,			// コントローラー接続時の効果音
	SOUND_LABEL_SE_TITLE_DECIDE,	// タイトル画面の決定音
	SOUND_LABEL_SE_COLLISION,		// プレイヤー同士の衝突音
	SOUND_LABEL_SE_ENERGY_00,		// プレイヤー1のゲージ充填音
	SOUND_LABEL_SE_ENERGY_01,		// プレイヤー2のゲージ充填音
	SOUND_LABEL_SE_ENERGY_02,		// プレイヤー3のゲージ充填音
	SOUND_LABEL_SE_ENERGY_03,		// プレイヤー4のゲージ充填音
	SOUND_LABEL_SE_DROP,			// プレイヤーの落下音
	SOUND_LABEL_SE_GRASSDASH,		// プレイヤーのダッシュ音
	SOUND_LABEL_SE_HIPDROP,			// プレイヤーのヒップドロップ音
	SOUND_LABEL_SE_JUMP,			// プレイヤーのジャンプ音
	SOUND_LABEL_SE_PAUSE_DECISION,	// ポーズの決定音
	SOUND_LABEL_SE_PAUSE_SELECTION,	// ポーズの選択音
	SOUND_LABEL_SE_PAUSE_TRANSITION,// ポーズの遷移音
	SOUND_LABEL_SE_ROAR,			// ポーズの咆哮音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label/*, int nNumSound*/);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
void RestartSound(SOUND_LABEL label);

#endif

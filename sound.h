//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 石原颯馬
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_CONNECT = 0,		// コントローラー接続時の効果音
	SOUND_LABEL_SE_TITLE_DECIDE,	// タイトル画面の決定音
	SOUND_LABEL_SE_COLLISION,		// プレイヤー同士の衝突音
	SOUND_LABEL_SE_ENERGY_00,		// プレイヤー1のゲージ充填音
	SOUND_LABEL_SE_ENERGY_01,		// プレイヤー2のゲージ充填音
	SOUND_LABEL_SE_ENERGY_02,		// プレイヤー3のゲージ充填音
	SOUND_LABEL_SE_ENERGY_03,		// プレイヤー4のゲージ充填音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
void RestartSound(SOUND_LABEL label);

#endif

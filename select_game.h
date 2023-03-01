/*==========================================================================================================================================================

//ゲームメニューの選択処理[select_game.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _SELECT_GAME_H_
#define _SELECT_GAME_H_

//選択できるゲームモード
typedef enum
{
	SelectGameMode_PVP = 0,	//落とし合うゲーム
	SelectGameMode_HDR,		//ヒップドロップレースゲーム
	SelectGameMode_MAX
}SelectGameMode;

//選択メニュー
typedef enum
{
	SelectGameMenu_BG = 0,
	SelectGameMenu_PVP,
	SelectGameMenu_HDR,
	SelectGameMenu_MAX
}SelectGameMenu;

//ゲーム選択情報の構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float		fWidth;		// 幅
	float		fHeight;	// 高さ
	bool		bChoice;	// 選択されているかどうか
}SelectGame;

//プロトタイプ宣言
void InitSelectGame(void);
void LoadSelectGame(void);
void SetVertexSelectGame(void);

void UninitSelectGame(void);
void UpdateSelectGame(void);
void DrawSelectGame(void);

void ChooseGameMode(void);
void DecideGameMode(void);
SelectGameMode GetSelectGameMode(void);

#endif 
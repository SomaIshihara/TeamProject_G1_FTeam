//==========================================
//
//プレイヤー選択プログラム[SelPlayer.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "SelPlayer.h"
#include "TypeFrame.h"
#include "conversioninput.h"
#include "model.h"

//========================
//初期化処理
//========================
void InitSelPlayer(void)
{
	InitTypeFrame();
}

//========================
//終了処理
//========================
void UninitSelPlayer(void)
{
	UninitTypeFrame();
}

//========================
//更新処理
//========================
void UpdateSelPlayer(void)
{
	UpdateTypeFrame();
}

//========================
//描画処理
//========================
void DrawSelPlayer(void)
{
	DrawTypeFrame();
}
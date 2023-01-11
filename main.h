/*==========================================================================================================================================================

メインの処理[main.h]
Author:大宮愛羅

============================================================================================================================================================*/

#ifndef _MAIN_H_						//このマクロ定義を一度もされていなかったら
#define _MAIN_H_

#include <Windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"
#include <stdlib.h>
#include <stdio.h>

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

//マクロ定義
#define SCREEN_WIDTH	(1280)			   //ウインドウの幅
#define SCREEN_HEIGHT	(720)	           //ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//頂点情報(2D)構造体
typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	float rhw;		 //座標変換用係数(1.0fで固定)
	D3DCOLOR col;	 //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
} VERTEX_2D;

//頂点情報(3D)構造体
typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col;	 //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
} VERTEX_3D;

//画面(モード)の種類
typedef enum
{
	MODE_NONE = 0,
	MODE_TITLE,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_GAMERESULT,
	MODE_MAX
} MODE;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void); //デバイス
void SetMode(MODE mode); //画面のモード設定
MODE GetMode(void); //画面モードの取得

#endif
#pragma once

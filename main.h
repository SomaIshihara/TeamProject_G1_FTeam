//==========================================
//
//ゲーム全般を管理するプログラムのヘッダ[main.h]
//Author:石原颯馬
//
//==========================================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <XInput.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	//ビルド時の警告対処
#include "dinput.h"
#include "xaudio2.h"

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

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

//マクロ定義
#define CLASS_NAME			"WindowClass"
#define SCREEN_WIDTH		(1280)		//クライアント領域の幅
#define SCREEN_HEIGHT		(720)		//クライアント領域の高さ
#define MAX_FPS				(60)		//最大フレームレート
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//2D
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//3D
#define FIX_ROT(x)			((float)fmod(x + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI)	//角度を-PI~PIに修正

//頂点情報[2D]
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換係数（1.0fで固定）
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
} VERTEX_2D;

//頂点情報[3D]
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//カラー
	D3DXVECTOR2 tex;	//テクスチャ
} VERTEX_3D;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetShowCursor(bool bDisp);		//カーソル表示・非表示切り替え（trueで表示・falseで非表示）
void SetMode(MODE mode); //画面のモード設定
MODE GetMode(void); //画面モードの取得

#endif // !_MAIN_H_

//==========================================
//
//ポーズのヘッダ[pause.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//****************************//
//		ポーズの状態		  //
//****************************//
typedef enum
{
	PAUSE_CONTINUE = 0,
	PAUSE_RETRY,
	PAUSE_QUIT,
	PAUSE_MAX
}PAUSE;

//****************************//
//		ポーズの構造体		  //
//****************************//
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 VtxZero;		//Vtx[0]頂点座標
	D3DXVECTOR3 VtxOne;			//Vtx[1]頂点座標
	D3DXVECTOR3 VtxTwo;			//Vtx[2]頂点座標
	D3DXVECTOR3 VtxThree;		//Vtx[3]頂点座標
}Pause;

//****************************//
//		プロトタイプ宣言	  //
//****************************//
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPause(PAUSE Pause);
void LoadPause(void);
PAUSE *GetPause(void);

#endif 

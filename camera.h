/*==========================================================================================================================================================

カメラの処理[camera.h]
Author:大宮愛羅

============================================================================================================================================================*/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define NUM_CAMERA		(4)			//カメラの数

//カメラの構造体
typedef struct
{
	D3DXVECTOR3		posV;			// 視点
	D3DXVECTOR3		posR;			// 注視点
	D3DXVECTOR3		vecU;			// 上方向ベクトル
	D3DXVECTOR3		rot;			// 向き
	float			fLength;		// 距離
	float			fMaxLength;		// 最大描画距離
	D3DXMATRIX		mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX		mtxView;		// ビューマトリックス
	D3DVIEWPORT9	viewport;		// ビューポート
} Camera;

//プロトタイプ宣言
void InitCamera(void);					//初期化処理
void UninitCamera(void);				//終了処理
void UpdateCamera(void);				//更新処理
void SetCamera(int nIdx);				//設定処理
void MoveCamera(void);					//カメラの移動処理
void UpdatePosVCamera(int nCntCamera);	//視点の位置更新
Camera *GetCamera(void);				//カメラの取得
void SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera);		//カメラの位置設定

#endif
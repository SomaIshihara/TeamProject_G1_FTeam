/*==========================================================================================================================================================

リザルト用カメラ処理[resultCamera.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _RESULTCAMERA_H_
#define _RESULTCAMERA_H_

#include "main.h"

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXVECTOR3 rot;			//向き
	float		fLength;		//視点と注視点の距離
	D3DXMATRIX	mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX	mtxview;		//ビューマトリックス
}ResultCamera;

void InitResultCamera(void);
void UninitResultCamera(void);
void UpdateResultCamera(void);

//カメラの移動処理
void MoveResultCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot);
void MovePosV(void);
void SpinPosV(void);
void ResetResultCameraPos(void);
void FixResultCameraRot(void);
void SetResultCamera(void);
ResultCamera *GetResultCamera(void);
void LengthResultCamera(void);

#endif 
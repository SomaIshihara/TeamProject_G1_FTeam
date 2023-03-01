/*==========================================================================================================================================================

リザルト用カメラ処理[resultCamera.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "resultCamera.h"
#include "input.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 100.0f, -100.0f)	//視点の初期位置
#define POS_POSR			D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//注視点の初期位置
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//上方向ベクトルの初期値
#define VIEW_ANGLE			(45.0f)								//視野角
#define MOVE_CAMERA_SPEED	(2.0f)								//カメラ・注視点座標の移動量
#define MIN_DRAW_Z			(10.0f)								//描画処理の最低奥行き
#define MAX_DRAW_Z			(2000.0f)							//描画処理の最高奥行き
#define MIN_LENGTH			(100.0f)							//視点と注視点の最低距離
#define MAX_LENGTH			(300.0f)							//視点と注視点の最大距離
#define CHANGE_LENGHT		(0.5f)								//長さの移動量
#define SPIN_ROT			(0.05f)								//回転量
#define COR_ROT_POSV		(3.14f)								//角度計算による視点操作の調整値
#define COR_ROT_POSR		(0.0f)								//角度計算による注視点操作の調整値
#define DRAW_LENGTH			(500.0f)							//オブジェクトを描画させてあげる範囲

//グローバル変数
ResultCamera				g_ResultCamera;							//カメラの情報

//------------------------------------------------
//			リザルト用カメラの初期化処理
//------------------------------------------------
void InitResultCamera(void)
{
	ResetResultCameraPos();

	g_ResultCamera.vecU = POS_VECU;

	D3DXVECTOR3 fDiffPos = g_ResultCamera.posR - g_ResultCamera.posV;	//視点と注視点の位置の差
	fDiffPos.x = powf(fDiffPos.x, 2.0f);					//視点同士の位置の差を２乗
	fDiffPos.z = powf(fDiffPos.z, 2.0f);					//視点同士の位置の差を２乗

	g_ResultCamera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y方向の角度を計算

	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = powf(g_ResultCamera.posR.x - g_ResultCamera.posV.x, 2.0f);	//2乗
	PosDiffY = powf(g_ResultCamera.posR.y - g_ResultCamera.posV.y, 2.0f);	//2乗
	PosDiffZ = powf(g_ResultCamera.posR.z - g_ResultCamera.posV.z, 2.0f);	//2乗

	g_ResultCamera.fLength = sqrtf(PosDiffX + PosDiffY + PosDiffZ);	//長さを算出

}

//------------------------------------------------
//			リザルト用カメラの終了処理
//------------------------------------------------
void UninitResultCamera(void)
{

}

//------------------------------------------------
//			リザルト用カメラの更新処理
//------------------------------------------------
void UpdateResultCamera(void)
{

}

//視点移動
void MovePosV(void)
{

}

//視点回転
void SpinPosV(void)
{

}

//角度修正
void FixResultCameraRot(void)
{

}

//カメラの移動処理
void MoveResultCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = pParentPos->x + sinf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
	pChildPos->z = pParentPos->z + cosf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
}

//カメラの位置リセット
void ResetResultCameraPos(void)
{
	g_ResultCamera.posR = ZERO_SET;
	g_ResultCamera.posV = POS_POSV;
}

//視点間の長さ処理
void LengthResultCamera(void)
{

}

//------------------------------------------------
//			リザルト用カメラの設定処理
//------------------------------------------------
void SetResultCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

												//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&g_ResultCamera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_ResultCamera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_ResultCamera.mtxProjection);

	//ビューマトリックス
	D3DXMatrixIdentity(&g_ResultCamera.mtxview);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_ResultCamera.mtxview,
		&g_ResultCamera.posV,		//視点
		&g_ResultCamera.posR,		//注視点
		&g_ResultCamera.vecU);	//上方向ベクトル

							//ビューマトリックス
	pDevice->SetTransform(D3DTS_VIEW, &g_ResultCamera.mtxview);
}

//リザルト用カメラの情報取得
ResultCamera *GetResultCamera(void)
{
	return &g_ResultCamera;
}
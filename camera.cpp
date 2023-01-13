/*==========================================================================================================================================================

カメラの処理[camera.cpp]
Author:大宮愛羅

============================================================================================================================================================*/

#include "camera.h"
#include "input.h"
#include "player.h"

//注視点情報
#define POSR_SPEED	(10.0f)			//注視点の移動速度
#define POSR_WIDTH	(0.0f)			//注視点の幅
#define POSR_HEIGHT	(0.0f)			//注視点の高さ
#define POSR_DEPTH	(0.0f)			//注視点の奥行き

//視点情報
#define POSV_ROTSPEED	(0.05f)		//視点の回転速度
#define POSV_SPEED		(10.0f)		//視点の移動速度
#define POSV_ADD		(400)		//視点の上下移動
#define POSV_DISTANCE	(100)		//視点の距離
#define POSV_WIDTH		(0.0f)		//視点の幅
#define POSV_HEIGHT		(50.0f)		//視点の高さ
#define POSV_DEPTH		(0.0f)		//視点の奥行き

//上方向ベクトル情報
#define VECU_WIDTH      (0.0f)		//上方向ベクトルの幅
#define VECU_HEIGHT		(1.0f)		//上方向ベクトルの高さ
#define VECV_DEPTH		(0.0f)		//上方向ベクトルの奥行き

//カメラ情報
#define CAMERA_ROT		(45.0f)		//カメラの向き
#define CAMERA_POS		(10.0f)		//カメラの位置

//描画範囲
#define DRAWING_AREA	(5000.0f)	//最大描画範囲

//位置の差分を2乗
#define DIFF_TIMES		(2.0f)		//2乗


//グローバル変数
Camera g_Camera;						//カメラの情報
float  g_MaxDrawCamera = DRAWING_AREA;	//最大描画範囲

//カメラの初期化
void InitCamera(void)
{
	g_Camera.posV = D3DXVECTOR3(POSV_WIDTH, POSV_HEIGHT, POSV_DEPTH);	//視点
	g_Camera.posR = D3DXVECTOR3(POSR_WIDTH, POSR_HEIGHT, POSR_DEPTH);	//注視点
	g_Camera.vecU = D3DXVECTOR3(VECU_WIDTH, VECU_HEIGHT, VECV_DEPTH);	//上方向ベクトル

	Player * pPlayer = GetPlayer();

	//視点
	g_Camera.posV.x = pPlayer->pos.x;					//視点X
	g_Camera.posV.y = pPlayer->pos.y + POSV_ADD;		//視点Y
	g_Camera.posV.z = pPlayer->pos.z - POSV_DISTANCE;	//視点Z

	//注視点
	g_Camera.posR.x = pPlayer->pos.x;		//注視点X
	g_Camera.posR.y = pPlayer->pos.y;		//注視点Y
	g_Camera.posR.z = pPlayer->pos.z;		//注視点Z

	//それぞれの位置の差分を格納する変数
	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = pow(g_Camera.posR.x - g_Camera.posV.x, DIFF_TIMES);	//２乗
	PosDiffY = pow(g_Camera.posR.y - g_Camera.posV.y, DIFF_TIMES);	//２乗
	PosDiffZ = pow(g_Camera.posR.z - g_Camera.posV.z, DIFF_TIMES);	//２乗

	//長さの算出
	g_Camera.fLength = sqrtf(PosDiffX + PosDiffY + PosDiffZ);
}

//カメラの終了処理
void UninitCamera(void)
{

}

//カメラの更新処理
void UpdateCamera(void)
{

}


//カメラの設定処理
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, D3DXToRadian(CAMERA_ROT), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, CAMERA_POS, g_MaxDrawCamera);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.posV, &g_Camera.posR, &g_Camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//カメラの取得
Camera *GetCamera(void)
{
	return &g_Camera;
}
/*==========================================================================================================================================================

カメラの処理[camera.cpp]
Author:大宮愛羅

============================================================================================================================================================*/

#include "camera.h"
#include "input.h"
#include "player.h"

//注視点移動速度(上下)
#define POSR_SPEED	(10.0f)

//視点移動速度(上下・左右)
#define POSV_ROTSPEED	(0.05f)
#define POSV_SPEED	(10.0f)

//視点の距離
#define POSV_DISTANCE	(1000)
#define POSV_UP			(400)

//グローバル変数
Camera g_Camera;					//カメラの情報
float  g_MaxDrawCamera = 50000.0f;	//最大描画範囲

									//カメラの初期化
void InitCamera(void) //初期化処理
{
	g_Camera.posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	Player * pPlayer = GetPlayer();

	//視点
	g_Camera.posV.x = pPlayer->pos.x;
	g_Camera.posV.y = pPlayer->pos.y + POSV_UP;
	g_Camera.posV.z = pPlayer->pos.z - POSV_DISTANCE;

	//注視点
	g_Camera.posR.x = pPlayer->pos.x;
	g_Camera.posR.y = pPlayer->pos.y;
	g_Camera.posR.z = pPlayer->pos.z;

	//それぞれの位置の差分を格納する変数
	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = pow(g_Camera.posR.x - g_Camera.posV.x, 2.0f);	//２乗
	PosDiffY = pow(g_Camera.posR.y - g_Camera.posV.y, 2.0f);	//２乗
	PosDiffZ = pow(g_Camera.posR.z - g_Camera.posV.z, 2.0f);	//２乗

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
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, g_MaxDrawCamera);

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
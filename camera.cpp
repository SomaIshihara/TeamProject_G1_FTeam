/*==========================================================================================================================================================

カメラの処理[camera.cpp]
Author:大宮愛羅

============================================================================================================================================================*/
#include "camera.h"
#include "input.h"
#include "player.h"

//注視点情報
#define POSR_SPEED		(10.0f)		//注視点の移動速度
#define POSR_WIDTH		(0.0f)		//注視点の幅
#define POSR_HEIGHT		(0.0f)		//注視点の高さ
#define POSR_DEPTH		(0.0f)		//注視点の奥行き

//視点情報
#define POSV_ROTSPEED	(0.05f)		//視点の回転速度
#define POSV_SPEED		(10.0f)		//視点の移動速度
#define POSV_ADD		(100)		//視点の上下移動
#define POSV_DISTANCE	(100)		//視点の距離
#define POSV_WIDTH		(0.0f)		//視点の幅
#define POSV_HEIGHT		(1050.0f)	//視点の高さ
#define POSV_DEPTH		(1000.0f)	//視点の奥行き

//上方向ベクトル情報
#define VECU_OVER		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	//上方向ベクトル

//カメラ情報
#define ANGLE_OF_VIEW	(45.0f)		//画角

//描画範囲
#define MIN_DRAW		(10.0f)		//最小描画範囲
#define MAX_DRAW		(40000.0f)	//最大描画範囲

//位置の差分を2乗
#define DIFF_TIMES		(2.0f)		//2乗

//グローバル変数
Camera g_Camera[NUM_CAMERA];		//カメラの情報

//カメラの初期化
void InitCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

		g_Camera[nCntCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntCamera % 2);	//原点Ⅹ位置代入
		g_Camera[nCntCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntCamera / 2);	//原点Ｙ位置代入
		g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 2;						//画面幅初期化
		g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT / 2;					//画面高さ初期化
		g_Camera[nCntCamera].viewport.MinZ = 0.0f;
		g_Camera[nCntCamera].viewport.MaxZ = 1.0f;

		//注視点の位置更新
		UpdatePosVCamera(nCntCamera);
	}
}

//カメラの終了処理
void UninitCamera(void)
{

}

//カメラの更新処理
void UpdateCamera(void)
{
	//カメラの移動処理
	MoveCamera();
}

//カメラの設定処理
void SetCamera(int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビューポートの設定
	pDevice->SetViewport(&g_Camera[nIdx].viewport);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera[nIdx].mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_Camera[nIdx].mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_Camera[nIdx].viewport.Width / (float)g_Camera[nIdx].viewport.Height, MIN_DRAW, g_Camera[nIdx].fMaxLength);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera[nIdx].mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera[nIdx].mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_Camera[nIdx].mtxView, &g_Camera[nIdx].posV, &g_Camera[nIdx].posR, &g_Camera[nIdx].vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera[nIdx].mtxView);
}

//カメラの移動処理
void MoveCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
#ifdef _DEBUG
		//視点の上下
		if (GetKeyboardPress(DIK_T) == true)
		{
			g_Camera[nCntCamera].posV.y += POSV_SPEED;
		}
		if (GetKeyboardPress(DIK_B) == true)
		{
			g_Camera[nCntCamera].posV.y -= POSV_SPEED;
		}

		//視点の前後
		if (GetKeyboardPress(DIK_Y) == true)
		{
			g_Camera[nCntCamera].fLength += POSV_SPEED;
		}
		if (GetKeyboardPress(DIK_N) == true)
		{
			g_Camera[nCntCamera].fLength -= POSV_SPEED;
		}

		//視点の左右
		if (GetKeyboardPress(DIK_Z) == true)
		{
			g_Camera[nCntCamera].rot.y -= POSV_ROTSPEED;
		}
		if (GetKeyboardPress(DIK_C) == true)
		{
			g_Camera[nCntCamera].rot.y += POSV_ROTSPEED;
		}
#endif // _DEBUG

		//注視点の位置更新
		UpdatePosVCamera(nCntCamera);
	}
}

//視点の位置更新
void UpdatePosVCamera(int nCntCamera)
{
	//プレイヤー情報取得
	Player *pPlayer = GetPlayer();

	//対象のプレイヤーに注視点を合わせる
	pPlayer += nCntCamera;

	g_Camera[nCntCamera].posR = pPlayer->pos;

	//視点の位置更新
	g_Camera[nCntCamera].posV.x = g_Camera[nCntCamera].posR.x + sinf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
	g_Camera[nCntCamera].posV.z = g_Camera[nCntCamera].posR.z + cosf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
	g_Camera[nCntCamera].posV.y += pPlayer->move.y;
}

//カメラの取得
Camera *GetCamera(void)
{
	return &g_Camera[0];
}

//=========================================
//カメラの位置設定処理
//Author:石原颯馬
//=========================================
void SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera)
{
	//設定
	g_Camera[nNumCamera].posV = posV;	//視点
	g_Camera[nNumCamera].posR = posR;	//注視点

	//上方向ベクトルだけ固定
	g_Camera[nNumCamera].vecU = VECU_OVER;	//上方向ベクトル

	//それぞれの位置の差分を格納する変数
	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_Camera[nNumCamera].posR.x - g_Camera[nNumCamera].posV.x, DIFF_TIMES);	//２乗
	PosDiffZ = powf(g_Camera[nNumCamera].posR.z - g_Camera[nNumCamera].posV.z, DIFF_TIMES);	//２乗

	//長さの算出
	g_Camera[nNumCamera].fLength = sqrtf(PosDiffX + PosDiffZ);

	//視点の位置更新
	UpdatePosVCamera(nNumCamera);
}
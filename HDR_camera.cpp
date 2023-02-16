/*==========================================================================================================================================================

カメラの処理[HDR_camera.cpp]
Author:飯田洲暉 (大宮愛羅  平澤詩苑  石原颯馬)

============================================================================================================================================================*/
#include "HDR_camera.h"
#include "input.h"
#include "PvP_player.h"
#include "camera_frame.h"
#include "input.h"

//注視点情報
#define POSR_SPEED		(10.0f)		//注視点の移動速度
#define POSR_WIDTH		(0.0f)		//注視点の幅
#define POSR_HEIGHT		(0.0f)		//注視点の高さ
#define POSR_DEPTH		(0.0f)		//注視点の奥行き

//視点情報
#define POSV_ROTSPEED	(0.05f)		//視点の回転速度
#define POSV_SPEED		(10.0f)		//視点の移動速度
#define POSR_ADD		(8.0f)		//注視点の上下移動
#define POSV_DISTANCE	(100.0f)	//視点の距離
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

//カメラ単体の場合の視点座標
#define ALONE_CAMERA_POS	(D3DXVECTOR3(0.0f, 400.0f, 500.0f))

//３人称視点時のマクロ
#define TPS_LENGTH		(150.0f)	//注視点～視点間の距離
#define TPS_posV_HEIGHT	(100.0f)	//視点の高さ
#define TPS_posR_HEIGHT	(30.0f)		//注視点の高さ

//グローバル変数
HDRCamera		g_HDRCamera[NUM_HDRCAMERA];	//カメラの情報
NumHDRCamera	g_NumHDRCameraType;		//カメラ分割の情報
bool		g_bChase_HDR = true;		//プレイヤーに注視点を置くかどうか				TRUE：追いかける　　false：原点を見る
bool		g_bTPS_HDR = false;			//３人称か、定点カメラか ただし、４分割時のみ	TRUE：３人称		false：定点カメラ

//=========================================
//カメラの位置設定処理
//Author:石原颯馬
//=========================================
void InitSetHDRCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumHDRCamera)
{
	//設定
	g_HDRCamera[nNumHDRCamera].posV = posV;	//視点
	g_HDRCamera[nNumHDRCamera].posR = posR;	//注視点

										//上方向ベクトルだけ固定
	g_HDRCamera[nNumHDRCamera].vecU = VECU_OVER;	//上方向ベクトル

											//それぞれの位置の差分を格納する変数
	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_HDRCamera[nNumHDRCamera].posR.x - g_HDRCamera[nNumHDRCamera].posV.x, DIFF_TIMES);	//２乗
	PosDiffZ = powf(g_HDRCamera[nNumHDRCamera].posR.z - g_HDRCamera[nNumHDRCamera].posV.z, DIFF_TIMES);	//２乗

																							//長さの算出
	g_HDRCamera[nNumHDRCamera].fLength = sqrtf(PosDiffX + PosDiffZ);

	//視点の位置更新
	UpdatePosVHDRCamera(nNumHDRCamera);
}

//=========================================
//カメラの初期化処理
//=========================================
void InitHDRCamera(NumHDRCamera type)
{
	//基本情報の初期化処理
	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
	{
		g_HDRCamera[nCntHDRCamera].rot = ZERO_SET;
	}

	g_bChase_HDR = true;		//プレイヤーを追いかける
	g_bTPS_HDR = true;			//３人称カメラに設定
	Set_NumHDRCamera(type);	//カメラの数によるカメラ情報の初期化
}

//=========================================
//カメラの終了処理
//=========================================
void UninitHDRCamera(void)
{

}

//=========================================
//カメラの更新処理
//=========================================
void UpdateHDRCamera(void)
{
	//追従  ON / OFF 切り替え
	if (GetKeyboardTrigger(DIK_F4))
	{
		g_bChase_HDR = g_bChase_HDR ? false : true;

		//追従がOFF
		if (g_bChase_HDR == false)
		{
			//注視点を０にする
			for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
			{
				g_HDRCamera[nCntHDRCamera].posR = ZERO_SET;
			}
		}
	}

	//３人称　ON / OFF 切り替え
	if (GetKeyboardTrigger(DIK_F6))
	{
		g_bTPS_HDR = g_bTPS_HDR ? false : true;
	}
	
	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
	{
		//カメラが使われている
		if (g_HDRCamera[nCntHDRCamera].bUse == true)
		{
			//カメラの移動処理
			MoveHDRCamera(nCntHDRCamera);
		}
	}
}

//=========================================
//カメラの設定処理
//=========================================
void SetHDRCamera(int nIdx)
{
	//カメラが使われている
	if (g_HDRCamera[nIdx].bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//ビューポートの設定
		pDevice->SetViewport(&g_HDRCamera[nIdx].viewport);

		//プロジェクションマトリックスの初期化
		D3DXMatrixIdentity(&g_HDRCamera[nIdx].mtxProjection);

		//プロジェクションマトリックスの作成
		D3DXMatrixPerspectiveFovLH(&g_HDRCamera[nIdx].mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_HDRCamera[nIdx].viewport.Width / (float)g_HDRCamera[nIdx].viewport.Height, MIN_DRAW, g_HDRCamera[nIdx].fMaxLength);

		//プロジェクションマトリックスの設定
		pDevice->SetTransform(D3DTS_PROJECTION, &g_HDRCamera[nIdx].mtxProjection);

		//ビューマトリックスの初期化
		D3DXMatrixIdentity(&g_HDRCamera[nIdx].mtxView);

		//ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_HDRCamera[nIdx].mtxView, &g_HDRCamera[nIdx].posV, &g_HDRCamera[nIdx].posR, &g_HDRCamera[nIdx].vecU);

		//ビューマトリックスの設定
		pDevice->SetTransform(D3DTS_VIEW, &g_HDRCamera[nIdx].mtxView);
	}
}

//=========================================
//カメラの台数別　設定処理
//=========================================
void Set_NumHDRCamera(NumHDRCamera type)
{
	int nCntHDRCamera = 0;				//カウンター初期化
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得
	g_NumHDRCameraType = type;			//カメラの分割情報格納

	switch (type)
	{
		/*----------------------------------------------------------------
		俯瞰で全体が見える視点の処理
		----------------------------------------------------------------*/
	case NumHDRCamera_ONLY:
	{
		g_HDRCamera[nCntHDRCamera].posV = ALONE_CAMERA_POS;			//視点座標初期化
		g_HDRCamera[nCntHDRCamera].posR = ZERO_SET;					//注視点座標初期化

		g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;				//最大描画距離設定

		g_HDRCamera[nCntHDRCamera].viewport.X = 0.0f;					//原点Ⅹ位置代入
		g_HDRCamera[nCntHDRCamera].viewport.Y = 0.0f;					//原点Ｙ位置代入
		g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH;		//画面幅初期化
		g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT;	//画面高さ初期化
		g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
		g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
		g_HDRCamera[nCntHDRCamera].bUse = true;
	}
	break;

	/*----------------------------------------------------------------
	横２分割の視点の処理
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA_HALF; nCntHDRCamera++)
		{
			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

			g_HDRCamera[nCntHDRCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntHDRCamera % 2);	//原点Ⅹ位置代入
			g_HDRCamera[nCntHDRCamera].viewport.Y = NIL_F;									//原点Ｙ位置代入
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH / 2;						//画面幅初期化
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT;						//画面高さ初期化
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//注視点の位置更新
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	縦２分割の視点の処理
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_HIGH_row:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA_HALF; nCntHDRCamera++)
		{
			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

			g_HDRCamera[nCntHDRCamera].viewport.X = NIL_F;									//原点Ⅹ位置代入
			g_HDRCamera[nCntHDRCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntHDRCamera % 2);	//原点Ｙ位置代入
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH;							//画面幅初期化
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT / 2;					//画面高さ初期化
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//注視点の位置更新
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	各プレイヤー専用のカメラの処理
	----------------------------------------------------------------*/
	case NumHDRCamera_FOUR_Separate:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
		{
			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

			g_HDRCamera[nCntHDRCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntHDRCamera % 2);	//原点Ⅹ位置代入
			g_HDRCamera[nCntHDRCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntHDRCamera / 2);	//原点Ｙ位置代入
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH / 2;						//画面幅初期化
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT / 2;					//画面高さ初期化
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//注視点の位置更新
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}
	break;
	}

	//画面分割の枠を設定
	//SetUseFrame(type);

	//各カメラの設定
	for (int nCntUse = 0; nCntUse < NUM_HDRCAMERA; nCntUse++)
	{
		//上のスイッチ文で使われることになったカメラ
		if (nCntUse <= nCntHDRCamera)
		{
			//注視点の位置設定
			SetPosRHDRCamera(nCntHDRCamera);

			//視点の位置設定
			UpdatePosVHDRCamera(nCntUse);
		}

		//設定されたカメラ以外のカメラを使用していないようにする
		else
		{
			g_HDRCamera[nCntUse].bUse = false;
		}
	}
}

//カメラの移動処理
void MoveHDRCamera(int nCntHDRCamera)
{
#ifdef _DEBUG
	//----------------------
	//		視点の移動	
	//----------------------
	//視点の上下
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_HDRCamera[nCntHDRCamera].posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		g_HDRCamera[nCntHDRCamera].posV.y -= POSV_SPEED;
	}

	//視点の前後
	if (GetKeyboardPress(DIK_N) == true)
	{
		g_HDRCamera[nCntHDRCamera].fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_HDRCamera[nCntHDRCamera].fLength -= POSV_SPEED;
	}

	//視点の左右
	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_HDRCamera[nCntHDRCamera].rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		g_HDRCamera[nCntHDRCamera].rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		注視点の移動	
	//------------------------
	//注視点の上下
	if (GetKeyboardPress(DIK_I) == true)
	{
		g_HDRCamera[nCntHDRCamera].posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_HDRCamera[nCntHDRCamera].posR.y -= POSR_ADD;
	}
#endif // _DEBUG

	//注視点の位置設定
	SetPosRHDRCamera(nCntHDRCamera);

	//視点カメラ更新
	UpdatePosVHDRCamera(nCntHDRCamera);
}

//３人称視点
void TPS_ChaseHDRCamera(int nCntHDRCamera, D3DXVECTOR3 rot)
{
	//プレイヤーの角度の逆に設定
	g_HDRCamera[nCntHDRCamera].rot.y = (D3DX_PI - rot.y);

	//角度修正
	FIX_ROT(g_HDRCamera[nCntHDRCamera].rot.y);

	//視点の高さ設定
	g_HDRCamera[nCntHDRCamera].posV.y = g_HDRCamera[nCntHDRCamera].posR.y + TPS_posV_HEIGHT;
	g_HDRCamera[nCntHDRCamera].posR.y += TPS_posR_HEIGHT;
	
	//注視点～視点　間の距離設定
	g_HDRCamera[nCntHDRCamera].fLength = TPS_LENGTH;

	//視点位置更新
	UpdatePosVHDRCamera(nCntHDRCamera);
}

//視点の位置更新
void UpdatePosVHDRCamera(int nCntHDRCamera)
{
	//視点の位置更新
	g_HDRCamera[nCntHDRCamera].posV.x = g_HDRCamera[nCntHDRCamera].posR.x + sinf(D3DX_PI - g_HDRCamera[nCntHDRCamera].rot.y) * g_HDRCamera[nCntHDRCamera].fLength;
	g_HDRCamera[nCntHDRCamera].posV.z = g_HDRCamera[nCntHDRCamera].posR.z + cosf(D3DX_PI - g_HDRCamera[nCntHDRCamera].rot.y) * g_HDRCamera[nCntHDRCamera].fLength;
}

//注視点の位置設定
void SetPosRHDRCamera(int nCntHDRCamera)
{
	//追従 または ３人称が  ON
	if (g_bChase_HDR == true || g_bTPS_HDR == true)
	{
		//プレイヤー情報取得
		Player *pPlayer = GetPlayer();

		//対象のプレイヤーに注視点を合わせる
		g_HDRCamera[nCntHDRCamera].posR = pPlayer[nCntHDRCamera].pos;

		//３人称視点
		if (g_bTPS_HDR)
		{
			//3人称視点設定
			TPS_ChaseHDRCamera(nCntHDRCamera, pPlayer[nCntHDRCamera].rot);
		}
	}
}

//カメラの取得
HDRCamera *GetHDRCamera(void)
{
	return &g_HDRCamera[0];
}
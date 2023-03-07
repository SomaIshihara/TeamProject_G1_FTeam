/*==========================================================================================================================================================

リザルト用カメラ処理[resultCamera.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "resultCamera.h"
#include "debugproc.h"
#include "conversioninput.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 400.0f, -200.0f)	//視点の初期位置
#define POS_POSR			D3DXVECTOR3(0.0f, 300.0f, 0.0f)		//注視点の初期位置
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//上方向ベクトルの初期値
#define VIEW_ANGLE			(45.0f)								//視野角
#define MOVE_CAMERA_SPEED	(3.0f)								//カメラ・注視点座標の移動量
#define SPIN_CAMERA_SPEED	(0.03f)								//カメラの回転スピード
#define MIN_DRAW_Z			(10.0f)								//描画処理の最低奥行き
#define MAX_DRAW_Z			(2000.0f)							//描画処理の最高奥行き
#define MIN_LENGTH			(100.0f)							//視点と注視点の最低距離
#define MAX_LENGTH			(300.0f)							//視点と注視点の最大距離
#define CHANGE_LENGHT		(0.5f)								//長さの移動量
#define SPIN_ROT			(0.05f)								//回転量
#define COR_ROT_POSV		(3.14f)								//角度計算による視点操作の調整値
#define COR_ROT_POSR		(0.0f)								//角度計算による注視点操作の調整値
#define DRAW_LENGTH			(500.0f)							//オブジェクトを描画させてあげる範囲
#define WAVE_CAMERA_INFO	"data\\CSV\\WaveCamera.csv"			//アニメーションカメラの情報ファイル

//グローバル変数
ResultCamera		g_ResultCamera;						//カメラの情報
WAVECamera			g_WaveCamera;						//カメラのウェーブ情報
AnimResCamera		g_AnimResCamera[WAVECamera_MAX];	//アニメーションカメラの情報

//------------------------------------------------
//			リザルト用カメラの初期化処理
//------------------------------------------------
void InitResultCamera(void)
{
	//アニメーション用カメラ情報の初期化
	LoadWaveCamera();

	g_ResultCamera.vecU = POS_VECU;						//上方向ベクトル
	g_WaveCamera = WAVECamera_01_SideWays;				//ウェーブ情報初期化
	SetNextWave(&g_ResultCamera, g_AnimResCamera[0]);	//初期位置設定

	//---------------------------------
	//カメラの角度や距離を算出
	//---------------------------------
	{
		D3DXVECTOR3 fDiffPos = g_ResultCamera.posR - g_ResultCamera.posV;	//視点と注視点の位置の差
		fDiffPos.x = powf(fDiffPos.x, 2.0f);					//視点同士の位置の差を２乗
		fDiffPos.z = powf(fDiffPos.z, 2.0f);					//視点同士の位置の差を２乗

		g_ResultCamera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y方向の角度を計算

		float PosDiffX, PosDiffZ;

		PosDiffX = powf(g_ResultCamera.posR.x - g_ResultCamera.posV.x, 2.0f);	//2乗
		PosDiffZ = powf(g_ResultCamera.posR.z - g_ResultCamera.posV.z, 2.0f);	//2乗

		g_ResultCamera.fLength = sqrtf(PosDiffX + PosDiffZ);	//長さを算出

		//位置設定
		MoveResultCamera(g_ResultCamera.posR, &g_ResultCamera.posV, D3DX_PI);
	}
}

//リザルト用カメラのアニメーション情報初期化
void LoadWaveCamera(void)
{
	FILE *pFile;
	char aData[256];
	AnimResCamera *pAnimCamera = &g_AnimResCamera[0];

	//ファイル読み込み
	pFile = fopen(WAVE_CAMERA_INFO, "r");

	//ファイルが読み込めている
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);	//最初のセルの項目名読み込み

		for (int nCntWave = 0; nCntWave < WAVECamera_MAX; nCntWave++, pAnimCamera++)
		{
			//一行読み込み
			fscanf(pFile, "%s", &aData[0]);

			//経過フレーム初期化
			pAnimCamera->nFrameCounter = 0;

			//全体フレーム
			pAnimCamera->nWholeFrame = atoi(strtok(&aData[0], ","));

			//フレーム到達判定初期化
			pAnimCamera->bEnd = false;

			//硬直フレーム初期化
			pAnimCamera->nWaitCount = 0;

			//全体硬直フレーム
			pAnimCamera->WaitTime = atoi(strtok(NULL, ","));

			//**************************************************
			//					視点	開始位置
			//**************************************************
			pAnimCamera->StartPosV.x = atof(strtok(NULL, ","));
			pAnimCamera->StartPosV.y = atof(strtok(NULL, ","));
			pAnimCamera->StartPosV.z = atof(strtok(NULL, ","));

			//**************************************************
			//				   注視点	開始位置
			//**************************************************
			pAnimCamera->StartPosR.x = atof(strtok(NULL, ","));
			pAnimCamera->StartPosR.y = atof(strtok(NULL, ","));
			pAnimCamera->StartPosR.z = atof(strtok(NULL, ","));

			//**************************************************
			//					視点	目標位置
			//**************************************************
			pAnimCamera->OffsetPosV.x = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosV.y = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosV.z = atof(strtok(NULL, ","));

			//**************************************************
			//					注視点	目標位置
			//**************************************************
			pAnimCamera->OffsetPosR.x = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosR.y = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosR.z = atof(strtok(NULL, ","));

			//**************************************************
			//					視点	移動量
			//**************************************************
			pAnimCamera->movePosV.x = (pAnimCamera->OffsetPosV.x - pAnimCamera->StartPosV.x) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosV.y = (pAnimCamera->OffsetPosV.y - pAnimCamera->StartPosV.y) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosV.z = (pAnimCamera->OffsetPosV.z - pAnimCamera->StartPosV.z) / pAnimCamera->nWholeFrame;

			//**************************************************
			//					注視点	移動量
			//**************************************************
			pAnimCamera->movePosR.x = (pAnimCamera->OffsetPosR.x - pAnimCamera->StartPosR.x) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosR.y = (pAnimCamera->OffsetPosR.y - pAnimCamera->StartPosR.y) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosR.z = (pAnimCamera->OffsetPosR.z - pAnimCamera->StartPosR.z) / pAnimCamera->nWholeFrame;
		}
	}
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
	//カメラの移動処理（今は使用しない
#if 0
	//視点移動		//注視点移動	//視点回転
	MovePosV();		MovePosR();		SpinPosV();

	//長さ切り替え
	LengthCamera();

	//位置設定
	MoveResultCamera(g_ResultCamera.posR, &g_ResultCamera.posV, D3DX_PI);
#endif

	//カメラのウェーブ処理
	WaveResultCamera();
	
	int nWave = g_WaveCamera;
	ResultCamera Camera = g_ResultCamera;
	AnimResCamera AnimCam = g_AnimResCamera[nWave];

	if (GetKeyboardTrigger(DIK_BACKSPACE))
	{
		UninitResultCamera();
		InitResultCamera();
	}
}

//カメラのアニメーション処理
void WaveResultCamera(void)
{
	//ウェーブが終了していない
	if (g_WaveCamera != WAVECamera_MAX)
	{
		int nWave = g_WaveCamera;								//現在のウェーブを取得
		ResultCamera *pCamera = &g_ResultCamera;				//カメラのポインタ取得
		AnimResCamera *pAnimCamera = &g_AnimResCamera[nWave];	//アニメーション用カメラのポインタ

		//未だ全体フレームに到達していない
		if (!pAnimCamera->bEnd)
		{
			//視点・注視点移動
			pCamera->posV += pAnimCamera->movePosV;
			pCamera->posR += pAnimCamera->movePosR;

			//経過フレーム加算
			pAnimCamera->nFrameCounter++;

			//経過フレームが全体フレームに達した
			if (pAnimCamera->nWholeFrame <= pAnimCamera->nFrameCounter)
			{
				//全体フレームに到達した
				pAnimCamera->bEnd = true;
			}
		}
		//全体フレームに到達している
		else
		{
			//硬直フレーム加算
			pAnimCamera->nWaitCount++;

			if (pAnimCamera->WaitTime <= pAnimCamera->nWaitCount)
			{
				//次のウェーブへ
				g_WaveCamera = (WAVECamera)(g_WaveCamera + 1);

				//ウェーブが終了していない
				if (g_WaveCamera != WAVECamera_MAX)
				{
					//次のアニメーションカメラへ
					pAnimCamera++;

					//次のウェーブへ
					SetNextWave(pCamera, *pAnimCamera);
				}
			}
		}

		PrintDebugProc("\nウェーブ番号：%d\n", nWave);
		PrintDebugProc("経過フレーム：%d        硬直フレーム：%d\n", pAnimCamera->nFrameCounter, pAnimCamera->nWaitCount);
		PrintDebugProc("全体フレーム：%d    全体硬直フレーム：%d\n\n", pAnimCamera->nWholeFrame, pAnimCamera->WaitTime);

		PrintDebugProc("*******        現在        *******\n");
		PrintDebugProc("視点Ⅹ位置：%f    注視点Ⅹ位置：%f\n", pCamera->posV.x, pCamera->posR.x);
		PrintDebugProc("視点Ｙ位置：%f    注視点Ｙ位置：%f\n", pCamera->posV.y, pCamera->posR.y);
		PrintDebugProc("視点Ｚ位置：%f    注視点Ｚ位置：%f\n\n", pCamera->posV.z, pCamera->posR.z);

		PrintDebugProc("*******        目標        *******\n");
		PrintDebugProc("視点Ⅹ位置：%f    注視点Ⅹ位置：%f\n", pAnimCamera->OffsetPosV.x, pAnimCamera->OffsetPosR.x);
		PrintDebugProc("視点Ｙ位置：%f    注視点Ｙ位置：%f\n", pAnimCamera->OffsetPosV.y, pAnimCamera->OffsetPosR.y);
		PrintDebugProc("視点Ｚ位置：%f    注視点Ｚ位置：%f\n", pAnimCamera->OffsetPosV.z, pAnimCamera->OffsetPosR.z);

	}

	else
	{
		PrintDebugProc("ウェーブ終了\n");
	}
}

//次のウェーブへ
void SetNextWave(ResultCamera *pCamera, AnimResCamera AnimCamera) 
{
	//視点・注視点を初期位置へ
	pCamera->posV = AnimCamera.StartPosV;
	pCamera->posR = AnimCamera.StartPosR;
}

//視点移動
void MovePosV(void)
{
	//視点、注視点ともに左に移動
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_ResultCamera.posV.x -= g_ResultCamera.posR.x -= cosf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
		g_ResultCamera.posV.z -= g_ResultCamera.posR.z -= sinf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//視点、注視点ともに右に移動
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_ResultCamera.posV.x += g_ResultCamera.posR.x += cosf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
		g_ResultCamera.posV.z += g_ResultCamera.posR.z += sinf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//視点上昇
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_ResultCamera.posV.y += MOVE_CAMERA_SPEED;
	}

	//視点下降
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_ResultCamera.posV.y -= MOVE_CAMERA_SPEED;
	}
}

//注視点移動
void MovePosR(void)
{
	if (GetKeyboardTrigger(DIK_LSHIFT) == true)
	{
		//注視点上昇
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_ResultCamera.posR.y += MOVE_CAMERA_SPEED;
		}

		//注視点下降
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_ResultCamera.posR.y -= MOVE_CAMERA_SPEED;
		}
	}
	else
	{
		//注視点上昇
		if (GetKeyboardPress(DIK_T) == true)
		{
			g_ResultCamera.posR.y += MOVE_CAMERA_SPEED;
		}

		//注視点下降
		if (GetKeyboardPress(DIK_H) == true)
		{
			g_ResultCamera.posR.y -= MOVE_CAMERA_SPEED;
		}
	}
}

//視点回転
void SpinPosV(void)
{
	if (GetKeyboardPress(DIK_Z))
	{
		g_ResultCamera.rot.y -= SPIN_CAMERA_SPEED;
	}

	if (GetKeyboardPress(DIK_C))
	{
		g_ResultCamera.rot.y += SPIN_CAMERA_SPEED;
	}

	//角度修正
	FIX_ROT(g_ResultCamera.rot.y);
}

//視点間の長さ処理
void LengthCamera(void)
{
	//距離を近づける
	if (GetKeyboardPress(DIK_UP))
	{
		g_ResultCamera.fLength -= MOVE_CAMERA_SPEED;
	}

	//距離を近づける
	if (GetKeyboardPress(DIK_DOWN))
	{
		g_ResultCamera.fLength += MOVE_CAMERA_SPEED;
	}

	//最低距離を下回った
	if (g_ResultCamera.fLength < MIN_LENGTH)
	{
		//最低距離に修正
		g_ResultCamera.fLength = MIN_LENGTH;
	}
}

//視点回転
void SpinResultPosV(void)
{

}

//カメラの移動処理
void MoveResultCamera(D3DXVECTOR3 ParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = ParentPos.x + sinf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
	pChildPos->z = ParentPos.z + cosf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
}

//カメラの位置リセット
void ResetResultCameraPos(void)
{
	g_ResultCamera.posR = POS_POSR;
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
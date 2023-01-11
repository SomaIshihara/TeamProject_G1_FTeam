/*==========================================================================================================================================================

壁の処理[wall.cpp]
Author:大宮愛羅

============================================================================================================================================================*/

#include "main.h"
#include "wall.h"
//#include "player.h"
//#include "input.h"

#define MAX_WALL	(4)

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;		//テクスチャへのポインタ
D3DXMATRIX				g_mtxWorldWall;				//ワールドマトリックス
Wall					g_aWall[MAX_WALL];			//壁の情報

													//壁の初期化
void InitWall(void)
{
	//デバイス
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall003.jpg",
		&g_pTextureWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall, NULL);

	//壁を読み込む
	LoadWall();

	//ポインタを設定
	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点座標へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, +g_aWall[nCntWall].fDepth);	//左上端
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, -g_aWall[nCntWall].fDepth);	//右上端
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, 0.0f, +g_aWall[nCntWall].fDepth);							//左下端
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, 0.0f, -g_aWall[nCntWall].fDepth);							//右下端

																														//法線ベクトルの設定
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//左上端
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//右上端
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//左下端
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//右下端

															//頂点カラー設定
		pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//左上端
		pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//右上端
		pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//左下端
		pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//右下端

															//テクスチャ座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);				//左上端
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);				//右上端
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);				//左下端
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);				//右下端
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//壁の終了処理
void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//壁の更新処理
void UpdateWall(void)
{

}

//壁の描画処理
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

								 //頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldWall);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * VTX_MAX, 2);
		}
	}
}

//壁を読み込む
void LoadWall(void)
{
	int nLetter = 0;	//文字sw
	int nColumn = 0;	//列
	int nLine = 0;		//行
	char aData[100];	//文字列

	//ファイルポインタ
	FILE *pFile;

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	//バッファの初期化
	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data/CSV/Wall.csv", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ファイルから一文字ずつ読み込む
		while (fgetc(pFile) != '\n');

		while (nLetter != EOF)
		{//EOF以外の時
			while (1)
			{//一列の文字列を作る
				nLetter = fgetc(pFile);

				if (nLetter == EOF)
				{//EOFの時
					break;		//処理を抜ける
				}

				if (nLetter != '\n'&& nLetter != ',')
				{
					//カンマor改行出なければ文字をつなぐ
					strcat(aData, (const char*)&nLetter);
				}
				else
				{//それ以外の時
					break;		//処理を抜ける
				}
			}

			switch (nColumn)
			{
				//種類
			case 0:
				g_aWall[nLine].nType = atoi(aData);
				break;

				//壁の向き
			case 1:
				g_aWall[nLine].wallAngle = (WALLANGLE)atoi(aData);

				switch (g_aWall[nLine].wallAngle)
				{
					//左向き
				case WALLANGLE_LEFT:
					g_aWall[nLine].rot.y = D3DX_PI * 0.5f;
					break;

					//右向き
				case WALLANGLE_RIGHT:
					g_aWall[nLine].rot.y = D3DX_PI * -0.5f;
					break;

					//後向き
				case WALLANGLE_BACK:
					g_aWall[nLine].rot.y = D3DX_PI;
					break;

					//前向き
				case WALLANGLE_FRONT:
					g_aWall[nLine].rot.y = 0.0f;
					break;
				}

				//処理を抜ける
				break;

				//幅
			case 2:
				g_aWall[nLine].fWidth = atof(aData);
				break;

				//高さ
			case 3:
				g_aWall[nLine].fHeight = atof(aData);
				break;

				//X座標
			case 4:
				g_aWall[nLine].pos.x = atof(aData);
				break;

				//Y座標
			case 5:
				g_aWall[nLine].pos.y = atof(aData);
				break;

				//Z座標
			case 6:
				g_aWall[nLine].pos.z = atof(aData);
				break;
			}

			//バッファの初期化
			memset(aData, 0, sizeof(aData));

			//列数を足す
			nColumn++;

			if (nLetter == '\n')
			{//読み込んだ文字が改行だった時、
			 // 使用している状態にする
				g_aWall[nLine].bUse = true;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, +g_aWall[nLine].fDepth);	//左上端
				pVtx[1].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, -g_aWall[nLine].fDepth);	//右上端
				pVtx[2].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, 0.0f, +g_aWall[nLine].fDepth);						//左下端
				pVtx[3].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, 0.0f, -g_aWall[nLine].fDepth);						//右下端

				pVtx += 4;

				//列数を初期化して行数を増やす
				nColumn = 0;
				nLine++;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//壁との当たり判定
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bCollision = false;		//当たったかどうかの判定
	D3DXVECTOR3 vecLine;			//オブジェクトの頂点 0番目から端までのベクトル(境界線ベクトル)
	D3DXVECTOR3 vecToPos;			//オブジェクトの頂点 0番目から見た対象の位置への角度


									//ポインタを設定
	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点座標へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{//壁が使用されている
			switch (g_aWall[nCntWall].wallAngle)
			{
				//左向きの壁に当たった時
			case WALLANGLE_LEFT:
				if (pPosOld->x <= g_aWall[nCntWall].pos.x && pPos->x >= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;
				}
				break;

				//右向きの壁に当たった時
			case WALLANGLE_RIGHT:
				if (pPosOld->x >= g_aWall[nCntWall].pos.x && pPos->x <= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;
				}
				break;

				//前向きの壁に当たった時
			case WALLANGLE_FRONT:
				if (pPosOld->z <= g_aWall[nCntWall].pos.z && pPos->z >= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;
				}
				break;

				//後ろ向きの壁に当たった時
			case WALLANGLE_BACK:
				if (pPosOld->z >= g_aWall[nCntWall].pos.z && pPos->z <= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;
				}
				break;
			}

		}
	}

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += 4)
	{
#if 0
		vecToPos = *pPos - pVtx[0].pos;										//対象の位置から壁の原点までのベクトル計算
		vecLine = pVtx[1].pos - pVtx[0].pos;								//次の頂点から頂点0番目のベクトル計算

																			//外積計算
		fVecAnswer = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);	//対象の位置へのベクトル計算

																			//現在の向きが[負](左) である
		if (fVecAnswer <= 0.0f)
		{
			bCollision = true;		//当たった
			pPos = pPosOld;
			pMove->x = 0.0f;		//移動量をゼロにする
			pMove->z = 0.0f;		//移動量をゼロにする
			break;					//for文を抜ける
		}
#endif
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();

	return bCollision;	//判定を返す
}
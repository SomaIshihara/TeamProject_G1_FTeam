/*==========================================================================================================================================================

壁の処理[wall.cpp]
Author:大宮愛羅

============================================================================================================================================================*/

#include "main.h"
#include "wall.h"
#include "player.h"
#include "input.h"

//マクロ定義
#define MAX_WALL		(4)		//壁の最大数
#define MAX_COLOR		(255)	//色の最大値
#define MAX_STRING		(100)	//文字列の最大数
#define DISPLAY_ADD		(4)		//表示する分だけ加算

//位置情報
#define MIN_POS			(0.0f)	//位置の最小値

//法線情報
#define NOR_WIDTH		(0.0f)	//法線の幅
#define NOR_HEGHT		(1.0f)	//法線の高さ
#define	NOR_DEPTH		(0.0f)	//法線の奥行き

//テクスチャ情報
#define MAX_TEX			(1.0f)	//テクスチャの最大値
#define MIN_TEX			(0.0f)	//テクスチャの最小値

//CSV関連
#define START_LE_NUM	(0)		//文字の開始番号
#define START_CO_NUM	(0)		//列の開始番号
#define START_LI_NUM	(0)		//行の開始番号

//壁の向き
#define ANGLE_R			(-0.5f)	//右向き
#define ANGLE_L			(0.5f)	//左向き
#define ANGLE_F			(0.0f)	//正面向き

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
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, MIN_POS, +g_aWall[nCntWall].fDepth);						//左下端
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, MIN_POS, -g_aWall[nCntWall].fDepth);						//右下端

		//法線ベクトルの設定
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//左上端
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//右上端
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//左下端
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//右下端

		//頂点カラー設定
		pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//左上端
		pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//右上端
		pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//左下端
		pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//右下端

		//テクスチャ座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(MIN_TEX, MIN_TEX);				//左上端
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_TEX, MIN_TEX);				//右上端
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(MIN_TEX, MAX_TEX);				//左下端
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_TEX, MAX_TEX);				//右下端
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
	int nLetter = START_LE_NUM;	//文字
	int nColumn = START_CO_NUM;	//列
	int nLine = START_LI_NUM;	//行
	char aData[MAX_STRING];		//文字列

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
			case WALLCASE_TYPE:
				g_aWall[nLine].nType = atoi(aData);
				break;

				//壁の向き
			case WALLCASE_WALLANGLE:
				g_aWall[nLine].wallAngle = (WALLANGLE)atoi(aData);

				switch (g_aWall[nLine].wallAngle)
				{
					//左向き
				case WALLANGLE_LEFT:
					g_aWall[nLine].rot.y = D3DX_PI * ANGLE_L;
					break;

					//右向き
				case WALLANGLE_RIGHT:
					g_aWall[nLine].rot.y = D3DX_PI * ANGLE_R;
					break;

					//後向き
				case WALLANGLE_BACK:
					g_aWall[nLine].rot.y = D3DX_PI;
					break;

					//前向き
				case WALLANGLE_FRONT:
					g_aWall[nLine].rot.y = ANGLE_F;
					break;
				}

				//処理を抜ける
				break;

				//幅
			case WALLCASE_WIDTH:
				g_aWall[nLine].fWidth = (float)atof(aData);
				break;

				//高さ
			case WALLCASE_HEIGHT:
				g_aWall[nLine].fHeight = (float)atof(aData);
				break;

				//X座標
			case WALLCASE_POSX:
				g_aWall[nLine].pos.x = (float)atof(aData);
				break;

				//Y座標
			case WALLCASE_POSY:
				g_aWall[nLine].pos.y = (float)atof(aData);
				break;

				//Z座標
			case WALLCASE_POSZ:
				g_aWall[nLine].pos.z = (float)atof(aData);
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
				pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, +g_aWall[nLine].fDepth);	//左上端
				pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, -g_aWall[nLine].fDepth);	//右上端
				pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, MIN_POS, +g_aWall[nLine].fDepth);					//左下端
				pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, MIN_POS, -g_aWall[nLine].fDepth);					//右下端

				pVtx += DISPLAY_ADD;

				//列数を初期化して行数を増やす
				nColumn = START_CO_NUM;
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

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();

	return bCollision;	//判定を返す
}

//================================================
//壁の設定処理（モデルビューワーファイルから設定）
//Author:石原颯馬
//Memo:旧仕様も残してあるがデータの構造が違うため別関数用意）
//================================================

/*==========================================================================================================================================================

壁の処理[wall.cpp]
Author:大宮愛羅  平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "wall.h"
#include "PvP_player.h"
#include "input.h"

//マクロ定義
#define MAX_WALL		(4)		//壁の最大数
#define WALL_NOR		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	//壁の法線ベクトル

//崩れ去るときのマクロ定義
#define COLLAPSE_FALL			(0.1f)	//崩れ落ちるスピード
#define COLLAPSE_CLEAR_SPEED	(0.02f)	//透明値の減算度
#define COLLAPSE_VIBE_WIDTH		(5.0f)	//揺れ幅

//テクスチャ情報
#define MAX_TEX			(1.0f)	//テクスチャの最大値
#define MIN_TEX			(0.0f)	//テクスチャの最小値

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;		//テクスチャへのポインタ
D3DXMATRIX				g_mtxWorldWall;				//ワールドマトリックス
Wall					g_aWall[MAX_WALL];			//壁の情報

//==============================================================
//壁の初期化処理
//===============================================================
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

	//頂点情報を設定する
	SetWallVertex();
}

//壁を読み込む
void LoadWall(void)
{
	int nLetter = 0;		//文字
	int nColumn = 0;		//列
	int nLine = 0;			//行
	char aData[MAX_MOJI];	//文字列
	FILE *pFile;			//ファイルポインタ

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

			//読み込んだ文字列を変数に代入
			switch (nColumn)
			{
				//種類
			case WALLCASE_TYPE:
				g_aWall[nLine].nType = atoi(aData);
				break;

				//壁の向き
			case WALLCASE_WALLANGLE:
				g_aWall[nLine].wallAngle = (WALLANGLE)atoi(aData);
				//処理を抜ける
				break;

				//幅
			case WALLCASE_WIDTH:
				g_aWall[nLine].fWidth = fatof(aData);
				break;

				//高さ
			case WALLCASE_HEIGHT:
				g_aWall[nLine].fHeight = fatof(aData);
				break;

				//奥行き
			case WALLCASE_DEPTH:
				g_aWall[nLine].fDepth = fatof(aData);
				break;

				//X座標
			case WALLCASE_POSX:
				g_aWall[nLine].pos.x = fatof(aData);
				break;

				//Y座標
			case WALLCASE_POSY:
				g_aWall[nLine].pos.y = fatof(aData);
				break;

				//Z座標
			case WALLCASE_POSZ:
				g_aWall[nLine].pos.z = fatof(aData);
				break;
			}

			//バッファの初期化
			memset(aData, 0, sizeof(aData));

			//列数を足す
			nColumn++;

			if (nLetter == '\n')
			{//読み込んだ文字が改行だった時、

				g_aWall[nLine].rot = ZERO_SET;		// 向き初期化
				g_aWall[nLine].fAlpha = 1.0f;		// 透明度初期化
				g_aWall[nLine].bSideVibe = false;	// 震え初期化
				g_aWall[nLine].fSideVibe = 0.0f;	// 震えの幅初期化
				g_aWall[nLine].bCollision = false;	// 当たり判定初期化
				g_aWall[nLine].bDisp = true;		// 描画する

				nColumn = 0;					// 列数を初期化
				nLine++;						// 行数を増やす
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//頂点情報の設定処理
void SetWallVertex(void)
{
	//ポインタを設定
	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点座標へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		//現在の壁のポインターを設定
		Wall *pWall = &g_aWall[nCntWall];

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-pWall->fWidth + pWall->fSideVibe, +pWall->fHeight, +pWall->fDepth + pWall->fSideVibe);	//左上端
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+pWall->fWidth + pWall->fSideVibe, +pWall->fHeight, -pWall->fDepth + pWall->fSideVibe);	//右上端
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-pWall->fWidth + pWall->fSideVibe, 0.0f, +pWall->fDepth + pWall->fSideVibe);				//左下端
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+pWall->fWidth + pWall->fSideVibe, 0.0f, -pWall->fDepth + pWall->fSideVibe);				//右下端

		//法線ベクトルの設定
		pVtx[VTX_LE_UP].nor = WALL_NOR;		//左上端
		pVtx[VTX_RI_UP].nor = WALL_NOR;		//右上端
		pVtx[VTX_LE_DO].nor = WALL_NOR;		//左下端
		pVtx[VTX_RI_DO].nor = WALL_NOR;		//右下端

											//頂点カラー設定
		pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//左上端
		pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//右上端
		pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//左下端
		pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//右下端

											//テクスチャ座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(MIN_TEX, MIN_TEX);	//左上端
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_TEX, MIN_TEX);	//右上端
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(MIN_TEX, MAX_TEX);	//左下端
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_TEX, MAX_TEX);	//右下端
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//==============================================================
//壁の終了処理
//===============================================================
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

//==============================================================
//壁の更新処理
//===============================================================
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//崩れ去る処理
		CollapseWall(nCntWall);

		if (GetKeyboardTrigger(DIK_TAB))
		{
			g_aWall[nCntWall].bCollision = true;
		}
	}

	//頂点情報を設定する
	SetWallVertex();
}

//一度当たった壁が崩れ去る処理
void CollapseWall(int nCntWall)
{
	//壁のポインタ―を設定
	Wall *pWall = &g_aWall[nCntWall];

	//一度当てられていて、まだ透明になっていない
	if (pWall->bCollision == true && pWall->bDisp == true && pWall->fAlpha > 0.0f)
	{
		//右に震えている
		if (pWall->bSideVibe)
		{
			pWall->bSideVibe = false;					//左に震えさせる
			pWall->fSideVibe = -COLLAPSE_VIBE_WIDTH;	//左に震えさせる
		}

		//左に震えている
		else
		{
			pWall->bSideVibe = true;					//右に震えさせる
			pWall->fSideVibe = +COLLAPSE_VIBE_WIDTH;	//右に震えさせる
		}

		//位置を下げる
		pWall->pos.y -= COLLAPSE_FALL;

		//透明度を減算
		pWall->fAlpha -= COLLAPSE_CLEAR_SPEED;

		//透明以下になった
		if (pWall->fAlpha <= 0.0f)
		{
			pWall->fAlpha = 0.0f;	//透明にする
			pWall->bDisp = false;	//描画しない
			pWall->fSideVibe = 0.0f;//震えを止める
		}
	}
}

//==============================================================
//壁の描画処理
//===============================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//両面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bDisp == true)
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

	//通常カリングにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//壁との当たり判定
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bCollision = false;		//当たったかどうかの判定
	D3DXVECTOR3 vecLine;			//オブジェクトの頂点 0番目から端までのベクトル(境界線ベクトル)
	D3DXVECTOR3 vecToPos;			//オブジェクトの頂点 0番目から見た対象の位置への角度

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点座標へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//まだ　当てられていなくて、描画もされている
		if (g_aWall[nCntWall].bCollision == false && g_aWall[nCntWall].bDisp == true)
		{//壁が使用されている
			switch (g_aWall[nCntWall].wallAngle)
			{
				//左向きの壁に当たった時
			case WALLANGLE_LEFT:
				if (pPosOld->x <= g_aWall[nCntWall].pos.x && pPos->x >= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;		//Xの位置に戻す
					g_aWall[nCntWall].
						bCollision =		//壁に
						bCollision = true;	//当たった
				}
				break;

				//右向きの壁に当たった時
			case WALLANGLE_RIGHT:
				if (pPosOld->x >= g_aWall[nCntWall].pos.x && pPos->x <= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;		//Xの位置に戻す
					g_aWall[nCntWall].
						bCollision =		//壁に
						bCollision = true;	//当たった
				}
				break;

				//前向きの壁に当たった時
			case WALLANGLE_FRONT:
				if (pPosOld->z <= g_aWall[nCntWall].pos.z && pPos->z >= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;		//Zの位置に戻す
					g_aWall[nCntWall].
						bCollision =		//壁に
						bCollision = true;	//当たった
				}
				break;

				//後ろ向きの壁に当たった時
			case WALLANGLE_BACK:
				if (pPosOld->z >= g_aWall[nCntWall].pos.z && pPos->z <= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;		//Zの位置に戻す
					g_aWall[nCntWall].
						bCollision =		//壁に
						bCollision = true;	//当たった
				}
				break;
			}
			
			//当たったら処理を抜ける
			if (bCollision == true)
			{
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
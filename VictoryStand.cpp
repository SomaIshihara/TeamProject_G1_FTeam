/*==========================================================================================================================================================

表彰台処理[VictoryStand.h]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "VictoryStand.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "sound.h"

#define VICTORYSTAND_TEX	(10)	// 表彰台のテクスチャの最大数
#define NUM_VICTORYSTAND	(1)		// 表彰台の総数
#define VICTORYSTAND_XFILE	"data\\MODEL\\victorystand\\Stand_No4.x"	// 表彰台のファイル名

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTextureVictoryStand[VICTORYSTAND_TEX] = {};	// テクスチャへのポインタ
LPD3DXMESH					g_pMeshVictoryStand[NUM_VICTORYSTAND];			// メッシュ(頂点情報)へのポインタ
D3DXMATRIX					g_mtxWorldVictoryStand;							// ワールドマトリックス
LPD3DXBUFFER				g_pBuffMatVictoryStand[NUM_VICTORYSTAND];		// マテリアルへのポインタ
DWORD						g_dwNumMatVictoryStand[NUM_VICTORYSTAND];		// マテリアルの数
VictoryStand				g_VictoryStand[NUM_VICTORYSTAND];				// 表彰台の情報

//----------------------------------------------------
//				表彰台の初期化処理
//----------------------------------------------------
void InitVictoryStand(void)
{
	//表彰台の情報初期化
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//大賞の表彰台のポインタ取得
		VictoryStand *pVicStand = &g_VictoryStand[nCntVicStd];

		pVicStand->pos = ZERO_SET;		// 位置初期化
		pVicStand->rot = ZERO_SET;		// 向き初期化
		pVicStand->fLandheight = 0.0f;	// 着地位置初期化
		pVicStand->bUse = true;		// 使われていないようにする
		g_pMeshVictoryStand[nCntVicStd] = NULL;		// メッシュ初期化
		g_pBuffMatVictoryStand[nCntVicStd] = NULL;	// マテリアルへのポインタ初期化
		g_dwNumMatVictoryStand[nCntVicStd] = NULL;	// マテリアルの数初期化

		//Xファイルの読み込み and テクスチャ設定
		InitVicStdMeshMatrix(nCntVicStd);

		//最小・最大位置設定
		SearchVictoryStand_Land_Pos(nCntVicStd, &pVicStand->fLandheight);
	}
}

//メッシュやマテリアルの初期化・設定処理
void InitVicStdMeshMatrix(int nCntMetrix)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイル読み込み
	D3DXLoadMeshFromX(VICTORYSTAND_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatVictoryStand[nCntMetrix], NULL, &g_dwNumMatVictoryStand[nCntMetrix], &g_pMeshVictoryStand[nCntMetrix]);

	//マテリアルデータへのポインタを取得
	D3DXMATERIAL *pMat ;
	pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nCntMetrix]->GetBufferPointer();

	//表彰台のテクスチャ設定
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nCntMetrix]; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureVictoryStand[nCntMat]);
		}
	}
}

//当たり判定に使用する最小・最大値設定処理
void SearchVictoryStand_Land_Pos(int nCntVicStd, float *pfLandHeight)
{
	int		nNumVtx;		//頂点数
	DWORD	dwSizeFVF;		//貯点フォーマットのサイズ
	BYTE	*pVtxBuff;		//頂点バッファへのポインタ

							//頂点数を取得
	nNumVtx = g_pMeshVictoryStand[nCntVicStd]->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshVictoryStand[nCntVicStd]->GetFVF());

	//頂点バッファをロック
	g_pMeshVictoryStand[nCntVicStd]->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	//頂点座標の代入
	D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

	//頂点フォーマットのサイズ分、ポインタを進める
	pVtxBuff += dwSizeFVF;

								//当たり判定に使う頂点の最小・最大位置を取得
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 現在表彰台の取得した頂点座標のＹ座標が、変数に代入されている着地位置より高ければ、上書きする
		*pfLandHeight = *pfLandHeight < vtx.y ? vtx.y : *pfLandHeight;

		//頂点フォーマットのサイズ分、ポインタを進める
		pVtxBuff += dwSizeFVF;

		//頂点座標の代入
		vtx = *(D3DXVECTOR3 *)pVtxBuff;
	}

	//頂点バッファをアンロック
	g_pMeshVictoryStand[nCntVicStd]->UnlockVertexBuffer();
}

//----------------------------------------------
//				表彰台の終了処理
//----------------------------------------------
void UninitVictoryStand(void)
{
	//メッシュとマテリアルの破棄
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//メッシュ破棄
		if (g_pMeshVictoryStand[nCntVicStd] != NULL)
		{
			g_pMeshVictoryStand[nCntVicStd]->Release();
			g_pMeshVictoryStand[nCntVicStd] = NULL;
		}

		//マテリアル破棄
		if (g_pBuffMatVictoryStand[nCntVicStd] != NULL)
		{
			g_pBuffMatVictoryStand[nCntVicStd]->Release();
			g_pBuffMatVictoryStand[nCntVicStd] = NULL;
		}
	}

	//テクスチャ破棄
	for (int nCntTex = 0; nCntTex < VICTORYSTAND_TEX; nCntTex++)
	{
		if (g_pTextureVictoryStand[nCntTex] != NULL)
		{
			g_pTextureVictoryStand[nCntTex]->Release();
			g_pTextureVictoryStand[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------
//				表彰台の更新処理
//----------------------------------------------
void UpdateVictoryStand(void)
{

}

//----------------------------------------------------
//					表彰台の描画処理
//----------------------------------------------------
void DrawVictoryStand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//使われている
		if (g_VictoryStand[nCntVicStd].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldVictoryStand);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_VictoryStand[nCntVicStd].rot.y, g_VictoryStand[nCntVicStd].rot.x, g_VictoryStand[nCntVicStd].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_VictoryStand[nCntVicStd].pos.x, g_VictoryStand[nCntVicStd].pos.y, g_VictoryStand[nCntVicStd].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldVictoryStand);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nCntVicStd]->GetBufferPointer();

			//マテリアル設定
			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nCntVicStd]; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureVictoryStand[nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshVictoryStand[nCntVicStd]->DrawSubset(nCntMat);
			}

			//マテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//表彰台の当たり判定
bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove)
{
	bool bCollision = false;		//当たったかどうかの判定

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{

	}

	//判定を返す
	return bCollision;
}

//----------------------------------------------------
//					表彰台の設定処理
//----------------------------------------------------
void SetVictoryStand(D3DXVECTOR3 Setpos)
{
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//表彰台が使われていない
		if (g_VictoryStand[nCntVicStd].bUse == false)
		{
			g_VictoryStand[nCntVicStd].pos = Setpos;		//位置設定
			g_VictoryStand[nCntVicStd].rot = ZERO_SET;	//向き設定

			g_VictoryStand[nCntVicStd].bUse = true;		//使う
			break;//処理を抜ける
		}
	}
}
/*==========================================================================================================================================================

表彰台処理[VictoryStand.h]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "conversioninput.h"
#include "VictoryStand.h"
#include "resultPlayer.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "sound.h"

#define VICTORYSTAND_TEX	(10)				// 表彰台のテクスチャの最大数
#define NUM_VICTORYSTAND	(4)					// 表彰台の総数
#define VICSTD_FILL_HEIGHT	(2.0f)				// 初期埋め立て量補正値
#define VICSTD_APPEAR_TIME	(CONVERT_FPS(5))	// 表彰台のあらわれる時間
#define VICSTD_APPEAR_SPEED	(0.3f)				// 表彰台のあらわれるスピード

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTextureVictoryStand[VICTORYSTAND_TEX] = {};	// テクスチャへのポインタ
LPD3DXMESH					g_pMeshVictoryStand[NUM_VICTORYSTAND];			// メッシュ(頂点情報)へのポインタ
D3DXMATRIX					g_mtxWorldVictoryStand;							// ワールドマトリックス
LPD3DXBUFFER				g_pBuffMatVictoryStand[NUM_VICTORYSTAND];		// マテリアルへのポインタ
DWORD						g_dwNumMatVictoryStand[NUM_VICTORYSTAND];		// マテリアルの数
VictoryStand				g_VictoryStand[NUM_VICTORYSTAND];				// 表彰台の情報
float						g_fLandPoint[NUM_VICTORYSTAND];					// 表彰台ごとの着地点を代入（同率を考慮して）

//テスト用の順位
const int c_nTestRank[NUM_VICTORYSTAND] = { 1, 3, 2, 0 };

//表彰台の�]ファイル名
const char *c_apVicStdFilePath[NUM_VICTORYSTAND] = {
	"data\\MODEL\\victorystand\\Stand_No1.x",
	"data\\MODEL\\victorystand\\Stand_No2.x",
	"data\\MODEL\\victorystand\\Stand_No3.x",
	"data\\MODEL\\victorystand\\Stand_No4.x",
};

//----------------------------------------------------
//				表彰台の初期化処理
//----------------------------------------------------
void InitVictoryStand(void)
{	
	//順位を保存
	int nRank;

	//着地点設定
	SearchVictoryStand_Land_Pos();

	//表彰台の情報初期化
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//対象の表彰台のポインタ取得
		VictoryStand *pVicStand = &g_VictoryStand[nCntVicStd];

		pVicStand->pos = GetPlayer_RESULT()[nCntVicStd].pos;	// 位置初期化
		pVicStand->pos.y = 0.0f;								// 高さ座標だけ０にする
		pVicStand->rot = ZERO_SET;								// 向き初期化
		pVicStand->bUse = true;									// 使われていないようにする
		nRank = pVicStand->nRank = 0;		// 順位を初期化
		pVicStand->fLandheight = g_fLandPoint[nRank];			// 着地点を初期化

		//最初は地中に埋める
		pVicStand->pos.y -= pVicStand->fLandheight + VICSTD_FILL_HEIGHT;
	}
}

//当たり判定に使用する最小・最大値設定処理
void SearchVictoryStand_Land_Pos(void)
{
	for (int nCntLandPos = 0; nCntLandPos < NUM_VICTORYSTAND; nCntLandPos++)
	{
		g_pMeshVictoryStand[nCntLandPos] = NULL;	// メッシュ初期化
		g_pBuffMatVictoryStand[nCntLandPos] = NULL;	// マテリアルへのポインタ初期化
		g_dwNumMatVictoryStand[nCntLandPos] = NULL;	// マテリアルの数初期化

		/***********************************************
			Xファイルの読み込み and テクスチャ設定
		*************************************************/
		InitVicStdModel_Tex(nCntLandPos);

		int		nNumVtx;		//頂点数
		DWORD	dwSizeFVF;		//貯点フォーマットのサイズ
		BYTE	*pVtxBuff;		//頂点バッファへのポインタ

		//頂点数を取得
		nNumVtx = g_pMeshVictoryStand[nCntLandPos]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshVictoryStand[nCntLandPos]->GetFVF());

		//頂点バッファをロック
		g_pMeshVictoryStand[nCntLandPos]->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

		//頂点フォーマットのサイズ分、ポインタを進める
		pVtxBuff += dwSizeFVF;

		//当たり判定に使う頂点の最小・最大位置を取得
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 現在表彰台の取得した頂点座標のＹ座標が、変数に代入されている着地位置より高ければ、上書きする
			g_fLandPoint[nCntLandPos] = g_fLandPoint[nCntLandPos] < vtx.y ? vtx.y : g_fLandPoint[nCntLandPos];

			//頂点フォーマットのサイズ分、ポインタを進める
			pVtxBuff += dwSizeFVF;

			//頂点座標の代入
			vtx = *(D3DXVECTOR3 *)pVtxBuff;
		}

		//頂点バッファをアンロック
		g_pMeshVictoryStand[nCntLandPos]->UnlockVertexBuffer();
	}
}

//メッシュやマテリアルの初期化・設定処理
void InitVicStdModel_Tex(int nCntModex) 
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイル読み込み
	D3DXLoadMeshFromX(c_apVicStdFilePath[nCntModex], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatVictoryStand[nCntModex], NULL, &g_dwNumMatVictoryStand[nCntModex], &g_pMeshVictoryStand[nCntModex]);

	//表彰台のテクスチャ設定
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nCntModex]; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nCntModex]->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			for (int nCntTex = 0; nCntTex < VICTORYSTAND_TEX; nCntTex++)
			{
				if (g_pTextureVictoryStand[nCntTex] == NULL)
				{
					//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureVictoryStand[nCntTex]);
					break;
				}
			}
		}
	}
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
	//表彰台のポインタを取得
	VictoryStand *pVicStd = &g_VictoryStand[0];

	//リザルトが始まってから表彰台のあらわれる時間になったら
	if (VICSTD_APPEAR_TIME <= nCounterResultTime())
	{
		for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
		{
			pVicStd->pos.y += VICSTD_APPEAR_SPEED;

			//表彰台が地面から浮いてしまったら
			if (0.0f < pVicStd->pos.y)
			{
				//地面に戻す
				pVicStd->pos.y = 0.0f;
			}
		}
	}
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
	VictoryStand *pVicStd = &g_VictoryStand[0];	//表彰台のポインタを取得

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
	{
		//使われている
		if (pVicStd->bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldVictoryStand);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pVicStd->rot.y, pVicStd->rot.x, pVicStd->rot.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pVicStd->pos.x, pVicStd->pos.y, pVicStd->pos.z);

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

				//与えられた順位別のモデル(パーツ)の描画
				g_pMeshVictoryStand[pVicStd->nRank]->DrawSubset(nCntMat);
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
		//文字数削減のために、対象の表彰台の情報を格納
		VictoryStand VicStd = g_VictoryStand[nCntVicStd];

		//最初は地中に埋まっているので、着地点を都度、計算する
		float LandPoint = VicStd.pos.y + VicStd.fLandheight;

		//対象のプレイヤーと表彰台の�]位置が一致している（表彰台の位置は、プレイヤーの順位ごとに真下に配置するので、�]座標以外の判定は無し
		if (pPos->x == VicStd.pos.x)
		{
			if (pPos->y <= LandPoint)		//現在は着地点以下の高さにいる
			{
				//プレイヤーの位置を着地点に修正する
				pPos->y = LandPoint;
			}
		}
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
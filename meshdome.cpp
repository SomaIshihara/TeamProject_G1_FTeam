//--------------------------------------------------------------------------------------------------------
//
//メッシュドームの屋根・奈落[meshdome.cpp]
//Author:平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshdome.h"
#include "color.h"

//マクロ
#define NUM_MESHDOME				(1)			//メッシュドームの数
#define MESHDOME_WIDTH				(2000.0f)	//背景の広さ
#define MESHDOME_HEIGHT				(1000.0f)	//背景の高さ
#define MESHDOME_TEX_RESOLUTION		(3.0f)		//背景の解像度
#define MESHDOME_SPLIT				(64)		//背景の頂点数
#define MESHDOME_SEPALATE			(2)			//背景の縦の分割数
#define MESHDOME_NUM_OVERLAP		(2)			//最初と最後の頂点が重なる数
//#define MESHDOME_ALL_VERTEX			(MESHDOME_SPLIT * MESHDOME_SEPALATE + MESHDOME_NUM_OVERLAP)	//全体の頂点数  本来のマクロ
#define MESHDOME_ALL_VERTEX			(65)	//全体の頂点数		試験用のマクロ

//メッシュドームの構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	float			fRadius;	//半径
}MeshDome;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureMeshDome = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshDome = NULL;
D3DXMATRIX				g_mtxWorldMeshDome;
MeshDome				g_MeshDome;

//--------------------------------------------------------------------------------------------------------
//メッシュドームの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitMeshDome(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//設定用方向ベクトル
	D3DXVECTOR3 aVecDir;

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureMeshDome);

	//ドーム情報の初期化
	g_MeshDome.pos = ZERO_SET;
	g_MeshDome.rot = ZERO_SET;
	g_MeshDome.fRadius = MESHDOME_WIDTH;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshDome, NULL);

	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	/*for (int nCntMeshDome = 0; nCntMeshDome < NUM_MESHDOME; nCntMeshDome++, pVtx += MESHDOME_ALL_VERTEX)
	{
		float fAngle = D3DX_PI;	//角度

		//頂点座標の設定
		for (int nCntVtx = 0; nCntVtx <= MESHDOME_SPLIT; nCntVtx++)
		{
			//対象の頂点座標番号と、高さ以外が同じ情報を持つ頂点番号
			int nDissimilar = MESHDOME_SPLIT + nCntVtx + 1;

			pVtx[nCntVtx].pos = D3DXVECTOR3(
				sinf(fAngle) * g_MeshDome.fRadius,
				MESHDOME_HEIGHT,
				cosf(fAngle) * g_MeshDome.fRadius);

			pVtx[nDissimilar].pos = D3DXVECTOR3(
				sinf(fAngle) * g_MeshDome.fRadius,
				0.0f,
				cosf(fAngle) * g_MeshDome.fRadius);

			//最後の頂点は、最初の頂点と同じにする
			if (nCntVtx == MESHDOME_SPLIT)
			{
				pVtx[nCntVtx].pos = D3DXVECTOR3(
					sinf(D3DX_PI) * g_MeshDome.fRadius,
					MESHDOME_HEIGHT,
					cosf(D3DX_PI) * g_MeshDome.fRadius);

				pVtx[nDissimilar].pos = D3DXVECTOR3(
					sinf(D3DX_PI) * g_MeshDome.fRadius,
					0.0f,
					cosf(D3DX_PI) * g_MeshDome.fRadius);
			}

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nDissimilar].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntVtx].col = XCOL_WHITE;
			pVtx[nDissimilar].col = XCOL_WHITE;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * (MESHDOME_TEX_RESOLUTION / MESHDOME_SPLIT), 0.0f);
			pVtx[nDissimilar].tex = D3DXVECTOR2(nCntVtx * (MESHDOME_TEX_RESOLUTION / MESHDOME_SPLIT), 1.0f);

			fAngle -= (D3DX_PI * 2.0f) / MESHDOME_SPLIT;
		}
	}*/

	int nNumVtx = 0;		//頂点番号
	//天面の出っ張り頂点の設定
	pVtx[nNumVtx++].pos.x = g_MeshDome.pos.x;
	pVtx[nNumVtx++].pos.y = g_MeshDome.pos.y + MESHDOME_HEIGHT;
	pVtx[nNumVtx++].pos.z = g_MeshDome.pos.z;

	//横の分割数　‐　天面の出っ張りの１頂点　回数分 for文を回す
	for (int nCntDevideY = 0; nCntDevideY < MESHDOME_SPLIT - 1; nCntDevideY++)
	{

	}

	//ベクトルを正規化する
	D3DXVec3Normalize(&aVecDir, &aVecDir);

	//頂点バッファのアンロック
	g_pVtxBuffMeshDome->Unlock();

	//--------------------------------------
	//		インデックスバッファの生成
	//--------------------------------------
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshDome, NULL);

	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	/*for (int nCntIdx = 0; nCntIdx < MESHDOME_ALL_VERTEX; nCntIdx++)
	{
		if (nCntIdx % EVENPARITY == NOPARITY)
		{
			pIdx[nCntIdx] = nCntIdx / EVENPARITY;
		}
		if (nCntIdx % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + ODDPARITY + MESHDOME_SPLIT;
		}
	}*/
	
	//インデックスバッファのアンロック
	g_pIdxBuffMeshDome->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitMeshDome(void)
{
	//インデックスの破棄
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateMeshDome(void)
{

}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの描画処理
//--------------------------------------------------------------------------------------------------------
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshDome);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshDome);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHDOME_ALL_VERTEX, 0, MESHDOME_ALL_VERTEX);
}
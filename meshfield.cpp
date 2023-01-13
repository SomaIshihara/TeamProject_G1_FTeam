//==========================================
//
//メッシュフィールドプログラム[meshfield.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "player.h"
#include "meshfield.h"

//マクロ定義       ファイル読み込みに変える可能性あり
#define BESIDE					(2)						//横の分割数
#define VERTICAL				(2)						//縦の分割数
#define BESIDE_LENGTH			(250.0f)				//横の長さ
#define VERTICAL_LENGTH			(250.0f)				//縦の長さ
#define MAX_FIELD				(1)					//メッシュフィールド最大数

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;			//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXMATRIX g_mtxWorldMeshfield;							//ワールドマトリックス

MESHFIELD g_MeshField[MAX_FIELD];

//====================================================================
//メッシュフィールドの初期化処理
//====================================================================
void InitMeshfield(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	WORD *pIdx;					//インデックス情報へのポインタ


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field001.jpg",
		&g_pTextureMeshfield);

	//構造体を初期化
	for (int nCntfield = 0; nCntfield < MAX_FIELD; nCntfield++)
	{
		g_MeshField[nCntfield].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshField[nCntfield].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshField[nCntfield].fLength_X = BESIDE_LENGTH;
		g_MeshField[nCntfield].fLength_Z = VERTICAL_LENGTH;
		g_MeshField[nCntfield].nBlock_X = BESIDE;
		g_MeshField[nCntfield].nBlock_Z = VERTICAL;
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_MeshField[0].nBlock_X + 1)*(g_MeshField[0].nBlock_Z + 1),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield, NULL);


	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_MeshField[0].nBlock_X + 1) * 2 * g_MeshField[0].nBlock_Z +
		(g_MeshField[0].nBlock_Z - 1) * 2),
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntVtxZ = 0; nCntVtxZ < g_MeshField[0].nBlock_X + 1; nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < g_MeshField[0].nBlock_Z + 1; nCntVtxX++)
		{
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].pos
				= D3DXVECTOR3(
					(nCntVtxX - g_MeshField[0].nBlock_X / 2) * g_MeshField[0].fLength_X,
					0.0f,
					(nCntVtxZ - g_MeshField[0].nBlock_Z / 2) * g_MeshField[0].fLength_Z);

			//法線の設定
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ頂点座標の設定
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].tex = D3DXVECTOR2(1.0f*nCntVtxX, 1.0f*nCntVtxZ);
		}
	}

	g_pVtxBuffMeshfield->Unlock();

	//インデックスバッファをロックし頂点情報へのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定

	int nCntIdx = 0;

	for (int nCntDepth = 0; nCntDepth < g_MeshField[0].nBlock_X; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < g_MeshField[0].nBlock_Z + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((g_MeshField[0].nBlock_Z + 1) * (nCntDepth + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((g_MeshField[0].nBlock_Z + 1) * nCntDepth);
			nCntIdx += 2;
		}
		if (nCntDepth < g_MeshField[0].nBlock_X - 1)
		{
			pIdx[nCntIdx] = ((g_MeshField[0].nBlock_Z + 1) * (nCntDepth + 1)) - 1;
			pIdx[nCntIdx + 1] = (g_MeshField[0].nBlock_Z + 1) * (nCntDepth + 2);
			nCntIdx += 2;
		}
	}

	g_pIdxBuffMeshField->Unlock();
}

//====================================================================
//メッシュフィールドの終了処理
//====================================================================
void UninitMeshfield(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//====================================================================
//メッシュフィールドの更新処理
//====================================================================
void UpdateMeshfield(void)
{

}

//====================================================================
//メッシュフィールドの描画処理
//====================================================================
void DrawMeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntfield = 0; nCntfield < MAX_FIELD; nCntfield++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldMeshfield);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField[nCntfield].rot.y, g_MeshField[nCntfield].rot.x, g_MeshField[nCntfield].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MeshField[nCntfield].pos.x, g_MeshField[nCntfield].pos.y, g_MeshField[nCntfield].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshField);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshfield);

		//描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
			(g_MeshField[0].nBlock_X + 1)*(g_MeshField[0].nBlock_Z + 1),
			0,
			g_MeshField[0].nBlock_X*g_MeshField[0].nBlock_Z * 2 + (g_MeshField[0].nBlock_Z - 1) * 4);
	}
}
//--------------------------------------------------------------------------------------------------------
//
//メッシュの断面図[meshfault.h]
//Author:菊地陽人	平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshfault.h"

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureMeshFault = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshFault = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshFault = NULL;
D3DXVECTOR3				g_posMeshFault;
D3DXVECTOR3				g_rotMeshFault;
D3DXMATRIX				g_mtxWorldMeshFault;	//ワールドマトリックス

//--------------------------------------------------------------------------------------------------------
//断面の初期化処理
//--------------------------------------------------------------------------------------------------------
void InitMeshFault(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureMeshFault);

	//ポリゴンの初期化
	g_posMeshFault = ZERO_SET;
	g_rotMeshFault = ZERO_SET;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshFault, NULL);

	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffMeshFault->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntMeshFault = 0; nCntMeshFault < MESHFAULT_SPLIT + 1; nCntMeshFault++)
	{
		float	VtxPos_X = sinf(D3DX_PI * (1.0f - (2.0f / MESHFAULT_SPLIT * nCntMeshFault))) * MESHFAULT_WIDTH,
				VtxPos_Z = cosf(D3DX_PI * (1.0f - (2.0f / MESHFAULT_SPLIT * nCntMeshFault))) * MESHFAULT_WIDTH;

		//原点位置と同じ高さの頂点座標を設定
		pVtx[nCntMeshFault].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//上で設定した頂点座標の真下の頂点座標を設定
		pVtx[MESHFAULT_SPLIT + nCntMeshFault + 1].pos = D3DXVECTOR3(VtxPos_X, MESHFAULT_BOTTOM, VtxPos_Z);

		if (nCntMeshFault == MESHFAULT_SPLIT)
		{
			pVtx[nCntMeshFault].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH,
				MESHFAULT_BOTTOM,
				cosf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH);

			pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH,
				0.0f,
				cosf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH);
		}

		//法線ベクトルの設定
		pVtx[nCntMeshFault].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntMeshFault].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[nCntMeshFault].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 0.0f);
		pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 1.0f);
	}

	//頂点バッファのアンロック
	g_pVtxBuffMeshFault->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshFault, NULL);

	WORD*pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffMeshFault->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	for (int nCntMeshFault = 0; nCntMeshFault < MESHFAULT_SPLIT * 2 + 2; nCntMeshFault++)
	{
		if (nCntMeshFault % EVENPARITY == NOPARITY)
		{
			pIdx[nCntMeshFault] = nCntMeshFault / EVENPARITY;
		}
		if (nCntMeshFault % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntMeshFault] = (nCntMeshFault / EVENPARITY) + ODDPARITY + MESHFAULT_SPLIT;
		}
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshFault->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//断面の終了処理
//--------------------------------------------------------------------------------------------------------
void UninitMeshFault(void)
{
	//インデックスの破棄
	if (g_pIdxBuffMeshFault != NULL)
	{
		g_pIdxBuffMeshFault->Release();
		g_pIdxBuffMeshFault = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureMeshFault != NULL)
	{
		g_pTextureMeshFault->Release();
		g_pTextureMeshFault = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshFault != NULL)
	{
		g_pVtxBuffMeshFault->Release();
		g_pVtxBuffMeshFault = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//断面の更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateMeshFault(void)
{

}

//--------------------------------------------------------------------------------------------------------
//断面の描画処理
//--------------------------------------------------------------------------------------------------------
void DrawMeshFault(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshFault);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshFault.y, g_rotMeshFault.x, g_rotMeshFault.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshFault, &g_mtxWorldMeshFault, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshFault.x, g_posMeshFault.y, g_posMeshFault.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshFault, &g_mtxWorldMeshFault, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshFault);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshFault, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshFault);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshFault);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (MESHFAULT_SPLIT * 2 + 2), 0, (MESHFAULT_SPLIT * 2 + 2));
}
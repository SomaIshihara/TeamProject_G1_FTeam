//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.h]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "charge_cylinder.h"

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureChargeCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeCylinder = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffChargeCylinder = NULL;
D3DXVECTOR3				g_posChargeCylinder;
D3DXVECTOR3				g_rotChargeCylinder;
D3DXMATRIX				g_mtxWorldChargeCylinder;	//ワールドマトリックス

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitChargeCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureChargeCylinder);

	//チャージシリンダーの初期化
	g_posChargeCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotChargeCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeCylinder, NULL);

	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntChargeCylinder = 0; nCntChargeCylinder < CHARGESYLINDER_SPLIT + 1; nCntChargeCylinder++)
	{
		pVtx[nCntChargeCylinder].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) * CHARGESYLINDER_WIDTH,
			CHARGESYLINDER_HEIGHT,
			cosf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) * CHARGESYLINDER_WIDTH);

		pVtx[CHARGESYLINDER_SPLIT + nCntChargeCylinder + 1].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) *CHARGESYLINDER_WIDTH,
			0.0f,
			cosf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) * CHARGESYLINDER_WIDTH);

		if (nCntChargeCylinder == CHARGESYLINDER_SPLIT)
		{
			pVtx[nCntChargeCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *CHARGESYLINDER_WIDTH,
				CHARGESYLINDER_HEIGHT,
				cosf(D3DX_PI * 1.0f) * CHARGESYLINDER_WIDTH);

			pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *CHARGESYLINDER_WIDTH,
				0.0f,
				cosf(D3DX_PI * 1.0f) * CHARGESYLINDER_WIDTH);
		}

		//法線ベクトルの設定
		pVtx[nCntChargeCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntChargeCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[nCntChargeCylinder].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 0.0f);
		pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 1.0f);
	}

	//頂点バッファのアンロック
	g_pVtxBuffChargeCylinder->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffChargeCylinder, NULL);

	WORD*pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffChargeCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	for (int nCntChargeCylinder = 0; nCntChargeCylinder < CHARGESYLINDER_SPLIT * 2 + 2; nCntChargeCylinder++)
	{
		if (nCntChargeCylinder % EVENPARITY == NOPARITY)
		{
			pIdx[nCntChargeCylinder] = nCntChargeCylinder / EVENPARITY;
		}
		if (nCntChargeCylinder % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntChargeCylinder] = (nCntChargeCylinder / EVENPARITY) + ODDPARITY + CHARGESYLINDER_SPLIT;
		}
	}
	//インデックスバッファのアンロック
	g_pIdxBuffChargeCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitChargeCylinder(void)
{
	//インデックスの破棄
	if (g_pIdxBuffChargeCylinder != NULL)
	{
		g_pIdxBuffChargeCylinder->Release();
		g_pIdxBuffChargeCylinder = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureChargeCylinder != NULL)
	{
		g_pTextureChargeCylinder->Release();
		g_pTextureChargeCylinder = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffChargeCylinder != NULL)
	{
		g_pVtxBuffChargeCylinder->Release();
		g_pVtxBuffChargeCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateChargeCylinder(void)
{

}

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの描画処理
//--------------------------------------------------------------------------------------------------------
void DrawChargeCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldChargeCylinder);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotChargeCylinder.y, g_rotChargeCylinder.x, g_rotChargeCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posChargeCylinder.x, g_posChargeCylinder.y, g_posChargeCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldChargeCylinder);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffChargeCylinder, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffChargeCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureChargeCylinder);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (CHARGESYLINDER_SPLIT * 2 + 2), 0, (CHARGESYLINDER_SPLIT * 2 + 2));
}
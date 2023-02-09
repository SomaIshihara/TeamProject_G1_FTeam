//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.cpp]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "charge_cylinder.h"
#include "color.h"
#include "player.h"
#include "input.h"

//マクロ定義
#define NUM_CHARGE_CYLINDER			(4)		//シリンダー数
#define CHARGE_MAX_RADIUS			(60.0f)	//最大半径
#define CHARGE_SPREAD_SPEED			(4.0f)	//半径増加量　　SPREAD　＝　広がる
#define CHARGECYLINDER_ALL_VTX		(CHARGESYLINDER_SPLIT * 2 + 2)
#define CHARGE_TOPPART_SPREAD		(1.3f)	//上部の部分だけの広がり倍率

//断面情報の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	float			fRadius;	//半径の大きさ
	bool			bUse;		//使われているかどうか
}ChargeCylinder;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureChargeCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeCylinder = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffChargeCylinder = NULL;
D3DXMATRIX				g_mtxWorldChargeCylinder;	//ワールドマトリックス
ChargeCylinder			g_ChargeCylinder[NUM_CHARGE_CYLINDER];

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitChargeCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureChargeCylinder);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * CHARGECYLINDER_ALL_VTX * NUM_CHARGE_CYLINDER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeCylinder, NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * CHARGECYLINDER_ALL_VTX * NUM_CHARGE_CYLINDER, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffChargeCylinder, NULL);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//チャージシリンダーの情報の初期化
		g_ChargeCylinder[nCntCylinder].pos = ZERO_SET;
		g_ChargeCylinder[nCntCylinder].rot = ZERO_SET;
		g_ChargeCylinder[nCntCylinder].bUse = true;
		g_ChargeCylinder[nCntCylinder].fRadius = CHARGESYLINDER_RADIUS;
	}

	//頂点情報の設定処理
	SetChargeCylinderVertex();

	//インデックス番号の設定処理
	SetChargeCylinderIndex();
}

//断面の頂点情報の設定処理
void SetChargeCylinderVertex(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//Y軸の角度
		float Rot = D3DX_PI;

		//頂点座標の設定
		for (int nCntChargeCylinder = 0; nCntChargeCylinder <= CHARGESYLINDER_SPLIT; nCntChargeCylinder++)
		{
			float	VtxPos_X = sinf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius,	//Ｘ座標
					VtxPos_Z = cosf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius;	//Ｚ座標
			int		nNumBottomVtx = CHARGESYLINDER_SPLIT + nCntChargeCylinder + 1;	//対象の頂点の真下の頂点番号

			//原点位置と同じ高さの頂点座標を設定
			pVtx[nCntChargeCylinder].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

			//上で設定した頂点座標の真上の頂点座標を設定
			pVtx[nNumBottomVtx].pos = D3DXVECTOR3(VtxPos_X * CHARGE_TOPPART_SPREAD, CHARGESYLINDER_HEIGHT, VtxPos_Z * CHARGE_TOPPART_SPREAD);

			//１周したときの頂点座標
			if (nCntChargeCylinder == CHARGESYLINDER_SPLIT)
			{
				pVtx[nCntChargeCylinder].pos = pVtx[0].pos;						//最初の頂点座標を代入
				pVtx[nNumBottomVtx].pos = pVtx[CHARGESYLINDER_SPLIT + 1].pos;	//最初の上部の頂点座標を代入
			}

			//法線ベクトルの設定
			pVtx[nCntChargeCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nNumBottomVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntChargeCylinder].col = XCOL_WHITE;
			pVtx[nNumBottomVtx].col = XCOL_WHITE;

			//テクスチャ座標の設定
			pVtx[nCntChargeCylinder].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 0.0f);
			pVtx[nNumBottomVtx].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 1.0f);

			//角度を　全体の角度÷分割数で割った答え分、引く
			Rot -= ONE_LAP / CHARGESYLINDER_SPLIT;
		}

		//1つのシリンダーの頂点数分、ポインターをずらす
		pVtx += CHARGECYLINDER_ALL_VTX;
	}

	//頂点バッファのアンロック
	g_pVtxBuffChargeCylinder->Unlock();
}

//断面のインデックス番号の設定処理
void SetChargeCylinderIndex(void)
{
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffChargeCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//インデックス番号データの設定
		for (int nCntIdx = 0; nCntIdx < CHARGECYLINDER_ALL_VTX; nCntIdx++)
		{
			//カウンターが偶数の時
			if (nCntIdx % EVENPARITY == NOPARITY)
			{
				//シリンダー上部の頂点番号を記憶
				pIdx[nCntIdx] = nCntIdx / EVENPARITY;
			}

			//カウンターが奇数の時
			if (nCntIdx % EVENPARITY == ODDPARITY)
			{
				//シリンダー下部の頂点番号を記憶
				pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + ODDPARITY + CHARGESYLINDER_SPLIT;
			}
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
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//チャージシリンダーが使われている
		if (g_ChargeCylinder[nCntCylinder].bUse)
		{
			//シリンダーの位置をプレイヤーと同じにする
			g_ChargeCylinder[nCntCylinder].pos = pPlayer++->pos;

			//シリンダーの広がる処理
			SpreadChargeCylinder(nCntCylinder);

			//シリンダーの頂点情報の設定処理
			SetChargeCylinderVertex();
		}
	}
}

//シリンダーの広がる処理
void SpreadChargeCylinder(int nCntCylinder)
{
	//半径を広げる
	g_ChargeCylinder[nCntCylinder].fRadius += CHARGE_SPREAD_SPEED;

	//最大半径に到達した
	if (g_ChargeCylinder[nCntCylinder].fRadius >= CHARGE_MAX_RADIUS)
	{
		//半径を０にする
		g_ChargeCylinder[nCntCylinder].fRadius = 0.0f;
	}
}

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの描画処理
//--------------------------------------------------------------------------------------------------------
void DrawChargeCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//両面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldChargeCylinder);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ChargeCylinder[nCntCylinder].rot.y, g_ChargeCylinder[nCntCylinder].rot.x, g_ChargeCylinder[nCntCylinder].rot.z);
		D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ChargeCylinder[nCntCylinder].pos.x, g_ChargeCylinder[nCntCylinder].pos.y, g_ChargeCylinder[nCntCylinder].pos.z);
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

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, CHARGECYLINDER_ALL_VTX, 0, CHARGECYLINDER_ALL_VTX);
	}

	//両面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
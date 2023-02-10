//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.h]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "charge_cylinder.h"
#include "color.h"
#include "player.h"
#include "input.h"

//マクロ定義
#define NUM_CHARGE_CYLINDER				(4)		//シリンダー数
#define CHARGE_CYLINDER_MOVE			(3.0f)		//エフェクトのアタックタイプの変化量
#define CHARGE_CYLINDER_CLEAACCEL		(0.1f)		//エフェクトの透明加速度

//断面情報の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	float			fRadius;	//半径の大きさ
	float			fAlpha;		//透明度	
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

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//チャージシリンダーの初期化
		g_ChargeCylinder[nCntCylinder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ChargeCylinder[nCntCylinder].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ChargeCylinder[nCntCylinder].bUse = false;

		//頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeCylinder, NULL);

		//頂点情報の設定処理
		SetChargeCylinderVertex(nCntCylinder);

		//インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffChargeCylinder, NULL);

		//インデックス番号の設定処理
		SetChargeCylinderIndex();
	}
}

//断面の頂点情報の設定処理
void SetChargeCylinderVertex(int nCntCylinder)
{
	//半径を設定
	g_ChargeCylinder[nCntCylinder].fRadius = CHARGESYLINDER_WIDTH;

	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void**)&pVtx, 0);

	float Rot = D3DX_PI;	//Y軸の角度

							//頂点座標の設定
	for (int nCntChargeCylinder = 0; nCntChargeCylinder <= CHARGESYLINDER_SPLIT; nCntChargeCylinder++)
	{
		float	VtxPos_X = sinf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius,					//Ｘ座標
			VtxPos_Z = cosf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius;					//Ｚ座標
		int		nNumBottomVtx = CHARGESYLINDER_SPLIT + nCntChargeCylinder + 1;	//対象の頂点の真下の頂点番号

																		//原点位置と同じ高さの頂点座標を設定
		pVtx[nCntChargeCylinder].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//上で設定した頂点座標の真下の頂点座標を設定
		pVtx[nNumBottomVtx].pos = D3DXVECTOR3(VtxPos_X, CHARGESYLINDER_HEIGHT, VtxPos_Z);

		//１周したときの頂点座標
		if (nCntChargeCylinder == CHARGESYLINDER_SPLIT)
		{
			pVtx[nCntChargeCylinder].pos = pVtx[0].pos;						//最初の頂点座標を代入
			pVtx[nNumBottomVtx].pos = pVtx[CHARGESYLINDER_SPLIT + 1].pos;	//最初の足元の頂点座標を代入
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

	//頂点バッファのアンロック
	g_pVtxBuffChargeCylinder->Unlock();
}

//断面のインデックス番号の設定処理
void SetChargeCylinderIndex(void)
{
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
	if (GetKeyboardPress(DIK_F) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_CYLINDER; nCntEffect++)
		{
			SetChargeCylinder(g_ChargeCylinder[nCntEffect].pos, nCntEffect);
		}
	}

	SetChargeCylinderPos();

	//エフェクトのサイズ更新  (頂点座標の更新もするので、このUpdate関数の最後が望ましい)
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_CYLINDER; nCntEffect++)
	{
		UpdateChargeCylinderSize(nCntEffect);
	}
}

//エフェクトのサイズ更新処理
void UpdateChargeCylinderSize(int nEffect)
{
	if (g_ChargeCylinder[nEffect].fRadius <= CHARGESYLINDER_WIDTH_MAX)
	{
		//エフェクトの大きさを拡大
		g_ChargeCylinder[nEffect].fRadius += CHARGE_CYLINDER_MOVE;

	}

	//エフェクトの大きさが規定値になった
	if (g_ChargeCylinder[nEffect].fRadius >= CHARGESYLINDER_WIDTH)
	{
		g_ChargeCylinder[nEffect].fAlpha -= CHARGE_CYLINDER_CLEAACCEL;

		if (g_ChargeCylinder[nEffect].fAlpha <= 0.0f)
		{
			//エフェクト本来の大きさに直す
			g_ChargeCylinder[nEffect].fRadius = CHARGESYLINDER_WIDTH;
			g_ChargeCylinder[nEffect].bUse = false;
		}

	}

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

												//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_ChargeCylinder[nEffect].fRadius, 0.0f, +g_ChargeCylinder[nEffect].fRadius);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_ChargeCylinder[nEffect].fRadius, 0.0f, +g_ChargeCylinder[nEffect].fRadius);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_ChargeCylinder[nEffect].fRadius, 0.0f, -g_ChargeCylinder[nEffect].fRadius);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_ChargeCylinder[nEffect].fRadius, 0.0f, -g_ChargeCylinder[nEffect].fRadius);

	pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeCylinder[nEffect].fAlpha);
	pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeCylinder[nEffect].fAlpha);
	pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeCylinder[nEffect].fAlpha);
	pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeCylinder[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffChargeCylinder->Unlock();
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

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldChargeCylinder);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
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

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (CHARGESYLINDER_SPLIT * 2 + 2), 0, (CHARGESYLINDER_SPLIT * 2 + 2));
	}
}

//エフェクトの位置設定
void SetChargeCylinderPos()
{
	Player *pPlayer = GetPlayer();

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_CYLINDER; nCntEffect++, pPlayer++)
	{
		//対象のエフェクトが使われている
		if (g_ChargeCylinder[nCntEffect].bUse == true)
		{
			//エフェクトの位置をプレイヤーの位置にする
			g_ChargeCylinder[nCntEffect].pos = pPlayer->pos;
		}

	}
}

//エフェクトの設定処理
void SetChargeCylinder(D3DXVECTOR3 pos, int nCntType)
{
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_CYLINDER; nCntEffect++)
	{
		//対象のエフェクトが使われていない
		if (g_ChargeCylinder[nCntType].bUse == false)
		{
			g_ChargeCylinder[nCntType].fRadius = CHARGESYLINDER_WIDTH;		//サイズを初期化

			g_ChargeCylinder[nCntType].fAlpha = 1.0f;		//透明度
			g_ChargeCylinder[nCntType].bUse = true;		//使われている状態に
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.cpp]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "charge_cylinder.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//マクロ定義
#define CHARGECYLINDER_HEIGHT				(20.0f)						// シリンダーの高さ
#define CHARGECYLINDER_SPLIT				(32)						// シリンダーの分割数
#define CHARGECYLINDER_TEX_RESOLUTION		(3.0f)						// シリンダーのテクスチャの幅
#define NUM_CHARGE_CYLINDER					(4)							// シリンダーの数
#define CHARGECYLINDER_ALL_VTX		(CHARGECYLINDER_SPLIT * 2 + 2)		// シリンダーの最大頂点・インデックス数
#define CHARGE_MAX_RADIUS					(40.0f)						// 最大半径
#define CHARGE_SPREAD_SPEED					(3.5f)						// 半径増加量	SPREAD ＝ 広がる
#define CHARGE_TOPPART_SPREAD				(1.3f)						// 上部の部分だけの広がり倍率
#define CHARGE_HEIGHT_EXTEND				(2.0f)						// 高さが伸び縮みするスピード
#define CHARGE_TEX_PASS				"data/TEXTURE/ChargeCylinder.png"	// チャージシリンダーのテクスチャパス

//チャージシリンダー情報の構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	float			fRadius;	// 半径の大きさ
	float			fHeight;	// 高さ
	bool			bExtend;	// 高さの伸び縮み判定		true:伸びる		false:縮む
	bool			bUse;		// 使われているかどうか
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
	D3DXCreateTextureFromFile(pDevice, CHARGE_TEX_PASS, &g_pTextureChargeCylinder);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * CHARGECYLINDER_ALL_VTX * NUM_CHARGE_CYLINDER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeCylinder, NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * CHARGECYLINDER_ALL_VTX * NUM_CHARGE_CYLINDER, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffChargeCylinder, NULL);

	//チャージシリンダーの情報の初期化
	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		g_ChargeCylinder[nCntCylinder].pos = ZERO_SET;
		g_ChargeCylinder[nCntCylinder].rot = ZERO_SET;
		g_ChargeCylinder[nCntCylinder].fRadius = 0.0f;
		g_ChargeCylinder[nCntCylinder].fHeight = 0.0f;
		g_ChargeCylinder[nCntCylinder].bUse = false;
		g_ChargeCylinder[nCntCylinder].bExtend = true;

		//頂点情報の設定処理
		SetChargeCylinderVertex(nCntCylinder);

		//インデックス番号の設定処理
		SetChargeCylinderIndex(nCntCylinder);
	}
}

//断面の頂点情報の設定処理
void SetChargeCylinderVertex(int nCntCylinder)
{
	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void* *)&pVtx, 0);

	//ポインターをずらす
	pVtx += CHARGECYLINDER_ALL_VTX * nCntCylinder;

	float Rot = D3DX_PI;	//Y軸の角度	
	float LapSepRot = ONE_LAP / CHARGECYLINDER_SPLIT;	//１周の分割割合を求める
	float gauge = GetPlayer()[nCntCylinder].moveGauge;		//プレイヤーのゲージ量を格納
	float TexSep_U = CHARGECYLINDER_TEX_RESOLUTION / CHARGECYLINDER_SPLIT;//テクスチャの横の分割の割合を求める
	D3DXCOLOR col = D3DXCOLOR(1.0f * gauge, 1.0f - gauge * 0.3f, 1.0f - gauge, 1.0f);//プレイヤーのゲージ量に応じたグラデーション

	//頂点座標の設定
	for (int nCntVtx = 0; nCntVtx <= CHARGECYLINDER_SPLIT; nCntVtx++)
	{
		float	VtxPos_X = sinf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius,	//Ｘ座標
				VtxPos_Z = cosf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius;	//Ｚ座標
		int		nNumTopVtx = CHARGECYLINDER_SPLIT + nCntVtx + 1;		//対象の頂点の上部の頂点番号

		//原点位置と同じ高さの頂点座標を設定
		pVtx[nCntVtx].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//上で設定した頂点座標の真上の頂点座標を設定
		pVtx[nNumTopVtx].pos = D3DXVECTOR3(VtxPos_X * CHARGE_TOPPART_SPREAD, g_ChargeCylinder[nCntCylinder].fHeight, VtxPos_Z * CHARGE_TOPPART_SPREAD);

		//１周したときの頂点座標
		if (nCntVtx == CHARGECYLINDER_SPLIT)
		{
			pVtx[nCntVtx].pos = pVtx[0].pos;							//最初の頂点座標を代入
			pVtx[nNumTopVtx].pos = pVtx[CHARGECYLINDER_SPLIT + 1].pos;	//最初の上部の頂点座標を代入
		}

		//法線ベクトルの設定
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nNumTopVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntVtx].col = col;
		pVtx[nNumTopVtx].col = col;

		//テクスチャ座標の設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 1.0f);
		pVtx[nNumTopVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 0.0f);

		//角度を　全体の角度÷分割数で割った答え分、引く
		Rot -= LapSepRot;
	}

	//頂点バッファのアンロック
	g_pVtxBuffChargeCylinder->Unlock();
}

//断面のインデックス番号の設定処理
void SetChargeCylinderIndex(int nCntCylinder)
{
	WORD *pIdx;
	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffChargeCylinder->Lock(0, 0, (void* *)&pIdx, 0);

	pIdx += CHARGECYLINDER_ALL_VTX * nCntCylinder;			//ポインターをずらす
	int nBottomNumber = ODDPARITY + CHARGECYLINDER_SPLIT;	//シリンダー下部の基準頂点番号

	//インデックス番号データの設定
	for (int nCntIdx = 0; nCntIdx < CHARGECYLINDER_ALL_VTX; nCntIdx++)
	{
		//カウンターが偶数の場合、後半以降の計算式 ↓ の答えがゼロになる	奇数の場合は、シリンダー下部の基準頂点番号が加算される
		pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + (nBottomNumber * (nCntIdx % EVENPARITY));
	}

	//インデックスバッファのアンロック
	g_pIdxBuffChargeCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitChargeCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureChargeCylinder != NULL)
	{
		g_pTextureChargeCylinder->Release();
		g_pTextureChargeCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffChargeCylinder != NULL)
	{
		g_pVtxBuffChargeCylinder->Release();
		g_pVtxBuffChargeCylinder = NULL;
	}

	//インデックスの破棄
	if (g_pIdxBuffChargeCylinder != NULL)
	{
		g_pIdxBuffChargeCylinder->Release();
		g_pIdxBuffChargeCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//チャージシリンダーの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateChargeCylinder(void)
{
	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//チャージシリンダーが使われている
		if (g_ChargeCylinder[nCntCylinder].bUse == true)
		{
			//プレイヤーの位置に置く
			SetChargeCylinderPos(nCntCylinder);

			//シリンダーの広がる処理
			SpreadChargeCylinder(nCntCylinder);

			//シリンダーを伸び縮みさせる処理
			ExtendChargeCylinder(nCntCylinder);

			//シリンダーの頂点情報の設定処理
			SetChargeCylinderVertex(nCntCylinder);
		}
	}
}

// プレイヤーの位置に設定する
void SetChargeCylinderPos(int nCntPos)
{
	//シリンダーの位置をプレイヤーと同じにする
	g_ChargeCylinder[nCntPos].pos = GetPlayer()[nCntPos].pos;
}

// シリンダーの広がる処理
void SpreadChargeCylinder(int nCntSpread)
{
	//半径を広げる
	g_ChargeCylinder[nCntSpread].fRadius += CHARGE_SPREAD_SPEED;

	//最大半径に到達した
	if (g_ChargeCylinder[nCntSpread].fRadius >= CHARGE_MAX_RADIUS)
	{
		//使わなくする
		g_ChargeCylinder[nCntSpread].bUse = false;
	}
}

// チャージシリンダーの伸び縮みする処理
void ExtendChargeCylinder(int nCntExtend)
{
	//伸びる
	if (g_ChargeCylinder[nCntExtend].bExtend == true)
	{
		//伸びる
		g_ChargeCylinder[nCntExtend].fHeight += CHARGE_HEIGHT_EXTEND;

		//最大の高さを超えた
		if (g_ChargeCylinder[nCntExtend].fHeight > CHARGECYLINDER_HEIGHT)
		{
			g_ChargeCylinder[nCntExtend].fHeight = CHARGECYLINDER_HEIGHT;	//最大の高さにする
			g_ChargeCylinder[nCntExtend].bExtend = false;					//次回から縮ませる
		}
	}

	//縮む
	else
	{
		//伸びる
		g_ChargeCylinder[nCntExtend].fHeight = CHARGE_HEIGHT_EXTEND;

		//最低の高さを超えた
		if (g_ChargeCylinder[nCntExtend].fHeight < 0.0f)
		{
			g_ChargeCylinder[nCntExtend].fHeight = 0.0f;	//最低の高さにする
			g_ChargeCylinder[nCntExtend].bExtend = true;	//次回から伸ばす
		}
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

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//シリンダーが使われている
		if (g_ChargeCylinder[nCntCylinder].bUse == true)
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

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, CHARGECYLINDER_ALL_VTX, 0, CHARGECYLINDER_ALL_VTX);
		}
	}

	//通常カリングにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

// シリンダーの設定処理
void SetChargeCylinder(D3DXVECTOR3 pos, int nCntSet)
{
	//対象シリンダーが使われていない
	if (g_ChargeCylinder[nCntSet].bUse == false)
	{
		g_ChargeCylinder[nCntSet].pos = pos;			// 位置反映
		g_ChargeCylinder[nCntSet].rot = ZERO_SET;		// 向き初期化
		g_ChargeCylinder[nCntSet].fRadius = 0.0f;		// 半径初期化
		g_ChargeCylinder[nCntSet].fHeight = 0.0f;		// 高さ初期化
		g_ChargeCylinder[nCntSet].bExtend = true;		// 伸びる
		g_ChargeCylinder[nCntSet].bUse = true;			// 使用する
	}
}
//==========================================
//
//エフェクトプログラム[charge_effect.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "game.h"
#include "charge_effect.h"
#include "color.h"
#include "player.h"
#include "input.h"

//テクスチャの情報
#define NUM_CHARGE_EFFECT				(4)			//テクスチャの最大表示数

#define CHARGE_EFFECT_SIZE				(80.0f)		//エフェクトのサイズ
#define EFFECT_CHARGE_MOVE				(3.5f)		//エフェクトのチャージタイプの変化量

//マクロ定義
#define	CHARGE_EFFECT_TEX_PASS		"data\\TEXTURE\\charge_effect002.png"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeEffect = NULL;				//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureChargeEffect = NULL;	//テクスチャのポインタ
D3DXMATRIX				mtxWorldChargeEffect;							//ワールドマトリックス
ChargeEffect			g_ChargeEffect[NUM_CHARGE_EFFECT];					//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得	

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		CHARGE_EFFECT_TEX_PASS,
		&g_pTextureChargeEffect);
	
	
	//エフェクトの位置を設定
	SetChargeEffectPos();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VTX_MAX * NUM_CHARGE_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffChargeEffect, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		//g_ChargeEffect[nCntEffect].nType = EFFECTTYPE_CHARGE;	//種類初期化
		g_ChargeEffect[nCntEffect].nCntLoop = 0;				//ループ回数初期化
		g_ChargeEffect[nCntEffect].fSize = CHARGE_EFFECT_SIZE;		//サイズ初期化
		g_ChargeEffect[nCntEffect].fAlpha = 1.0f;				//透明度初期化
		g_ChargeEffect[nCntEffect].bUse = false;				//使われていない状態に
		g_ChargeEffect[nCntEffect].bUseCharge = false;		//使われていない状態に

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_ChargeEffect[nCntEffect].fSize, 0.0f, +g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_ChargeEffect[nCntEffect].fSize, 0.0f, +g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_ChargeEffect[nCntEffect].fSize, 0.0f, -g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_ChargeEffect[nCntEffect].fSize, 0.0f, -g_ChargeEffect[nCntEffect].fSize);

		//nor(法線)の設定
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//テクスチャ頂点座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffChargeEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitChargeEffect(void)
{
	
	//テクスチャの破棄
	if (g_pTextureChargeEffect != NULL)
	{
		g_pTextureChargeEffect->Release();
		g_pTextureChargeEffect = NULL;
	}
	

	//バッファの破棄
	if (g_pVtxBuffChargeEffect != NULL)
	{
		g_pVtxBuffChargeEffect->Release();
		g_pVtxBuffChargeEffect = NULL;
	}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateChargeEffect(void)
{
	if (GetKeyboardPress(DIK_M) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
		{
			SetChargeEffect(g_ChargeEffect[nCntEffect].pos, nCntEffect);
		}
	}
	
	//エフェクトの位置を設定
	SetChargeEffectPos();

	//エフェクトのサイズ更新  (頂点座標の更新もするので、このUpdate関数の最後が望ましい)
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		UpdateChargeEffectSize(nCntEffect);
	}
}

//エフェクトのサイズ更新
void UpdateChargeEffectSize(int nEffect)
{
	
		//エフェクトの大きさを拡大
		g_ChargeEffect[nEffect].fSize += EFFECT_CHARGE_MOVE;

		//エフェクトの大きさがゼロになった
		if (g_ChargeEffect[nEffect].fSize >= CHARGE_EFFECT_SIZE)
		{
			//エフェクト本来の大きさに直す
			g_ChargeEffect[nEffect].fSize = CHARGE_EFFECT_SIZE;
			//g_ChargeEffect[nEffect].nCntLoop++;		//ループ回数加算
			g_ChargeEffect[nEffect].bUse = false;
		}

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_ChargeEffect[nEffect].fSize, 0.0f, +g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_ChargeEffect[nEffect].fSize, 0.0f, +g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_ChargeEffect[nEffect].fSize, 0.0f, -g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_ChargeEffect[nEffect].fSize, 0.0f, -g_ChargeEffect[nEffect].fSize);

	pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeEffect[nEffect].fAlpha);
	pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeEffect[nEffect].fAlpha);
	pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeEffect[nEffect].fAlpha);
	pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_ChargeEffect[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffChargeEffect->Unlock();
}

//=================================
//エフェクトの描画処理
//=================================
void DrawChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		if (g_ChargeEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldChargeEffect);

			//Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_ChargeEffect[nCntEffect].pos.x, g_ChargeEffect[nCntEffect].pos.y, g_ChargeEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&mtxWorldChargeEffect, &mtxWorldChargeEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldChargeEffect);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffChargeEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureChargeEffect);

			//描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, VTX_MAX * nCntEffect, 2);

			//Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
		}
	}
}

//エフェクトの位置設定
void SetChargeEffectPos()
{
	Player *pPlayer = GetPlayer();

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++, pPlayer++)
	{
		//対象のエフェクトが使われている
		if (g_ChargeEffect[nCntEffect].bUse == true)
		{
			//エフェクトの位置をプレイヤーの位置にする
			g_ChargeEffect[nCntEffect].pos = pPlayer->pos;
		}

	}
}

//エフェクトの設定処理
void SetChargeEffect(D3DXVECTOR3 pos, int nCntType)
{
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		//対象のエフェクトが使われていない
		if (g_ChargeEffect[nCntType].bUse == false)
		{
			g_ChargeEffect[nCntType].fSize = 0.0f;		//サイズを初期化
			
			g_ChargeEffect[nCntType].nCntLoop = 0;		//ループ回数初期化
			g_ChargeEffect[nCntType].fAlpha = 1.0f;		//透明度
			g_ChargeEffect[nCntType].bUse = true;		//使われている状態に
			//break;
		}
	}
}
//"data\\TEXTURE\\AttackEffect.png",
//if (GetKeyboardPress(DIK_X) == true)
//{
//	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
//	{
//		SetEffect(g_ChargeEffect[nCntEffect].pos, nCntEffect, EFFECTTYPE_ATTACK);
//	}
//}
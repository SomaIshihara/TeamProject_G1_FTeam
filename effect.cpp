//==========================================
//
//ポーズプログラム[effect.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "game.h"
#include "effect.h"
#include "color.h"
#include "player.h"

//テクスチャの情報
#define NUM_EFFECT_TEX			(1)						//テクスチャの数
#define NUM_EFFECT				(4)						//テクスチャの最大数

const char *c_pFileNameEffect[] =
{
	"data\\TEXTURE\\charge_effect001.png"
};

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;				//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureEffect[NUM_EFFECT_TEX] = {};	//テクスチャのポインタ
D3DXMATRIX				mtxWorldEffect;							//ワールドマトリックス
Effect					g_Effect[NUM_EFFECT];					//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT_TEX; nCntEffect++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_pFileNameEffect[nCntEffect],
			&g_pTextureEffect[nCntEffect]);
	}
	
	//エフェクトの位置を設定
	SetEffectPos();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VTX_MAX * NUM_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-40.0f, +40.0f, 0.0f);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+40.0f, +40.0f, 0.0f);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-40.0f, -40.0f, 0.0f);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+40.0f, -40.0f, 0.0f);

		//nor(法線)の設定
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = RGBA_RED;
		pVtx[VTX_RI_UP].col = RGBA_RED;
		pVtx[VTX_LE_DO].col = RGBA_RED;
		pVtx[VTX_RI_DO].col = RGBA_RED;

		//テクスチャ頂点座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT_TEX; nCntEffect++)
	{
		//テクスチャの破棄
		if (g_pTextureEffect[nCntEffect] != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}
	}

	//バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateEffect(void)
{
	//エフェクトの位置を設定
	SetEffectPos();
}

//=================================
//エフェクトの描画処理
//=================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorldEffect);

		//Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

		//ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&mtxWorldEffect, NULL, &mtxView);//逆行列を求める
															  /*g_Billboard.mtxWorld._12 = 0.0f;
															  g_Billboard.mtxWorld._21 = 0.0f;*/
		mtxWorldEffect._41 = 0.0f;
		mtxWorldEffect._42 = 0.0f;
		mtxWorldEffect._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Effect[nCntEffect].pos.x, g_Effect[nCntEffect].pos.y, g_Effect[nCntEffect].pos.z);

		D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorldEffect);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEffect[0]);

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

//エフェクトの位置設定
void SetEffectPos(void)
{
	Player *pPlayer = GetPlayer();

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++, pPlayer++)
	{
		//エフェクトの位置をプレイヤーの位置にする
		g_Effect[nCntEffect].pos = pPlayer->pos;
	}
}
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
#include "input.h"

//テクスチャの情報
#define NUM_EFFECT				(4)			//テクスチャの最大数
#define MAX_EFFECT				(128)		//テクスチャ最大数

#define EFFECT_SIZE				(80.0f)		//エフェクトのサイズ
#define EFFECT_CHARGE_MOVE		(3.5f)		//エフェクトのチャージタイプの変化量
#define EFFECT_ATTACK_MOVE		(8.0f)		//エフェクトのアタックタイプの変化量

//テクスチャのパス名
const char *c_pFileNameEffect[EFFECTTYPE_MAX] =
{
	"data\\TEXTURE\\charge_effect002.png",
	"data\\TEXTURE\\AttackEffect.png",
};

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;				//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureEffect[EFFECTTYPE_MAX] = {};	//テクスチャのポインタ
D3DXMATRIX				mtxWorldEffect;							//ワールドマトリックス
Effect					g_Effect[NUM_EFFECT];					//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得	

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_pFileNameEffect[nCntEffect],
			&g_pTextureEffect[nCntEffect]);
	}
	
	//エフェクトの位置を設定
	SetEffectPos();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VTX_MAX * MAX_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_Effect[nCntEffect].nType = EFFECTTYPE_CHARGE;	//種類初期化
		g_Effect[nCntEffect].nCntLoop = 0;				//ループ回数初期化
		g_Effect[nCntEffect].fSize = EFFECT_SIZE;		//サイズ初期化
		g_Effect[nCntEffect].fAlpha = 1.0f;				//透明度初期化
		g_Effect[nCntEffect].bUse = false;				//使われていない状態に
		g_Effect[nCntEffect].bUseCharge = false;		//使われていない状態に

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fSize, 0.0f, +g_Effect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fSize, 0.0f, +g_Effect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fSize, 0.0f, -g_Effect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fSize, 0.0f, -g_Effect[nCntEffect].fSize);

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

	g_pVtxBuffEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
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
	if (GetKeyboardPress(DIK_X) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
		{
			SetEffect(g_Effect[nCntEffect].pos ,nCntEffect, EFFECTTYPE_ATTACK);
		}
	}

	if (GetKeyboardPress(DIK_M) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
		{
			SetEffect(g_Effect[nCntEffect].pos, nCntEffect, EFFECTTYPE_CHARGE);
		}
	}
	
	//エフェクトの位置を設定
	SetEffectPos();

	//エフェクトのサイズ更新  (頂点座標の更新もするので、このUpdate関数の最後が望ましい)
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		UpdateEffectSize(nCntEffect);
	}
}

//エフェクトのサイズ更新
void UpdateEffectSize(int nEffect)
{
	//エフェクトの種類によるサイズの更新
	switch (g_Effect[nEffect].nType)
	{
	case EFFECTTYPE_CHARGE:
	{
		//エフェクトの大きさを拡大
		g_Effect[nEffect].fSize += EFFECT_CHARGE_MOVE;

		//エフェクトの大きさがゼロになった
		if (g_Effect[nEffect].fSize >= EFFECT_SIZE)
		{
			//エフェクト本来の大きさに直す
			g_Effect[nEffect].fSize = EFFECT_SIZE;
			//g_Effect[nEffect].nCntLoop++;		//ループ回数加算
			g_Effect[nEffect].bUse = false;

	/*		if (g_Effect[nEffect].nCntLoop >= 5)
			{
				
				g_Effect[nEffect].nCntLoop = 0;
			}*/
		}
	}
	break;

	case EFFECTTYPE_ATTACK:
	{

		//エフェクトの大きさを拡大
		g_Effect[nEffect].fSize += EFFECT_ATTACK_MOVE;

		//エフェクトの大きさが本来の大きさを超えた
		if (EFFECT_SIZE <= g_Effect[nEffect].fSize)
		{
			g_Effect[nEffect].fAlpha -= 0.1f;

			if (g_Effect[nEffect].fAlpha <= 0.0f)
			{
				//使われていない状態に
				g_Effect[nEffect].bUse = false;
			}
			
			g_Effect[nEffect].nCntLoop = 0;				//ループ回数初期化
		}
	}
	}

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Effect[nEffect].fSize, 0.0f, +g_Effect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Effect[nEffect].fSize, 0.0f, +g_Effect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Effect[nEffect].fSize, 0.0f, -g_Effect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Effect[nEffect].fSize, 0.0f, -g_Effect[nEffect].fSize);

	pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);
	pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);
	pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);
	pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
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
		if (g_Effect[nCntEffect].bUse == true)
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
			pDevice->SetTexture(0, g_pTextureEffect[g_Effect[nCntEffect].nType]);

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
void SetEffectPos(void)
{
	Player *pPlayer = GetPlayer();

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++, pPlayer++)
	{
		//対象のエフェクトが使われている
		if (g_Effect[nCntEffect].bUse == true)
		{
			//エフェクトの位置をプレイヤーの位置にする
			g_Effect[nCntEffect].pos = pPlayer->pos;
		}
		//エフェクトのタイプがチャージなら、追従する
		if (g_Effect[nCntEffect].nType == EFFECTTYPE_CHARGE)
		{
			
		}
	}
}

//エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos, int nCntType, EFFECTTYPE type)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//対象のエフェクトが使われていない
		if (g_Effect[nCntEffect].bUse == false)
		{
			//種類がチャージ
			if (type == EFFECTTYPE_CHARGE)
			{
				g_Effect[nCntEffect].fSize = 0.0f;		//サイズを初期化
			}

			//種類が当たった時のもの
			else if (type == EFFECTTYPE_ATTACK)
			{
				g_Effect[nCntEffect].fSize = 0.0f;		//サイズを初期化
			}

			g_Effect[nCntEffect].nType = type;		//種類設定
			g_Effect[nCntEffect].nCntLoop = 0;		//ループ回数初期化
			g_Effect[nCntEffect].fAlpha = 1.0f;
			g_Effect[nCntEffect].bUse = true;		//使われている状態に
		}
	}
}
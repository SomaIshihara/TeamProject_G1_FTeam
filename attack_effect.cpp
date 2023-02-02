//==========================================
//
//エフェクトプログラム[charge_effect.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "game.h"
#include "attack_effect.h"
#include "color.h"
#include "player.h"
#include "input.h"

//テクスチャの情報
#define NUM_ATTACK_EFFECT				(4)			//テクスチャの最大表示数

#define ATTACK_EFFECT_SIZE				(40.0f)		//エフェクトのサイズ
#define EFFECT_ATTACK_MOVE				(8.0f)		//エフェクトのアタックタイプの変化量

//マクロ定義
#define	CHARGE_EFFECT_TEX_PASS		"data\\TEXTURE\\AttackEffect.png"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackEffect = NULL;				//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureAttackEffect = NULL;	//テクスチャのポインタ
D3DXMATRIX				mtxWorldAttackEffect;							//ワールドマトリックス
AttackEffect			g_AttackEffect[NUM_ATTACK_EFFECT];					//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitAttackEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得	

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		CHARGE_EFFECT_TEX_PASS,
		&g_pTextureAttackEffect);
	
	
	//エフェクトの位置を設定
	SetAttackEffectPos();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VTX_MAX * NUM_ATTACK_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffAttackEffect, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffAttackEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		//g_AttackEffect[nCntEffect].nType = EFFECTTYPE_CHARGE;	//種類初期化
		g_AttackEffect[nCntEffect].nCntLoop = 0;				//ループ回数初期化
		g_AttackEffect[nCntEffect].fSize = ATTACK_EFFECT_SIZE;		//サイズ初期化
		g_AttackEffect[nCntEffect].fAlpha = 1.0f;				//透明度初期化
		g_AttackEffect[nCntEffect].fResize = 0.0f;
		g_AttackEffect[nCntEffect].bUse = false;				//使われていない状態に
	

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_AttackEffect[nCntEffect].fSize, 0.0f, +g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_AttackEffect[nCntEffect].fSize, 0.0f, +g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_AttackEffect[nCntEffect].fSize, 0.0f, -g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_AttackEffect[nCntEffect].fSize, 0.0f, -g_AttackEffect[nCntEffect].fSize);

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

	g_pVtxBuffAttackEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitAttackEffect(void)
{
	
	//テクスチャの破棄
	if (g_pTextureAttackEffect != NULL)
	{
		g_pTextureAttackEffect->Release();
		g_pTextureAttackEffect = NULL;
	}
	

	//バッファの破棄
	if (g_pVtxBuffAttackEffect != NULL)
	{
		g_pVtxBuffAttackEffect->Release();
		g_pVtxBuffAttackEffect = NULL;
	}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateAttackEffect(void)
{
	if (GetKeyboardPress(DIK_X) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
		{
			SetAttackEffect(g_AttackEffect[nCntEffect].pos, nCntEffect);
		}
	}
	
	//エフェクトの位置を設定
	SetAttackEffectPos();

	//エフェクトのサイズ更新  (頂点座標の更新もするので、このUpdate関数の最後が望ましい)
	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		UpdateAttackEffectSize(nCntEffect);
	}
}

//エフェクトのサイズ更新
void UpdateAttackEffectSize(int nEffect)
{
	g_AttackEffect[nEffect].fResize += EFFECT_ATTACK_MOVE;

	//エフェクトの大きさを拡大
	g_AttackEffect[nEffect].fSize += g_AttackEffect[nEffect].fResize;

	//エフェクトの大きさが規定値になった
	if (g_AttackEffect[nEffect].fSize >= ATTACK_EFFECT_SIZE)
	{
		g_AttackEffect[nEffect].fAlpha -= 0.2f;

		if (g_AttackEffect[nEffect].fAlpha <= 0.0f)
		{
			//エフェクト本来の大きさに直す
			g_AttackEffect[nEffect].fSize = ATTACK_EFFECT_SIZE;
			g_AttackEffect[nEffect].bUse = false;
		}

	}

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffAttackEffect->Lock(0, 0, (void**)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_AttackEffect[nEffect].fSize, 0.0f, +g_AttackEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_AttackEffect[nEffect].fSize, 0.0f, +g_AttackEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_AttackEffect[nEffect].fSize, 0.0f, -g_AttackEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_AttackEffect[nEffect].fSize, 0.0f, -g_AttackEffect[nEffect].fSize);

	pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);
	pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);
	pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);
	pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffAttackEffect->Unlock();
}

//=================================
//エフェクトの描画処理
//=================================
void DrawAttackEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		if (g_AttackEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldAttackEffect);

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
			D3DXMatrixTranslation(&mtxTrans, g_AttackEffect[nCntEffect].pos.x, g_AttackEffect[nCntEffect].pos.y, g_AttackEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&mtxWorldAttackEffect, &mtxWorldAttackEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldAttackEffect);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffAttackEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureAttackEffect);

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
void SetAttackEffectPos()
{
	Player *pPlayer = GetPlayer();

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++, pPlayer++)
	{
		//対象のエフェクトが使われている
		if (g_AttackEffect[nCntEffect].bUse == true)
		{
			//エフェクトの位置をプレイヤーの位置にする
			g_AttackEffect[nCntEffect].pos = pPlayer->pos;
		}

	}
}

//エフェクトの設定処理
void SetAttackEffect(D3DXVECTOR3 pos, int nCntType)
{
	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		//対象のエフェクトが使われていない
		if (g_AttackEffect[nCntType].bUse == false)
		{
			g_AttackEffect[nCntType].fSize = 0.0f;		//サイズを初期化
			
			g_AttackEffect[nCntType].nCntLoop = 0;		//ループ回数初期化
			g_AttackEffect[nCntType].fAlpha = 1.0f;		//透明度
			g_AttackEffect[nCntType].fResize = 0.0f;	//サイズ変更値
			g_AttackEffect[nCntType].bUse = true;		//使われている状態に
			//break;
		}
	}
}
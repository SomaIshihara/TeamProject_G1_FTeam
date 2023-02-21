//==========================================
//
//落下防止柵プログラム[fence.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "fence.h"
#include "model.h"
#include "camera.h"

//マクロ

//グローバル
Model g_aFenceModel[MAX_USE_FENCE] = {};

//========================
//初期化処理
//========================
void InitFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL *pMat;	//マテリアルポインタ
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファポインタ

	//変数初期化
	for (int nCntInitBP = 0; nCntInitBP < MAX_USE_FENCE; nCntInitBP++)
	{
		g_aFenceModel[nCntInitBP].aParts[0] = {};
	}
}

//========================
//終了処理
//========================
void UninitFence(void)
{
	for (int nCount = 0; nCount < MAX_USE_FENCE; nCount++)
	{//NULL入れるのみ
		//メッシュの破棄
		if (g_aFenceModel[nCount].aParts[0].pMesh != NULL)
		{
			g_aFenceModel[nCount].aParts[0].pMesh->Release();
			g_aFenceModel[nCount].aParts[0].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aFenceModel[nCount].aParts[0].pBuffMat != NULL)
		{
			g_aFenceModel[nCount].aParts[0].pBuffMat->Release();
			g_aFenceModel[nCount].aParts[0].pBuffMat = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdateFence(void)
{
	
}

//========================
//描画処理
//========================
void DrawFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (g_aFence[nCount].bUse == true)
		{
			//ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_aFence[nCount].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFence[nCount].rot.y, g_aFence[nCount].rot.x, g_aFence[nCount].rot.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxRot);

			//位置反映
			D3DXMatrixTranslation(&mtxTrans, g_aFence[nCount].pos.x, g_aFence[nCount].pos.y, g_aFence[nCount].pos.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aFence[nCount].mtxWorld);

			//マテリアルデータへのポインタ取得
			pMat = (D3DXMATERIAL*)g_aBPrint[g_aFence[nCount].bpidx].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBPrint[g_aFence[nCount].bpidx].dwNumMat; nCntMat++)
			{
				//マテリアル設定
				D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;
				//ダメージ状態なら赤追加
				if (g_aFence[nCount].state == OBJSTATE_DAMAGE)
				{
					changeMat.Diffuse.r = 1.0f * OBJ_RED_ALPHA + pMat[nCntMat].MatD3D.Diffuse.r * (1.0f - OBJ_RED_ALPHA);
					changeMat.Diffuse.g = 0.0f * OBJ_RED_ALPHA + pMat[nCntMat].MatD3D.Diffuse.g * (1.0f - OBJ_RED_ALPHA);
					changeMat.Diffuse.b = 0.0f * OBJ_RED_ALPHA + pMat[nCntMat].MatD3D.Diffuse.b * (1.0f - OBJ_RED_ALPHA);
				}
				pDevice->SetMaterial(&changeMat);

				//テクスチャ設定
				pDevice->SetTexture(0, g_aBPrint[g_aFence[nCount].bpidx].apTexture[nCntMat]);

				//モデル描画
				g_aBPrint[g_aFence[nCount].bpidx].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	
	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//表示処理
//========================
void SetFence(BLUEPRINTIDX bpidx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bLifeUse, int nLife)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (g_aFence[nCntObj].bUse == false)
		{
			//引数の情報を追加
			g_aFence[nCntObj].bpidx = bpidx;
			g_aFence[nCntObj].pos = pos;
			g_aFence[nCntObj].rot = rot;
			g_aFence[nCntObj].bLifeUse = bLifeUse;
			g_aFence[nCntObj].nLife = nLife;

			//体力を使用する場合のみ影設定
			if (g_aFence[nCntObj].bLifeUse == true)
			{
				//影設定
				g_aFence[nCntObj].nIdxShadow = SetShadow();
			}

			//使用していることにする
			g_aFence[nCntObj].bUse = true;

			//オブジェクト数増やす
			g_nNumObj++;

			//抜ける
			break;
		}
	}
}
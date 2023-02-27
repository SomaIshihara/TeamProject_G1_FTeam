//==========================================
//
//落下防止フェンスプログラム[fence.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "fence.h"
#include "model.h"
#include "camera.h"
#include "meshfield.h"

//マクロ
#define FENCE_FORCE			(50.0f)		//フェンスを中心に寄せる
#define FENCE_DESTROY_POS	(-800.0f)	//フェンスを消す位置

//グローバル
Fence g_aFence[MAX_USE_FENCE];

//========================
//初期化処理
//========================
void InitFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//変数初期化
	for (int nCntfence = 0; nCntfence < MAX_USE_FENCE; nCntfence++)
	{
		g_aFence[nCntfence] = {};
		SetFence(GetMeshField()->fRadius - FENCE_FORCE, D3DXVECTOR3(0.0f, FIX_ROT(((float)nCntfence / MAX_USE_FENCE) * 2 * D3DX_PI), 0.0f));
	}
}

//========================
//終了処理
//========================
void UninitFence(void)
{
	
}

//========================
//更新処理
//========================
void UpdateFence(void)
{
	for (int nCntFence = 0; nCntFence < MAX_USE_FENCE; nCntFence++)
	{
		if (g_aFence[nCntFence].bUse == true)
		{
			//落とす
			g_aFence[nCntFence].nFallTime++;

			g_aFence[nCntFence].move.y = -(ACCELERATION_GRAVITY * g_aFence[nCntFence].nFallTime / MAX_FPS);

			//移動後がy<0であり、現在の位置が、フィールド以上の高さにいるなら移動量消す
			if (g_aFence[nCntFence].pos.y + g_aFence[nCntFence].move.y < 0.0f && g_aFence[nCntFence].pos.y >= 0.0f)
			{
				//見えないくらい落ちたら消す
				if (g_aFence[nCntFence].pos.y + g_aFence[nCntFence].move.y < FENCE_DESTROY_POS)
				{
					g_aFence[nCntFence].bUse = false;
				}
				else
				{
					//原点位置からのプレイヤーの距離を計算
					float fLength = sqrtf(powf((g_aFence[nCntFence].pos.x), 2) + powf((g_aFence[nCntFence].pos.z), 2));

					//原点位置からの距離が、フィールドの半径以下　　なら、フィールドに乗っている
					if (fLength <= GetMeshField()->fRadius)
					{
						g_aFence[nCntFence].move.y = 0.0f;
						g_aFence[nCntFence].nFallTime = 0;
						g_aFence[nCntFence].pos.y = 0.0f;
					}
					else
					{
						g_aFence[nCntFence].bCollision = false;
					}
				}
			}

			//移動量変更
			g_aFence[nCntFence].pos.y += g_aFence[nCntFence].move.y;
		}
	}
}

//========================
//描画処理
//========================
void DrawFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxScall, mtxRot, mtxTrans, mtxTexture;	//計算用
	D3DXMATRIX mtxInvNor;			//法線用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//フェンスモデル取得
	Model fenceModel = GetXObject(OBJECTTYPE_FENCE);

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	for (int nCount = 0; nCount < MAX_USE_FENCE; nCount++)
	{
		if (g_aFence[nCount].bUse == true)
		{
			//ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_aFence[nCount].mtxWorld);

			//拡縮を反映
			D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFence[nCount].rot.y, g_aFence[nCount].rot.x, g_aFence[nCount].rot.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxRot);

			//位置反映
			D3DXMatrixTranslation(&mtxTrans, g_aFence[nCount].pos.x, g_aFence[nCount].pos.y, g_aFence[nCount].pos.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aFence[nCount].mtxWorld);

			//テクスチャ拡縮
			D3DXMatrixScaling(&mtxTexture, FENCE_SCALE, FENCE_SCALE, 1.0f);
			pDevice->SetTransform(D3DTS_TEXTURE0, &mtxTexture);

			//マテリアルデータへのポインタ取得
			pMat = (D3DXMATERIAL*)fenceModel.aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)fenceModel.aParts[0].dwNumMatModel; nCntMat++)
			{
				//マテリアル設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, fenceModel.aParts[0].apTexture[nCntMat]);

				//モデル描画
				fenceModel.aParts[0].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	
	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//表示処理
//========================
void SetFence(float fLength, D3DXVECTOR3 rot)
{
	fLength -= 10.0f;
	for (int nCntObj = 0; nCntObj < MAX_USE_FENCE; nCntObj++)
	{
		if (g_aFence[nCntObj].bUse == false)
		{
			//引数の情報を追加
			g_aFence[nCntObj].pos.x = fLength * sinf(rot.y);
			g_aFence[nCntObj].pos.y = 0.0f;
			g_aFence[nCntObj].pos.z = fLength * cosf(rot.y);
			g_aFence[nCntObj].posOld = g_aFence[nCntObj].pos;
			g_aFence[nCntObj].rot = rot;
			g_aFence[nCntObj].nFallTime = 0;

			//使用していることにする
			g_aFence[nCntObj].bUse = true;
			g_aFence[nCntObj].bCollision = true;

			//抜ける
			break;
		}
	}
}

//========================
//取得処理
//========================
Fence *GetFence(void)
{
	return &g_aFence[0];
}
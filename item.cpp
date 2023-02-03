//==========================================
//
//アイテムプログラム[item.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "item.h"
#include "player.h"


//グローバル変数宣言     
LPDIRECT3DTEXTURE9		g_pTextureItem[10] = {};	//テクスチャへにポインタ
LPD3DXMESH				g_pMeshItem = NULL;	//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatItem = NULL;	//マテリアルへのポインタ
DWORD					g_dwNumMatItem = 0;	//マテリアルの数
D3DXMATRIX				g_mtxWorldItem;			//ワールドマトリックス
Item					g_Item;					//ボーナスの情報

//========================
//初期化処理
//========================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL	  *pMat;					//マテリアルへのポインタ

												//xファイルの読み込み
	D3DXLoadMeshFromX({ "data/MODEL/item/itembox.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatItem,
		NULL,
		&g_dwNumMatItem,
		&g_pMeshItem);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する　

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureItem[nCntMat]);
		}
	}

	//初期設定
	g_Item.pos = ZERO_SET;
	g_Item.rot = ZERO_SET;
	g_Item.move = ZERO_SET;
	g_Item.DespawnLimit = 0;
	g_Item.fAlpha = 1.0f;					//透明度の設定
	g_Item.buse = false;
}

//========================
//終了処理
//========================
void UninitItem(void)
{
	//メッシュの破棄
	if (g_pMeshItem != NULL)
	{
		g_pMeshItem->Release();
		g_pMeshItem = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatItem != NULL)
	{
		g_pBuffMatItem->Release();
		g_pBuffMatItem = NULL;
	}
}

//========================
//更新処理
//========================
void UpdateItem(void) 
{
	if (g_Item.buse == true)
	{
		//g_ParticleCounter--;

		/*if (g_ParticleCounter <= 0)
		{
			SetParticle(g_Item.pos, 10.0f, 30, PARTICLE_NORMAL);

			g_ParticleCounter = PARTICLE_LIMIT;
		};*/

		////位置の更新
		//g_Item.pos.x += g_Item.move.x;
		//g_Item.pos.z += g_Item.move.z;

		////慣性の設定
		//g_Item.move.x += (NIL_F - g_Item.move.x) * 1.0f;
		//g_Item.move.z += (NIL_F - g_Item.move.z) * 1.0f;
	}
}

//========================
//描画処理
//========================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9	  matDef;					//現在のマテリアル保存用
	D3DXMATERIAL	  *pMat;					//マテリアルデータへのポインタ

												//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	/*if (g_Item.buse == true)
	{*/
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldItem);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Item.rot.y, g_Item.rot.x, g_Item.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_Item.pos.x, g_Item.pos.y, g_Item.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
		{
			//マテリアルの色設定
			pMat[nCntMat].MatD3D.Diffuse.a = g_Item.fAlpha;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItem[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshItem->DrawSubset(nCntMat);
		}
	//}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//アイテムとプレイヤーの当たり判定処理
//========================
void CollisionIP(int nPlayerNum)
{
	Player *pPlayer = GetPlayer();

	pPlayer[nPlayerNum].nATKItemTime = 300;

}

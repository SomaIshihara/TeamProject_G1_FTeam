//==========================================
//
//アイテムプログラム[item.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "item.h"
#include "player.h"

//マクロ定義
#define MAX_ITEM			(5)			//最大数
#define INIT_POS_Y			(200.0f)	//初期のY位置
#define INIT_POS_XZ			(200.0f)	//初期の外位置
#define RATIO_MOVE			(100.0f)	//移動量の割合
#define COLLISION_SIZE_XZ	(30.0f)		//縦横の当たり判定サイズ
#define COLLISION_SIZE_Y	(15.0f)		//高さの当たり判定サイズ

//グローバル変数宣言     
LPDIRECT3DTEXTURE9		g_pTextureItem[10] = {};	//テクスチャへにポインタ
LPD3DXMESH				g_pMeshItem = NULL;	//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatItem = NULL;	//マテリアルへのポインタ
DWORD					g_dwNumMatItem = 0;	//マテリアルの数
D3DXMATRIX				g_mtxWorldItem;			//ワールドマトリックス
Item					g_Item[MAX_ITEM];		//アイテムの情報

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
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[MAX_ITEM].pos = ZERO_SET;
		g_Item[MAX_ITEM].rot = ZERO_SET;
		g_Item[MAX_ITEM].move = ZERO_SET;
		g_Item[MAX_ITEM].DespawnLimit = 0;
		g_Item[MAX_ITEM].fAlpha = 0.0f;					//透明度の設定
		g_Item[MAX_ITEM].buse = false;
	}
	
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
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == true)
		{
		
		}
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

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldItem);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Item[MAX_ITEM].rot.y, g_Item[MAX_ITEM].rot.x, g_Item[MAX_ITEM].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_Item[MAX_ITEM].pos.x, g_Item[MAX_ITEM].pos.y, g_Item[MAX_ITEM].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
			{
				//マテリアルの色設定
				pMat[nCntMat].MatD3D.Diffuse.a = g_Item[MAX_ITEM].fAlpha;

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureItem[nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshItem->DrawSubset(nCntMat);
			}
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//設定処理
//========================
void SetItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == false)
		{
			g_Item[MAX_ITEM].RespawnDelay -= 1;//リスポーンディレイ減少

			if (g_Item[MAX_ITEM].RespawnDelay <= 0)
			{
				g_Item[MAX_ITEM].DespawnLimit = 0;
				g_Item[MAX_ITEM].buse = true;
			}
		}
	}
}

//========================
//アイテムとプレイヤーの当たり判定処理
//========================
void CollisionIP(int nPlayerNum)
{
	Player *pPlayer = GetPlayer();//プレイヤー情報取得

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == true)
		{
			if (pPlayer[nPlayerNum].pos.x >= g_Item[MAX_ITEM].pos.x - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.x <= g_Item[MAX_ITEM].pos.x + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z >= g_Item[MAX_ITEM].pos.z - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z <= g_Item[MAX_ITEM].pos.z + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.y >= g_Item[MAX_ITEM].pos.y - COLLISION_SIZE_Y
				&&pPlayer[nPlayerNum].pos.y <= g_Item[MAX_ITEM].pos.y + COLLISION_SIZE_Y)
			{//プレイヤーがの範囲内に入ったとき

				pPlayer[nPlayerNum].nATKItemTime = 300;
				//使われていない状態にする
				g_Item[MAX_ITEM].RespawnDelay = 3;
				g_Item[MAX_ITEM].fAlpha = 1.0f;
				g_Item[MAX_ITEM].buse = false;
			}
		}
	}
}

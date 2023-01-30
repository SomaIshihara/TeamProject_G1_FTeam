//============================================================================================
//
//ボーナスの処理[bonus.cpp]
//Author:藤原龍輝
//
//============================================================================================
#include"main.h"
#include"color.h"
#include"input.h"
#include"score.h"
#include"bonus.h"

//****************************//
//		　 マクロ定義		  //
//****************************//
#define INIT_POS_Y	(300.0f)	//初期のY位置

//****************************//
//	   グローバル変数宣言     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTextureBonus[10] = {};	//テクスチャへにポインタ
LPD3DXMESH				g_pMeshBonus	    = NULL;	//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatBonus		= NULL;	//マテリアルへのポインタ
DWORD					g_dwNumMatBonus		= 0;	//マテリアルの数
D3DXMATRIX				g_mtxWorldBonus;			//ワールドマトリックス
Bonus					g_Bonus;					//ボーナスの情報
//===================================================
//ボーナスの初期化処理
//===================================================
void InitBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL	  *pMat;					//マテリアルへのポインタ

	//xファイルの読み込み
	D3DXLoadMeshFromX({ "data/MODEL/bird/bonus_bird.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBonus,
		NULL,
		&g_dwNumMatBonus,
		&g_pMeshBonus);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatBonus->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する　

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureBonus[nCntMat]);
		}
	}

	//初期設定
	g_Bonus.pos = D3DXVECTOR3(NIL_F, INIT_POS_Y, NIL_F);
	g_Bonus.rot = ZERO_SET;
	g_Bonus.buse = true;
}
//===================================================
//ボーナスの終了処理
//===================================================
void UninitBonus(void)
{
	//メッシュの破棄
	if (g_pMeshBonus != NULL)
	{
		g_pMeshBonus->Release();
		g_pMeshBonus = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatBonus != NULL)
	{
		g_pBuffMatBonus->Release();
		g_pBuffMatBonus = NULL;
	}
}
//===================================================
//ボーナスの更新処理
//===================================================
void UpdateBonus(void)
{

}
//===================================================
//ボーナスの描画処理
//===================================================
void DrawBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9	  matDef;					//現在のマテリアル保存用
	D3DXMATERIAL	  *pMat;					//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (g_Bonus.buse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldBonus);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Bonus.rot.y, g_Bonus.rot.x, g_Bonus.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldBonus, &g_mtxWorldBonus, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_Bonus.pos.x, g_Bonus.pos.y, g_Bonus.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldBonus, &g_mtxWorldBonus, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBonus);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatBonus->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBonus[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshBonus->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
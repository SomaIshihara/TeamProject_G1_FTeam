//==========================================
//
//モデル関係プログラム[model.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "model.h"
#include "file.h"

//グローバル変数
Model g_aModel[ANIMAL_MAX];	//モデル構造体

//========================
//初期化処理
//========================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	
	//変数初期化
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aModel[nCntModel].aParts[nCntParts] = {};
			g_aModel[nCntModel].aParts[nCntParts].bUse = false;
		}
		
		//使用するモーション類読み込み
		LoadMotionViewerFile(GetMotionFilePath(nCntModel), &g_aModel[nCntModel]);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (SUCCEEDED(D3DXLoadMeshFromX(
				&g_aModel[nCntModel].aModelFileName[nCntParts][0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aModel[nCntModel].aParts[nCntParts].pBuffMat,
				NULL,
				&g_aModel[nCntModel].aParts[nCntParts].dwNumMatModel,
				&g_aModel[nCntModel].aParts[nCntParts].pMesh)))
			{//読み込み成功したら
			 //テクスチャ読み込み
				D3DXMATERIAL *pMat;	//マテリアルポインタ

									//マテリアル情報に対するポインタ取得
				pMat = (D3DXMATERIAL *)g_aModel[nCntModel].aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntTex = 0; nCntTex < (int)g_aModel[nCntModel].aParts[nCntParts].dwNumMatModel; nCntTex++)
				{
					if (pMat[nCntTex].pTextureFilename != NULL)
					{
						//テクスチャ読み込み
						D3DXCreateTextureFromFile(pDevice,
							pMat[nCntTex].pTextureFilename,
							&g_aModel[nCntModel].aParts[nCntParts].apTexture[nCntTex]);
					}
				}

				//使用している状態にする
				g_aModel[nCntModel].aParts[nCntParts].bUse = true;
			}
			else
			{//失敗したら1モデル分終了
				break;
			}
		}
	}
}

//========================
//終了処理（ここのみ破棄（Release）すればGetModelで取得した方はNULL入れるだけでOK）
//========================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			//メッシュの破棄
			if (g_aModel[nCntModel].aParts[nCntParts].pMesh != NULL)
			{
				g_aModel[nCntModel].aParts[nCntParts].pMesh->Release();
				g_aModel[nCntModel].aParts[nCntParts].pMesh = NULL;
			}

			//マテリアルの破棄
			if (g_aModel[nCntModel].aParts[nCntParts].pBuffMat != NULL)
			{
				g_aModel[nCntModel].aParts[nCntParts].pBuffMat->Release();
				g_aModel[nCntModel].aParts[nCntParts].pBuffMat = NULL;
			}
		}
	}
}

//========================
//モデル取得処理
//========================
//指定した動物のモデルすべてを返します
//このプログラムで取得したものすべてを返すため、メッシュ・マテリアルポインタも同じものが入ります
//そのため終了処理をするときはUninitModelでメッシュ・マテリアルポインタを破棄とNULL代入、取得した方はNULL代入のみ行うこと
//========================
Model GetModel(ANIMAL animal)
{
	return g_aModel[animal];
}
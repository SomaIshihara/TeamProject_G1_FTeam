//==========================================
//
//モデル関係プログラム[model.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "model.h"

//グローバル変数
Model g_aModel[ANIMAL_MAX];	//モデル構造体

//ファイルパス
const char *c_pFileNameModel[] =
{
	"data/MODEL/ino/ino_body.x",
	"data/MODEL/ino/ino_head.x",
	"data/MODEL/ino/ino_left_forefoot.x",
	"data/MODEL/ino/ino_left_foretoes.x",
	"data/MODEL/ino/ino_left_hindleg.x",
	"data/MODEL/ino/ino_left_hindtoes.x",
	"data/MODEL/ino/ino_right_forefoot.x",
	"data/MODEL/ino/ino_right_foretoes.x",
	"data/MODEL/ino/ino_right_hindleg.x",
	"data/MODEL/ino/ino_right_hindtoes.x"
};

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
	}

	//Xファイル読み込み
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (SUCCEEDED(D3DXLoadMeshFromX(
				c_pFileNameModel[nCntParts],
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
#if 0
		//階層構造設定
		//体
		g_aModel[nCntModel].aParts[0].mIdxModelParent = -1;
		g_aModel[nCntModel].aParts[0].posOffset = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
		g_aModel[nCntModel].aParts[0].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//頭
		g_aModel[nCntModel].aParts[1].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[1].posOffset = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aModel[nCntModel].aParts[1].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//左腕
		g_aModel[nCntModel].aParts[2].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[2].posOffset = D3DXVECTOR3(-5.0f, 7.0f, 0.0f);
		g_aModel[nCntModel].aParts[2].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//左手
		g_aModel[nCntModel].aParts[3].mIdxModelParent = 2;
		g_aModel[nCntModel].aParts[3].posOffset = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].aParts[3].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//右腕
		g_aModel[nCntModel].aParts[4].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[4].posOffset = D3DXVECTOR3(5.0f, 7.0f, 0.0f);
		g_aModel[nCntModel].aParts[4].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//右手
		g_aModel[nCntModel].aParts[5].mIdxModelParent = 4;
		g_aModel[nCntModel].aParts[5].posOffset = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].aParts[5].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//左腿
		g_aModel[nCntModel].aParts[6].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[6].posOffset = D3DXVECTOR3(-3.0f, -8.0f, 0.0f);
		g_aModel[nCntModel].aParts[6].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//左足
		g_aModel[nCntModel].aParts[7].mIdxModelParent = 6;
		g_aModel[nCntModel].aParts[7].posOffset = D3DXVECTOR3(0.0f, -12.0f, 0.0f);
		g_aModel[nCntModel].aParts[7].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//右腿
		g_aModel[nCntModel].aParts[8].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[8].posOffset = D3DXVECTOR3(3.0f, -8.0f, 0.0f);
		g_aModel[nCntModel].aParts[8].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//右足
		g_aModel[nCntModel].aParts[9].mIdxModelParent = 8;
		g_aModel[nCntModel].aParts[9].posOffset = D3DXVECTOR3(0.0f, -12.0f, 0.0f);
		g_aModel[nCntModel].aParts[9].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
#endif
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
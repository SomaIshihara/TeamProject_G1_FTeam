//==========================================
//
//リザルトプレイヤープログラムの処理[hdr_player.cpp]
//Author:平澤詩苑　石原颯馬
//
//==========================================
#include "resultPlayer.h"
#include "select_game.h"

//マクロ定義

//グローバル変数
Player_RESULT g_aPlayerResult[MAX_USE_GAMEPAD];

//========================
//初期化処理
//========================
void InitPlayer_RESULT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

												//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//変数初期化
		g_aPlayerResult[nCntPlayer].pos = ZERO_SET;
		g_aPlayerResult[nCntPlayer].posOld = ZERO_SET;
		g_aPlayerResult[nCntPlayer].move = ZERO_SET;
		g_aPlayerResult[nCntPlayer].rot = ZERO_SET;
		g_aPlayerResult[nCntPlayer].bHipDrop = false;
		g_aPlayerResult[nCntPlayer].bHipDropSpin = false;
		g_aPlayerResult[nCntPlayer].nHipDropWait = 0;

		g_aPlayerResult[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayerResult[nCntPlayer].nRank = -1;

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aPlayerResult[nCntPlayer].animalInst[nCntParts] = {};
		}
	}
}

//========================
//終了処理
//========================
void UninitPlayer_RESULT(void)
{

}

//========================
//更新処理
//========================
void UpdatePlayer_RESULT(void)
{
	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{

	}
}

//========================
//描画処理
//========================
void DrawPlayer_RESULT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

									//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//プレイヤーの数だけ繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		Model useAnimal;

		//ゲームモードに応じて取得するものを変更
		if (GetSelectGameMode() == SelectGameMode_PVP)
		{
			useAnimal = GetAnimal(GetPlayer()[nCntPlayer].animal);
		}
		else if (GetSelectGameMode() == SelectGameMode_HDR)
		{
			useAnimal = GetAnimal(GetPlayer_HDR()[nCntPlayer].animal);
		}

		//"プレイヤーの"ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_aPlayerResult[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerResult[nCntPlayer].rot.y, g_aPlayerResult[nCntPlayer].rot.x, g_aPlayerResult[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].mtxWorld, &g_aPlayerResult[nCntPlayer].mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayerResult[nCntPlayer].pos.x, g_aPlayerResult[nCntPlayer].pos.y, g_aPlayerResult[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].mtxWorld, &g_aPlayerResult[nCntPlayer].mtxWorld, &mtxTrans);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerResult[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
			D3DXMATRIX mtxParent;					//親のマトリ

			//ここ新仕様
			//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
			g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos = useAnimal.aParts[nCntParts].posOffset;
			g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot = useAnimal.aParts[nCntParts].rotOffset;

			//"モデルの"ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot.y, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot.x, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot.z);
			D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

			//位置反映
			D3DXMatrixTranslation(&mtxTransModel, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos.x, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos.y, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos.z);
			D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

			//パーツの親マトリ設定
			if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
			{
				mtxParent = g_aPlayerResult[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_aPlayerResult[nCntPlayer].mtxWorld;
			}

			//パーツのマトリと親マトリをかけ合わせる
			D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

			//"モデルの"ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//マテリアルデータへのポインタ取得
			pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
			{
				//マテリアル設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

				//モデル描画
				useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
			}
		}
		
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//リザルト用プレイヤーの情報を取得
Player_RESULT *GetPlayer_RESULT(void)
{
	return &g_aPlayerResult[0];
}
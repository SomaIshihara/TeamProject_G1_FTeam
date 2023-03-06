//==========================================
//
//リザルトプレイヤープログラムの処理[hdr_player.cpp]
//Author:平澤詩苑　石原颯馬
//
//==========================================
#include "resultPlayer.h"
#include "select_game.h"
#include "VictoryStand.h"
#include "color.h"

//マクロ定義
#define FALL_RES_PLAYER			(3.0f)		//リザルトプレイヤーの落下速度

//グローバル変数
Player_RESULT g_ResultPlayer[MAX_USE_GAMEPAD];				/*リザルト用プレイヤーの情報*/
const D3DXVECTOR3 c_ResultPlayerPos[MAX_USE_GAMEPAD] = {	/*プレイヤーの初期位置設定*/
	D3DXVECTOR3(-75.0f, 300.0f, 0.0f),
	D3DXVECTOR3(-25.0f, 300.0f, 0.0f),
	D3DXVECTOR3(+25.0f, 300.0f, 0.0f),
	D3DXVECTOR3(+75.0f, 300.0f, 0.0f),
};

//========================
//初期化処理
//========================
void InitPlayer_RESULT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	Player_RESULT *pResPlayer = &g_ResultPlayer[0];

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++, pResPlayer++)
	{
		//変数初期化
		pResPlayer->pos = 
		pResPlayer->posOld = c_ResultPlayerPos[nCntPlayer];
		pResPlayer->move = ZERO_SET;
		pResPlayer->rot = ZERO_SET;
		pResPlayer->bHipDrop = false;
		pResPlayer->bHipDropSpin = false;
		pResPlayer->nHipDropWait = 0;

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts] = {};
		}
	}

	//ポインタを再取得
	pResPlayer = &g_ResultPlayer[0];

	//ゲームモードに応じて取得するものを変更
	switch (GetSelGameMode())
	{
	case SelGameMode_PVP:	//ゲームモード「PVP」の場合の初期化処理
		InitResultPlayerType_GAMEMODE_PVP(pResPlayer);
		break;

	case SelGameMode_HDR:	//ゲームモード「HDR」の場合の初期化処理
		InitResultPlayerType_GAMEMODE_HDR(pResPlayer);
		break;
	}
}

//========================
//ゲームモードが PVP のときの初期化処理
//========================
void InitResultPlayerType_GAMEMODE_PVP(Player_RESULT *pResPlayer)
{
	//PVP プレイヤーのポインタ取得
	Player *pPVPPlayer = GetPlayer();

	for (int nCntPVP = 0; nCntPVP < MAX_USE_GAMEPAD; nCntPVP++, pPVPPlayer++, pResPlayer++)
	{
		//プレイヤーのスコアを取得
		pResPlayer->nRank = pPVPPlayer->nScore;
	}
}

//========================
//ゲームモードが HDR のときの初期化処理
//========================
void InitResultPlayerType_GAMEMODE_HDR(Player_RESULT *pResPlayer)
{
	//HDR のプレイヤー情報取得
	Player_HDR *pHDRPlayer = GetPlayer_HDR();

	for (int nCntHDR = 0; nCntHDR < MAX_USE_GAMEPAD; nCntHDR++, pResPlayer++, pHDRPlayer++)
	{
		//プレイヤーの順位を取得
		pResPlayer->nRank = pHDRPlayer->nRank;
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
		//対象のプレイヤーのポインタを取得
		Player_RESULT *pPlayer = &g_ResultPlayer[nCntPlayer];

		//前回の位置を記憶
		pPlayer->posOld = pPlayer->pos;

		//プレイヤーを落下させる
		pPlayer->pos.y -= FALL_RES_PLAYER;

		//表彰台への当たり判定
		CollisionVictoryStand(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move);
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
		//プレイヤーのモデルを取得
		Model useAnimal = GetAnimal(g_ResultPlayer[nCntPlayer].animal);

		//"プレイヤーの"ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_ResultPlayer[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ResultPlayer[nCntPlayer].rot.y, g_ResultPlayer[nCntPlayer].rot.x, g_ResultPlayer[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].mtxWorld, &g_ResultPlayer[nCntPlayer].mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, g_ResultPlayer[nCntPlayer].pos.x, g_ResultPlayer[nCntPlayer].pos.y, g_ResultPlayer[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].mtxWorld, &g_ResultPlayer[nCntPlayer].mtxWorld, &mtxTrans);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ResultPlayer[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
			D3DXMATRIX mtxParent;					//親のマトリ

			//ここ新仕様
			//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos = useAnimal.aParts[nCntParts].posOffset;
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot = useAnimal.aParts[nCntParts].rotOffset;

			//"モデルの"ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.y, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.x, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.z);
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

			//位置反映
			D3DXMatrixTranslation(&mtxTransModel, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.x, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.y, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.z);
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

			//パーツの親マトリ設定
			if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
			{
				mtxParent = g_ResultPlayer[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_ResultPlayer[nCntPlayer].mtxWorld;
			}

			//パーツのマトリと親マトリをかけ合わせる
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

			//"モデルの"ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//マテリアルデータへのポインタ取得
			pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
			{
				//ゴースト用
				D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

				if (nCntParts == MAX_PARTS - 1)
				{//ゼッケンの時は色変更
					matChange.Diffuse = c_aColPlayer[nCntPlayer];
				}

				//マテリアルの設定
				pDevice->SetMaterial(&matChange);

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
	return &g_ResultPlayer[0];
}
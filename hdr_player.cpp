#include "main.h"
#include "HDRgame.h"
#include "HDR_player.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "sound.h"
#include "meshfield.h"

//グローバル変数
Player_HDR g_aPlayerHDRHDR[MAX_USE_GAMEPAD];
int g_nIdxShadow_HDR = -1;

//プロトタイプ宣言
void ControllKeyboardPlayer_HDR(int nPlayerNum);
void ControllGPadPlayer_HDR(int nPlayerNum);

void JumpPlayer_HDR(int nJumpPlayer);

//初期位置向き
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-75.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(-25.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(25.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(75.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
};

//[デバッグ用]AI挙動させるプレイヤー指定（コントローラーが刺さっていればそれを優先）
const bool c_aAIMove[MAX_USE_GAMEPAD] = { false,true,true,true };

//========================
//初期化処理
//========================
void InitPlayer_HDR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

												//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//変数初期化
		g_aPlayerHDRHDR[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayerHDRHDR[nCntPlayer].posOld = g_aPlayerHDRHDR[nCntPlayer].pos;
		g_aPlayerHDRHDR[nCntPlayer].move = ZERO_SET;
		g_aPlayerHDRHDR[nCntPlayer].moveV0 = ZERO_SET;
		g_aPlayerHDRHDR[nCntPlayer].rot = ZERO_SET;
		g_aPlayerHDRHDR[nCntPlayer].jumpTime = 0;
		g_aPlayerHDRHDR[nCntPlayer].bJump = false;
		g_aPlayerHDRHDR[nCntPlayer].bHipDrop = false;
		g_aPlayerHDRHDR[nCntPlayer].bHipDropSpin = false;
		g_aPlayerHDRHDR[nCntPlayer].nHipDropWait = 0;

		g_aPlayerHDRHDR[nCntPlayer].animal = ANIMAL_WILDBOAR;

		g_aPlayerHDRHDR[nCntPlayer].model = GetModel(g_aPlayerHDRHDR[nCntPlayer].animal);
		g_aPlayerHDRHDR[nCntPlayer].bUsePlayer = GetUseController_HDR(nCntPlayer);
	}

	//[デバッグ]コントローラーが接続されていなければ1Pのみ有効化する
	if (GetUseControllerNum_HDR() == 0)
	{
		g_aPlayerHDRHDR[0].bUsePlayer = true;
	}
}
//========================
//終了処理
//========================
void UninitPlayer_HDR(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//プレイヤーの数だけ処理する
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{//取得した方なのでNULL入れるだけでOK
			g_aPlayerHDRHDR[nCntPlayer].model.aParts[nCntParts].pMesh = NULL;
			g_aPlayerHDRHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat = NULL;
		}
	}
}
//========================
//更新処理
//========================
void UpdatePlayer_HDR(void)
{
	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//現在の位置を前回の位置にする
		g_aPlayerHDRHDR[nCntPlayer].posOld = g_aPlayerHDRHDR[nCntPlayer].pos;

		//ジャンプ時間を増やす
		g_aPlayerHDRHDR[nCntPlayer].jumpTime++;

		if (g_aPlayerHDRHDR[nCntPlayer].bUsePlayer == true)
		{//使用時のみ行う

			//接続されているか確認して切断されていたらプレイヤーを消す（例外としてコントローラーがつながっていないときは無視）
			if (GetUseControllerNum_HDR() != 0)
			{
				g_aPlayerHDR[nCntPlayer].bUsePlayer = GetUseController_HDR(nCntPlayer);
			}
			else
			{
				ControllKeyboardPlayer_HDR(nCntPlayer);
			}

			//各プレイヤーの操作
			ControllGPadPlayer_HDR(nCntPlayer);
		}

		//使用されているかにかかわらず行う
		g_aPlayerHDR[nCntPlayer].move.y = -15;

		//ジャンプ量設定
		if (g_aPlayerHDR[nCntPlayer].bHipDrop == true)
		{

		}
		else
		{
			g_aPlayerHDR[nCntPlayer].move.y = g_aPlayerHDR[nCntPlayer].moveV0.y - (9.8f * g_aPlayerHDR[nCntPlayer].jumpTime / MAX_FPS);
		}

		//普通に移動
		g_aPlayerHDR[nCntPlayer].pos += g_aPlayerHDR[nCntPlayer].move;
	
		if (g_aPlayerHDR[nCntPlayer].pos.y < 0)
		{
			g_aPlayerHDR[nCntPlayer].pos.y = 0;
			g_aPlayerHDR[nCntPlayer].bJump = false;
		}
	}
}
//========================
//描画処理
//========================
void DrawPlayer_HDR(void)
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
		//"プレイヤーの"ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerHDR[nCntPlayer].rot.y, g_aPlayerHDR[nCntPlayer].rot.x, g_aPlayerHDR[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayerHDR[nCntPlayer].pos.x, g_aPlayerHDR[nCntPlayer].pos.y, g_aPlayerHDR[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxTrans);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].bUse == true)
			{
				D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
				D3DXMATRIX mtxParent;					//親のマトリ

														//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
				g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].posOffset;
				g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rotOffset;

				//"モデルの"ワールドマトリックス初期化
				D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxRotModel);

				//位置反映
				D3DXMatrixTranslation(&mtxTransModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxTransModel);

				//パーツの親マトリ設定
				if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent != -1)
				{
					mtxParent = g_aPlayerHDR[nCntPlayer].model.aParts[g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayerHDR[nCntPlayer].mtxWorld;
				}

				//パーツのマトリと親マトリをかけ合わせる
				D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxParent);

				//"プレイヤーの"ワールドマトリックス設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//マテリアルデータへのポインタ取得
				pMat = (D3DXMATERIAL*)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャ設定
					pDevice->SetTexture(0, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

					//モデル描画
					g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				/*------------------------------------------------------------------
				影の描画		Author:平澤詩苑 石原颯馬
				--------------------------------------------------------------------*/
				if (g_aPlayerHDR[nCntPlayer].pos.y >= 0.0f)
				{
					D3DXMATRIX	mtxShadow;		//シャドウマトリックス
					D3DLIGHT9	light;			//ライト情報
					D3DXVECTOR4	posLight;		//ライトの位置
					D3DXVECTOR3	pos, normal;	//平面上の任意の点、法線ベクトル
					D3DXPLANE	plane;			//平面情報

												//ライトの位置を設定
					pDevice->GetLight(0, &light);
					posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

					//平面情報を生成
					pos = ZERO_SET;
					normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					D3DXPlaneFromPointNormal(&plane, &pos, &normal);

					//シャドウマトリックスの初期化
					D3DXMatrixIdentity(&mtxShadow);

					//シャドウマトリックスの作成
					D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
					D3DXMatrixMultiply(&mtxShadow, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxShadow);

					//シャドウマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

					//マテリアルデータへのポインタを取得
					pMat = (D3DXMATERIAL *)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//マテリアルデータ複製

																		//黒色に設定						//自己発光を無くす
						MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

						//マテリアル設定
						pDevice->SetMaterial(&MatCopy);

						//テクスチャ設定
						pDevice->SetTexture(0, NULL);

						//モデル描画
						g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//========================
//プレイヤーのキーボード操作
//========================
void ControllKeyboardPlayer_HDR(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayerHDR[nPlayerNum].bHipDrop == false)
	{
		//ジャンプ・ヒップドロップ
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_aPlayerHDR[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayerHDR[nPlayerNum].bJump)
			{
				//HipDropPlayer(nPlayerNum);		//プレイヤーのヒップドロップ処理
			}
			else
			{
				JumpPlayer_HDR(nPlayerNum);			//プレイヤーのジャンプ処理
			}
		}
	}
}
//========================
//プレイヤーのゲームパッド操作
//========================
void ControllGPadPlayer_HDR(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayerHDR[nPlayerNum].bHipDrop == false)
	{
		//ジャンプ・ヒップドロップ
		if (GetGamepadTrigger(nPlayerNum, XINPUT_GAMEPAD_A) == true && g_aPlayerHDR[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayerHDR[nPlayerNum].bJump)
			{
				//HipDropPlayer(nPlayerNum);		//プレイヤーのヒップドロップ処理
			}
			else
			{
				JumpPlayer_HDR(nPlayerNum);			//プレイヤーのジャンプ処理
			}
		}
	}
}
//========================
//ジャンプの処理
//========================
void JumpPlayer_HDR(int nJumpPlayer)
{
	PlaySound(SOUND_LABEL_SE_JUMP);

	g_aPlayerHDR[nJumpPlayer].moveV0.y = 7.7f;//移動量設定
	g_aPlayerHDR[nJumpPlayer].jumpTime = 0;	//ジャンプ時間リセット
	g_aPlayerHDR[nJumpPlayer].bJump = true;
	/*g_aPlayerHDR[nJumpPlayer].stat = PLAYERSTAT_JUMP;*/
}
//========================
//取得処理
//========================
Player_HDR *GetPlayer_HDR(void)
{
	return &g_aPlayerHDR[0];
}
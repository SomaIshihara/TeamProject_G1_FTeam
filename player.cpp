//==========================================
//
//プレイヤープログラム[player.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "game.h"
#include "player.h"
#include "model.h"
#include "input.h"
#include "wall.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "Gauge.h"

//マクロ
#define PLAYER_MOVE_SPEED	(20.0f)		//プレイヤー移動速度20.0f
#define PLAYER_JUMP_SPEED	(7.7f)		//プレイヤージャンプ速度7.7f
#define ACCELERATION_GRAVITY (9.8f)		//重力加速度
#define PLAYER_WEIGHT		(50)		//質量
#define PLAYER_POWER_LEVEL	(3)			//パワーレベル
#define PLAYER_POWER_ADD	(0.01f)		//移動の強さの増加値
#define DUMP_COEF			(0.04f)		//減衰係数
#define DEBUG_PLAYER_MOVE_SPEED	(5.0f)	//[デバッグ用]普通に移動するときの移動量
#define DECIMAL_PLACE		(1)			//小数点第何位まで移動していることにするか

#define TEST_SIZE_WIDTH		(15.0f)
#define TEST_SIZE_HEIGHT	(15.0f)

//ダッシュ関連マクロ

//#define ADD_GAUGE			(10.0f)	//ゲージ増加量
//#define MAX_GAUGE			(25.0f)	//ゲージ最大値
//#define GAUGE_BONUS			(15.0f)	//ボーナスゲージ

//向き
#define ROT_WA	(-0.75f * D3DX_PI)	//左上
#define ROT_WD	(0.75f * D3DX_PI)	//右上
#define ROT_SA	(-0.25f * D3DX_PI)	//左下
#define ROT_SD	(0.25f * D3DX_PI)	//右下
#define ROT_W	(1.0f * D3DX_PI)	//上
#define ROT_A	(-0.5f * D3DX_PI)	//左
#define ROT_S	(0.0f * D3DX_PI)	//下
#define ROT_D	(0.5f * D3DX_PI)	//右

//プロト
void MovePlayer(int nPadNum);

void CollisionPP(int nPlayerNum);	//プレイヤー同士の衝突判定

//グローバル変数
Player g_aPlayer[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;

//初期位置向き
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(-50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
};

//========================
//初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = ZERO_SET;
		g_aPlayer[nCntPlayer].rot = ZERO_SET;
		g_aPlayer[nCntPlayer].moveGauge = 0;
		g_aPlayer[nCntPlayer].jumpTime = 0;

		g_aPlayer[nCntPlayer].faceColliderLeft = D3DXVECTOR3(15.0f, 0.0f, 15.0f);
		g_aPlayer[nCntPlayer].faceColliderRight = D3DXVECTOR3(-15.0f, 0.0f, 15.0f);

		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].bUsePlayer = GetUseController(nCntPlayer);
		g_aPlayer[nCntPlayer].bUsePlayer = true;
	}

	//[デバッグ]コントローラーが接続されていなければ1Pのみ有効化する
	if (GetUseControllerNum() == 0) 
	{
		g_aPlayer[0].bUsePlayer = true;
	}
}

//========================
//終了処理
//========================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//プレイヤーの数だけ処理する
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{//取得した方なのでNULL入れるだけでOK
			g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh = NULL;
			g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdatePlayer(void)
{
	//デバッグ表示
	PrintDebugProc("[パラメータ]\n");

	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < 1; nCntPlayer++)
	{
		//現在の位置を前回の位置にする
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		if (g_aPlayer[nCntPlayer].bUsePlayer == true)
		{
			//接続されているか確認して切断されていたらプレイヤーを消す（例外としてコントローラーがつながっていないときは無視）
			if (GetUseControllerNum() != 0)
			{
				g_aPlayer[nCntPlayer].bUsePlayer = GetUseController(nCntPlayer);
			}

			

			//ジャンプ時間を増やす
			g_aPlayer[nCntPlayer].jumpTime++;
			//キーボード操作時の動作
#if 1
			//移動方法（ダッシュ）押して離す
			if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
				&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
			{//もうこれ動いてるって言わないよね（ほぼ動いていない）
				if (GetKeyboardPress(DIK_SPACE) == true)
				{//スペースキーは押された
					g_aPlayer[nCntPlayer].moveGauge += PLAYER_POWER_ADD;

					if (g_aPlayer[nCntPlayer].moveGauge >= PLAYER_POWER_MAX)
					{
						g_aPlayer[nCntPlayer].moveGauge = PLAYER_POWER_MAX;
					}
				}
				else if (GetKeyboardRelease(DIK_SPACE) == true)
				{//SPACEキーが離された
					//進行方向の設定
					g_aPlayer[nCntPlayer].move.x = -sinf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = -cosf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;


					g_aPlayer[nCntPlayer].move.y = PLAYER_JUMP_SPEED * g_aPlayer[nCntPlayer].moveGauge;//移動量設定
					g_aPlayer[nCntPlayer].jumpTime = 0;	//ジャンプ時間リセット

					g_aPlayer[nCntPlayer].moveGauge = 0;
				}
			}
			else
			{
				g_aPlayer[nCntPlayer].moveGauge = 0;
			}

			//ジャンプ
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				g_aPlayer[nCntPlayer].move.y = PLAYER_JUMP_SPEED;//移動量設定
				g_aPlayer[nCntPlayer].jumpTime = 0;	//ジャンプ時間リセット
			}
#endif
			//ゲームパッドの操作
			//移動方法（ダッシュ）押して離す
			if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
				&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
			{//もうこれ動いてるって言わないよね（ほぼ動いていない）
				if (GetGamepadPress(nCntPlayer, XINPUT_GAMEPAD_X) == true)
				{//Aボタンが押された
					g_aPlayer[nCntPlayer].moveGauge += PLAYER_POWER_ADD;
					if (g_aPlayer[nCntPlayer].moveGauge >= PLAYER_POWER_MAX)
					{
						g_aPlayer[nCntPlayer].moveGauge = PLAYER_POWER_MAX;
					}
				}
				else if (GetGamepadRelease(nCntPlayer, XINPUT_GAMEPAD_X) == true)
				{//Aボタンが離された
					//進行方向の設定
					g_aPlayer[nCntPlayer].move.x = -sinf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = -cosf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;
										
					g_aPlayer[nCntPlayer].moveGauge = 0;
				}
			}
			else
			{
				g_aPlayer[nCntPlayer].moveGauge = 0;
			}

			//ジャンプ
			if (GetGamepadTrigger(nCntPlayer, XINPUT_GAMEPAD_A) == true)
			{
				g_aPlayer[nCntPlayer].move.y = PLAYER_JUMP_SPEED;//移動量設定
				g_aPlayer[nCntPlayer].jumpTime = 0;	//ジャンプ時間リセット
			}

			//ボタン操作に応じてプレイヤー移動 - (ACCELERATION_GRAVITY * g_aPlayer[nCntPlayer].jumpTime / MAX_FPS);
			g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
			g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y - (ACCELERATION_GRAVITY * g_aPlayer[nCntPlayer].jumpTime / MAX_FPS);
			g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;

			//[仮]一旦Y=0.0fになったら着地
			if (g_aPlayer[nCntPlayer].pos.y <= 0.0f)
			{
				g_aPlayer[nCntPlayer].pos.y = 0.0f;
			}

			//[デバッグ用]普通に移動する処理
			MovePlayer(nCntPlayer);

			//当たり判定類
			CollisionPP(nCntPlayer);

			//移動量減衰
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			//g_aPlayer[nCntPlayer].move.y += (0 - g_aPlayer[nCntPlayer].move.y) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			

			//影位置設定
			//一旦なし

			//デバッグ表示（強さ表示）
			PrintDebugProc("[%d]:Power = %f\n", nCntPlayer, g_aPlayer[nCntPlayer].moveGauge);
		}
	}
}

//========================
//描画処理
//========================
void DrawPlayer(void)
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
		D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (g_aPlayer[nCntPlayer].model.aParts[nCntParts].bUse == true)
			{
				D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
				D3DXMATRIX mtxParent;					//親のマトリ

				//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
				g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos = g_aPlayer[nCntPlayer].model.aParts[nCntParts].posOffset;
				g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot = g_aPlayer[nCntPlayer].model.aParts[nCntParts].rotOffset;

				//"モデルの"ワールドマトリックス初期化
				D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.y, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.x, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxRotModel);

				//位置反映
				D3DXMatrixTranslation(&mtxTransModel, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.x, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.y, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxTransModel);

				//パーツの親マトリ設定
				if (g_aPlayer[nCntPlayer].model.aParts[nCntParts].mIdxModelParent != -1)
				{
					mtxParent = g_aPlayer[nCntPlayer].model.aParts[g_aPlayer[nCntPlayer].model.aParts[nCntParts].mIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayer[nCntPlayer].mtxWorld;
				}

				//パーツのマトリと親マトリをかけ合わせる
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxParent);

				//"プレイヤーの"ワールドマトリックス設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//マテリアルデータへのポインタ取得
				pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					//マテリアル設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャ設定
					pDevice->SetTexture(0, g_aPlayer[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

					//モデル描画
					g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				/*------------------------------------------------------------------
				影の描画		Author:平澤詩苑
				--------------------------------------------------------------------*/
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
				D3DXMatrixMultiply(&mtxShadow, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxShadow);

				//シャドウマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL *)g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//マテリアルデータ複製

					//黒色に設定						//自己発光を無くす
					MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

					//マテリアル設定
					pDevice->SetMaterial(&MatCopy);

					//テクスチャ設定
					pDevice->SetTexture(0, NULL);

					//モデル描画
					g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//プレイヤー同士の衝突判定処理
//========================
void CollisionPP(int nPlayerNum)
{
	//=pos0~pos3の説明==================
	// pos3		pos2
	//	・<-----・		矢印:vecLine
	//	｜		↑
	//	｜		｜
	//	↓		｜
	//	・----->・
	// pos0		pos1
	//==================================

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMoveLeft, vecMoveRight;

	float fAreaARight, fAreaALeft, fAreaBRight, fAreaBLeft;
	float fAreaAUp, fAreaADown, fAreaBUp, fAreaBDown;


	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)// && g_aPlayer[nCntOtherPlayer].bUsePlayer == true
		{
			//各頂点求める
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_HEIGHT);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos0.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos0.y = 0.0f;
			pos0.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos0---------------------------------------------------------------------------------------------------------------------------

			//-pos1---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_HEIGHT);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos1.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos1.y = 0.0f;
			pos1.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos1---------------------------------------------------------------------------------------------------------------------------

			//-pos2---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_HEIGHT);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//PI - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(D3DX_PI - fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos2.x = g_aPlayer[nCntOtherPlayer].pos.x - sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = g_aPlayer[nCntOtherPlayer].pos.z + cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//-pos3---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_HEIGHT);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//-PI + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-D3DX_PI - fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos3.x = g_aPlayer[nCntOtherPlayer].pos.x - sinf(rot) * fLength;
			pos3.y = 0.0f;
			pos3.z = g_aPlayer[nCntOtherPlayer].pos.z + cosf(rot) * fLength;
			//-pos3---------------------------------------------------------------------------------------------------------------------------

			//ベクトル求める
			//move
			vecMoveLeft = g_aPlayer[nPlayerNum].pos - g_aPlayer[nPlayerNum].posOld;
			vecMoveRight = g_aPlayer[nPlayerNum].pos - g_aPlayer[nPlayerNum].posOld;

			//X
			//右方向の計算
			vecLineRight = pos1 - pos0;
			vecToPosRight = g_aPlayer[nPlayerNum].pos - pos0;
			vecToPosOldRight = g_aPlayer[nPlayerNum].posOld - pos0;

			//左方向の計算
			vecLineLeft = pos3 - pos2;
			vecToPosLeft = g_aPlayer[nPlayerNum].pos - pos2;
			vecToPosOldLeft = g_aPlayer[nPlayerNum].posOld - pos2;

			//Z
			//上方向の計算
			vecLineUp = pos2 - pos1;
			vecToPosUp = g_aPlayer[nPlayerNum].pos - pos1;
			vecToPosOldUp = g_aPlayer[nPlayerNum].posOld - pos1;
			//下方向の計算
			vecLineDown = pos0 - pos3;
			vecToPosDown = g_aPlayer[nPlayerNum].pos - pos3;
			vecToPosOldDown = g_aPlayer[nPlayerNum].posOld - pos3;

			//当たり判定本番
			//X
			//面積求める
			fAreaARight = TASUKIGAKE(vecToPosRight.x, vecToPosRight.z, vecMoveLeft.x, vecMoveLeft.z);
			fAreaALeft = TASUKIGAKE(vecToPosLeft.x, vecToPosLeft.z, vecMoveLeft.x, vecMoveLeft.z);
			fAreaBRight = TASUKIGAKE(vecLineRight.x, vecLineRight.z, vecMoveLeft.x, vecMoveLeft.z);
			fAreaBLeft = TASUKIGAKE(vecLineLeft.x, vecLineLeft.z, vecMoveLeft.x, vecMoveLeft.z);

			//左側AND範囲内
			if ((vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) <= 0.0f && (vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f)
			{
				if (fAreaARight / fAreaBRight >= 0.0f && fAreaARight / fAreaBRight <= 1.0f)
				{
					//1.0f = pushback
					float fRate = fAreaARight / fAreaBRight;
					g_aPlayer[nPlayerNum].pos.x = pos0.x + (vecLineRight.x * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					g_aPlayer[nPlayerNum].pos.z = pos0.z + (vecLineRight.z * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					break;
				}
			}
			else if ((vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) <= 0.0f && (vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f)
			{
				if (fAreaALeft / fAreaBLeft >= 0.0f && fAreaALeft / fAreaBLeft <= 1.0f)
				{
					float fRate = fAreaALeft / fAreaBLeft;
					g_aPlayer[nPlayerNum].pos.x = pos2.x + (vecLineLeft.x * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					g_aPlayer[nPlayerNum].pos.z = pos2.z + (vecLineLeft.z * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					break;
				}
			}

			//Z
			//面積求める
			fAreaAUp = TASUKIGAKE(vecToPosUp.x, vecToPosUp.z, vecMoveLeft.x, vecMoveLeft.z);
			fAreaADown = TASUKIGAKE(vecToPosDown.x, vecToPosDown.z, vecMoveLeft.x, vecMoveLeft.z);
			fAreaBUp = TASUKIGAKE(vecLineUp.x, vecLineUp.z, vecMoveLeft.x, vecMoveLeft.z);
			fAreaBDown = TASUKIGAKE(vecLineDown.x, vecLineDown.z, vecMoveLeft.x, vecMoveLeft.z);

			//左側AND範囲内
			if ((vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) <= 0.0f && (vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f)
			{
				if (fAreaAUp / fAreaBUp >= 0.0f && fAreaAUp / fAreaBUp <= 1.0f)
				{
					float fRate = fAreaAUp / fAreaBUp;
					g_aPlayer[nPlayerNum].pos.x = pos1.x + (vecLineUp.x * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					g_aPlayer[nPlayerNum].pos.z = pos1.z + (vecLineUp.z * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					break;
				}
			}
			else if ((vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) <= 0.0f && (vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f)
			{
				if (fAreaADown / fAreaBDown >= 0.0f && fAreaADown / fAreaBDown <= 1.0f)
				{
					float fRate = fAreaADown / fAreaBDown;
					g_aPlayer[nPlayerNum].pos.x = pos3.x + (vecLineDown.x * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					g_aPlayer[nPlayerNum].pos.z = pos3.z + (vecLineDown.z * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
					break;
				}
			}
		}
	}
}

//========================
//[デバッグ用]プレイヤーの移動処理
//========================
void MovePlayer(int nPadNum)
{
	//モデル移動
	//ゲームパッド部
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X方向のスティックが傾いている
		if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y方向のスティックも傾いている
			g_aPlayer[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y方向のスティックだけ傾いている
		g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	//キーボード部
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else
	{
		return;
	}

	//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
	g_aPlayer[nPadNum].pos.x += g_aPlayer[nPadNum].move.x;
	g_aPlayer[nPadNum].pos.z += g_aPlayer[nPadNum].move.z;

	//移動量消す
	g_aPlayer[nPadNum].move.x = 0.0f;
	g_aPlayer[nPadNum].move.z = 0.0f;
}
//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
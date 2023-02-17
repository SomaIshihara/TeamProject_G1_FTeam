//==========================================
//
//プレイヤープログラム[player.cpp]
//Author:石原颯馬  平澤詩苑
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "PvP_player.h"
#include "model.h"
#include "input.h"
#include "wall.h"
#include "score.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "gauge.h"
#include "sound.h"
#include "charge_cylinder.h"
#include "charge_effect.h"
#include "attack_effect.h"
#include "tremor_effect.h"
#include "item.h"
#include "meshfield.h"
#include "bonus.h"
#include "particle.h"
#include "comai.h"

//マクロ
#define PLAYER_HIPDROP_WAIT		(15)		//ヒップドロップの「開始・着地」硬直時間
#define PLAYER_BLOWING_POWER	(5.0f)		//ヒップドロップされた方の移動量
#define PLAYER_HIPDROP_POWER	(-15.0f)	//ヒップドロップするときの移動量
#define PLAYER_ROTATE_SPEED		(0.02f * D3DX_PI)	//回転速度
#define PLAYER_HIPSPIN_SPEED	(-0.5f)		//ヒップドロップスピンの回転値
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//ヒップドロップスピンしたときの１周判定をとる値

//移動量関係
#define PLAYER_MOVE_SPEED		(20.0f)		//プレイヤー移動速度
#define PLAYER_JUMP_SPEED		(7.7f)		//プレイヤージャンプ速度
#define ACCELERATION_GRAVITY	(9.8f)		//重力加速度
#define PLAYER_WEIGHT			(50)		//質量
#define PLAYER_POWER_ADD		(0.025f)	//移動の強さの増加値
#define DUMP_COEF				(0.04f)		//減衰係数
#define DEBUG_PLAYER_MOVE_SPEED	(5.0f)		//[デバッグ用]普通に移動するときの移動量
#define DECIMAL_PLACE			(1)			//小数点第何位まで移動していることにするか
#define DOWN_HEIGHT				(-1200.0f)	//ダウン判定とする高さ
#define HIPDROP_RADIUS			(100.0f)		//ヒップドロップ判定範囲

//アイテム関係
#define ACCELERATION_CONS		(0.5f)		//加速定数（1.0で全部渡す）
#define ACCELERATION_ITEMMAG	(1.5f)		//アイテム所持中の強化倍率
#define DEFANCE_CONS			(0.0f)		//防御定数（1.0で完全防御）
#define DEFANCE_ITEMADD			(0.3f)		//アイテム所持中の強化量
#define INVINCIBLE_ATK			(1.0f)		//無敵状態の自分の変換割合
#define INVINCIBLE_DEF			(0.0f)		//無敵状態の相手の変換割合

//ゴースト化状態
#define GOAST_ALPHA				(0.25f)		//不透明度
#define GOAST_FLASHSTART		(240)		//点滅開始する残り時間
#define GOAST_FLASHPULSE		(20)		//点滅の切り替え時間

#define PLAYER_SIZE_WIDTH		(25.0f)
#define PLAYER_SIZE_HEIGHT		(15.0f)
#define PLAYER_SIZE_DEPTH		(40.0f)

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
void ControllKeyboardPlayer(int nPlayerNum);
void ControllGPadPlayer(int nPlayerNum);
void ControllAI(int nPlayerNum);

void MovePlayer(int nPadNum);
void RotatePlayer(int nPadNum);		//MovePlayer のrot.y の計算式だけを残しています

void ChargePlayer(int nChargePlayer);	//チャージ処理
void DashPlayer(int nDashPlayer);		//ダッシュ処理
void JumpPlayer(int nJumpPlayer);		//ジャンプ処理
void HipDropPlayer(int nHipDropPlayer);	//ヒップドロップ処理

void CollisionPP(int nPlayerNum);	//プレイヤー同士の衝突判定
void CollisionHipDropPP(int nPlayerNum);		//ヒップドロップ時の衝突判定
void DownPlayer(int nDownPlayerNum);	//ダウンしたプレイヤーの処理
void RespawnPlayer(int nRespawnPlayer);	//リスポーン処理

void DecrementItemTime(int nPlayerNum);	//アイテムカウントをすべて減らす
void ItemStateParticle(int nPlayerNum);	//アイテムパーティクル表示処理

//グローバル変数
Player g_aPlayer[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;
bool g_bDebugMove = false;	//[デバッグ用]自由に動き回れるかどうか
Player *g_pNotMove[MAX_USE_GAMEPAD];	//移動していないプレイヤー検知用ポインタ

//初期位置向き
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(-50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
};

//[デバッグ用]AI挙動させるプレイヤー指定（コントローラーが刺さっていればそれを優先）
const bool c_aAIMove[MAX_USE_GAMEPAD] = { false,true,true,true };

//========================
//初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//変数初期化
		g_aPlayer[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = ZERO_SET;
		g_aPlayer[nCntPlayer].moveV0 = ZERO_SET;
		g_aPlayer[nCntPlayer].rot = ZERO_SET;
		g_aPlayer[nCntPlayer].moveGauge = 0;
		g_aPlayer[nCntPlayer].jumpTime = 0;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].bHipDrop = false;
		g_aPlayer[nCntPlayer].bHipDropSpin = false;
		g_aPlayer[nCntPlayer].nHipDropWait = 0;
		g_aPlayer[nCntPlayer].bNotMove = true;
		g_aPlayer[nCntPlayer].nRespawnPosNum = nCntPlayer;

		g_aPlayer[nCntPlayer].faceCollider[0] = D3DXVECTOR3(PLAYER_SIZE_WIDTH, 0.0f, PLAYER_SIZE_WIDTH);
		g_aPlayer[nCntPlayer].faceCollider[1] = D3DXVECTOR3(-PLAYER_SIZE_WIDTH, 0.0f, PLAYER_SIZE_WIDTH);

		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].lastAtkPlayer = -1;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;
		g_aPlayer[nCntPlayer].stat = PLAYERSTAT_WAIT;

		g_aPlayer[nCntPlayer].nATKItemTime = 0;
		g_aPlayer[nCntPlayer].nDEFItemTime = 0;
		g_aPlayer[nCntPlayer].nGhostItemTime = 0;
		g_aPlayer[nCntPlayer].bInvincible = false;
		g_aPlayer[nCntPlayer].nInvincibleTime = 0;

		g_aPlayer[nCntPlayer].pAI = NULL;

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].bUsePlayer = GetUseController_PvP(nCntPlayer);

		g_pNotMove[nCntPlayer] = &g_aPlayer[nCntPlayer];

		//プレイヤー・AI設定
		if (GetUseController_PvP(nCntPlayer) == true)
		{//プレイヤーは普通に使用していることにする
			g_aPlayer[nCntPlayer].bUsePlayer = true;
		}
		else if (c_aAIMove[nCntPlayer] == true)
		{//AIは脳みそポインタもらって使用していることにする
			g_aPlayer[nCntPlayer].pAI = GetAI();
			g_aPlayer[nCntPlayer].bUsePlayer = true;
		}
	}

	//[デバッグ]コントローラーが接続されていなければ1Pのみ有効化する
	if (GetUseControllerNum_PvP() == 0) 
	{
		g_aPlayer[0].bUsePlayer = true;
	}

	//その他変数
	g_bDebugMove = false;
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
	//[デバッグ用]自由移動設定
	if (GetKeyboardTrigger(DIK_F8))
	{
		g_bDebugMove = g_bDebugMove ? false : true;
	}

	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//前回の情報初期化
		g_aPlayer[nCntPlayer].lastAtkPlayer = -1;

		//現在の位置を前回の位置にする
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

		//ジャンプ時間を増やす
		g_aPlayer[nCntPlayer].jumpTime++;

		//パーティクル表示
		ItemStateParticle(nCntPlayer);

		//アイテム持続時間減らす
		DecrementItemTime(nCntPlayer);

		if (g_aPlayer[nCntPlayer].bUsePlayer == true)
		{//使用時のみ行う
			if (g_aPlayer[nCntPlayer].pAI == NULL)
			{//プレイヤー
				//接続されているか確認して切断されていたらプレイヤーを消す（例外としてコントローラーがつながっていないときは無視）
				if (GetUseControllerNum_PvP() != 0)
				{
					g_aPlayer[nCntPlayer].bUsePlayer = GetUseController_PvP(nCntPlayer);
				}
				else
				{
					ControllKeyboardPlayer(nCntPlayer);
				}

				//各プレイヤーの操作
				ControllGPadPlayer(nCntPlayer);
			}
			else
			{//AI
				//AIがコントローラー操作
				SelectAIMove(&g_aPlayer[nCntPlayer]);

				//AI移動
				ControllAI(nCntPlayer);
			}

			//当たり判定類
			if (g_aPlayer[nCntPlayer].nGhostItemTime <= 0)
			{//ゴースト化状態でなければ
				CollisionPP(nCntPlayer);
				CollisionIP(nCntPlayer);

				if (g_aPlayer[nCntPlayer].stat == PLAYERSTAT_HIPDROP)
				{//ヒップドロップ中なら
					CollisionHipDropPP(nCntPlayer);
				}
			}
		}

		//使用されているかにかかわらず行う
		//ジャンプ量設定
		g_aPlayer[nCntPlayer].move.y = g_aPlayer[nCntPlayer].moveV0.y - (ACCELERATION_GRAVITY * g_aPlayer[nCntPlayer].jumpTime / MAX_FPS);

		//移動後がy<0であり、現在の位置が、フィールド以上の高さにいるなら移動量消す
		if (g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].move.y < 0.0f && g_aPlayer[nCntPlayer].pos.y >= 0.0f)
		{
			//原点位置からのプレイヤーの距離を計算
			float fLength = sqrtf(powf((g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].move.x), 2) +
				powf((g_aPlayer[nCntPlayer].pos.z + g_aPlayer[nCntPlayer].move.z), 2));

			//原点位置からの距離が、フィールドの半径以下　　なら、フィールドに乗っている
			if (fLength <= GetMeshField()->fRadius)
			{
				if (g_aPlayer[nCntPlayer].bHipDrop == true)
				{//ヒップドロップしてたならエフェクト出す
					SetTremorEffect(g_aPlayer[nCntPlayer].pos);
					g_aPlayer[nCntPlayer].bHipDrop = false;    //ヒップドロップしてない
				}
				g_aPlayer[nCntPlayer].bJump = false;
				g_aPlayer[nCntPlayer].moveV0.y = 0.0f;
				g_aPlayer[nCntPlayer].move.y = 0.0f;
				g_aPlayer[nCntPlayer].jumpTime = 0;
				g_aPlayer[nCntPlayer].pos.y = 0.0f;
				g_aPlayer[nCntPlayer].nHipDropWait = 0;
			}
			else
			{
				g_aPlayer[nCntPlayer].stat = PLAYERSTAT_FALL;
			}
		}

		if (g_aPlayer[nCntPlayer].pos.y <= DOWN_HEIGHT)
		{//落ち切った
			DownPlayer(nCntPlayer);
		}

		//ボーナスの当たり判定
		CollisionBonus(g_aPlayer[nCntPlayer].pos, nCntPlayer);
	}

	//移動量と衝突判定をもとに移動する
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//移動検知
		if (g_aPlayer[nCntPlayer].bNotMove == true)
		{
			if (fabsf(g_aPlayer[nCntPlayer].move.x) > 0.0f || fabsf(g_aPlayer[nCntPlayer].move.z) > 0.0f)
			{
				g_aPlayer[nCntPlayer].bNotMove = false;
			}
		}

		if (g_aPlayer[nCntPlayer].lastAtkPlayer == -1)
		{//ぶつかってないまたは移動量交換済み		
			//ヒップドロップしておらず、ヒップドロップの硬直中でもない
			if (g_aPlayer[nCntPlayer].bHipDrop == false && g_aPlayer[nCntPlayer].nHipDropWait <= 0)
			{
				//普通に移動
				g_aPlayer[nCntPlayer].pos += g_aPlayer[nCntPlayer].move;
			}
		}
		else
		{//ぶつかった
			//移動量交換
			D3DXVECTOR3 moveTmp1 = g_aPlayer[nCntPlayer].move;
			D3DXVECTOR3 moveTmp2 = g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].move;

			//割合設定
			float fPowerConvertion1, fPowerConvertion2;
			if (g_aPlayer[nCntPlayer].bInvincible == true)
			{//我無敵也(自分0%,相手100%)
				fPowerConvertion1 = INVINCIBLE_DEF;
				fPowerConvertion2 = INVINCIBLE_ATK;
			}
			else if (g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].bInvincible == true)
			{//相手無敵也(自分100%,相手0%)
				fPowerConvertion1 = INVINCIBLE_ATK;
				fPowerConvertion2 = INVINCIBLE_DEF;
			}
			else
			{//どっちもむてきじゃないよ
				fPowerConvertion1 = ACCELERATION_CONS * (g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayer[nCntPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);

				fPowerConvertion2 = ACCELERATION_CONS * (g_aPlayer[nCntPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);
			}

			g_aPlayer[nCntPlayer].move = moveTmp2 * fPowerConvertion1;
			g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].move = moveTmp1 * fPowerConvertion2;

			//移動量交換済み扱いにする
			g_aPlayer[nCntPlayer].lastAtkPlayer = -1;
			g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].lastAtkPlayer = -1;

			//普通に移動
			g_aPlayer[nCntPlayer].pos += g_aPlayer[nCntPlayer].move;
		}

		//移動量減衰
		g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
		g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;
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
					//ゴースト化状態を考慮した変更用マテリアル変数
					D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

					//ゴースト化状態なら半透明で設定
					if (g_aPlayer[nCntPlayer].nGhostItemTime > GOAST_FLASHSTART)
					{
						matChange.Diffuse.a = GOAST_ALPHA;
					}
					else if (g_aPlayer[nCntPlayer].nGhostItemTime > 0 && g_aPlayer[nCntPlayer].nGhostItemTime % (GOAST_FLASHPULSE * 2) < GOAST_FLASHPULSE)
					{
						matChange.Diffuse.a = GOAST_ALPHA;
					}
					//マテリアル設定
					pDevice->SetMaterial(&matChange);

					//テクスチャ設定
					pDevice->SetTexture(0, g_aPlayer[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

					//モデル描画
					g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				/*------------------------------------------------------------------
				影の描画		Author:平澤詩苑 石原颯馬
				--------------------------------------------------------------------*/
				if (g_aPlayer[nCntPlayer].pos.y >= 0.0f)
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
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//プレイヤーのチャージ処理
//========================
void ChargePlayer(int nChargePlayer)
{
	g_aPlayer[nChargePlayer].moveGauge += PLAYER_POWER_ADD;

	if (g_aPlayer[nChargePlayer].moveGauge >= PLAYER_POWER_MAX)
	{
		g_aPlayer[nChargePlayer].moveGauge = PLAYER_POWER_MAX;
		StopSound(SOUND_LABEL_SE_ENERGY_00);
	}
	if (g_aPlayer[nChargePlayer].moveGauge <= PLAYER_POWER_ADD)
	{
		PlaySound(SOUND_LABEL_SE_ENERGY_00);
	}

	g_aPlayer[nChargePlayer].stat = PLAYERSTAT_CHARGE;
	SetChargeEffect(g_aPlayer[nChargePlayer].pos, nChargePlayer);
	SetChargeCylinder(g_aPlayer[nChargePlayer].pos, nChargePlayer);
}

//========================
//プレイヤーのダッシュ処理
//========================
void DashPlayer(int nDashPlayer)
{
	PlaySound(SOUND_LABEL_SE_GRASSDASH);

	//進行方向の設定
	g_aPlayer[nDashPlayer].move.x = -sinf(g_aPlayer[nDashPlayer].rot.y) * g_aPlayer[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;
	g_aPlayer[nDashPlayer].move.z = -cosf(g_aPlayer[nDashPlayer].rot.y) * g_aPlayer[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;

	g_aPlayer[nDashPlayer].stat = PLAYERSTAT_DASH;
	g_aPlayer[nDashPlayer].moveGauge = 0;
}

//========================
//プレイヤーのヒップドロップ処理
//========================
void HipDropPlayer(int nHipDropPlayer)
{
	PlaySound(SOUND_LABEL_SE_HIPDROP);

	g_aPlayer[nHipDropPlayer].moveV0.y = PLAYER_HIPDROP_POWER;		//ヒップドロップの降下速度を代入
	g_aPlayer[nHipDropPlayer].move.x = 0.0f;						//X・Zの移動量消す
	g_aPlayer[nHipDropPlayer].move.z = 0.0f;
	g_aPlayer[nHipDropPlayer].moveGauge = 0.0f;
	g_aPlayer[nHipDropPlayer].jumpTime = 0;							//ジャンプ時間リセット
	g_aPlayer[nHipDropPlayer].bHipDrop = true;						//ヒップドロップしている
	g_aPlayer[nHipDropPlayer].bHipDropSpin = true;					//スピンしている
	g_aPlayer[nHipDropPlayer].stat = PLAYERSTAT_HIPDROP;
}

//========================
//プレイヤーのジャンプ処理
//========================
void JumpPlayer(int nJumpPlayer)
{
	PlaySound(SOUND_LABEL_SE_JUMP);

	g_aPlayer[nJumpPlayer].moveV0.y = PLAYER_JUMP_SPEED;//移動量設定
	g_aPlayer[nJumpPlayer].jumpTime = 0;	//ジャンプ時間リセット
	g_aPlayer[nJumpPlayer].bJump = true;
	g_aPlayer[nJumpPlayer].stat = PLAYERSTAT_JUMP;
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
	D3DXVECTOR3 vecMove[2];

	//2は2頂点の2
	float fAreaARight[2], fAreaALeft[2], fAreaBRight[2], fAreaBLeft[2];
	float fAreaAUp[2], fAreaADown[2], fAreaBUp[2], fAreaBDown[2];

	//未反映の位置考慮
	D3DXVECTOR3 posTemp = g_aPlayer[nPlayerNum].pos + g_aPlayer[nPlayerNum].move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)
		{
			//各頂点求める
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - PLAYER_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - PLAYER_SIZE_DEPTH);

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
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + PLAYER_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - PLAYER_SIZE_DEPTH);

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
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + PLAYER_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + PLAYER_SIZE_DEPTH);

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
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - PLAYER_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + PLAYER_SIZE_DEPTH);

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
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				vecMove[nCntCollision] = (posTemp + g_aPlayer[nPlayerNum].faceCollider[nCntCollision]) - (g_aPlayer[nPlayerNum].posOld + g_aPlayer[nPlayerNum].faceCollider[nCntCollision]);
			}

			//X
			//右方向の計算
			vecLineRight = pos1 - pos0;
			vecToPosRight = posTemp - pos0;
			vecToPosOldRight = g_aPlayer[nPlayerNum].posOld - pos0;

			//左方向の計算
			vecLineLeft = pos3 - pos2;
			vecToPosLeft = posTemp - pos2;
			vecToPosOldLeft = g_aPlayer[nPlayerNum].posOld - pos2;

			//Z
			//上方向の計算
			vecLineUp = pos2 - pos1;
			vecToPosUp = posTemp - pos1;
			vecToPosOldUp = g_aPlayer[nPlayerNum].posOld - pos1;
			//下方向の計算
			vecLineDown = pos0 - pos3;
			vecToPosDown = posTemp - pos3;
			vecToPosOldDown = g_aPlayer[nPlayerNum].posOld - pos3;

			//当たり判定本番
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				//X
				//面積求める
				fAreaARight[nCntCollision] = TASUKIGAKE(vecToPosRight.x, vecToPosRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaALeft[nCntCollision] = TASUKIGAKE(vecToPosLeft.x, vecToPosLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBRight[nCntCollision] = TASUKIGAKE(vecLineRight.x, vecLineRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBLeft[nCntCollision] = TASUKIGAKE(vecLineLeft.x, vecLineLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) <= 0.0f && (vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f)
				{
					if (fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] >= 0.0f && fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayer[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							//1.0f = pushback
							float fRate = fAreaARight[nCntCollision] / fAreaBRight[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos0.x + (vecLineRight.x * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos0.z + (vecLineRight.z * fRate) - cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) <= 0.0f && (vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f)
				{
					if (fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] >= 0.0f && fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayer[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							float fRate = fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos2.x + (vecLineLeft.x * fRate) + sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos2.z + (vecLineLeft.z * fRate) + cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
			
				//Z
				//面積求める
				fAreaAUp[nCntCollision] = TASUKIGAKE(vecToPosUp.x, vecToPosUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaADown[nCntCollision] = TASUKIGAKE(vecToPosDown.x, vecToPosDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBUp[nCntCollision] = TASUKIGAKE(vecLineUp.x, vecLineUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBDown[nCntCollision] = TASUKIGAKE(vecLineDown.x, vecLineDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) <= 0.0f && (vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f)
				{
					if (fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] >= 0.0f && fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayer[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							float fRate = fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos1.x + (vecLineUp.x * fRate) + cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos1.z + (vecLineUp.z * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) <= 0.0f && (vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f)
				{
					if (fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] >= 0.0f && fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//動いてる
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayer[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							float fRate = fAreaADown[nCntCollision] / fAreaBDown[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos3.x + (vecLineDown.x * fRate) - cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos3.z + (vecLineDown.z * fRate) + sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
			}
		}
	}
}

//========================
//ヒップドロップ時の衝突処理
//========================
void CollisionHipDropPP(int nPlayerNum)
{
	//=pos0~pos2の説明==================
	//
	//		・g_ap[nPNum].posOld
	//		↓
	//		・g_ap[nPNum].pos
	// pos1		pos0
	//	・<-----・		矢印:vecLine
	//	｜
	//	｜
	//	↓
	//　・
	// pos2
	//==================================

	D3DXVECTOR3 pos0, pos1, pos2;
	D3DXVECTOR3 vecLineX, vecToPosX, vecToPosOldX;
	D3DXVECTOR3 vecLineZ, vecToPosZ, vecToPosOldZ;
	D3DXVECTOR3 vecMove;
	float fAreaAX, fAreaBX, fAreaAZ, fAreaBZ;

	//未反映の位置考慮
	D3DXVECTOR3 posTemp = g_aPlayer[nPlayerNum].pos + g_aPlayer[nPlayerNum].move;

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
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + PLAYER_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + PLAYER_SIZE_DEPTH);

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
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - PLAYER_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + PLAYER_SIZE_DEPTH);

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
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - PLAYER_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - PLAYER_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(D3DX_PI - fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//角度に応じて頂点の位置をずらす
			pos2.x = g_aPlayer[nCntOtherPlayer].pos.x - sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = g_aPlayer[nCntOtherPlayer].pos.z + cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//ベクトル求める
			//move
			vecMove = posTemp - g_aPlayer[nPlayerNum].posOld;

			//X
			vecLineX = pos1 - pos0;
			vecToPosX = posTemp - pos0;
			vecToPosOldX = g_aPlayer[nPlayerNum].posOld - pos0;

			//Z
			vecLineZ = pos2 - pos1;
			vecToPosZ = posTemp - pos1;
			vecToPosOldZ = g_aPlayer[nPlayerNum].posOld - pos1;

			//当たり判定本番
			//X
			//面積求める
			fAreaAX = TASUKIGAKE(vecToPosX.x, vecToPosX.y, vecMove.x, vecMove.y);
			fAreaBX = TASUKIGAKE(vecLineX.x, vecLineX.y, vecMove.x, vecMove.y);
			fAreaAZ = TASUKIGAKE(vecToPosZ.z, vecToPosZ.y, vecMove.z, vecMove.y);
			fAreaBZ = TASUKIGAKE(vecLineZ.z, vecLineZ.y, vecMove.z, vecMove.y);
			//左側AND範囲内
			/*if ((vecLineX.y * vecToPosX.x) - (vecLineX.x * vecToPosX.y) <= 0.0f && (vecLineX.y * vecToPosOldX.x) - (vecLineX.x * vecToPosOldX.y) >= 0.0f)
			{*/
			float fHeight = posTemp.y - g_aPlayer[nCntOtherPlayer].pos.y;
			if (fHeight <= PLAYER_SIZE_HEIGHT)
			{
				float fRadius = PYTHAGORAS(g_aPlayer[nCntOtherPlayer].pos.x - posTemp.x,
					g_aPlayer[nCntOtherPlayer].pos.z - posTemp.z);

				if (fRadius <= HIPDROP_RADIUS)
				{
					//移動量計算
					float fAngleHipDrop = atan2f(g_aPlayer[nCntOtherPlayer].pos.x - posTemp.x,
						g_aPlayer[nCntOtherPlayer].pos.z - posTemp.z);
					g_aPlayer[nCntOtherPlayer].move.x = sinf(fAngleHipDrop) * PLAYER_BLOWING_POWER;
					g_aPlayer[nCntOtherPlayer].move.z = -cosf(fAngleHipDrop) * PLAYER_BLOWING_POWER;

					//ちょっと飛ばす
					g_aPlayer[nCntOtherPlayer].moveV0.y = PLAYER_BLOWING_POWER;
					g_aPlayer[nCntOtherPlayer].jumpTime = 0;

					//攻撃された扱いにする
					g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
				}
			}
		}
	}
}

//========================
//[デバッグ用]プレイヤーの処理（キーボード）
//========================
void ControllKeyboardPlayer(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayer[nPlayerNum].bHipDrop == false)
	{
		//キーボード操作時の動作
		//移動方法（ダッシュ）押して離す
		if ((int)(g_aPlayer[nPlayerNum].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
			&& (int)(g_aPlayer[nPlayerNum].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
		{//もうこれ動いてるって言わないよね（ほぼ動いていない）
			if (GetKeyboardPress(DIK_SPACE) == true)
			{//スペースキーは押された
			 //プレイヤーのチャージ処理
				ChargePlayer(nPlayerNum);
			}
			else
			{
				g_aPlayer[nPlayerNum].stat = PLAYERSTAT_WAIT;
			}
			if (GetKeyboardRelease(DIK_SPACE) == true)
			{//SPACEキーが離された
			 //プレイヤーのダッシュ処理
				DashPlayer(nPlayerNum);
			}
		}
		else
		{
			g_aPlayer[nPlayerNum].moveGauge = 0;
		}

		//ジャンプ・ヒップドロップ
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_aPlayer[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayer[nPlayerNum].bJump)
			{
				HipDropPlayer(nPlayerNum);									//プレイヤーのヒップドロップ処理
				g_aPlayer[nPlayerNum].nHipDropWait = PLAYER_HIPDROP_WAIT;	//ヒップドロップの開始硬直を設定
			}
			else
			{
				JumpPlayer(nPlayerNum);			//プレイヤーのジャンプ処理
			}

			MovePlayer(nPlayerNum);
		}

		//回転
		if (GetKeyboardPress(DIK_SPACE) == false)
		{//チャージ中ではない
			RotatePlayer(nPlayerNum);
		}
	}
	//ヒップドロップ中
	else
	{
		//ヒップドロップ硬直時間がある
		if (g_aPlayer[nPlayerNum].nHipDropWait > 0)
		{
			g_aPlayer[nPlayerNum].nHipDropWait--;		//硬直時間を減らしていく
		}
		else
		{
			//落下させる
			g_aPlayer[nPlayerNum].pos.y += g_aPlayer[nPlayerNum].moveV0.y;
		}
	}
}

//========================
//プレイヤーの処理（ゲームパッド）
//========================
void ControllGPadPlayer(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayer[nPlayerNum].bHipDrop == false)
	{
		if ((int)(g_aPlayer[nPlayerNum].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
			&& (int)(g_aPlayer[nPlayerNum].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
		{//もうこれ動いてるって言わないよね（ほぼ動いていない）
			if (GetGamepadPress(nPlayerNum, XINPUT_GAMEPAD_X) == true)
			{//Xボタンが押された					 
			 //プレイヤーのチャージ処理
				ChargePlayer(nPlayerNum);
			}
			else
			{
				g_aPlayer[nPlayerNum].stat = PLAYERSTAT_WAIT;
			}

			if (GetGamepadRelease(nPlayerNum, XINPUT_GAMEPAD_X) == true)
			{//Xボタンが離された
			 //プレイヤーのダッシュ処理
				DashPlayer(nPlayerNum);
			}
		}
		else
		{
			g_aPlayer[nPlayerNum].moveGauge = 0;
		}

		//ジャンプ・ヒップドロップ
		if (GetGamepadTrigger(nPlayerNum, XINPUT_GAMEPAD_A) == true && g_aPlayer[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayer[nPlayerNum].bJump)
			{
				HipDropPlayer(nPlayerNum);									//プレイヤーのヒップドロップ処理
				g_aPlayer[nPlayerNum].nHipDropWait = PLAYER_HIPDROP_WAIT;	//ヒップドロップの開始硬直を設定
			}
			else
			{
				JumpPlayer(nPlayerNum);			//プレイヤーのジャンプ処理
			}

			MovePlayer(nPlayerNum);
		}

		//回転
		if (GetGamepadPress(nPlayerNum, XINPUT_GAMEPAD_X) == false)
		{//Xボタンが押されていない
			RotatePlayer(nPlayerNum);
		}
	}
	//ヒップドロップ中
	else
	{
		//ヒップドロップ硬直時間がある
		if (g_aPlayer[nPlayerNum].nHipDropWait > 0)
		{
			g_aPlayer[nPlayerNum].nHipDropWait--;		//硬直時間を減らしていく
		}
		else
		{
			//落下させる
			g_aPlayer[nPlayerNum].pos.y += g_aPlayer[nPlayerNum].moveV0.y;
		}
	}
}

//========================
//AIの処理
//========================
void ControllAI(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayer[nPlayerNum].bHipDrop == false)
	{
		if ((int)(g_aPlayer[nPlayerNum].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
			&& (int)(g_aPlayer[nPlayerNum].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
		{//もうこれ動いてるって言わないよね（ほぼ動いていない）
			if (g_aPlayer[nPlayerNum].pAI->controll.bPushX)
			{//Xボタンが押された					 
			 //プレイヤーのチャージ処理
				ChargePlayer(nPlayerNum);
			}
			else if (g_aPlayer[nPlayerNum].pAI->controll.bPushXRele)
			{//Xボタンが離された
			 //プレイヤーのダッシュ処理
				DashPlayer(nPlayerNum);
			}
		}
		else
		{
			g_aPlayer[nPlayerNum].moveGauge = 0;
		}

		//ジャンプ・ヒップドロップ
		if (g_aPlayer[nPlayerNum].pAI->controll.bPushA == true && g_aPlayer[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayer[nPlayerNum].bJump)
			{
				HipDropPlayer(nPlayerNum);									//プレイヤーのヒップドロップ処理
				g_aPlayer[nPlayerNum].nHipDropWait = PLAYER_HIPDROP_WAIT;	//ヒップドロップの開始硬直を設定
			}
			else
			{
				JumpPlayer(nPlayerNum);			//プレイヤーのジャンプ処理
			}

			MovePlayer(nPlayerNum);
		}

		//回転
		if (g_aPlayer[nPlayerNum].pAI->controll.bPushX == false)
		{//仮
			RotatePlayer(nPlayerNum);
		}
	}
	//ヒップドロップ中
	else
	{
		//ヒップドロップ硬直時間がある
		if (g_aPlayer[nPlayerNum].nHipDropWait > 0)
		{
			g_aPlayer[nPlayerNum].nHipDropWait--;		//硬直時間を減らしていく
		}
		else
		{
			//落下させる
			g_aPlayer[nPlayerNum].pos.y += g_aPlayer[nPlayerNum].moveV0.y;
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
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y方向のスティックだけ傾いている
		g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
	}
	//キーボード部
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
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
//プレイヤーの向き変更処理
//========================
void RotatePlayer(int nPadNum)
{
	if (!g_bDebugMove)
	{//リリース用
	 //ゲームパッド部
		if (g_aPlayer[nPadNum].pAI == NULL)
		{//プレイヤー
			if (GetLStickX(nPadNum) > 0)
			{
				g_aPlayer[nPadNum].rot.y += PLAYER_ROTATE_SPEED;
			}
			else if (GetLStickX(nPadNum) < 0)
			{
				g_aPlayer[nPadNum].rot.y -= PLAYER_ROTATE_SPEED;
			}
			//キーボード部
			else if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer[nPadNum].rot.y -= PLAYER_ROTATE_SPEED;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer[nPadNum].rot.y += PLAYER_ROTATE_SPEED;
			}
		}
		else
		{//AI
			switch (g_aPlayer[nPadNum].pAI->controll.stick)
			{
			case AISTICK_LEFT:
				g_aPlayer[nPadNum].rot.y -= PLAYER_ROTATE_SPEED;
				break;
			case AISTICK_RIGHT:
				g_aPlayer[nPadNum].rot.y += PLAYER_ROTATE_SPEED;
				break;
			case AISTICK_NEUTRAL:
				break;
			}
		}
	}
	else
	{//デバッグ用
		//モデル移動
		//ゲームパッド部
		if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
		{//X方向のスティックが傾いている
			g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
		}
		else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y方向のスティックだけ傾いている
			g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
		}
		//キーボード部
		else if (GetKeyboardPress(DIK_W) == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
			}
			else
			{
				g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
			}
			else
			{
				g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
			}
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
		}
		else
		{
			return;
		}
	}
}

//========================
//ダウンしたプレイヤーの処理
//========================
void DownPlayer(int nDownPlayerNum)
{
	if (g_aPlayer[nDownPlayerNum].nNumHitPlayer != -1)
	{//当てられて落ちた場合
		g_aPlayer[g_aPlayer[nDownPlayerNum].nNumHitPlayer].nScore++;
		AddScore(1, g_aPlayer[nDownPlayerNum].nNumHitPlayer);
	}
	else
	{//ただ単に自滅した場合
		for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
		{
			if (nCntOtherPlayer != nDownPlayerNum)
			{
				g_aPlayer[nCntOtherPlayer].nScore++;
				AddScore(1, nCntOtherPlayer);
			}
		}
	}

	PlaySound(SOUND_LABEL_SE_DROP);

	RespawnPlayer(nDownPlayerNum);
}

//========================
//プレイヤーのリスポーン処理
//========================
void RespawnPlayer(int nRespawnPlayer)
{
	//各ステータス再設定
	//位置設定（かぶらないようにする）
	for (int nCntNUse = 0; nCntNUse < MAX_USE_GAMEPAD; nCntNUse++)
	{
		if (g_pNotMove[nCntNUse] == NULL || g_pNotMove[nCntNUse]->bNotMove == false)
		{
			//前にリスポーンした位置番号のポインタをNULLにする
			if (g_pNotMove[g_aPlayer[nRespawnPlayer].nRespawnPosNum] == &g_aPlayer[nRespawnPlayer])
			{
				g_pNotMove[g_aPlayer[nRespawnPlayer].nRespawnPosNum] = NULL;
			}			

			//新しい位置を入れる
			g_aPlayer[nRespawnPlayer].nRespawnPosNum = nCntNUse;
			g_aPlayer[nRespawnPlayer].pos = D3DXVECTOR3(c_aPosRot[nCntNUse][0].x, 70.0f, c_aPosRot[nCntNUse][0].z);
			//角度も調整するならここに書く

			//動いていないことにする
			g_aPlayer[nRespawnPlayer].bNotMove = true;

			//参照用ポインタ代入
			g_pNotMove[nCntNUse] = &g_aPlayer[nRespawnPlayer];
			break;
		}
	}

	//その他設定
	g_aPlayer[nRespawnPlayer].posOld = g_aPlayer[nRespawnPlayer].pos;
	g_aPlayer[nRespawnPlayer].move = ZERO_SET;
	g_aPlayer[nRespawnPlayer].moveV0 = ZERO_SET;
	g_aPlayer[nRespawnPlayer].rot = ZERO_SET;
	g_aPlayer[nRespawnPlayer].moveGauge = 0;
	g_aPlayer[nRespawnPlayer].jumpTime = 0;
	g_aPlayer[nRespawnPlayer].nHipDropWait = 0;
	g_aPlayer[nRespawnPlayer].bJump = true;
	g_aPlayer[nRespawnPlayer].bHipDrop = false;
	g_aPlayer[nRespawnPlayer].bHipDropSpin = false;

	g_aPlayer[nRespawnPlayer].lastAtkPlayer = -1;
	g_aPlayer[nRespawnPlayer].nNumHitPlayer = -1;
	g_aPlayer[nRespawnPlayer].stat = PLAYERSTAT_WAIT;

	g_aPlayer[nRespawnPlayer].nATKItemTime = 0;
	g_aPlayer[nRespawnPlayer].nDEFItemTime = 0;
	g_aPlayer[nRespawnPlayer].nGhostItemTime = 0;
}

//========================
//アイテムカウント減らす処理
//========================
void DecrementItemTime(int nPlayerNum)
{
	g_aPlayer[nPlayerNum].nATKItemTime--;
	g_aPlayer[nPlayerNum].nDEFItemTime--;
	g_aPlayer[nPlayerNum].nGhostItemTime--;
}

//========================
//アイテムパーティクル表示処理
//========================
void ItemStateParticle(int nPlayerNum)
{
	if (g_aPlayer[nPlayerNum].nATKItemTime > 0)
	{
		SetParticle(g_aPlayer[nPlayerNum].pos, 12.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_ATK);
		SetParticle(g_aPlayer[nPlayerNum].pos, 7.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_ATK);
	}


	if (g_aPlayer[nPlayerNum].nDEFItemTime > 0)
	{
		SetParticle(g_aPlayer[nPlayerNum].pos, 5.0f, 15, PARTICLE_NORMAL, OBJECT_PLAYER_DEF);
		SetParticle(g_aPlayer[nPlayerNum].pos, 7.0f, 15, PARTICLE_NORMAL, OBJECT_PLAYER_DEF);
	}


	if (g_aPlayer[nPlayerNum].nGhostItemTime > 0)
	{
		//SetParticle(g_aPlayer[nPlayerNum].pos, 12.0f, 10, PARTICLE_NORMAL, OBJECT_PLAYER_GOAST);
		//SetParticle(g_aPlayer[nPlayerNum].pos, 12.0f, 10, PARTICLE_NORMAL, OBJECT_PLAYER_GOAST);
	}
}

//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
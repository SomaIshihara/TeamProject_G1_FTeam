//==========================================
//
//プレイヤープログラム[player.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "file.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "wall.h"
#include "meshfield.h"
#include "object.h"
#include "debugproc.h"
#include <assert.h>

//マクロ
#define PLAYER_MOVE_SPEED	(7.0f)	//プレイヤー移動速度
#define DUMP_COEF			(0.4f)	//減衰係数
#define BULLET_SPEED		(20.0f)	//弾の速さ

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
void CollisionWallPlayer(int nNumber);
void CollisionObjPlayer(int nNumber);
void CollisionEnemyPlayer(int nNumber);
void SetMotion(MOTIONTYPE type);
void UpdateMotion(void);

//グローバル変数
Player g_player;
int g_nIdxShadow = -1;

//========================
//初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	Camera *pCamera = GetCamera();
	Model *pModel = GetModel(MODELNAME_EXITHUMAN);

	//変数初期化
	g_player.pos = PLAYER_POS;
	g_player.posOld = g_player.pos;
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.bIntoSafeArea = false;
	g_player.bInfection = false;
	g_player.mMotion = {};
	GetMotionInfo(&g_player.mMotion.aMotionInfo[0]);
	SetMotion(MOTIONTYPE_NEUTRAL);
	
	g_player.nNumModel = 0;

	g_nIdxShadow = -1;

	//対角線の長さ・角度
	g_player.fLength = sqrtf(EXITHUMAN_WIDTH * EXITHUMAN_WIDTH + EXITHUMAN_HEIGHT * EXITHUMAN_HEIGHT) * 0.5f;
	g_player.fAngle = atan2f(EXITHUMAN_WIDTH, EXITHUMAN_HEIGHT);

	//Xファイル読み込み
	for (int nCntModel = 0; nCntModel < EXITHUMAN_MODEL_NUM; nCntModel++)
	{
		g_player.aModel[nCntModel] = pModel[nCntModel];
#if 0
		D3DXLoadMeshFromX(
			c_pFileNamePlayer[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[nCntModel].pBuffMat,
			NULL,
			&g_player.aModel[nCntModel].dwNumMatModel,
			&g_player.aModel[nCntModel].pMesh);

		//テクスチャ読み込み
		D3DXMATERIAL *pMat;	//マテリアルポインタ

		//マテリアル情報に対するポインタ取得
		pMat = (D3DXMATERIAL *)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntTex = 0; nCntTex < (int)g_player.aModel[nCntModel].dwNumMatModel; nCntTex++)
		{
			if (pMat[nCntTex].pTextureFilename != NULL)
			{
				//テクスチャ読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntTex].pTextureFilename,
					&g_player.aModel[nCntModel].apTexture[nCntTex]);
			}
		}
#endif
	}

#if 0
	//階層構造設定
	//体
	g_player.aModel[0].nIdxModelParent = -1;
	g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
	g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//頭
	g_player.aModel[1].nIdxModelParent = 0;
	g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//左腕
	g_player.aModel[2].nIdxModelParent = 0;
	g_player.aModel[2].pos = D3DXVECTOR3(-5.0f, 7.0f, 0.0f);
	g_player.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//左手
	g_player.aModel[3].nIdxModelParent = 2;
	g_player.aModel[3].pos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	g_player.aModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//右腕
	g_player.aModel[4].nIdxModelParent = 0;
	g_player.aModel[4].pos = D3DXVECTOR3(5.0f, 7.0f, 0.0f);
	g_player.aModel[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//右手
	g_player.aModel[5].nIdxModelParent = 4;
	g_player.aModel[5].pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	g_player.aModel[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//左腿
	g_player.aModel[6].nIdxModelParent = 0;
	g_player.aModel[6].pos = D3DXVECTOR3(-3.0f, -8.0f, 0.0f);
	g_player.aModel[6].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//左足
	g_player.aModel[7].nIdxModelParent = 6;
	g_player.aModel[7].pos = D3DXVECTOR3(0.0f, -12.0f, 0.0f);
	g_player.aModel[7].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//右腿
	g_player.aModel[8].nIdxModelParent = 0;
	g_player.aModel[8].pos = D3DXVECTOR3(3.0f, -8.0f, 0.0f);
	g_player.aModel[8].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//右足
	g_player.aModel[9].nIdxModelParent = 8;
	g_player.aModel[9].pos = D3DXVECTOR3(0.0f, -12.0f, 0.0f);
	g_player.aModel[9].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
#endif
	//影設定
	g_nIdxShadow = SetShadow();
}

//========================
//終了処理
//========================
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < EXITHUMAN_MODEL_NUM; nCntModel++)
	{
		//model.cppの方でReleaseするからこっちはNULL入れるだけでいいよ
		g_player.aModel[nCntModel].pMesh = NULL;
		g_player.aModel[nCntModel].pBuffMat = NULL;
#if 0
		//メッシュの破棄
		if (g_player.aModel[nCntModel].pMesh != NULL)
		{
			g_player.aModel[nCntModel].pMesh->Release();
			g_player.aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_player.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.aModel[nCntModel].pBuffMat->Release();
			g_player.aModel[nCntModel].pBuffMat = NULL;
		}
#endif
	}
}

//========================
//更新処理
//========================
void UpdatePlayer(void)
{
	//現在の位置を前回の位置にする
	g_player.posOld = g_player.pos;

	//カメラ向きに応じてプレイヤーの向き旋回
	g_player.rot.y = -(float)fmod(GetCamera()->rot.y + 1.0f * D3DX_PI + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;

	//モデル移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_W + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		if (g_player.mMotion.motionType != MOTIONTYPE_MOVE)
		{
			//SetMotion(MOTIONTYPE_MOVE);
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_S + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		if (g_player.mMotion.motionType != MOTIONTYPE_MOVE)
		{
			//SetMotion(MOTIONTYPE_MOVE);
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_A + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
		g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		if (g_player.mMotion.motionType != MOTIONTYPE_MOVE)
		{
			//SetMotion(MOTIONTYPE_MOVE);
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_D + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
		g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		if (g_player.mMotion.motionType != MOTIONTYPE_MOVE)
		{
			//SetMotion(MOTIONTYPE_MOVE);
		}
	}
	else
	{
		SetMotion(MOTIONTYPE_NEUTRAL);
	}

	//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
	g_player.pos.x += g_player.move.x;
	g_player.pos.z += g_player.move.z;

	//壁当たり判定
	CollisionWallPlayer(0);

	//オブジェクト当たり判定
	CollisionObjPlayer(0);

	//敵当たり判定
	CollisionEnemyPlayer(0);

	GetCamera()->posV.x += g_player.pos.x - g_player.posOld.x;
	GetCamera()->posV.z += g_player.pos.z - g_player.posOld.z;
	GetCamera()->posR.x += g_player.pos.x - g_player.posOld.x;
	GetCamera()->posR.z += g_player.pos.z - g_player.posOld.z;

	//移動量減衰
	g_player.move.x += (0 - g_player.move.x) * DUMP_COEF;
	g_player.move.z += (0 - g_player.move.z) * DUMP_COEF;

	//セーフゾーン判定
	if (g_player.pos.x >= SAFEAREA_MIN_X && g_player.pos.x <= SAFEAREA_MAX_X && g_player.pos.z >= SAFEAREA_MIN_Z && g_player.pos.z <= SAFEAREA_MAX_Z)
	{
		g_player.bIntoSafeArea = true;
	}
	else
	{
		g_player.bIntoSafeArea = false;
	}

	//GetMouseClickTrigger
	if (GetMouseClickRepeate(MOUSE_CLICK_LEFT) == true)
	{
		SetBullet(g_player.pos + D3DXVECTOR3(0.0f, 40.0f, 0.0f) , BULLET_SPEED, g_player.rot.y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//モーション設定
	UpdateMotion();
	
	g_player.mMotion.mCounterMotion++;
	if (g_player.mMotion.mCounterMotion >= g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].mFrame)
	{
		g_player.mMotion.mKey++;
		g_player.mMotion.mCounterMotion = 0;
	}

	if (g_player.mMotion.mKey >= g_player.mMotion.mNumKey)
	{
		if (g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].mLoop == true)
		{
			g_player.mMotion.mKey = 0;
		}
		else
		{
			SetMotion(MOTIONTYPE_NEUTRAL);
		}
	}

	//影位置設定
	SetPositionShadow(g_nIdxShadow, g_player.pos);
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

	//"プレイヤーの"ワールドマトリックス初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//"プレイヤーの"ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < EXITHUMAN_MODEL_NUM; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
		D3DXMATRIX mtxParent;					//親のマトリ

		//"モデルの"ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		//位置反映
		D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		//パーツの親マトリ設定
		if (g_player.aModel[nCntModel].mIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].mIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld;
		}

		//パーツのマトリと親マトリをかけ合わせる
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxParent);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

		//マテリアルデータへのポインタ取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].dwNumMatModel; nCntMat++)
		{
			//プレイヤーは青
			//マテリアル設定
			D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;
			//ダメージ状態なら赤追加
			changeMat.Diffuse = D3DXCOLOR(0.0f, 0.45f, 0.74f, 1.0f);

			//マテリアル設定
			pDevice->SetMaterial(&changeMat);

			//テクスチャ設定
			pDevice->SetTexture(0, g_player.aModel[nCntModel].apTexture[nCntMat]);

			//モデル描画
			g_player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//壁当たり判定処理
//========================
void CollisionWallPlayer(int nNumber)
{
	Wall *wall = GetWall();
	D3DXVECTOR3 pos0, pos1;
	D3DXVECTOR3 vecMove, vecLine;
	D3DXVECTOR3 vecToPos, vecToPosOld;
	float fAreaA, fAreaB;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, wall++)
	{
		//使っているときだけ処理
		if (wall->bUse == true)
		{
			//移動ベクトル
			vecMove = g_player.pos - g_player.posOld;

			//位置計算
			pos0.x = wall->pos.x - (wall->fWidth / 2) * cosf(wall->rot.y);
			pos0.y = 0.0f;
			pos0.z = wall->pos.z + (wall->fWidth / 2) * sinf(wall->rot.y);

			pos1.x = wall->pos.x + (wall->fWidth / 2) * cosf(wall->rot.y);
			pos1.y = 0.0f;
			pos1.z = wall->pos.z - (wall->fWidth / 2) * sinf(wall->rot.y);

			vecLine = pos1 - pos0;	//境界線ベクトル
			vecToPos = g_player.pos - pos0;
			vecToPosOld = g_player.posOld - pos0;

			//面積求める
			fAreaA = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
			fAreaB = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);

			if ((vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z) >= 0.0f && (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0.0f)
			{
				if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
				{//ごっつん
					float fRate = fAreaA / fAreaB;
					g_player.pos.x = pos0.x + (vecLine.x * fRate) - sinf(wall->rot.y) / D3DX_PI * PUSHING_BACK;
					g_player.pos.z = pos0.z + (vecLine.z * fRate) - cosf(wall->rot.y) / D3DX_PI * PUSHING_BACK;
					break;
				}
			}
		}
	}
}

//========================
//オブジェクト当たり判定処理
//========================
void CollisionObjPlayer(int nNumber)
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

	BluePrint *pbprint = GetBluePrint();
	Object *pObject = GetObj();
	D3DXVECTOR3 pos0, pos1, pos2, pos3;
	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMove;
	float fAreaARight, fAreaALeft, fAreaBRight, fAreaBLeft;
	float fAreaAUp, fAreaADown, fAreaBUp, fAreaBDown;


	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++, pObject++)
	{
		if (pObject->bUse == true)
		{
			//各頂点求める
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = pObject->pos.x - (pObject->pos.x + (pbprint + pObject->bpidx)->vtxMin.x);
			fLengthZ = pObject->pos.z - (pObject->pos.z + (pbprint + pObject->bpidx)->vtxMin.z);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
			//0 - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - pObject->rot.y);

			//角度に応じて頂点の位置をずらす
			pos0.x = pObject->pos.x + sinf(rot) * fLength;
			pos0.y = 0.0f;
			pos0.z = pObject->pos.z - cosf(rot) * fLength;
			//-pos0---------------------------------------------------------------------------------------------------------------------------

			//-pos1---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = pObject->pos.x - (pObject->pos.x + (pbprint + pObject->bpidx)->vtxMax.x);
			fLengthZ = pObject->pos.z - (pObject->pos.z + (pbprint + pObject->bpidx)->vtxMin.z);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
			//0 + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - pObject->rot.y);
			
			//角度に応じて頂点の位置をずらす
			pos1.x = pObject->pos.x + sinf(rot) * fLength;
			pos1.y = 0.0f;
			pos1.z = pObject->pos.z - cosf(rot) * fLength;
			//-pos1---------------------------------------------------------------------------------------------------------------------------

			//-pos2---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = pObject->pos.x - (pObject->pos.x + (pbprint + pObject->bpidx)->vtxMax.x);
			fLengthZ = pObject->pos.z - (pObject->pos.z + (pbprint + pObject->bpidx)->vtxMax.z);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
			//PI - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(D3DX_PI - fAngle - pObject->rot.y);
			
			//角度に応じて頂点の位置をずらす
			pos2.x = pObject->pos.x - sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = pObject->pos.z + cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//-pos3---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = pObject->pos.x - (pObject->pos.x + (pbprint + pObject->bpidx)->vtxMin.x);
			fLengthZ = pObject->pos.z - (pObject->pos.z + (pbprint + pObject->bpidx)->vtxMax.z);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
			//-PI + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-D3DX_PI - fAngle - pObject->rot.y);
			
			//角度に応じて頂点の位置をずらす
			pos3.x = pObject->pos.x - sinf(rot) * fLength;
			pos3.y = 0.0f;
			pos3.z = pObject->pos.z + cosf(rot) * fLength;
			//-pos3---------------------------------------------------------------------------------------------------------------------------

			//ベクトル求める
			//move
			vecMove = g_player.pos - g_player.posOld;

			//X
			//右方向の計算
			vecLineRight = pos1 - pos0;
			vecToPosRight = g_player.pos - pos0;
			vecToPosOldRight = g_player.posOld - pos0;

			//左方向の計算
			vecLineLeft = pos3 - pos2;
			vecToPosLeft = g_player.pos - pos2;
			vecToPosOldLeft = g_player.posOld - pos2;

			//Z
			//上方向の計算
			vecLineUp = pos2 - pos1;
			vecToPosUp = g_player.pos - pos1;
			vecToPosOldUp = g_player.posOld - pos1;
			//下方向の計算
			vecLineDown = pos0 - pos3;
			vecToPosDown = g_player.pos - pos3;
			vecToPosOldDown = g_player.posOld - pos3;

			//当たり判定本番
			//X
			//面積求める
			fAreaARight = (vecToPosRight.z * vecMove.x) - (vecToPosRight.x * vecMove.z);
			fAreaALeft = (vecToPosLeft.z * vecMove.x) - (vecToPosLeft.x * vecMove.z);
			fAreaBRight = (vecLineRight.z * vecMove.x) - (vecLineRight.x * vecMove.z);
			fAreaBLeft = (vecLineLeft.z * vecMove.x) - (vecLineLeft.x * vecMove.z);

			//左側AND範囲内vecToPosOldOps
			if ((vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f && (vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) < 0.0f)
			{
				if (fAreaARight / fAreaBRight >= 0.0f && fAreaARight / fAreaBRight <= 1.0f)
				{//ごっつん
					float fRate = fAreaARight / fAreaBRight;
					g_player.pos.x = pos0.x + (vecLineRight.x * fRate) - sinf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					g_player.pos.z = pos0.z + (vecLineRight.z * fRate) - cosf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					break;
				}
			}
			else if ((vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f && (vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) < 0.0f)
			{
				if (fAreaALeft / fAreaBLeft >= 0.0f && fAreaALeft / fAreaBLeft <= 1.0f)
				{//ごっつん
					float fRate = fAreaALeft / fAreaBLeft;
					g_player.pos.x = pos2.x + (vecLineLeft.x * fRate) + sinf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					g_player.pos.z = pos2.z + (vecLineLeft.z * fRate) + cosf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					break;
				}
			}

			//Z
			//面積求める
			fAreaAUp = (vecToPosUp.z * vecMove.x) - (vecToPosUp.x * vecMove.z);
			fAreaADown = (vecToPosDown.z * vecMove.x) - (vecToPosDown.x * vecMove.z);
			fAreaBUp = (vecLineUp.z * vecMove.x) - (vecLineUp.x * vecMove.z);
			fAreaBDown = (vecLineDown.z * vecMove.x) - (vecLineDown.x * vecMove.z);

			//左側AND範囲内vecToPosOldOps
			if ((vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f && (vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) < 0.0f)
			{
				if (fAreaAUp / fAreaBUp >= 0.0f && fAreaAUp / fAreaBUp <= 1.0f)
				{//ごっつん
					float fRate = fAreaAUp / fAreaBUp;
					g_player.pos.x = pos1.x + (vecLineUp.x * fRate) + cosf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					g_player.pos.z = pos1.z + (vecLineUp.z * fRate) - sinf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					break;
				}
			}
			else if ((vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f && (vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) < 0.0f)
			{
				if (fAreaADown / fAreaBDown >= 0.0f && fAreaADown / fAreaBDown <= 1.0f)
				{//ごっつん
					float fRate = fAreaADown / fAreaBDown;
					g_player.pos.x = pos3.x + (vecLineDown.x * fRate) - cosf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					g_player.pos.z = pos3.z + (vecLineDown.z * fRate) + sinf(pObject->rot.y) / D3DX_PI * PUSHING_BACK;
					break;
				}
			}
		}
	}
}

//========================
//敵当たり判定処理
//========================
void CollisionEnemyPlayer(int nNumber)
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

	BluePrint *pbprint = GetBluePrint();
	Enemy *pEnemy = GetEnemy();
	D3DXVECTOR3 pos0, pos1, pos2, pos3;
	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMove;
	float fAreaARight, fAreaALeft, fAreaBRight, fAreaBLeft;
	float fAreaAUp, fAreaADown, fAreaBUp, fAreaBDown;


	for (int nCntObj = 0; nCntObj < MAX_ENEMY; nCntObj++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			//各頂点求める
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = -EXITHUMAN_WIDTH / 2;
			fLengthZ = -EXITHUMAN_HEIGHT / 2;

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - pEnemy->rot.y);

			//角度に応じて頂点の位置をずらす
			pos0.x = pEnemy->pos.x + sinf(rot) * fLength;
			pos0.y = 0.0f;
			pos0.z = pEnemy->pos.z - cosf(rot) * fLength;
			//-pos0---------------------------------------------------------------------------------------------------------------------------

			//-pos1---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = EXITHUMAN_WIDTH / 2;
			fLengthZ = -EXITHUMAN_HEIGHT / 2;

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//0 + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-fAngle - pEnemy->rot.y);

			//角度に応じて頂点の位置をずらす
			pos1.x = pEnemy->pos.x + sinf(rot) * fLength;
			pos1.y = 0.0f;
			pos1.z = pEnemy->pos.z - cosf(rot) * fLength;
			//-pos1---------------------------------------------------------------------------------------------------------------------------

			//-pos2---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = -EXITHUMAN_WIDTH / 2;
			fLengthZ = EXITHUMAN_HEIGHT / 2;

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//PI - 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(D3DX_PI - fAngle - pEnemy->rot.y);

			//角度に応じて頂点の位置をずらす
			pos2.x = pEnemy->pos.x - sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = pEnemy->pos.z + cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//-pos3---------------------------------------------------------------------------------------------------------------------------
			//頂点と中心の距離をXとZ別々で計算する
			fLengthX = EXITHUMAN_WIDTH / 2;
			fLengthZ = EXITHUMAN_HEIGHT / 2;

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//頂点と中心の距離を求める
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//頂点と中心の角度を求める
																	//-PI + 計算で出した角度 + オブジェクトの角度を -PI ~ PIに修正
			rot = FIX_ROT(-D3DX_PI - fAngle - pEnemy->rot.y);

			//角度に応じて頂点の位置をずらす
			pos3.x = pEnemy->pos.x - sinf(rot) * fLength;
			pos3.y = 0.0f;
			pos3.z = pEnemy->pos.z + cosf(rot) * fLength;
			//-pos3---------------------------------------------------------------------------------------------------------------------------

			//ベクトル求める
			//move
			vecMove = g_player.pos - g_player.posOld;

			//X
			//右方向の計算
			vecLineRight = pos1 - pos0;
			vecToPosRight = g_player.pos - pos0;
			vecToPosOldRight = g_player.posOld - pos0;

			//左方向の計算
			vecLineLeft = pos3 - pos2;
			vecToPosLeft = g_player.pos - pos2;
			vecToPosOldLeft = g_player.posOld - pos2;

			//Z
			//上方向の計算
			vecLineUp = pos2 - pos1;
			vecToPosUp = g_player.pos - pos1;
			vecToPosOldUp = g_player.posOld - pos1;
			//下方向の計算
			vecLineDown = pos0 - pos3;
			vecToPosDown = g_player.pos - pos3;
			vecToPosOldDown = g_player.posOld - pos3;

			//当たり判定本番
			//X
			//面積求める
			fAreaARight = (vecToPosRight.z * vecMove.x) - (vecToPosRight.x * vecMove.z);
			fAreaALeft = (vecToPosLeft.z * vecMove.x) - (vecToPosLeft.x * vecMove.z);
			fAreaBRight = (vecLineRight.z * vecMove.x) - (vecLineRight.x * vecMove.z);
			fAreaBLeft = (vecLineLeft.z * vecMove.x) - (vecLineLeft.x * vecMove.z);

			//左側AND範囲内vecToPosOldOps
			if ((vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f && (vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) < 0.0f)
			{
				if (fAreaARight / fAreaBRight >= 0.0f && fAreaARight / fAreaBRight <= 1.0f)
				{//ごっつん
					g_player.bInfection = true;
					break;
				}
			}
			else if ((vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f && (vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) < 0.0f)
			{
				if (fAreaALeft / fAreaBLeft >= 0.0f && fAreaALeft / fAreaBLeft <= 1.0f)
				{//ごっつん
					g_player.bInfection = true;
					break;
				}
			}

			//Z
			//面積求める
			fAreaAUp = (vecToPosUp.z * vecMove.x) - (vecToPosUp.x * vecMove.z);
			fAreaADown = (vecToPosDown.z * vecMove.x) - (vecToPosDown.x * vecMove.z);
			fAreaBUp = (vecLineUp.z * vecMove.x) - (vecLineUp.x * vecMove.z);
			fAreaBDown = (vecLineDown.z * vecMove.x) - (vecLineDown.x * vecMove.z);

			//左側AND範囲内vecToPosOldOps
			if ((vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f && (vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) < 0.0f)
			{
				if (fAreaAUp / fAreaBUp >= 0.0f && fAreaAUp / fAreaBUp <= 1.0f)
				{//ごっつん
					g_player.bInfection = true;
					break;
				}
			}
			else if ((vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f && (vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) < 0.0f)
			{
				if (fAreaADown / fAreaBDown >= 0.0f && fAreaADown / fAreaBDown <= 1.0f)
				{//ごっつん
					g_player.bInfection = true;
					break;
				}
			}
		}
	}
}
//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_player;
}

//========================
//モーション設定処理
//========================
void SetMotion(MOTIONTYPE type)
{
	g_player.mMotion.motionType = type;
	g_player.mMotion.mLoopMotion = g_player.mMotion.aMotionInfo[type].mLoop;
	g_player.mMotion.mNumKey = g_player.mMotion.aMotionInfo[type].mNumKey;
	g_player.mMotion.mKey = 0;
	g_player.mMotion.mCounterMotion = 0;
}

//========================
//モーション更新処理
//========================
void UpdateMotion(void)
{
	for (int CntModel = 0; CntModel < EXITHUMAN_MODEL_NUM; CntModel++)
	{
		//差分算出
		int nextKey = (g_player.mMotion.mKey + 1) % g_player.mMotion.mNumKey;

		float posDiffX = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[nextKey].aKey[CntModel].mPosX -
			g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mPosX;
		float posDiffY = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[nextKey].aKey[CntModel].mPosY -
			g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mPosY;
		float posDiffZ = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[nextKey].aKey[CntModel].mPosZ -
			g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mPosZ;
		float rotDiffX = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[nextKey].aKey[CntModel].mRotX -
			g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mRotX;
		float rotDiffY = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[nextKey].aKey[CntModel].mRotY -
			g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mRotY;
		float rotDiffZ = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[nextKey].aKey[CntModel].mRotZ -
			g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mRotZ;

		//位置向き算出
		float posDemandX = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mPosX +
			posDiffX * ((float)g_player.mMotion.mCounterMotion / g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].mFrame);
		float posDemandY = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mPosY +
			posDiffY * ((float)g_player.mMotion.mCounterMotion / g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].mFrame);
		float posDemandZ = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mPosZ +
			posDiffZ * ((float)g_player.mMotion.mCounterMotion / g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].mFrame);
		float rotDemandX = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mRotX +
			rotDiffX * ((float)g_player.mMotion.mCounterMotion / g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].mFrame);
		float rotDemandY = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mRotY +
			rotDiffY * ((float)g_player.mMotion.mCounterMotion / g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].mFrame);
		float rotDemandZ = g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].aKey[CntModel].mRotZ +
			rotDiffZ * ((float)g_player.mMotion.mCounterMotion / g_player.mMotion.aMotionInfo[g_player.mMotion.motionType].aKeyInfo[g_player.mMotion.mKey].mFrame);

		//パーツの位置向き設定
		g_player.aModel[CntModel].pos = g_player.aModel[CntModel].posOffset + D3DXVECTOR3(posDemandX, posDemandY, posDemandZ);
		g_player.aModel[CntModel].rot = g_player.aModel[CntModel].rotOffset + D3DXVECTOR3(rotDemandX, rotDemandY, rotDemandZ);
	}
}
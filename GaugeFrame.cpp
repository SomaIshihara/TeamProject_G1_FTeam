//==========================================
//
//プレイヤー関係プログラム[player.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "input.h"
#include "game.h"
#include "player.h"
#include "block.h"
#include "camera.h"
#include "file.h"
#include "spot.h"
#include "file.h"
#include "result.h"
#include "sound.h"
#include "vcursor.h"

//マクロ
#define GROUND_HEIGHT			(600.0f)			//地面
#define PLAYER_JUMP_HEIGHT		(15.7f)				//ジャンプ高さ
#define PLAYERTEX_SIZE_WIDTH	(264)
#define PLAYERTEX_SIZE_HEIGHT	(268)
#define PLAYERTEX_PATT_WIDTH	(4)
#define PLAYERTEX_PATT_HEIGHT	(2)
#define PLAYER_MOVE_SPEED		(5.5f)				//プレイヤーの移動速度
#define CURSOR_MOVE_SPEED		(10.0f)				//カーソルの移動速度
#define ANIM_SPEED				(7)					//アニメーションスピード
#define CONT_NUM				(2)					//対応コントローラー数
#define MODE_TEXNUM				(CONT_NUM * 2)		//モード表示のテクスチャ数
#define MODE_SIZE_WIDTH			(48)
#define MODE_SIZE_HEIGHT		(48)

//プロト
bool CollisionCursor(void);

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPlayer;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer;				//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffCursor;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureCursor[CONT_NUM];		//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffMode;				//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureMode[MODE_TEXNUM];		//テクスチャポインタ
Player g_Player;
LPD3DXFONT g_pFontPlayer = NULL;					//フォントへのポインタ
bool g_bNowBuilding;


//カーソルファイルパス
const char *c_apFileNameCursor[CONT_NUM] =
{
	"data\\TEXTURE\\Block_Cursor_01.png",
	"data\\TEXTURE\\Block_Cursor_02.png"
};

//モード表示テクスチャパス
const char *c_apFileNameMode[MODE_TEXNUM] =
{
	"data\\TEXTURE\\ChangeMode_01.png",
	"data\\TEXTURE\\ChangeMode_02.png",
	"data\\TEXTURE\\ChangeMode_11.png",
	"data\\TEXTURE\\ChangeMode_12.png"
};

//========================
//プレイヤー初期化処理
//========================
void InitPlayer(void)
{
	int nCount;
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	//プレイヤー
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\VA_Chara_Dot.png",
		&g_pTexturePlayer);

	//カーソル
	for (nCount = 0; nCount < sizeof c_apFileNameCursor / sizeof(char *); nCount++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFileNameCursor[nCount],
			&g_pTextureCursor[nCount]);
	}

	for (nCount = 0; nCount < sizeof c_apFileNameMode / sizeof(char *); nCount++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFileNameMode[nCount],
			&g_pTextureMode[nCount]);
	}

	//デバッグ表示用フォント生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFontPlayer);

	//頂点バッファの生成
	//プレイヤー
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffPlayer,
		NULL);

	//カーソル
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffCursor,
		NULL);

	//モード
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffMode,
		NULL);

	//構造体初期化
	g_Player.pos = RefSpot(SPOTTYPE_START, 0)->pos;
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.nDirectionMove = 0;
	g_Player.nCounterJumpTime = 0;
	g_Player.bJump = false;
	g_Player.nHaveBlock = 0;
	g_Player.bDisp = true;

	//変数初期化
	g_bNowBuilding = false;

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x - PLAYER_SIZE_WIDTH / 2;
	pVtx[0].pos.y = g_Player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + PLAYER_SIZE_WIDTH / 2;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x - PLAYER_SIZE_WIDTH / 2;
	pVtx[2].pos.y = g_Player.pos.y;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + PLAYER_SIZE_WIDTH / 2;
	pVtx[3].pos.y = g_Player.pos.y;
	pVtx[3].pos.z = 0.0f;

	//座標変換用係数設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//頂点バッファをアンロック
	g_pVtxbuffPlayer->Unlock();


	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffCursor->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = 300.0f - BLOCK_WIDTH / 2;
	pVtx[0].pos.y = 300.0f - BLOCK_HEIGHT / 2;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = 300.0f + BLOCK_WIDTH / 2;
	pVtx[1].pos.y = 300.0f - BLOCK_HEIGHT / 2;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = 300.0f - BLOCK_WIDTH / 2;
	pVtx[2].pos.y = 300.0f + BLOCK_HEIGHT / 2;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = 300.0f + BLOCK_WIDTH / 2;
	pVtx[3].pos.y = 300.0f + BLOCK_HEIGHT / 2;
	pVtx[3].pos.z = 0.0f;

	//座標変換用係数設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxbuffCursor->Unlock();


	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffMode->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(1210.0f, 16.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1210.0f + MODE_SIZE_WIDTH, 16.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(1210.0f, 16.0f + MODE_SIZE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1210.0f + MODE_SIZE_WIDTH, 16.0f + MODE_SIZE_HEIGHT, 0.0f);

	//座標変換用係数設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxbuffMode->Unlock();
}

//========================
//プレイヤー終了処理
//========================
void UninitPlayer(void)
{
	int nCount;	//カウンタ

	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	for (nCount = 0; nCount < CONT_NUM; nCount++)
	{
		if (g_pTextureCursor[nCount] != NULL)
		{
			g_pTextureCursor[nCount]->Release();
			g_pTextureCursor[nCount] = NULL;
		}
	}
	for (nCount = 0; nCount < MODE_TEXNUM; nCount++)
	{
		if (g_pTextureMode[nCount] != NULL)
		{
			g_pTextureMode[nCount]->Release();
			g_pTextureMode[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffPlayer != NULL)
	{
		g_pVtxbuffPlayer->Release();
		g_pVtxbuffPlayer = NULL;
	}
	if (g_pVtxbuffCursor != NULL)
	{
		g_pVtxbuffCursor->Release();
		g_pVtxbuffCursor = NULL;
	}
	if (g_pVtxbuffMode != NULL)
	{
		g_pVtxbuffMode->Release();
		g_pVtxbuffMode = NULL;
	}
}

//========================
//プレイヤー更新処理
//========================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;	//設定用ポインタ
	D3DXVECTOR3 moveTemp;

	//処理前の位置を保存
	g_Player.posOld = g_Player.pos;

	//ジャンプカウンタ加算
	g_Player.nCounterJumpTime++;

	//操作（マウス）
	if (GetMouseClickTrigger(MOUSE_CLICK_RIGHT) == true)
	{
		g_bNowBuilding = g_bNowBuilding ? false : true;
		//PlaySound(SOUND_LABEL_SE_CHANGEMODE);
	}
	else if (GetUseGamepad() == true && GetGamepadTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER) == true)
	{
		g_bNowBuilding = g_bNowBuilding ? false : true;
		//PlaySound(SOUND_LABEL_SE_CHANGEMODE);

		//プレイヤーの位置にカーソルを置く
		*GetpCursorPos() = g_Player.pos + D3DXVECTOR3(-GetCameraPos().x, -PLAYER_SIZE_HEIGHT / 2, 0.0f);
	}

	//配置モードのときは仮想カーソルを消す（なお画面内にいる必要がある）
	if (GetMousePos().x > 0.0f && GetMousePos().x < SCREEN_WIDTH && GetMousePos().y > 0.0f && GetMousePos().y < SCREEN_HEIGHT)
	{
		SetDispVCursor(!g_bNowBuilding);
	}

	//キーボード+マウスorゲームパッド識別
	if (GetUseGamepad() == true)
	{
		//操作（ゲームパッド）
		if (!g_bNowBuilding)
		{//ゲームパッドつながってる
			if (GetLStickX() > 0)
			{//Lスティックが右
				g_Player.move.x = ((float)GetLStickX() / STICK_MAX) * PLAYER_MOVE_SPEED;
				g_Player.nDirectionMove = 0;
				if (!g_Player.bJump)
				{
					g_Player.nCounterAnim++;
				}
			}
			else if (GetLStickX() < 0)
			{//Lスティックが左
				g_Player.move.x = ((float)GetLStickX() / STICK_MAX) * PLAYER_MOVE_SPEED;
				g_Player.nDirectionMove = 1;
				if (!g_Player.bJump)
				{
					g_Player.nCounterAnim++;
				}
			}
			else
			{
				g_Player.move.x = 0.0f;

				if (!g_Player.bJump)
				{//ジャンプしてない
					if (g_Player.nPatternAnim % 2 == 1)
					{
						g_Player.nCounterAnim++;
					}
				}
			}
		}
		else
		{
			//移動一時停止
			g_Player.move.x = 0.0f;

			//左右
			if (GetLStickX() > 0 || GetLStickX() < 0)
			{//Lスティックが傾いてる
				GetpCursorPos()->x += ((float)GetLStickX() / STICK_MAX) * CURSOR_MOVE_SPEED;
			}

			//上下
			if (GetLStickY() > 0 || GetLStickY() < 0)
			{//Lスティックが傾いてる
				GetpCursorPos()->y -= ((float)GetLStickY() / STICK_MAX) * CURSOR_MOVE_SPEED;
			}

			//操作
			if (GetGamepadTrigger(XINPUT_GAMEPAD_X) == true)
			{
				if (BlockCheck(true) == false && CollisionCursor() == false)
				{
					SetBlock(*GetpCursorPos(), BLOCKTYPE_V_NORMAL, PLACEBLOCK_PLAYER);
				}
			}
		}
	}
	else
	{
		//配置中でなければ移動可能
		if (!g_bNowBuilding)
		{
			//操作（キーボード）
			if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFTARROW) == true)
			{//左
				g_Player.move.x = -PLAYER_MOVE_SPEED;
				g_Player.nDirectionMove = 1;
				if (!g_Player.bJump)
				{
					g_Player.nCounterAnim++;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHTARROW) == true)
			{//右
				g_Player.move.x = PLAYER_MOVE_SPEED;
				g_Player.nDirectionMove = 0;
				if (!g_Player.bJump)
				{
					g_Player.nCounterAnim++;
				}
			}
			else
			{//静止
				g_Player.move.x = 0.0f;

				if (!g_Player.bJump)
				{//ジャンプしてない
					if (g_Player.nPatternAnim % 2 == 1)
					{
						g_Player.nCounterAnim++;
					}
				}
			}
		}
		else
		{
			//移動一時停止
			g_Player.move.x = 0.0f;

			//操作（マウス）
			if (GetMouseClickTrigger(MOUSE_CLICK_LEFT) == true)
			{
				if (BlockCheck(true) == false && CollisionCursor() == false)
				{
					SetBlock(*GetpCursorPos(), BLOCKTYPE_V_NORMAL, PLACEBLOCK_PLAYER);
				}
			}
		}
	}

	//アニメーション補正
	if(g_Player.bJump)
	{//ジャンプしてる
		if (g_Player.nPatternAnim % 2 == 0)
		{
			g_Player.nCounterAnim++;
		}
	}
	
	//アニメーション
	if (g_Player.nCounterAnim >= ANIM_SPEED)
	{
		g_Player.nCounterAnim = 0;
		g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 4;
	}

	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y -= g_Player.move.y - (ACCELERATION_GRAVITY * g_Player.nCounterJumpTime / 60);

	//ブロック当たり判定
	if (CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT, &g_Player.nCounterJumpTime) == true)
	{
		g_Player.bJump = false;

		//ジャンプするならする
		if (!g_bNowBuilding)
		{
			if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.bJump == false)
			{
				g_Player.bJump = true;
				g_Player.move.y = PLAYER_JUMP_HEIGHT;
				g_Player.nCounterJumpTime = 0;
				PlaySound(SOUND_LABEL_SE_JUMP);
			}
			else if (GetUseGamepad() == true && GetGamepadTrigger(XINPUT_GAMEPAD_A) == true && g_Player.bJump == false)
			{
				g_Player.bJump = true;
				g_Player.move.y = PLAYER_JUMP_HEIGHT;
				g_Player.nCounterJumpTime = 0;
				PlaySound(SOUND_LABEL_SE_JUMP);
			}
		}
	}

	//左端はぶつかる
	if (g_Player.pos.x < PLAYER_SIZE_WIDTH / 2)
	{
		g_Player.pos.x = PLAYER_SIZE_WIDTH / 2;
	}
	else if (g_Player.pos.x > GetMapData()->blockData.nMapWidth * BLOCK_WIDTH - PLAYER_SIZE_WIDTH / 2)
	{
		g_Player.pos.x = (float)GetMapData()->blockData.nMapWidth * BLOCK_WIDTH - PLAYER_SIZE_WIDTH / 2;
	}

	//仮:穴に落ちたらリスポ
	if (g_Player.pos.y >= 800.0f)
	{
#if 0
		g_Player.pos = RefSpot(SPOTTYPE_START, 0)->pos;
		g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.nDirectionMove = 0;
		g_Player.nCounterJumpTime = 0;
#endif
		g_Player.bDisp = false;
		SetResult(RESULT_OVER);
		SetGameState(GAMESTATE_END, 0);
		return;
	}

	//プレイヤー
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x - PLAYER_SIZE_WIDTH / 2 - GetCameraPos().x;
	pVtx[1].pos.x = g_Player.pos.x + PLAYER_SIZE_WIDTH / 2 - GetCameraPos().x;
	pVtx[2].pos.x = g_Player.pos.x - PLAYER_SIZE_WIDTH / 2 - GetCameraPos().x;
	pVtx[3].pos.x = g_Player.pos.x + PLAYER_SIZE_WIDTH / 2 - GetCameraPos().x;

	pVtx[0].pos.y = g_Player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[1].pos.y = g_Player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[2].pos.y = g_Player.pos.y;
	pVtx[3].pos.y = g_Player.pos.y;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim % 4), 0.5f * g_Player.nDirectionMove);
	pVtx[1].tex = D3DXVECTOR2(0.25f * ((g_Player.nPatternAnim % 4) + 1), 0.5f * g_Player.nDirectionMove);
	pVtx[2].tex = D3DXVECTOR2(0.25f * (g_Player.nPatternAnim % 4), 0.5f * (g_Player.nDirectionMove + 1));
	pVtx[3].tex = D3DXVECTOR2(0.25f * ((g_Player.nPatternAnim % 4) + 1), 0.5f * (g_Player.nDirectionMove + 1));

	//頂点バッファをアンロック
	g_pVtxbuffPlayer->Unlock();


	//カーソル
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffCursor->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = GetpCursorPos()->x - BLOCK_WIDTH / 2;
	pVtx[1].pos.x = GetpCursorPos()->x + BLOCK_WIDTH / 2;
	pVtx[2].pos.x = GetpCursorPos()->x - BLOCK_WIDTH / 2;
	pVtx[3].pos.x = GetpCursorPos()->x + BLOCK_WIDTH / 2;

	pVtx[0].pos.y = GetpCursorPos()->y - BLOCK_HEIGHT / 2;
	pVtx[1].pos.y = GetpCursorPos()->y - BLOCK_HEIGHT / 2;
	pVtx[2].pos.y = GetpCursorPos()->y + BLOCK_HEIGHT / 2;
	pVtx[3].pos.y = GetpCursorPos()->y + BLOCK_HEIGHT / 2;

	if (g_Player.nHaveBlock < 1 || CollisionCursor() == true || BlockCheck(false) == true)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f);
	}
	else
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	//頂点バッファをアンロック
	g_pVtxbuffCursor->Unlock();
}

//========================
//プレイヤー描画処理
//========================
void DrawPlayer(void)
{
	int nDispTex = 0;
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bDisp)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	if (g_bNowBuilding)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxbuffCursor, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャ設定
		if (GetUseGamepad() == true)
		{
			pDevice->SetTexture(0, g_pTextureCursor[1]);
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureCursor[0]);
		}

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		nDispTex++;
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffMode, 0, sizeof(VERTEX_2D));

	if (GetUseGamepad() == false)
	{
		nDispTex += 2;
	}

	pDevice->SetTexture(0, g_pTextureMode[nDispTex]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//プレイヤー情報取得
//========================
Player *GetPlayer(void)
{
	return &g_Player;
}

//========================
//プレイヤー位置表示
//========================
void DrawPos(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "\nPos:x = %d , y = %d", (int)g_Player.pos.x, (int)g_Player.pos.y);

	//テキスト描画
	g_pFontPlayer->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================
//プレイヤー移動量表示
//========================
void DrawSpeed(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "\n\nMove:x = %d , y = %d (t = %d)", (int)g_Player.move.x, 
		(int)(g_Player.move.y - (ACCELERATION_GRAVITY * g_Player.nCounterJumpTime / 60)), g_Player.nCounterJumpTime);

	//テキスト描画
	g_pFontPlayer->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================
//カーソルのプレイヤー当たり判定
//========================
bool CollisionCursor(void)
{
	if (GetpCursorPos()->x > g_Player.pos.x - PLAYER_SIZE_WIDTH / 2 - BLOCK_WIDTH / 2 - GetCameraPos().x &&
		GetpCursorPos()->x < g_Player.pos.x + PLAYER_SIZE_WIDTH / 2 + BLOCK_WIDTH / 2 - GetCameraPos().x)
	{
		if (GetpCursorPos()->y > g_Player.pos.y - PLAYER_SIZE_HEIGHT - BLOCK_HEIGHT / 2 &&
			GetpCursorPos()->y < g_Player.pos.y + BLOCK_HEIGHT / 2)
		{
			return true;
		}
	}
	return false;
}
//==========================================
//
//背景表示プログラム[bg.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "bg.h"
#include "player.h"

//マクロ
#define NUM_BG		(7)		//背景の数
#define TEX_USPEED	(0.25f)	//テクスチャスピード
#define TEX_WIDTH	(1.0f)	//背景の幅の使う座標
#define TEX_HEIGHT	(0.5f)	//背景の高さが使う座標
#define TEXU_SLIDE	(0.12f)	//U座標ずらすやつ

//ステージテクスチャパス（2TAは1～4のどれか）
#define BG_TEXTURE_STG1_PATH	"data\\TEXTURE\\BG_GrassLands_01.png"	//ステージ1のテクスチャパス
#define BG_TEXTURE_STG2_PATH	"data\\TEXTURE\\BG_GrassLands_02.png"	//ステージ2のテクスチャパス
#define BG_TEXTURE_STG3_PATH	"data\\TEXTURE\\BG_City_01.png"	//ステージ3のテクスチャパス
#define BG_TEXTURE_STG4_PATH	"data\\TEXTURE\\BG_City_02.png"	//ステージ4のテクスチャパス
#define BG_TEXTURE_STGLAST_PATH	""	//ステージLASTのテクスチャパス（MEMPhase1）
#define BG_TEXTURE_STGMEM2_PATH	""	//ステージMEMのテクスチャパス（Phase2）
#define BG_TEXTURE_STGMEM3_PATH	""	//ステージMEMのテクスチャパス（Phase3）


LPDIRECT3DTEXTURE9 g_apTextureBg[NUM_BG] = {};	//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBg = NULL;	//頂点バッファポインタ
int g_nBGSpeed;							//BGの流れる速度
float g_aTexU;					//テクスチャ座標の開始位置(U
float g_aTexV;					//テクスチャ座標の開始位置(V
float g_fTexVSpeed;						//V値移動の速度
LPD3DXFONT g_pFontBG = NULL;			//フォントへのポインタ

//========================
//背景初期化処理
//========================
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//背景読み込み
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURE_STG1_PATH,
		&g_apTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURE_STG2_PATH,
		&g_apTextureBg[1]);
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURE_STG3_PATH,
		&g_apTextureBg[2]);
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURE_STG4_PATH,
		&g_apTextureBg[3]);

	//テクスチャUV値の初期化
	g_aTexU = 0.0f;
	g_aTexV = 0.0f;

	//デバッグ表示用フォント生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFontBG);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBg,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	//座標変換用係数設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(g_aTexU, g_aTexV);
	pVtx[1].tex = D3DXVECTOR2(g_aTexU + TEX_WIDTH, g_aTexV);
	pVtx[2].tex = D3DXVECTOR2(g_aTexU, g_aTexV + TEX_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2(g_aTexU + TEX_WIDTH, g_aTexV + TEX_HEIGHT);

	//頂点バッファをアンロック
	g_pVtxbuffBg->Unlock();

	//BG速度初期化（x1.00）
	g_nBGSpeed = 100;
}

//========================
//背景終了処理
//========================
void UninitBg(void)
{
	int nCntTexBG;	//カウンタ

	//テクスチャの破棄
	for (nCntTexBG = 0; nCntTexBG < NUM_BG; nCntTexBG++)
	{
		if (g_apTextureBg[nCntTexBG] != NULL)
		{
			g_apTextureBg[nCntTexBG]->Release();
			g_apTextureBg[nCntTexBG] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffBg != NULL)
	{
		g_pVtxbuffBg->Release();
		g_pVtxbuffBg = NULL;
	}
}

//========================
//背景更新処理
//========================
void UpdateBg(void)
{
	if (GetGameState() != GAMESTATE_END)
	{
		VERTEX_2D *pVtx;					//設定用ポインタ
		Player *playerData = GetPlayer();	//プレイヤー情報のあるポインタ	

#if 1
		//テクスチャU値更新
		g_aTexU = (playerData->pos.x * TEX_USPEED / SCREEN_WIDTH) * TEX_WIDTH - TEXU_SLIDE;
#endif
		//テクスチャV値更新
		g_aTexV = (float)fmod(g_aTexV + 1.0f - BG_TEX_SPEED * g_nBGSpeed * 0.01f, 1.0f);

		//頂点バッファのロックと頂点情報へのポインタを取得
		g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_aTexU, g_aTexV);
		pVtx[1].tex = D3DXVECTOR2(g_aTexU + TEX_WIDTH, g_aTexV);
		pVtx[2].tex = D3DXVECTOR2(g_aTexU, g_aTexV + TEX_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2(g_aTexU + TEX_WIDTH, g_aTexV + TEX_HEIGHT);

		//頂点バッファをアンロック
		g_pVtxbuffBg->Unlock();
	}
}

//========================
//背景描画処理
//========================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	if (GetStage() == STAGE_TIMEATTACK)
	{
		pDevice->SetTexture(0, g_apTextureBg[3]);
	}
	else
	{
		pDevice->SetTexture(0, g_apTextureBg[GetStage() - 1]);
	}
	
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//デバッグ（位置）
//========================
void DrawTexU(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入(x
	wsprintf(&aStr[0], "\n\n\ntex.u:0.%d",(int)(g_aTexU * 100));

	//テキスト描画(x
	g_pFontBG->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================
//BGが流れる速度設定
//========================
void SetBGSpeed(int nSpeed)
{
	g_nBGSpeed = nSpeed;
}

//========================
//BGが流れる速度取得
//========================
int GetBGSpeed(void)
{
	return g_nBGSpeed;
}


#include "main.h"
#include "game.h"
#include "timer.h"
#include "player.h"
#include "camera.h"
#include "fade.h"

//マクロ定義
#define NUM_PLACE  (2)								 //スコアの桁数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//頂点バッファへのポインタ
TIME g_aTime[NUM_PLACE];
int g_nTime;											//タイムの値
int g_nTimeCounter;									//タイムのカウンター	
int g_nTimePattern;									//タイムのパターン

D3DXMATRIX mtxWorldTime;

//===============================
//タイムの初期化の処理 
//===============================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number01.png",
		&g_pTextureTime);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		g_aTime[nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	g_nTimePattern = 0;
	g_nTimeCounter = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
	{

		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(100.0f + nCntTime*50.0f, 100.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(150.0f + nCntTime*50.0f, 100.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(100.0f + nCntTime*50.0f, 200.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(150.0f + nCntTime*50.0f, 200.0f, 0.0);
			
		//rhwの設定
		pVtx[VTX_LE_UP].rhw = RHW;
		pVtx[VTX_RI_UP].rhw = RHW;
		pVtx[VTX_LE_DO].rhw = RHW;
		pVtx[VTX_RI_DO].rhw = RHW;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//===============================
//タイムの終了処理
//===============================
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//===============================
//タイムの更新処理
//===============================
void UpdateTime(void)
{
	Player model = *GetPlayer();
	Camera camera = *GetCamera();
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
	{
		g_aTime[nCntTime].pos = camera.posR + (D3DXVECTOR3(0.0f, 60.0f, 0.0f));

		////テクスチャ座標の設定
		//pVtx[0].pos = D3DXVECTOR3(-15.0f + (nCntTime * 15.0f), 20.0f, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(0.0f + (nCntTime * 15.0f), 20.0f, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(-15.0f + (nCntTime * 15.0f), 0.0f, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(0.0f + (nCntTime * 15.0f), 0.0f, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();


	g_nTimeCounter++;

	if ((g_nTimeCounter % 60) == 0)
	{//一定時間経過
		g_nTimeCounter = 0;		//カウンターを初期値に戻す

		AddTime(1);
	}

}

//===============================
//タイムの描画処理
//===============================
void DrawTime(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime*4, 2);
	}

}


//===============================
//タイムの設定処理
//===============================
void SetTime(int nTime)
{
	int aTexU[NUM_PLACE];
	int nCntTime;
	VERTEX_2D * pVtx;

	g_nTime = nTime;
	
	aTexU[0] = g_nTime % 100 / 10;
	aTexU[1] = g_nTime % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//===============================
//タイムの減算処理
//===============================
void AddTime(int nValue)
{
	/*if (GetGameState() == GAMESTATE_NORMAL )
	{
*/
		int aTexU[NUM_PLACE];
		int nCntTime;
		VERTEX_2D * pVtx;

		g_nTime -= nValue;

		aTexU[0] = g_nTime % 100 / 10;
		aTexU[1] = g_nTime % 10 / 1;

		//制限時間が0になったらゲームオーバー
		if (g_nTime == 0)
		{
			SetFade(MODE_TITLE);
		}

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 1.0f);
		}
		//頂点バッファをアンロックする
		g_pVtxBuffTime->Unlock();
	//}
}

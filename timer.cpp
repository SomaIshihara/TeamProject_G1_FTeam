

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

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

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
		sizeof(VERTEX_3D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
	{

		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - 10.0f, g_aTime[nCntTime].pos.y + 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + 10.0f, g_aTime[nCntTime].pos.y + 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - 10.0f, g_aTime[nCntTime].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + 10.0f, g_aTime[nCntTime].pos.y, 0.0);
			
		//nor(法線)の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += 4)
	{
		g_aTime[nCntTime].pos = camera.posR + (D3DXVECTOR3(0.0f, 60.0f, 0.0f));

		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(-15.0f + (nCntTime * 15.0f), 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f + (nCntTime * 15.0f), 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-15.0f + (nCntTime * 15.0f), 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f + (nCntTime * 15.0f), 0.0f, 0.0f);
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
	LPDIRECT3DDEVICE9 	pDevice = GetDevice();		//デバイスの取得
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス


	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorldTime);

		//Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 10);


		//ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&mtxWorldTime, NULL, &mtxView);//逆行列を求める														
		mtxWorldTime._41 = 0.0f;
		mtxWorldTime._42 = 0.0f;
		mtxWorldTime._43 = 0.0f;

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aTime[nCntTime].pos.x, g_aTime[nCntTime].pos.y, g_aTime[nCntTime].pos.z);

		D3DXMatrixMultiply(&mtxWorldTime, &mtxWorldTime, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorldTime);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTime);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);

		//Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


		//アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	}
}
//{
//	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
//	int nCntTime;
//
//	pDevice = GetDevice();	    //デバイスの取得
//
//								//頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));
//
//	//頂点のフォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	//テクスチャの設定
//	pDevice->SetTexture(0, g_pTextureTime);
//
//	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
//	{
//		//ポリゴンの描画
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
//	}
//}

//===============================
//タイムの設定処理
//===============================
void SetTime(int nTime)
{
	int aTexU[NUM_PLACE];
	int nCntTime;
	VERTEX_3D * pVtx;

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
		VERTEX_3D * pVtx;

		g_nTime -= nValue;

		aTexU[0] = g_nTime % 100 / 10;
		aTexU[1] = g_nTime % 10 / 1;

		//制限時間が0になったらゲームオーバー
		/*if (g_nTime == 0)
		{
			SetFade(MODE_TITLE);
		}*/

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

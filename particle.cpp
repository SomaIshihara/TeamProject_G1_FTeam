#include"main.h"
#include"particle.h"
#include"bonus.h"
#include"Player.h"

//**********************************************************************
//グローバル変数
//**********************************************************************
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;			//テクスチャバッファ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffParticle = NULL;			//頂点バッファ
D3DXMATRIX				g_mtxWorldParticle;					//ワールドマトリックス
Particle				g_aParticle[MAX_PARTICLE];			//パーティクルの情報

D3DXVECTOR3 posTemp;

//======================================================================
//パーティクルの初期化処理
//======================================================================
void InitParticle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_3D *pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&g_pTextureParticle);

	//エフェクトのデータの初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += VTX_MAX)
	{
		//位置の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//色の設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}
//======================================================================
//パーティクルの終了処理
//======================================================================
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}
//======================================================================
//パーティクルの更新処理
//======================================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += VTX_MAX)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//使用されているとき

			//パーティクルの状態設定
			StateParticle(nCntParticle,pVtx);
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}
//======================================================================
//パーティクルの描画処理
//======================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();  //デバイスの取得
	D3DXMATRIX		  mtxTrans;				  //計算用マトリックス
	D3DXMATRIX		  mtxView;				  //ビューマトリックス取得用

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	//アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParticle);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向ける
			D3DXMatrixInverse(&g_mtxWorldParticle, NULL, &mtxView);		//逆行列を求める
			g_mtxWorldParticle._41 = 0.0f;
			g_mtxWorldParticle._42 = 0.0f;
			g_mtxWorldParticle._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, 2);
		}
	}

	//元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
//======================================================================
//パーティクルのセット処理
//======================================================================
void SetParticle(D3DXVECTOR3 pos, float fRadius, int nLife, PARTICLE nType, PARTICLE_OBJECT nObject)
{
	VERTEX_3D *pVtx;
	int ParticleCount = 0;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			//位置の設定
			g_aParticle[nCntParticle].pos = pos;

			//半径の設定
			g_aParticle[nCntParticle].fRadius = fRadius;

			//寿命の数値保存
			g_aParticle[nCntParticle].nLifeTemp = g_aParticle[nCntParticle].nLife;

			//種類の設定
			g_aParticle[nCntParticle].nType = nType;

			//オブジェクト種類の設定
			g_aParticle[nCntParticle].nObject = nObject;

			switch (nType)
			{
				//通常のパーティクル
			case PARTICLE_NORMAL:

				//移動量の設定
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;

				//寿命の設定
				g_aParticle[nCntParticle].nLife = (rand() % 30 - 15) + nLife;

				ParticleCount++;

				break;

				//吸収のパーティクル
			case PARTICLE_ACSORPTION:

				g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].pos.x + sinf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;
				g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].pos.y + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;
				g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].pos.z + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;

				//パーティクルの移動方向設定
				DirectionParticle(nCntParticle);

				//寿命の設定
				g_aParticle[nCntParticle].nLife = (rand() % 30 - 15) + nLife;

				ParticleCount++;

				break;
			}

			//パーティクルが使われている状態にする
			g_aParticle[nCntParticle].bUse = true;
		}

		if (nType == PARTICLE_NORMAL && ParticleCount == NORMAL_PARTICLE)
		{
			break;
		}
		else if (nType == PARTICLE_ACSORPTION && ParticleCount == ACSORPTION_PARTICLE)
		{
			break;
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}
//======================================================================
//パーティクルの状態設定
//======================================================================
void StateParticle(int nCount,VERTEX_3D *pVtx)
{
	//ボーナスの情報取得
	Bonus pBonus = GetBonus();

	//寿命の減少
	g_aParticle[nCount].nLife--;

	//座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCount].fRadius, g_aParticle[nCount].fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCount].fRadius, g_aParticle[nCount].fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCount].fRadius, -g_aParticle[nCount].fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCount].fRadius, -g_aParticle[nCount].fRadius, 0.0f);

	//移動量の設定
	g_aParticle[nCount].pos.x += g_aParticle[nCount].move.x;
	g_aParticle[nCount].pos.y += g_aParticle[nCount].move.y;
	g_aParticle[nCount].pos.z += g_aParticle[nCount].move.z;

	//パーティクルの状態処理
	switch (g_aParticle[nCount].nType)
	{
		//******************//
		//==================//
		//通常のパーティクル//
		//==================//
		//******************//
	case PARTICLE_NORMAL:

		g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.3f;
		g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.3f;
		g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.3f;

		//**********************//
		//パーティクルの種類処理//
		//**********************//
		switch (g_aParticle[nCount].nObject)
		{

			//プレイヤーパーティクル
		case OBJECT_PLAYER_ATK:

			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.1f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.1f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.1f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.1f);

			break;

		case OBJECT_PLAYER_DEF:

			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);

			break;

		case OBJECT_PLAYER_GOAST:

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));

			break;

			//ボーナスパーティクル
		case OBJECT_BONUS:

			if (pBonus.nLife == 3)
			{
				pVtx[0].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[1].col = D3DXCOLOR(0.6f, 0.2f, 0.4f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[2].col = D3DXCOLOR(0.4f, 0.2f, 0.6f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.8f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			}
			else if (pBonus.nLife == 2)
			{
				pVtx[0].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[1].col = D3DXCOLOR(0.6f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[2].col = D3DXCOLOR(0.4f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[1].col = D3DXCOLOR(0.6f, 0.4f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[2].col = D3DXCOLOR(0.4f, 0.6f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[3].col = D3DXCOLOR(0.2f, 0.8f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			}
			
			break;
		}

		break;

		//******************//
		//==================//
		//吸収のパーティクル//
		//==================//
		//******************//
	case PARTICLE_ACSORPTION:

		g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.2f;
		g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.2f;
		g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.2f;

		//**********************//
		//パーティクルの種類処理//
		//**********************//
		switch (g_aParticle[nCount].nObject)
		{

			//プレイヤーパーティクル
		case OBJECT_PLAYER_ATK:

			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f);

			break;

		case OBJECT_PLAYER_DEF:

			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);

			break;

		case OBJECT_PLAYER_GOAST:

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;

			//ボーナスパーティクル
		case OBJECT_BONUS:

			if (pBonus.nLife == 3)
			{
				pVtx[0].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[1].col = D3DXCOLOR(0.6f, 0.2f, 0.4f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[2].col = D3DXCOLOR(0.4f, 0.2f, 0.6f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.8f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			}
			else if (pBonus.nLife == 2)
			{
				pVtx[0].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[1].col = D3DXCOLOR(0.6f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[2].col = D3DXCOLOR(0.4f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[1].col = D3DXCOLOR(0.6f, 0.4f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[2].col = D3DXCOLOR(0.4f, 0.6f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
				pVtx[3].col = D3DXCOLOR(0.2f, 0.8f, 0.2f, ((float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp));
			}

			break;
		}
	}

	//パーティクルの破棄
	if (g_aParticle[nCount].nLife <= 0)
	{
		g_aParticle[nCount].bUse = false;
	}
}
//======================================================================
//パーティクルの移動方向設定
//======================================================================
void DirectionParticle(int nCount)
{
	Player *pPlayer = GetPlayer();

	Bonus pBonus = GetBonus();

	switch (g_aParticle[nCount].nObject)
	{
		//プレイヤーパーティクル
	case OBJECT_PLAYER_ATK:

		posTemp = pPlayer->pos;

		break;

	case OBJECT_PLAYER_DEF:

		posTemp = pPlayer->pos;

		break;

	case OBJECT_PLAYER_GOAST:

		posTemp = pPlayer->pos;

		break;

		//ボーナスパーティクル
	case OBJECT_BONUS:

		posTemp = pBonus.pos;

		break;
	}

	if (posTemp.x > g_aParticle[nCount].pos.x)
	{
		g_aParticle[nCount].move.x = 10;

		if (posTemp.z > g_aParticle[nCount].pos.z)
		{
			g_aParticle[nCount].move.z = 10;

			if (posTemp.y > g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = 10;
			}
			else if (posTemp.y < g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = -10;
			}
		}
		else if (posTemp.z < g_aParticle[nCount].pos.z)
		{
			g_aParticle[nCount].move.z = -10;

			if (posTemp.y > g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = 10;
			}
			else if (posTemp.y < g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = -10;
			}
		}
	}
	else if (posTemp.x < g_aParticle[nCount].pos.x)
	{
		g_aParticle[nCount].move.x = -10;

		if (posTemp.z > g_aParticle[nCount].pos.z)
		{
			g_aParticle[nCount].move.z = 10;

			if (posTemp.y > g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = 10;
			}
			else if (posTemp.y < g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = -10;
			}
		}
		else if (posTemp.z < g_aParticle[nCount].pos.z)
		{
			g_aParticle[nCount].move.z = -10;

			if (posTemp.y > g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = 10;
			}
			else if (posTemp.y < g_aParticle[nCount].pos.y)
			{
				g_aParticle[nCount].move.y = -10;
			}
		}
	}
}
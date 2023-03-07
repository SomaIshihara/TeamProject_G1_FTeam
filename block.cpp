//==========================================
//
//アイテムプログラム[Block.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "Block.h"
#include "HDR_player.h"
#include "sound.h"

//マクロ定義
#define MAX_PLAYER			(4)			//最大数
#define MAX_BLOCK_TEX		(10)		//テクスチャの最大数
#define MAX_POS				(5)			//リスポーン位置最大数
#define INIT_POS_Y			(200.0f)	//初期のY位置
#define INIT_POS_XZ			(200.0f)	//初期の外位置
#define COLLISION_SIZE_XZ	(30.0f)		//縦横の当たり判定サイズ

//プロト
BLOCKTYPE SelectBlock(void);

//グローバル変数宣言     
LPDIRECT3DTEXTURE9		g_pTextureBlock[MAX_BLOCK_TEX] = {};	//テクスチャへにポインタ
LPD3DXMESH				g_pMeshBlock = NULL;					//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatBlock = NULL;					//マテリアルへのポインタ
DWORD					g_dwNumMatBlock = 0;					//マテリアルの数
D3DXMATRIX				g_mtxWorldBlock;						//ワールドマトリックス
Block					g_Block[MAX_BLOCK * MAX_PLAYER];		//ブロックの情報
int						g_aUseBlockNum[BLOCKTYPE_MAX];			//ブロック個数

//ブロック体力
const int				c_aBlockLife[BLOCKTYPE_MAX] = { 100,200,400 };

//配置数
const int				c_aBlockNum[BLOCKTYPE_MAX] = { 25,30,25 };

//========================
//初期化処理
//========================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL	  *pMat;					//マテリアルへのポインタ

	//xファイルの読み込み
	D3DXLoadMeshFromX({ "data/MODEL/block/block.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBlock,
		NULL,
		&g_dwNumMatBlock,
		&g_pMeshBlock);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatBlock->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBlock; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する　

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureBlock[nCntMat]);
		}
	}

	//初期設定
	//初期化
	for (int nCntBlockType = 0; nCntBlockType < BLOCKTYPE_MAX; nCntBlockType++)
	{
		g_aUseBlockNum[nCntBlockType] = c_aBlockNum[nCntBlockType];
	}

	//ブロック情報設定
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		int nCount = 0;

		//初期化
		for (int nCntBlockType = 0; nCntBlockType < BLOCKTYPE_MAX; nCntBlockType++)
		{
			g_aUseBlockNum[nCntBlockType] = c_aBlockNum[nCntBlockType];
		}

		for (int nCntBlock = 0 + (MAX_BLOCK * nCntPlayer); nCntBlock < MAX_BLOCK * MAX_PLAYER; nCntBlock++)
		{
			g_Block[nCntBlock].pos = D3DXVECTOR3(225.0f - 150 * nCntPlayer, 20.0f + nCount * 80, 0.0f);

			g_Block[nCntBlock].type = SelectBlock();
			g_Block[nCntBlock].fWidth = COLLISION_SIZE_XZ;
			g_Block[nCntBlock].buse = false;
			g_Block[nCntBlock].nLife = 0;
			nCount++;

			if (nCount == MAX_BLOCK)
			{
				break;
			}
		}
	}

	SetBlock();
}

//========================
//終了処理
//========================
void UninitBlock(void)
{
	//メッシュの破棄
	if (g_pMeshBlock != NULL)
	{
		g_pMeshBlock->Release();
		g_pMeshBlock = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatBlock != NULL)
	{
		g_pBuffMatBlock->Release();
		g_pBuffMatBlock = NULL;
	}

	//マテリアルの破棄
	for (int nCntTex = 0; nCntTex < MAX_BLOCK_TEX; nCntTex++)
	{
		if (g_pTextureBlock[nCntTex] != NULL)
		{
			g_pTextureBlock[nCntTex]->Release();
			g_pTextureBlock[nCntTex] = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdateBlock(void)
{

}

//========================
//描画処理
//========================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9	  matDef;					//現在のマテリアル保存用
	D3DXMATERIAL	  *pMat;					//マテリアルデータへのポインタ

												//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBlock);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBlock, &g_mtxWorldBlock, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBlock);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatBlock->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBlock; nCntMat++)
			{

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshBlock->DrawSubset(nCntMat);
			}
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//ブロック選択処理
//========================
BLOCKTYPE SelectBlock(void)
{
	int nBlockType = rand() % BLOCKTYPE_MAX;

	if (g_aUseBlockNum[nBlockType] == 0)
	{//ブロックが配置上限に達している
		//2つ前（ループする）にする
		nBlockType = (nBlockType + (BLOCKTYPE_MAX - 1)) % BLOCKTYPE_MAX;

		if (g_aUseBlockNum[nBlockType] == 0)
		{//それも配置上限に達している
			//2つ前（ループする）にする
			nBlockType = (nBlockType + (BLOCKTYPE_MAX - 1)) % BLOCKTYPE_MAX;
			//絶対に置けるので終わり
		}
	}

	//置けるので設定
	g_aUseBlockNum[nBlockType]--;
	return (BLOCKTYPE)nBlockType;
}

//========================
//設定処理
//========================
void SetBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == false)
		{
			g_Block[nCntBlock].nLife = c_aBlockLife[g_Block[nCntBlock].type];
			g_Block[nCntBlock].buse = true;
		}
	}
}

//========================
//アイテムとプレイヤーの当たり判定処理
//========================
void CollisionBlock(int nPlayerNum)
{
	Player_HDR *pPlayer = GetPlayer_HDR();//プレイヤー情報取得

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == true)
		{
			if (pPlayer[nPlayerNum].pos.x >= g_Block[nCntBlock].pos.x - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.x <= g_Block[nCntBlock].pos.x + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z >= g_Block[nCntBlock].pos.z - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z <= g_Block[nCntBlock].pos.z + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.y >= g_Block[nCntBlock].pos.y - COLLISION_SIZE_Y
				&&pPlayer[nPlayerNum].pos.y <= g_Block[nCntBlock].pos.y + COLLISION_SIZE_Y)
			{//プレイヤーがの範囲内に入ったとき

				if (pPlayer[nPlayerNum].bHipDrop == true)
				{
					if (pPlayer[nPlayerNum].HipDropPower >= g_Block[nCntBlock].nLife)
					{//ブロックの体力よりパワー（ダメージ量）が大きい
						//ブロックを消す
						g_Block[nCntBlock].buse = false;								//対象のブロックを使用しない
						pPlayer[nPlayerNum].HipDropPower -= g_Block[nCntBlock].nLife;	//使用した分のダメージを減らす
						PlaySound(SOUND_LABEL_SE_HIPDROP, nCntBlock);					//ヒップドロップ音再生
					}
					else
					{
						g_Block[nCntBlock].nLife -= pPlayer[nPlayerNum].HipDropPower;
						pPlayer[nPlayerNum].HipDropPower = 0;
						pPlayer[nPlayerNum].move.y = 0;
						pPlayer[nPlayerNum].bHipDrop = false;
					}
				}
				else
				{
					pPlayer[nPlayerNum].pos.y = g_Block[nCntBlock].pos.y + COLLISION_SIZE_Y;
					pPlayer[nPlayerNum].move.y = 0;
					pPlayer[nPlayerNum].moveV0.y = 0;
					pPlayer[nPlayerNum].jumpTime = 0;
					pPlayer[nPlayerNum].bJump = false;
					pPlayer[nPlayerNum].bHipDrop = false;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------
//
//メッシュドームの屋根・奈落[meshdome.cpp]
//Author:平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshdome.h"
#include "color.h"

//マクロ
#define NUM_MESHDOME				(1)					//メッシュドームの数
#define MESHDOME_RADIS				(200.0f)			//背景の半径
#define MESHDOME_TEX_RESOLUTION		(3.0f)				//背景の解像度
#define MESHDOME_SPLIT				(8)					//背景の頂点数
#define MESHDOME_SEPALATE			(8)					//背景の縦の分割数
#define MESHDOME_NUM_OVERLAP		(2)					//最初と最後の頂点が重なる数
//#define MESHDOME_ALL_VERTEX			((MESHDOME_SPLIT * (MESHDOME_SEPALATE - 1)) + MESHDOME_NUM_OVERLAP)	//全体の頂点数
#define CREATE_SQUARE_NEED_TRIANGLE	(2)					//四角形を作るのに必要な三角形の数
#define TRIANGLE					(3)					//３角形に必要な頂点
#define SQUARE_NUM_VERTEX			(6)					//四角形の中に含まれる頂点の数(重複も含む)
#define ONE_LAP						(D3DX_PI * 2.0f)	//１周分の角度
#define MESHDOME_ALL_VERTEX			(19)				//全体の頂点数
#define MESHDOME_COVER_INDEX		(10)				//フタのインデックス数
#define MESHDOME_SIDE_INDEX			(MESHDOME_SEPALATE * (MESHDOME_SPLIT - MESHDOME_NUM_OVERLAP) * 2 * TRIANGLE)	//側面のインデックス数  1周分の分割数 × 側面だけのインデックス × 四角形に必要な三角形数 × 三角形の頂点数

//メッシュドームの構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	float			fRadius;	//半径
}MeshDome;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureMeshDome = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffCoverMeshDome = NULL;		//フタのインデックス番号
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffSideMeshDome = NULL;		//側面のインデックス番号
D3DXMATRIX				g_mtxWorldMeshDome;
MeshDome				g_MeshDome;

//--------------------------------------------------------------------------------------------------------
//メッシュドームの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitMeshDome(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureMeshDome);

	//ドーム情報の初期化
	g_MeshDome.pos = ZERO_SET;
	g_MeshDome.rot = ZERO_SET;
	g_MeshDome.fRadius = MESHDOME_RADIS;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshDome, NULL);

	//--------------------------------------
	//			頂点情報の設定
	//--------------------------------------
	SetMeshDomeVertexBuffer();

	//--------------------------------------
	//		インデックスバッファの生成
	//--------------------------------------
	//フタのインデックスバッファ
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_COVER_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffCoverMeshDome, NULL);
	
	//側面のインデックスバッファ
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_SIDE_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffSideMeshDome, NULL);

	//--------------------------------------
	//		インデックスバッファの設定
	//--------------------------------------
	SetMeshDomeIndexBuffer();
}

//頂点座標の設定
void SetMeshDomeVertexBuffer(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	int nNumVtx = 0;			//頂点番号
	float yRadian = 0.0f;		//縦で分割した時の横１周分のY座標が決まる角度
	D3DXVECTOR3 pos[MESHDOME_ALL_VERTEX];
	
	//天面の出っ張り頂点の設定
	pos[nNumVtx] = D3DXVECTOR3(g_MeshDome.pos.x, g_MeshDome.pos.y + MESHDOME_RADIS, g_MeshDome.pos.z);
	pVtx[nNumVtx].pos = D3DXVECTOR3(g_MeshDome.pos.x, g_MeshDome.pos.y + MESHDOME_RADIS, g_MeshDome.pos.z);
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
	nNumVtx++;
	
	//２番目の頂点から、横の分割数　‐　底面の出っ張りの１頂点　回数分 for文を回す
	for (int nCntDevideY = nNumVtx; nCntDevideY < 3 ; nCntDevideY++)
	{
		int nStrage = nNumVtx;

		//---------------------------------------------------------------------------------------------------------
		//	MEMO：縦に分割する数を使って原点から横１周分の高さまでの角度計算 (半周分で分割点が分かるので、3.14
		//---------------------------------------------------------------------------------------------------------
		yRadian += D3DX_PI / MESHDOME_SEPALATE;

		float TempLen = fabsf(sinf(yRadian) * g_MeshDome.fRadius);	//X・Zの半径
		float rot_Y = D3DX_PI;										//Y軸の角度
		float Height_Y = cosf(yRadian) * g_MeshDome.fRadius;		//Yの高さ

		for (int nCntDevideX = 0; nCntDevideX < MESHDOME_SPLIT; nCntDevideX++)
		{//横１周分の頂点座標を設定

			pos[nNumVtx] = D3DXVECTOR3(
				sinf(rot_Y) * TempLen,		//Xの位置
				Height_Y,					//Yの位置
				cosf(rot_Y) * TempLen);		//Zの位置

			pVtx[nNumVtx].pos = g_MeshDome.pos + pos[nNumVtx];

			pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nNumVtx].col = XCOL_WHITE;
			pVtx[nNumVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
			nNumVtx++;
			
			//角度を　全体の角度÷分割数で割った答え分、引く
			rot_Y -= ONE_LAP / MESHDOME_SPLIT;
		}

		pVtx[nNumVtx].pos = pVtx[nStrage].pos;
		pVtx[nNumVtx].nor = pVtx[nStrage].nor;
		pVtx[nNumVtx].col = pVtx[nStrage].col;
		pVtx[nNumVtx].tex = pVtx[nStrage].tex;
		nNumVtx++;
	}


	//頂点バッファのアンロック
	g_pVtxBuffMeshDome->Unlock();
}

//インデックス番号の設定
void SetMeshDomeIndexBuffer(void)
{
	WORD *pIdx;		//インデックスのポインタ

	//フタのインデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffCoverMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	int nNumIdx = 0;		/*インデックス番号*/
	int offsetIndex = 0;	/*天面の出っ張りの外に広がる頂点の基準番号（基本的に中心頂点の次に指定される頂点番号）*/

	//			／＼
	//		  ／天面＼
	//		／番号設定＼
	for (nNumIdx = 0; nNumIdx < MESHDOME_COVER_INDEX; nNumIdx++)
	{
		pIdx[nNumIdx] = nNumIdx;
	}
	
	//インデックスバッファのアンロック
	g_pIdxBuffCoverMeshDome->Unlock();

	
	//側面のインデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffSideMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	//	／----------------＼
	// <	側面番号設定	>
	//	＼----------------／
	nNumIdx = 0;				//インデックス番号初期化
	int StartIndex = pIdx[1];	/*開始Index番号*/
	int SideIndexLen = MESHDOME_SEPALATE * (MESHDOME_SPLIT - MESHDOME_NUM_OVERLAP) * 2 * TRIANGLE;	/*天面・底面を除いたIndex要素数*/
	int LapDiv = MESHDOME_SEPALATE * 2 * TRIANGLE;	/*１周分必要なIndex数*/
	int LapLastIndex = 0,							/*ループ時使用する最後のIndex*/
		LapBoobyIndex = 0;							/*ループ時使用する最後から2番目のIndex*/
	int CreateSquareCnt = 0;						/*作成した四角形の数*/

	for (int nCntHeight = 0; nCntHeight < MESHDOME_SEPALATE - 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < MESHDOME_SPLIT + 1; nCntWidth++)
		{
			pIdx[nNumIdx++] = (nCntWidth + ((MESHDOME_SPLIT + 1) * (nCntHeight + 1)));
			pIdx[nNumIdx++] = nCntWidth + ((MESHDOME_SPLIT + 1) * nCntHeight);
		}

		if (nCntHeight < MESHDOME_SEPALATE - 2)
		{
			pIdx[nNumIdx++] = ((MESHDOME_SPLIT + 1) * (nCntHeight + 1)) - 1;
			pIdx[nNumIdx++] = (MESHDOME_SPLIT + 1) * (nCntHeight + 2);
		}
	}

	//		＼底面番号／
	//		  ＼設定／
	//			＼／
	//最後の頂点番号（底面の先頂点)
	offsetIndex = MESHDOME_ALL_VERTEX - 1;
	int LoopIndex = offsetIndex;

	/*for (int i = MESHDOME_SEPALATE * TRIANGLE - 1; i >= 0; i--)
	{
		if (i % 3 == 0)
		{
			// 底面の先頂点  
			pIdx[nNumIdx++] = MESHDOME_ALL_VERTEX - 1;
			offsetIndex++;
		}
		else if (i % 3 == 1)
		{
			pIdx[nNumIdx++] = offsetIndex;
		}
		else if (i % 3 == 2)
		{
			//底面の先頂点の次に続く頂点番号が、先頂点の番号以上になったら、底面を作るときに宣言したループ番号に置き換えて、1周が完了する
			pIdx[nNumIdx++] = MESHDOME_ALL_VERTEX - 1 <= 1 + offsetIndex ? MESHDOME_ALL_VERTEX - 1 : 1 + offsetIndex;
		}
	}*/

	//インデックスバッファのアンロック
	g_pIdxBuffSideMeshDome->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitMeshDome(void)
{
	//フタのインデックスの破棄
	if (g_pIdxBuffCoverMeshDome != NULL)
	{
		g_pIdxBuffCoverMeshDome->Release();
		g_pIdxBuffCoverMeshDome = NULL;
	}

	//側面のインデックスの破棄
	if (g_pIdxBuffSideMeshDome != NULL)
	{
		g_pIdxBuffSideMeshDome->Release();
		g_pIdxBuffSideMeshDome = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateMeshDome(void)
{

}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの描画処理
//--------------------------------------------------------------------------------------------------------
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshDome);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshDome);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	//両面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//-----------------------------------------------------------
	//フタのインデックスを設定し、描画する
	//-----------------------------------------------------------
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffCoverMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHDOME_COVER_INDEX, 0, 8);
	
	//-----------------------------------------------------------
	//側面のインデックスを設定し、描画する
	//-----------------------------------------------------------
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffSideMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHDOME_SEPALATE * (MESHDOME_SPLIT - MESHDOME_NUM_OVERLAP) * 2 * TRIANGLE, 0, MESHDOME_SEPALATE * (MESHDOME_SPLIT - MESHDOME_NUM_OVERLAP) * 2 * TRIANGLE);

	//普通のカリングモードにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
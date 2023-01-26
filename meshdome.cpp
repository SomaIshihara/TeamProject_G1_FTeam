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
#define MESHDOME_SPLIT				(8)				//背景の頂点数
#define MESHDOME_SEPALATE			(8)					//背景の縦の分割数
#define MESHDOME_NUM_OVERLAP		(2)					//最初と最後の頂点が重なる数
#define MESHDOME_ALL_VERTEX			((MESHDOME_SPLIT * (MESHDOME_SEPALATE - 1)) + MESHDOME_NUM_OVERLAP)	//全体の頂点数
#define CREATE_SQUARE_NEED_TRIANGLE	(2)					//四角形を作るのに必要な三角形の数
#define TRIANGLE					(3)					//３角形に必要な頂点
#define SQUARE_NUM_VERTEX			(6)					//四角形の中に含まれる頂点の数(重複も含む)
#define ONE_LAP						(D3DX_PI * 2.0f)	//１周分の角度
#define MESHDOME_ALL_VERTEX			(19)				//全体の頂点数
#define MESHDOME_ALL_INDEX			(19)				//全体のインデックス数

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
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshDome = NULL;
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
	g_MeshDome.fRadius = MESHDOME_WIDTH;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshDome, NULL);

	//--------------------------------------
	//			頂点情報の設定
	//--------------------------------------
	SetMeshDomeVertexBuffer();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_ALL_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshDome, NULL);

	//--------------------------------------
	//		インデックスバッファの生成
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
	WORD *pIdx;
	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	int nNumIdx = 0;		/*インデックス番号*/

	int offsetIndex = 0;	/*天面の出っ張りの外に広がる頂点の基準番号（基本的に中心頂点の次に指定される頂点番号）*/

	//			／＼
	//		  ／天面＼
	//		／番号設定＼
	for (nNumIdx = 0; nNumIdx < 10; nNumIdx++)
	{
		pIdx[nNumIdx] = nNumIdx;
	}



	//			／＼
	//		  ／天面＼
	//		／番号設定＼			MESHDOME_SEPALATE * TRIANGLE	分割数分の三角形を作成するために、三角形に必要な頂点数3を乗算する
	/*for (int nCntIdx = 0; nCntIdx < MESHDOME_SEPALATE * TRIANGLE; nCntIdx++)
	{
		switch (nCntIdx % 3)
		{
		case 0:		//天面のでっぱり番号
			pIdx[nNumIdx++] = 0;
			break;

		case 1:		//２番目の頂点番号
			pIdx[nNumIdx++] = 1 + offsetIndex;
			break;

		case 2:		//最後の頂点番号
			int LapIndex = 2 + offsetIndex++;

			// インデックス番号が１周したとき、下の3項演算子が　Trueとなり、１を返す
			LapIndex = MESHDOME_SEPALATE < LapIndex ? pIdx[1] : LapIndex;
			
			//３角形最後のインデックス番号を代入
			pIdx[nNumIdx++] = LapIndex;
			break;
		}
	}*/
	
	//	／----------------＼
	// <	側面番号設定	>
	//	＼----------------／
	int StartIndex = pIdx[1];	/*開始Index番号*/
	int SideIndexLen = MESHDOME_SEPALATE * (MESHDOME_SPLIT - MESHDOME_NUM_OVERLAP) * 2 * TRIANGLE;	/*天面・底面を除いたIndex要素数*/
	int LapDiv = MESHDOME_SEPALATE * 2 * TRIANGLE;	/*１周分必要なIndex数*/
	int LapLastIndex = 0,							/*ループ時使用する最後のIndex*/
		LapBoobyIndex = 0;							/*ループ時使用する最後から2番目のIndex*/
	int CreateSquareCnt = 0;						/*作成した四角形の数*/

	for (int nCntSideIdx = 0; nCntSideIdx < SideIndexLen; nCntSideIdx++)
	{
		// 一周の頂点数を超えたら更新(初回も含む)
		if (nCntSideIdx % LapDiv == 0)
		{
			LapLastIndex = StartIndex;
			LapBoobyIndex = StartIndex + MESHDOME_SEPALATE;
			CreateSquareCnt++;
		}

		switch (nCntSideIdx % 6)
		{
		case 0:
		case 3:
		{
			pIdx[nNumIdx++] = StartIndex;
		}
		break;

		case 1:
		{
			pIdx[nNumIdx++] = StartIndex + MESHDOME_SEPALATE;
		}
		break;

		case 2:
		case 4:
		{
			if (nCntSideIdx > 0
				&& (nCntSideIdx % (LapDiv * CreateSquareCnt - 2) == 0 || nCntSideIdx % (LapDiv * CreateSquareCnt - 4) == 0))
			{
				// 1周したときのループ処理
				// 周回ポリゴンの最後から2番目のIndex
				pIdx[nNumIdx++] = LapBoobyIndex;
			}
			else
			{
				pIdx[nNumIdx++] = StartIndex + MESHDOME_SEPALATE + 1;
			}
		}
		break;

		case 5:
		{
			if (0 < nCntSideIdx && nCntSideIdx % (LapDiv * CreateSquareCnt - 1) == 0)
			{
				// 1周したときのループ処理
				// 周回ポリゴンの最後のIndex
				pIdx[nNumIdx++] = LapLastIndex;
			}
			else
			{
				pIdx[nNumIdx++] = StartIndex + 1;
			}

			// 開始Indexの更新 
			StartIndex++;
		}
		break;
		}
	}

	//		＼底面番号／
	//		  ＼設定／
	//			＼／
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		pIdx[nCnt] = nCnt;
	}


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
	g_pIdxBuffMeshDome->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitMeshDome(void)
{
	//インデックスの破棄
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
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

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, 10, 0, 8);		//フタ描画
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 16, MESHDOME_ALL_VERTEX + 1, 16);	//側面描画
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHDOME_ALL_VERTEX, 0, MESHDOME_ALL_VERTEX);
	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
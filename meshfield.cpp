//==========================================
//
//メッシュフィールドプログラム[meshfield.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "player.h"
#include "meshfield.h"
#include "file.h"
#include "color.h"
#include "input.h"

//マクロ定義		ファイル読み込みに変える可能性あり
#define NUM_MESHFIELD			(1)						//メッシュフィールド最大数
#define MESHFIELD_RADIUS		(353.5f)				//フィールドの半径
#define MESHFIELD_SPLIT			(64)					//分割数
#define CENTER_AND_ONELAP		(2)						//中心点と１周で重なる点
#define MESHFIELD_ALL_VERTEX	(MESHFIELD_SPLIT + 1)	//周辺の分割地点と中心
#define MESHFIELD_ALL_INDEX		(MESHFIELD_SPLIT + 2)	//周辺の分割地点と中心・１周で重なる点

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMeshfield = NULL;		//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXMATRIX				g_mtxWorldMeshfield;			//ワールドマトリックス

MESHFIELD				g_MeshField[NUM_MESHFIELD];

//====================================================================
//メッシュフィールドの初期化処理
//====================================================================
void InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//設定類は外部ファイルに移動
#if 0
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		GetTextureFilePath(0),
		&g_pTextureMeshfield);

	//構造体を初期化
	for (int nCntfield = 0; nCntfield < MAX_FIELD; nCntfield++)
	{
		g_MeshField[nCntfield].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshField[nCntfield].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshField[nCntfield].fLength_X = BESIDE_LENGTH;
		g_MeshField[nCntfield].fLength_Z = VERTICAL_LENGTH;
		g_MeshField[nCntfield].nBlock_X = BESIDE;
		g_MeshField[nCntfield].nBlock_Z = VERTICAL;
	}
#endif

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_ALL_VERTEX,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield, NULL);

	//頂点情報設定
	SetFieldVertex();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_ALL_INDEX,
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField, NULL);

	//インデックス情報を設定
	SetFieldIndex();
}

//メッシュフィールドの頂点情報を設定
void SetFieldVertex(void)
{
	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点番号
	int nNumVtx = 0;
	float Rot = D3DX_PI;	//角度

	pVtx[nNumVtx].pos = ZERO_SET;				//中心座標代入
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//法線
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.5f, 0.5f);
	nNumVtx++;

	//周辺の頂点情報の設定
	for (int nCntVtx = 0; nCntVtx < MESHFIELD_SPLIT; nCntVtx++, nNumVtx++)
	{
		pVtx[nNumVtx].pos =	D3DXVECTOR3(
				sinf(Rot) * g_MeshField[0].fRadius,		//Xの位置
				0.0f,									//Yの位置
				cosf(Rot) * g_MeshField[0].fRadius);	//Zの位置

		pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nNumVtx].col = XCOL_WHITE;

		float sin = sinf(Rot);
		float cos = cosf(Rot);

		float aTexU = 0.5f - 0.5f * sinf(Rot);	//テクスチャ幅
		float aTexV = 0.5f - 0.5f * cosf(Rot);	//テクスチャ高さ
		
		pVtx[nNumVtx].tex = D3DXVECTOR2(aTexU, aTexV);

		//角度を　全体の角度÷分割数で割った答え分、引く
		Rot -= ONE_LAP / MESHFIELD_SPLIT;
	}

	g_pVtxBuffMeshfield->Unlock();
}

//メッシュフィールドのインデックス情報を設定
void SetFieldIndex(void)
{
	WORD *pIdx;					//インデックス情報へのポインタ

	//インデックスバッファをロックし頂点情報へのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	int nNumIdx = 0;

	for (nNumIdx = 0; nNumIdx < MESHFIELD_ALL_INDEX; nNumIdx++)
	{
		int nTest = (nNumIdx / (MESHFIELD_ALL_INDEX - 1)) + (nNumIdx % (MESHFIELD_ALL_INDEX - 1));
		//下の計算式では、　ちょうど1周するときに答えが1になる　　それ以外では、カウンター÷分割数　の余りが番号として設定される
		pIdx[nNumIdx] = (nNumIdx / (MESHFIELD_ALL_INDEX - 1)) + (nNumIdx % (MESHFIELD_ALL_INDEX - 1));
	}

	g_pIdxBuffMeshField->Unlock();
}

//====================================================================
//メッシュフィールドの終了処理
//====================================================================
void UninitMeshfield(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//====================================================================
//メッシュフィールドの更新処理
//====================================================================
void UpdateMeshfield(void)
{

}

//====================================================================
//メッシュフィールドの描画処理
//====================================================================
void DrawMeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//両面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntfield = 0; nCntfield < NUM_MESHFIELD; nCntfield++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldMeshfield);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField[nCntfield].rot.y, g_MeshField[nCntfield].rot.x, g_MeshField[nCntfield].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MeshField[nCntfield].pos.x, g_MeshField[nCntfield].pos.y, g_MeshField[nCntfield].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshField);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshfield);

		//描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHFIELD_ALL_VERTEX, 0, MESHFIELD_SPLIT);
	}

	//普通のカリングモードにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//====================================================================
//メッシュフィールドの設定処理
//Author:石原颯馬
//Memo:メッシュフィールド複数個使用するならfile.cppの構造変えなきゃならんので言って
//====================================================================
void SetMeshField(int nTexNum, MESHFIELD mf)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		GetTextureFilePath(nTexNum),
		&g_pTextureMeshfield);

	g_MeshField[0] = mf;
}
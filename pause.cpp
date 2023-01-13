//==========================================
//
//ポーズプログラム[meshfield.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "player.h"
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "color.h"

//マクロ定義
#define NUM_TEXTURE		(4)		//ポーズ画像のファイル名

//ファイルパス
const char *c_pFileNamePause[] =
{
	"data/TEXTURE/pause.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
	"data/TEXTURE/quit.png"
};

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTexturePause = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//頂点バッファへのポインタ
PAUSE g_Pause;
Pause g_aPause[NUM_TEXTURE];

//=================================
//ポーズの初期化処理
//=================================
void InitPause(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_TEXTURE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//ポーズの読み込み処理
	LoadPause();

	//各情報読み込み
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++,pVtx +=4)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_pFileNamePause[nCnt], &g_pTexturePause);

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, NIL_F, NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(NIL_F, SCREEN_HEIGHT, NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);

		//rhwの設定
		pVtx[VTX_LE_UP].rhw = RHW;
		pVtx[VTX_RI_UP].rhw = RHW;
		pVtx[VTX_LE_DO].rhw = RHW;
		pVtx[VTX_RI_DO].rhw = RHW;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//テクスチャの座標
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}
//=================================
//ポーズの終了処理
//=================================
void UninitPause(void) 
{
	//テクスチャの破棄
	if (g_pTexturePause != NULL)
	{
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=================================
//ポーズの更新処理
//=================================
void UpdatePause(void)
{
	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			break;

		case PAUSE_RETRY:
			//モード設定（ゲーム画面に遷移)
			SetFade(MODE_GAME);
			break;

		case PAUSE_QUIT:
			//タイトル画面に遷移
			SetFade(MODE_TITLE);
			break;
		}
	}
}

//=================================
//ポーズの描画処理
//=================================
void DrawPause(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePause);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=================================
//ポーズの読み込み処理
//=================================
void LoadPause()
{
	return;

	int c = 0;			//一文字ずつ確認する変数
	int column = 1;		//列数を数える変数
	int line = 0;		//行数を数える処理

	char aData[1000];	//つなげる文字数

	FILE *pFile;		//ファイルポインタを宣言

	memset(aData, 0, sizeof(aData));

	// ファイルを開く
	pFile = fopen("data/CSV/pause.csv", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ファイルから一文ずつ読み込む
		while (fgetc(pFile) != '\n');

		while (c != EOF)
		{
			//1セル分の文字列を作る
			while (1)
			{
				c = fgetc(pFile);

				//末尾ならループを抜ける
				if (c == EOF)
				{
					break;
				}

				//カンマか改行でなければ、文字としてつなげる
				if (c != ',' && c != '\n')
				{
					strcat(aData, (const char*)&c);
				}
				//カンマか改行ならループを抜ける
				else
				{
					break;
				}
			}

			switch (column)
			{
			//atoi関数で数値として代入
			case 1: g_aPause[line].pos.x	  = (float)(atoi(aData));	break;		//X位置
			case 2: g_aPause[line].pos.y	  = (float)(atoi(aData));	break;		//Y位置
			case 3: g_aPause[line].VtxZero.x  = (float)(atoi(aData));	break;		//X0
			case 4: g_aPause[line].VtxZero.y  = (float)(atoi(aData));	break;		//Y0
			case 5: g_aPause[line].VtxOne.x   = (float)(atoi(aData));	break;		//Y幅
			case 6: g_aPause[line].VtxOne.y   = (float)(atoi(aData));	break;		//Y幅
			case 7: g_aPause[line].VtxTwo.x   = (float)(atoi(aData));	break;		//X高さ
			case 8: g_aPause[line].VtxTwo.y   = (float)(atoi(aData));	break;		//Y高さ
			case 9: g_aPause[line].VtxThree.x = (float)(atoi(aData));	break;		//Y高さ
			case 10: g_aPause[line].VtxThree.y = (float)(atoi(aData));	break;		//Y高さ
			}
			//バッファを初期化
			memset(aData, 0, sizeof(aData));

			//列数を足す
			column++;

			//もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
			if (c == '\n')
			{
				column = 1;
				line++;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//=================================
//ポーズの設定処理
//=================================
void SetPause(PAUSE Pause)
{
	g_Pause = Pause;
}

//=================================
//ポーズの取得
//=================================
PAUSE *GetPause(void)
{
	return &g_Pause;
}
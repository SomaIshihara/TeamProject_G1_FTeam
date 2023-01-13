//--------------------------------------------------------------------------------------------------------
//
//ライト処理[light.cpp]
//Author:菊地陽人
//
//--------------------------------------------------------------------------------------------------------

#include "main.h"
#include "light.h"
#include "input.h"

//マクロ定義
#define	NUM_LIGHT		(3)		//ライトの数

//グローバル変数
D3DLIGHT9 g_aLight[NUM_LIGHT];

//--------------------------------------------------------------------------------------------------------
//ライトの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXVECTOR3 aVecDir[NUM_LIGHT];		//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//ライトの情報をクリアする
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));
	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	//ライトの種類を設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	aVecDir[0] = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&aVecDir[0], &aVecDir[0]);		//ベクトルを正規化する
	g_aLight[0].Direction = aVecDir[0];

	aVecDir[1] = D3DXVECTOR3(-0.2f, 0.8f, -0.4f);
	D3DXVec3Normalize(&aVecDir[1], &aVecDir[1]);		//ベクトルを正規化する
	g_aLight[1].Direction = aVecDir[1];

	aVecDir[2] = D3DXVECTOR3(0.8f, -0.1f, 0.4f);
	D3DXVec3Normalize(&aVecDir[2], &aVecDir[2]);		//ベクトルを正規化する
	g_aLight[2].Direction = aVecDir[2];

	//ライトを設定する
	pDevice->SetLight(0, &g_aLight[0]);
	pDevice->SetLight(1, &g_aLight[1]);
	pDevice->SetLight(2, &g_aLight[2]);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);
}



//--------------------------------------------------------------------------------------------------------
//ライトの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitLight(void)
{

}



//--------------------------------------------------------------------------------------------------------
//ライトの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateLight(void)
{

}
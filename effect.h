//==========================================
//
//ポーズプログラム[effect.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
}Effect;

//プロトタイプ宣言
void InitEffect(void);		//初期化処理
void UninitEffect(void);	//終了処理
void UpdateEffect(void);	//更新処理
void DrawEffect(void);		//描画処理

#endif

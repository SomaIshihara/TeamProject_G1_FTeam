//==========================================
//
//弾表示プログラムのヘッダ[bullet.h]
//Author:石原颯馬
//
//==========================================
#ifndef _BG_H_
#define _BG_H_

//マクロ
#define BG_TEX_SPEED	(0.0025f)	//テクスチャの移動速度
#define BG_TEX_HEIGHT	(360)		//テクスチャの幅

//プロトタイプ宣言
void InitBg(void);
void UninitBg(void);
void UpdateBg(void);
void DrawBg(void);
void DrawTexU(void);
void SetBGSpeed(int nSpeed);
int GetBGSpeed(void);

#endif // !_BG_H_

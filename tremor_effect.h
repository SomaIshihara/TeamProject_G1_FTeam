//==========================================
//
//エフェクトプログラム[tremor_effect.h]
//Author:石原颯馬（ベース：飯田洲暉）
//
//==========================================
#ifndef _TREMOR_EFFECT_H_
#define _TREMOR_EFFECT_H_

#include "main.h"

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int			nType;		//エフェクトのタイプ
	int			nCounter;	//エフェクト発生開始から経過した時間
	//int			nCntLoop;	//ループした回数（今だけ）
	float		fSize;		//半径
	float		fAlpha;		//α値
	bool		bUse;		//使われているかどうか
	//bool		bUseTremor;	//チャージが使われているかどうか
}TremorEffect;

//プロトタイプ宣言
void InitTremorEffect(void);							//初期化処理
void UninitTremorEffect(void);							//終了処理
void UpdateTremorEffect(void);							//更新処理
void UpdateTremorEffectSize(int nEffect);				//エフェクトのサイズ更新
void DrawTremorEffect(void);							//描画処理
void SetTremorEffect(D3DXVECTOR3 pos);	//エフェクトを設定

#endif

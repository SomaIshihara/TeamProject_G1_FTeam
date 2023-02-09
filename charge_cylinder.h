//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.h]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//このマクロ定義がされてなかったら
#define _CHARGECYLINDER_H_	//2重インクルード防止のマクロを定義する

//マクロ
#define CHARGESYLINDER_RADIUS				(20.0f)	//背景の広さ
#define CHARGESYLINDER_HEIGHT				(10.0f)	//背景の高さ
#define CHARGESYLINDER_SPLIT				(32)	//背景の頂点数
#define CHARGESYLINDER_TEX_RESOLUTION		(3.0f)	//背景の解像度

//プロトタイプ宣言
void InitChargeCylinder(void);					// 初期化処理
void SetChargeCylinderVertex(void);				// 頂点情報の設定処理
void SetChargeCylinderIndex(void);				// インデックス番号の設定処理
void UninitChargeCylinder(void);				// 終了処理
void UpdateChargeCylinder(void);				// 更新処理
void SpreadChargeCylinder(int nCntCylinder);	// チャージシリンダーの広がる処理
void DrawChargeCylinder(void);					// 描画処理

#endif
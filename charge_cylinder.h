//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.h]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//このマクロ定義がされてなかったら
#define _CHARGECYLINDER_H_	//2重インクルード防止のマクロを定義する

//マクロ
#define CHARGESYLINDER_WIDTH_MAX			(20.0f)	//背景の広さ最大
#define CHARGESYLINDER_WIDTH				(10.0f)	//透明化開始判定の広さ
#define CHARGESYLINDER_HEIGHT				(10.0f)	//背景の高さ
#define CHARGESYLINDER_SPLIT				(32)		//背景の頂点数
#define CHARGESYLINDER_TEX_RESOLUTION		(3.0f)		//背景の解像度

//プロトタイプ宣言
void InitChargeCylinder(void);
void SetChargeCylinderVertex(int nCntCylinder);
void SetChargeCylinderIndex(void);
void UninitChargeCylinder(void);
void UpdateChargeCylinder(void);
void UpdateChargeCylinderSize(int nEffect);					//エフェクトのサイズ更新
void DrawChargeCylinder(void);
void SetChargeCylinderPos(void);
void SetChargeCylinder(D3DXVECTOR3 pos, int nCntType);

#endif
//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.h]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//このマクロ定義がされてなかったら
#define _CHARGECYLINDER_H_	//2重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitChargeCylinder(void);					// 初期化処理
void SetChargeCylinderVertex(int nCntCylinder);	// 頂点情報の設定処理
void SetChargeCylinderIndex(int nCntCylinder);	// インデックス番号の設定処理

void UninitChargeCylinder(void);			// 終了処理

void UpdateChargeCylinder(void);				// 更新処理
void SetChargeCylinderPos(int nCntPos);			// プレイヤーの位置に設定する
void SpreadChargeCylinder(int nCntSpread);		// チャージシリンダーの広がる処理
void ExtendChargeCylinder(int nCntExtend);		// チャージシリンダーの伸び縮みする処理

void DrawChargeCylinder(void);		// 描画処理

void SetChargeCylinder(D3DXVECTOR3 pos, int nCntSet);		// シリンダーを設定

#endif
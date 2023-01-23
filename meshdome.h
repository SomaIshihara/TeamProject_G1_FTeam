//--------------------------------------------------------------------------------------------------------
//
//メッシュドームの屋根・奈落[meshdome.h]
//Author:平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHDOME_H_	//このマクロ定義がされてなかったら
#define _MESHDOME_H_	//2重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitMeshDome(void);
void SetMeshDomeVertexBuffer(void);		//頂点座標の設定
 void SetMeshDomeIndexBuffer(void);		//インデックス番号の設定
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);

#endif
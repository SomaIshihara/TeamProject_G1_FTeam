//--------------------------------------------------------------------------------------------------------
//
//メッシュの断面図[meshfault.h]
//Author:菊地陽人	平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHFAULT_H_	//このマクロ定義がされてなかったら
#define _MESHFAULT_H_	//2重インクルード防止のマクロを定義する

//マクロ
#define MESHFAULT_WIDTH				(2000.0f)	//背景の広さ
#define MESHFAULT_BOTTOM			(-1000.0f)	//背景の足元
#define MESHFAULT_SPLIT				(32)		//背景の頂点数
#define MESHFAULT_TEX_RESOLUTION	(3.0f)		//背景の解像度

//プロトタイプ宣言
void InitMeshFault(void);
void UninitMeshMeshFault(void);
void UpdateMeshMeshFault(void);
void DrawMeshFault(void);

#endif
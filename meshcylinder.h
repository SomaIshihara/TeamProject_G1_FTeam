//--------------------------------------------------------------------------------------------------------
//
//メッシュシリンダーの背景[meshcylinder.h]
//Author:菊地陽人
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHCYLONDER_H_	//このマクロ定義がされてなかったら
#define _MESHCYLONDER_H_	//2重インクルード防止のマクロを定義する

#include"main.h"

//マクロ
#define MESHSYLINDER_WIDTH		(25.0f)		//背景の広さ
#define MESHSYLINDER_HEIGHT		(25.0f)		//背景の高さ
#define MESHSYLINDER_SPLIT		(8)		//背景の高さ

//プロトタイプ宣言
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif

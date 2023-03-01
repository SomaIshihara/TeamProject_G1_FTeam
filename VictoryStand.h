/*==========================================================================================================================================================

表彰台処理[VictoryStand.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _VICTORYSTAND_H_
#define _VICTORYSTAND_H_

//表彰台の情報構造体
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	float			fLandheight;	// 着地する高さ
	bool			bUse;			// 使用するかどうか
}VictoryStand;

void InitVictoryStand(void);
void InitVicStdMeshMatrix(int nCntMetrix);		//metrix = "mesh" + "matrix" の造語
void SearchVictoryStand_Land_Pos(int nCntVicStd, float *pfLandHeight);
void UninitVictoryStand(void);
void UpdateVictoryStand(void);
void DrawVictoryStand(void);

bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove);
void SetVictoryStand(D3DXVECTOR3 Setpos);

#endif
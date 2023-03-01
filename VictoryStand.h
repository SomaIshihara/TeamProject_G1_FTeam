/*==========================================================================================================================================================

�\���䏈��[VictoryStand.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _VICTORYSTAND_H_
#define _VICTORYSTAND_H_

//�\����̏��\����
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	float			fLandheight;	// ���n���鍂��
	bool			bUse;			// �g�p���邩�ǂ���
}VictoryStand;

void InitVictoryStand(void);
void InitVicStdMeshMatrix(int nCntMetrix);		//metrix = "mesh" + "matrix" �̑���
void SearchVictoryStand_Land_Pos(int nCntVicStd, float *pfLandHeight);
void UninitVictoryStand(void);
void UpdateVictoryStand(void);
void DrawVictoryStand(void);

bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove);
void SetVictoryStand(D3DXVECTOR3 Setpos);

#endif
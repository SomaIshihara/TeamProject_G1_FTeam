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
	int				nRank;			// ���ʂ��i���f���`��ł��g�p����
	bool			bAppear;		// �o�����邩�ǂ���
	bool			bUse;			// �g�p���邩�ǂ���
}VictoryStand;

void RankingSort(void);
void InitVictoryStand(void);
void InitVicStdModel_Tex(int nCntModex);	//modex = "model" + "tex" �̑���
void SearchVictoryStand_Land_Pos(void);
void UninitVictoryStand(void);
void UpdateVictoryStand(void);
void DrawVictoryStand(void);

bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold);
void SetVictoryStand(D3DXVECTOR3 Setpos);
void SetAppearVictoryStand(void);
void SkipVicStd_Player(void);

#endif
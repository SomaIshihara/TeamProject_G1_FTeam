/*==========================================================================================================================================================

�Q�[�W�̏���[gauge.h]
Author:��{����

============================================================================================================================================================*/
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

//�Q�[�W�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Gauge;

//�Q�[�W�̗�
typedef enum
{
	GAUGETYPE_BG = 0,
	GAUGETYPE_NORMAL,
	GAUGETYPE_FRAME,
	GAUGETYPE_MAX
}GAUGETYPE;

void InitGauge(void);		//����������
void UninitGauge(void);		//�I������
void UpdateGauge(void);		//�X�V����
void DrawGauge(void);		//�`�揈��

#endif
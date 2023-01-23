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
	D3DCOLOR col;			//�F	
	float fGaugeWidth;		//�Q�[�W�̕�
	float fWidth;			//��(�g�A�w�i)
	float fHeight;			//����(�g�A�w�i)
	bool bUse;				//�g�p���Ă��邩�ǂ���
} Gauge;

void InitGauge(void);		//����������
void UninitGauge(void);		//�I������
void UpdateGauge(void);		//�X�V����
void DrawGauge(void);		//�`�揈��

#endif

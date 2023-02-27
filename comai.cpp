//==========================================
//
//�R���s���[�^�[��AI�v���O����[comai.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "comai.h"
#include "PvP_player.h"
#include "input.h"
#include "meshfield.h"
#include "conversioninput.h"
#include <time.h>

//�}�N��
#define CHARGE_AVG			(1.0f)	//�`���[�W�̊�l
#define CHARGE_DIGIT		(100)	//�`���[�W�̏����_�̈ʒu�w��
#define HIPDROP_RADIUS		(100.0f)	//��������q�b�v�h���b�v�܂ł̋���
#define HIPDROP_RAND_DIGIT	(100)		//�q�b�v�h���b�v����m������
#define CHARGE_WAIT			(75)

//�v���g
void ChargeAI(Player *pCom);

//�O���[�o��
ComAIBrain g_aiCont[MAX_USE_GAMEPAD];
bool g_bUseAIPointer[MAX_USE_GAMEPAD];

const AIParam c_aAIParam[AIDIFF_MAX] = 
{
	{ 0.25f, 0.50f, 50, 10 },	//EASY
	{ 0.17f, 0.25f, 75, 5 },	//NORMAL
	{ 0.10f, 0.15f, 100, 5 }	//HARD
};

//========================
//����������
//========================
void InitComAI(void)
{
	srand((unsigned int)time(NULL));	//�����̏�����

	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{//�]�݂�������ۂɂ���
		g_aiCont[nCntAI] = {};
		g_aiCont[nCntAI].nCounterWaitTime = 0;
		g_aiCont[nCntAI].difficulty = AIDIFF_NORMAL;
		g_aiCont[nCntAI].bDoHipdrop = false;
		g_aiCont[nCntAI].bHipdropped = false;
		g_bUseAIPointer[nCntAI] = false;
	}
}

//========================
//�I������
//========================
void UninitComAI(void)
{
	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{//AI���g���Ă��Ȃ����Ƃɂ���
		g_bUseAIPointer[nCntAI] = false;
	}
}

//========================
//�I������
//========================
void SelectAIMove(Player *pCom)
{
	Player *pPlayer = GetPlayer();
	Player *pNearPlayer = NULL;
	float fMinLength = -1.0f;

	if (pCom->stat != PLAYERSTAT_FALL)
	{//�����Ă��Ȃ�
		SetButton(pCom->nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X, false);

		if (pCom->stat != PLAYERSTAT_DASH && pCom->stat != PLAYERSTAT_HIPDROP)
		{
			//��ԋ߂��v���C���[�̕��������悤�ɂ���
			//�߂��v���C���[��T��
			for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
			{
				if ((pPlayer + nCntPlayer) != pCom)
				{//�����łȂ����
				 //�����v�Z
					float fLength = PYTHAGORAS(((pPlayer + nCntPlayer)->pos.x - pCom->pos.x), ((pPlayer + nCntPlayer)->pos.z - pCom->pos.z));

					if (pNearPlayer == NULL || fMinLength >= fLength)
					{//���������Ă��Ȃ� �܂��� �����Ƌ߂��v���C���[������
					 //�����Ƌ߂��v���C���[�̃|�C���^���
						fMinLength = fLength;
						pNearPlayer = (pPlayer + nCntPlayer);
					}
				}
			}

			//�p�x��ς���i2DSTG�̃z�[�~���O�Z�p�g�p�j
			float fRotMove = pCom->rot.y;
			float fRotDest = atan2f(pNearPlayer->pos.x - pCom->pos.x, pNearPlayer->pos.z - pCom->pos.z);
			float fRotDiff = FIX_ROT(fRotDest - fRotMove + D3DX_PI);
			if ((int)(fabsf(fRotDiff) * 10) == 0)//10�ɂ��Ȃ��ƌ��������ăv���v������
			{
				SetStick(pCom->nPlayerNum, CONVSTICK_NEUTRAL);
				pCom->pAI->nCounterWaitTime++;
			}
			else
			{
				//���f
				if ((int)copysign(1, fRotDiff) < 0)
				{
					SetStick(pCom->nPlayerNum, CONVSTICK_LEFT);
				}
				else
				{
					SetStick(pCom->nPlayerNum, CONVSTICK_RIGHT);
				}

				//�ҋ@���ԃ��Z�b�g
				pCom->pAI->nCounterWaitTime = 0;
			}
		}

		//�X�e�B�b�N��������Ă��Ȃ��Ȃ�`���[�W
		if (GetStick(pCom->nPlayerNum) == CONVSTICK_NEUTRAL && pCom->pAI->nCounterWaitTime >= CHARGE_WAIT)
		{
			int nChargeWidth = (int)(((CHARGE_AVG + c_aAIParam[pCom->pAI->difficulty].fChargeAboutPlus) - (CHARGE_AVG - c_aAIParam[pCom->pAI->difficulty].fChargeAboutMinus)) * CHARGE_DIGIT);
			pCom->pAI->fChargePower = (float)(rand() % (nChargeWidth + 1)) / CHARGE_DIGIT + c_aAIParam[pCom->pAI->difficulty].fChargeAboutMinus;
			ChargeAI(pCom);

		}
		else if (GetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X) == true)
		{
			ChargeAI(pCom);
		}

		//���������Ȃ�q�b�v�h���b�v���
		float fRadiusAI = PYTHAGORAS(pCom->pos.x, pCom->pos.z);

		//���������Ȃ�W�����v
		if (GetMeshField()->fRadius - fRadiusAI <= HIPDROP_RADIUS)
		{
			if (pCom->pAI->bHipdropped == false)
			{
				pCom->pAI->bDoHipdrop = rand() % (HIPDROP_RAND_DIGIT + 1) <= c_aAIParam[pCom->pAI->difficulty].nHipdropRandom ? true : false;
				if (pCom->pAI->bDoHipdrop == true || pCom->stat == PLAYERSTAT_JUMP)
				{
					SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, true);
					if (pCom->stat == PLAYERSTAT_JUMP)
					{
						if (pCom->jumpTime > c_aAIParam[pCom->pAI->difficulty].nHipdropTime)
						{
							pCom->pAI->bHipdropped = true;
						}
						else
						{
							SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, true);
						}
					}
				}
				else
				{
					pCom->pAI->bHipdropped = true;
				}
			}
			else
			{
				SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, false);
			}
		}
		else
		{
			pCom->pAI->bHipdropped = false;
			SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, false);
		}
	}
	else
	{//�������̂�AI���R���g���[���[�����������OMG���Ă���
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, false);
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X, false);
		SetButton(pCom->nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X, false);
		SetStick(pCom->nPlayerNum, CONVSTICK_NEUTRAL);
	}
}

//========================
//�`���[�W�{�^������������
//========================
void ChargeAI(Player *pCom)
{
	if (pCom->moveGauge >= pCom->pAI->fChargePower)
	{
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X, false);
		SetButton(pCom->nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X, true);
		pCom->pAI->nCounterWaitTime = 0;
	}
	else
	{
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X, true);
	}
}

//========================
//�I������
//========================
ComAIBrain *GetAI(void)
{
	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{
		if (g_bUseAIPointer[nCntAI] == false)
		{
			g_bUseAIPointer[nCntAI] = true;
			return &g_aiCont[nCntAI];
		}
	}
	return NULL;
}
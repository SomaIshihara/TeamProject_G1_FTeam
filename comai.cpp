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
#define CHARGE_LENGTH_AVG	(600.0f)	//�`���[�W�̈ړ������̊�l
#define CHARGE_DIGIT		(100)		//�`���[�W�̏����_�̈ʒu�w��
#define HIPDROP_RADIUS		(100.0f)	//��������q�b�v�h���b�v�܂ł̋���
#define HIPDROP_RAND_DIGIT	(100)		//�q�b�v�h���b�v����m������
#define CHARGE_WAIT			(75)		//�`���[�W����O�̑ҋ@����
#define ONE_MORE_PUSH		(0.31f)		//�v���C���[�𗎂Ƃ����߂ɂ�������ꉟ��

//�v���g
void RotateAI(Player *pCom);
void ChargeAI(Player *pCom);

//�O���[�o��
ComAIBrain g_aAIBrain[MAX_USE_GAMEPAD];
bool g_bUseAIPointer[MAX_USE_GAMEPAD];

const AIParam c_aAIParam[AIDIFF_MAX] = 
{
	{ 0.50f, 0.25f, 30, 20 },	//EASY
	{ 0.25f, 0.17f, 60, 15 },	//NORMAL
	{ 0.15f, 0.10f, 90, 10 },	//HARD
	{ 0.00f, 0.50f, 0, 0 }		//VIEW
};

//========================
//����������
//========================
void InitComAI(void)
{
	srand((unsigned int)time(NULL));	//�����̏�����

	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{//�]�݂�������ۂɂ���
		g_aAIBrain[nCntAI] = {};
		g_aAIBrain[nCntAI].nCounterWaitTime = 0;
		g_aAIBrain[nCntAI].difficulty = AIDIFF_NORMAL;
		g_aAIBrain[nCntAI].bDoHipdrop = false;
		g_aAIBrain[nCntAI].bHipdropped = false;
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
	if (pCom->stat != PLAYERSTAT_FALL)
	{//�����Ă��Ȃ�
		RotateAI(pCom);

		//���炩����AI�̒��S����̋������߂�
		float fAILength = fabsf(D3DXVec3Length(&pCom->pos));

		//�X�e�B�b�N��������Ă��Ȃ��Ȃ�`���[�W
		if (pCom->pAI->nCounterWaitTime >= CHARGE_WAIT)
		{
			float fMoveLength = sqrtf(powf(fAILength, 2) + powf(GetMeshField()->fRadius, 2) - 2 * fAILength * GetMeshField()->fRadius * cosf(pCom->rot.y)) / CHARGE_LENGTH_AVG;	//�W���X�g�ȃ`���[�W�ʂ����߂�

			//���ŃA�o�E�g�ɂ��Ă���
			int nChargeWidth = (int)((((fMoveLength + c_aAIParam[pCom->pAI->difficulty].fChargeAboutPlus) - (fMoveLength - c_aAIParam[pCom->pAI->difficulty].fChargeAboutMinus)) * CHARGE_DIGIT) + ONE_MORE_PUSH);
			pCom->pAI->fChargePower = (float)(rand() % (nChargeWidth + 1)) / CHARGE_DIGIT + (fMoveLength - c_aAIParam[pCom->pAI->difficulty].fChargeAboutMinus);

			//�Œ�l�ȉ��Ȃ�Œ�l�ɂ���
			if (pCom->pAI->fChargePower < 1.0f / CHARGE_DIGIT)
			{
				pCom->pAI->fChargePower = 1.0f / CHARGE_DIGIT;
			}
			else if (pCom->pAI->fChargePower > PLAYER_POWER_MAX)
			{
				pCom->pAI->fChargePower = PLAYER_POWER_MAX;
			}

			ChargeAI(pCom);

		}
		else if (GetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X) == true)
		{
			ChargeAI(pCom);
		}

		//���������Ȃ�q�b�v�h���b�v���
		if (GetMeshField()->fRadius - fAILength <= HIPDROP_RADIUS)
		{
			if (pCom->pAI->bHipdropped == false)
			{
				pCom->pAI->bDoHipdrop = rand() % (HIPDROP_RAND_DIGIT + 1) <= c_aAIParam[pCom->pAI->difficulty].nHipdropRandom ? true : false;
				if (pCom->pAI->bDoHipdrop == true || pCom->stat == PLAYERSTAT_JUMP)
				{
					SetButton(pCom->nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A, true);
					if (pCom->stat == PLAYERSTAT_JUMP)
					{
						if (pCom->jumpTime > c_aAIParam[pCom->pAI->difficulty].nHipdropTime)
						{
							pCom->pAI->bHipdropped = true;
						}
						else
						{
							SetButton(pCom->nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A, false);
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
//��]����
//========================
void RotateAI(Player *pCom)
{
	Player *pPlayer = GetPlayer();
	Player *pNearPlayer = NULL;
	float fMinLength = -1.0f;

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
ComAIBrain *GetAI(AIDIFF diff)
{
	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{
		if (g_bUseAIPointer[nCntAI] == false)
		{
			g_bUseAIPointer[nCntAI] = true;
			g_aAIBrain[nCntAI].difficulty = diff;
			return &g_aAIBrain[nCntAI];
		}
	}
	return NULL;
}
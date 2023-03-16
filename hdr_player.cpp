//==========================================
//
//�v���C���[�v���O����[hdr_player.cpp]
//Author:�������P  ���V����
//
//==========================================
#include "main.h"
#include "HDRgame.h"
#include "HDR_player.h"
#include "model.h"
#include "conversioninput.h"
#include "debugproc.h"
#include "camera.h"
#include "HDR_camera.h"
#include "collision.h"
#include <assert.h>
#include "color.h"
#include "sound.h"
#include "meshfield.h"
#include "HipDropRankUI.h"
#include "block.h"
#include "rank.h"
#include "ai.h"
#include "TypeFrame.h"

//�}�N����`
#define PLAYER_HIPSPIN_SPEED	(-0.5f)				//�q�b�v�h���b�v�X�s���̉�]�l
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//�q�b�v�h���b�v�X�s�������Ƃ��̂P��������Ƃ�l
#define PLAYER_HIPSPIN_WAIT		(20)				//�q�b�v�h���b�v�X�s�����I����ċ}�~������܂ł̎���
#define PLAYER_MOVE_SPEED		(5.0f)				//���ʂɈړ�����Ƃ��̈ړ���
#define PLAYER_NORMAL_JUMP		(7.0f)				//�v���C���[�̒ʏ�W�����v��
#define PLAYER_GOAL_JUMP		(4.3f)				//�S�[����̃W�����v��
#define PLAYER_BODY_SIZE		(30.0f)				//�v���C���[�̓��̃T�C�Y

//����
#define ROT_WA	(+0.75f * D3DX_PI)	//����
#define ROT_WD	(-0.75f * D3DX_PI)	//�E��
#define ROT_SA	(+0.25f * D3DX_PI)	//����
#define ROT_SD	(-0.25f * D3DX_PI)	//�E��
#define ROT_W	(-1.0f * D3DX_PI)	//��
#define ROT_A	(+0.5f * D3DX_PI)	//��
#define ROT_S	(+0.0f * D3DX_PI)	//��
#define ROT_D	(-0.5f * D3DX_PI)	//�E

//�O���[�o���ϐ�
Player_HDR g_aPlayerHDR[MAX_USE_GAMEPAD];
int g_nIdxShadow_HDR = -1;

//�v���g�^�C�v�錾
void ControllPlayer_HDR(int nPlayerNum);
void MovePlayer(int nPadNum);

void JumpPlayer_HDR(int nJumpPlayer);			//�W�����v�̐ݒ菈��
void HipDropPlayer_HDR(int nHipDropPlayer);		//�q�b�v�h���b�v�̐ݒ菈��
void HipSpinPlayer_HDR(int nHipSpinPlayer);		//�q�b�v�X�s���̏���

//�����ʒu����
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-225.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(-75.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(75.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(225.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
};

//========================
//����������
//========================
void InitPlayer_HDR(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�ϐ�������
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//�ϐ�������
		g_aPlayerHDR[nCntPlayer].pos = g_aPlayerHDR[nCntPlayer].posOld = c_aPosRot[nCntPlayer][0];
		g_aPlayerHDR[nCntPlayer].move = g_aPlayerHDR[nCntPlayer].moveV0 = g_aPlayerHDR[nCntPlayer].rot = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].jumpTime = g_aPlayerHDR[nCntPlayer].nHipDropWait = 0;
		g_aPlayerHDR[nCntPlayer].bJump = false;
		g_aPlayerHDR[nCntPlayer].bHipDrop = false;
		g_aPlayerHDR[nCntPlayer].bHipDropSpin = false;
		g_aPlayerHDR[nCntPlayer].nRank = RANKING_4TH;
		g_aPlayerHDR[nCntPlayer].bGoal = false;
		g_aPlayerHDR[nCntPlayer].fBodySize = PLAYER_BODY_SIZE;
		g_aPlayerHDR[nCntPlayer].HipDropPower = 0;
		g_aPlayerHDR[nCntPlayer].nDestroyCounter = 0;

		g_aPlayerHDR[nCntPlayer].animal = ANIMAL_WILDBOAR;

		g_aPlayerHDR[nCntPlayer].fAIHeight = 0;
		g_aPlayerHDR[nCntPlayer].aiDiff = GetAIDiff();;
		g_aPlayerHDR[nCntPlayer].nAICT = 20;
		
		g_aPlayerHDR[nCntPlayer].model = GetAnimal(g_aPlayerHDR[nCntPlayer].animal);
	}
}

//========================
//�I������
//========================
void UninitPlayer_HDR(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//�v���C���[�̐�������������
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{//�擾�������Ȃ̂�NULL����邾����OK
			g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh = NULL;
			g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//========================
//�X�V����
//========================
void UpdatePlayer_HDR(void)
{
	if (*GetHDR_Ready() == HDR_Ready_OK)
	{
		//�v���C���[�l�����J��Ԃ�
		for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
		{
			if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
			{
				//�O��̈ʒu��ۑ�
				g_aPlayerHDR[nCntPlayer].posOld.x = g_aPlayerHDR[nCntPlayer].pos.x;
				g_aPlayerHDR[nCntPlayer].posOld.z = g_aPlayerHDR[nCntPlayer].pos.z;

				//�W�����v���Ԃ𑝂₷
				g_aPlayerHDR[nCntPlayer].jumpTime++;

				//AI�̃N�[���^�C�������炷
				g_aPlayerHDR[nCntPlayer].nAICT--;

				if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
				{//�g�p���̂ݍs��
				 //�v���C���[�ł��邩�m�F
				 //�ڑ�����Ă��邩�m�F���Đؒf����Ă�����v���C���[�������i��O�Ƃ��ăR���g���[���[���Ȃ����Ă��Ȃ��Ƃ��͖����j
					if (g_aPlayerHDR[nCntPlayer].bUseAI == false && GetUseControllerNum_HDR() != 0)
					{
						g_aPlayerHDR[nCntPlayer].bUsePlayer = GetUseController_HDR(nCntPlayer);
					}

					//�e�v���C���[�̑���
					ControllPlayer_HDR(nCntPlayer);
				}

				//�q�b�v�h���b�v�X�s���̍d����
				if (0 < g_aPlayerHDR[nCntPlayer].nHipDropWait)
				{
					HipSpinPlayer_HDR(nCntPlayer);
				}
				//�d�����Ŗ�����Η������x����
				else
				{
					g_aPlayerHDR[nCntPlayer].move.y = g_aPlayerHDR[nCntPlayer].moveV0.y - (9.8f * g_aPlayerHDR[nCntPlayer].jumpTime / MAX_FPS);
				}

				//�ړ�
				g_aPlayerHDR[nCntPlayer].pos += g_aPlayerHDR[nCntPlayer].move;

				//�v���C���[���n�ʂ�˂������Ă��܂���
				if (g_aPlayerHDR[nCntPlayer].pos.y < 0.0f)
				{
					g_aPlayerHDR[nCntPlayer].pos.y = 0.0f;
					g_aPlayerHDR[nCntPlayer].move.y = 0.0f;
					g_aPlayerHDR[nCntPlayer].bJump = false;
					g_aPlayerHDR[nCntPlayer].bHipDrop = false;

					//�܂��S�[�����Ă��Ȃ�
					if (!g_aPlayerHDR[nCntPlayer].bGoal)
					{
						g_aPlayerHDR[nCntPlayer].bGoal = true;	//�S�[������
						g_aPlayerHDR[nCntPlayer].nRank = SetRank(nCntPlayer);//���ʐݒ�
					}
				}

				//�u���b�N�̓����蔻��
				CollisionBlock(&g_aPlayerHDR[nCntPlayer]);

				//�t�F���X�Ƃ̓����蔻��
				CollisionFence(&g_aPlayerHDR[nCntPlayer]);

				PrintDebugProc("%dP�̃q�b�v�h���b�v�p���[�F%d\n", nCntPlayer + 1, g_aPlayerHDR[nCntPlayer].HipDropPower);
			}
		}
	}

	PrintDebugProc("�o�t�{���F%f\n", HipDropBuff());
}

//========================
//�`�揈��
//========================
void DrawPlayer_HDR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//�v���C���[�̐������J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
		{
			//"�v���C���[��"���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerHDR[nCntPlayer].rot.y, g_aPlayerHDR[nCntPlayer].rot.x, g_aPlayerHDR[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxRot);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayerHDR[nCntPlayer].pos.x, g_aPlayerHDR[nCntPlayer].pos.y, g_aPlayerHDR[nCntPlayer].pos.z);
			D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxTrans);

			//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].mtxWorld);

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].bUse == true)
				{
					g_aPlayerHDR[nCntPlayer].posOld_Coll.y = g_aPlayerHDR[nCntPlayer].pos.y;

					D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p
					D3DXMATRIX mtxParent;					//�e�̃}�g��

															//���ŃI�t�Z�b�g�����̂܂܎g���i�A�j���[�V�����g���悤�ɂȂ���������āj
					g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].posOffset;
					g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rotOffset;

					//"���f����"���[���h�}�g���b�N�X������
					D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

					//�����𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.z);
					D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxRotModel);

					//�ʒu���f
					D3DXMatrixTranslation(&mtxTransModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.z);
					D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxTransModel);

					//�p�[�c�̐e�}�g���ݒ�
					if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent != -1)
					{
						mtxParent = g_aPlayerHDR[nCntPlayer].model.aParts[g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent].mtxWorld;
					}
					else
					{
						mtxParent = g_aPlayerHDR[nCntPlayer].mtxWorld;
					}

					//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
					D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxParent);

					//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

					//�}�e���A���f�[�^�ւ̃|�C���^�擾
					pMat = (D3DXMATERIAL*)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						//�S�[�X�g�p
						D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

						if (nCntParts == MAX_PARTS - 1)
						{//�[�b�P���̎��͐F�ύX
							matChange.Diffuse = c_aColPlayer[nCntPlayer];
						}

						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&matChange);

						//�e�N�X�`���ݒ�
						pDevice->SetTexture(0, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

						//���f���`��
						g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}
		}
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�v���C���[�̃Q�[���p�b�h����
//========================
void ControllPlayer_HDR(int nPlayerNum)
{
	//�q�b�v�h���b�v���łȂ���Α���ł���
	if (g_aPlayerHDR[nPlayerNum].bHipDrop == false)
	{
		//�W�����v�E�q�b�v�h���b�v
		if (g_aPlayerHDR[nPlayerNum].bUseAI == false)
		{//�v���C���[
			if (GetButton(nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A) == true && g_aPlayerHDR[nPlayerNum].bHipDrop == false)
			{
				if (g_aPlayerHDR[nPlayerNum].bJump && g_aPlayerHDR[nPlayerNum].bGoal == false)
				{//�W�����v���ł���A�S�[�����Ă��Ȃ�
					HipDropPlayer_HDR(nPlayerNum);		//�v���C���[�̃q�b�v�h���b�v����
				}
				else if (g_aPlayerHDR[nPlayerNum].bJump == false)
				{//�W�����v���Ă��Ȃ�
					JumpPlayer_HDR(nPlayerNum);			//�v���C���[�̃W�����v����
				}
			}
		}
		else if (g_aPlayerHDR[nPlayerNum].nAICT <= 0)
		{
			if (g_aPlayerHDR[nPlayerNum].bJump && g_aPlayerHDR[nPlayerNum].bGoal == false)
			{//�W�����v���ł���A�S�[�����Ă��Ȃ�
				//�W�����v�ʂ����ɒB���Ă��邩�m�F����
				if (g_aPlayerHDR[nPlayerNum].pos.y >= g_aPlayerHDR[nPlayerNum].fHeightLastJump + g_aPlayerHDR[nPlayerNum].fAIHeight)
				{
					HipDropPlayer_HDR(nPlayerNum);		//�v���C���[�̃q�b�v�h���b�v����
				}
			}
			else if (g_aPlayerHDR[nPlayerNum].bJump == false)
			{//�W�����v���Ă��Ȃ�
				JumpPlayer_HDR(nPlayerNum);			//�v���C���[�̃W�����v����
				if (g_aPlayerHDR[nPlayerNum].bUseAI == true)
				{//AI�̃W�����v���x���w�菈��
					AIDIFF aiDiff = g_aPlayerHDR[nPlayerNum].aiDiff;
					int nRandAll = c_aAIParamHDR[aiDiff].nHDLevel_1 + c_aAIParamHDR[aiDiff].nHDLevel_2 +
						c_aAIParamHDR[aiDiff].nHDLevel_3 + c_aAIParamHDR[aiDiff].nHDLevel_Max;
					int nJumpRand = rand() % nRandAll;

					//�ǂ̃��x�������ʂ��鏈��
					if (nJumpRand < c_aAIParamHDR[aiDiff].nHDLevel_1)
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_NICE;
					}
					else if (nJumpRand < c_aAIParamHDR[aiDiff].nHDLevel_1 + c_aAIParamHDR[aiDiff].nHDLevel_2)
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_GOOD;
					}
					else if (nJumpRand < c_aAIParamHDR[aiDiff].nHDLevel_1 + c_aAIParamHDR[aiDiff].nHDLevel_2 + c_aAIParamHDR[aiDiff].nHDLevel_3)
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_GREAT;
					}
					else
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_PERFECT;
					}
				}
			}
		}
	}

	//�S�[����̐���s��
	if (g_aPlayerHDR[nPlayerNum].bGoal == true)
	{
		MovePlayer(nPlayerNum);
	}
}

//========================
//�v���C���[�����ʂɈړ����鏈��
//========================
void MovePlayer(int nPadNum)
{
	//���f���ړ�
	//�Q�[���p�b�h��
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X�����̃X�e�B�b�N���X���Ă���
		if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y�����̃X�e�B�b�N���X���Ă���
			g_aPlayerHDR[nPadNum].rot.y = FIX_ROT(atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum)) + D3DX_PI);
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
			g_aPlayerHDR[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = FIX_ROT(atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum)) + D3DX_PI);
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y�����̃X�e�B�b�N�����X���Ă���
		g_aPlayerHDR[nPadNum].rot.y = FIX_ROT(atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum)) + D3DX_PI);
		g_aPlayerHDR[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
	}
	//�L�[�{�[�h��
	else if (GetStick(nPadNum,INPUTTYPE_PRESS).y == CONVSTICK_UP)
	{
		if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_WA;
		}
		else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_WD;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_W;
		}
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).y == CONVSTICK_DOWN)
	{
		if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_SA;
		}
		else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_SD;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_S;
		}
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
	{
		g_aPlayerHDR[nPadNum].rot.y = ROT_A;
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
	{
		g_aPlayerHDR[nPadNum].rot.y = ROT_D;
	}

	//�L�[�������ꂽ��ړ�
	if (GetStick(nPadNum, INPUTTYPE_PRESS).x != CONVSTICK_NEUTRAL || GetStick(nPadNum, INPUTTYPE_PRESS).y != CONVSTICK_NEUTRAL)
	{
		g_aPlayerHDR[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerHDR[nPadNum].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_aPlayerHDR[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerHDR[nPadNum].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
	}
	else
	{//�����łȂ��̂ŏI��
		return;
	}

	//�{�^������ɉ����ăv���C���[�E�J�������_�E�����_�ړ�
	g_aPlayerHDR[nPadNum].pos.x += g_aPlayerHDR[nPadNum].move.x;
	g_aPlayerHDR[nPadNum].pos.z += g_aPlayerHDR[nPadNum].move.z;

	//�ړ��ʏ���
	g_aPlayerHDR[nPadNum].move.x = 0.0f;
	g_aPlayerHDR[nPadNum].move.z = 0.0f;
}

//========================
//�W�����v�̏���
//========================
void JumpPlayer_HDR(int nJumpPlayer)
{
	PlaySoundSE(SOUND_LABEL_SE_JUMP,nJumpPlayer);

	g_aPlayerHDR[nJumpPlayer].posOld.y = g_aPlayerHDR[nJumpPlayer].pos.y;
	g_aPlayerHDR[nJumpPlayer].jumpTime = 0;	//�W�����v���ԃ��Z�b�g
	g_aPlayerHDR[nJumpPlayer].fHeightLastJump = g_aPlayerHDR[nJumpPlayer].pos.y;
	g_aPlayerHDR[nJumpPlayer].bJump = true;

	//�S�[�����Ă������̃W�����v��
	if (g_aPlayerHDR[nJumpPlayer].bGoal)
	{
		g_aPlayerHDR[nJumpPlayer].moveV0.y = PLAYER_GOAL_JUMP;
	}
	//�܂��S�[�����Ă��Ȃ��Ƃ��̃W�����v��
	else
	{
		g_aPlayerHDR[nJumpPlayer].moveV0.y = PLAYER_NORMAL_JUMP;
	}
}

//========================
//�q�b�v�h���b�v�̏���
//========================
void HipDropPlayer_HDR(int nHipDropPlayer)
{
	//�v���C���[�̃W�����v�ʂ�ۑ�
	float fJumpHeight = g_aPlayerHDR[nHipDropPlayer].pos.y - g_aPlayerHDR[nHipDropPlayer].fHeightLastJump;

	//�q�b�v�h���b�v�ʍő�
	if (fJumpHeight >= HIPDROP_HEIGHT_PERFECT)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_PERFECT;	//�ő�p���[���
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_PERFECT);					//UI�ݒ�
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_04_PERFECT, nHipDropPlayer);		//�q�b�v�h���b�v�����N�̃T�E���h�Đ�
	}

	//�q�b�v�h���b�v�ʏ��ő�
	else if (fJumpHeight >= HIPDROP_HEIGHT_GREAT)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_GREAT;	//���ő�p���[���
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_GREAT);						//UI�ݒ�
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_03_GREAT, nHipDropPlayer);		//�q�b�v�h���b�v�����N�̃T�E���h�Đ�
	}

	//�q�b�v�h���b�v�ʎ��
	else if (fJumpHeight >= HIPDROP_HEIGHT_GOOD)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_GOOD;	//��߂̃p���[���
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_GOOD);					//UI�ݒ�
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_02_GOOD, nHipDropPlayer);	//�q�b�v�h���b�v�����N�̃T�E���h�Đ�
	}

	//�q�b�v�h���b�v�ʂ��Ȃ���
	else if	(fJumpHeight >= HIPDROP_HEIGHT_NICE)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_NICE;	//�Ŏ�p���[���
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_NICE);					//UI�ݒ�
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_01_NICE, nHipDropPlayer);		//�q�b�v�h���b�v�����N�̃T�E���h�Đ�
	}

	//�q�b�v�h���b�v�ʃi�V
	else
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = 0;	//�З̓i�V
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_BAD);	//UI�ݒ�
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_00_BAD, nHipDropPlayer);		//�q�b�v�h���b�v�����N�̃T�E���h�Đ�
	}

	//���Ԍo�߂̃o�t�i�͂�{�Ɂj
	g_aPlayerHDR[nHipDropPlayer].HipDropPower *= HipDropBuff();

	PlaySoundSE(SOUND_LABEL_SE_HIPSPIN, nHipDropPlayer);//�q�b�v�h���b�v���Đ�

	g_aPlayerHDR[nHipDropPlayer].move.y = 0.0f;						//�ʏ�̗������x���O�ɂ���
	g_aPlayerHDR[nHipDropPlayer].moveV0.y = -15.0f;					//�q�b�v�h���b�v�̍~�����x����
	g_aPlayerHDR[nHipDropPlayer].jumpTime = 0;						//�W�����v���ԃ��Z�b�g
	g_aPlayerHDR[nHipDropPlayer].bHipDrop = true;					//�q�b�v�h���b�v���Ă���
	g_aPlayerHDR[nHipDropPlayer].bHipDropSpin = true;				//�X�s�����Ă���
	g_aPlayerHDR[nHipDropPlayer].nHipDropWait = PLAYER_HIPSPIN_WAIT;//�q�b�v�h���b�v�X�s����̍d�����Ԑݒ�
}

//========================
//�q�b�v�X�s���̏���
//========================
void HipSpinPlayer_HDR(int nHipSpinPlayer)
{
	//�q�b�v�h���b�v�̃X�s����	
	if (g_aPlayerHDR[nHipSpinPlayer].bHipDropSpin)
	{
		//�O�]������				
		g_aPlayerHDR[nHipSpinPlayer].rot.x += PLAYER_HIPSPIN_SPEED;

		//�P������				
		if (g_aPlayerHDR[nHipSpinPlayer].rot.x <= PLAYER_HIPSPIN_LAP)
		{
			g_aPlayerHDR[nHipSpinPlayer].rot.x = 0.0f;			//��]�l�����ɖ߂�
			g_aPlayerHDR[nHipSpinPlayer].bHipDropSpin = false;	//�X�s�����I�����		
		}
	}

	//�q�b�v�h���b�v�d�����Ԃ�����
	else
	{
		g_aPlayerHDR[nHipSpinPlayer].nHipDropWait--;		//�d�����Ԃ����炵�Ă���
	}
}

//========================
//�v���C���[�w�菈��
//========================
void SetPlayerType_HDR(int nPlayerNum, bool bUse, bool bAIUse)
{
	//�v���C���[�g�p���邩�w��
	g_aPlayerHDR[nPlayerNum].bUsePlayer = bUse;

	//�v���C���[�g�p����Ȃ�AI�w��
	if (g_aPlayerHDR[nPlayerNum].bUsePlayer == true)
	{
		g_aPlayerHDR[nPlayerNum].bUseAI = bAIUse;
		//g_aAIMove_HDR[nPlayerNum] = bAIUse;
	}
	else
	{//�g�p���Ȃ��Ȃ�AI���g�p���Ȃ�
		g_aPlayerHDR[nPlayerNum].bUseAI = false;
		//g_aAIMove_HDR[nPlayerNum] = false;
	}
}

//========================
//�擾����
//========================
Player_HDR *GetPlayer_HDR(void)
{
	return &g_aPlayerHDR[0];
}
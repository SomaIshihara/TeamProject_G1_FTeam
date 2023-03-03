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
#include <assert.h>
#include "color.h"
#include "sound.h"
#include "meshfield.h"
#include "block.h"
#include "rank.h"

//�}�N����`
#define PLAYER_HIPSPIN_SPEED	(-0.5f)				//�q�b�v�h���b�v�X�s���̉�]�l
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//�q�b�v�h���b�v�X�s�������Ƃ��̂P��������Ƃ�l
#define PLAYER_HIPSPIN_WAIT		(20)				//�q�b�v�h���b�v�X�s�����I����ċ}�~������܂ł̎���
#define PLAYER_MOVE_SPEED	(5.0f)				//���ʂɈړ�����Ƃ��̈ړ���

//�q�b�v�h���b�v���x���ɕK�v�ȍ����i���̒l�ȏ㎟�̃��x���̒l�ȉ��j
#define HIPDROP_HEIGHT_LEVEL_1		(10)				//���x��1
#define HIPDROP_HEIGHT_LEVEL_2		(50)				//���x��2
#define HIPDROP_HEIGHT_LEVEL_3		(100)				//���x��3
#define HIPDROP_HEIGHT_LEVEL_MAX	(150)				//���x��MAX

//�q�b�v�h���b�v���x���ɉ���������
#define HIPDROP_POWER_LEVEL_1		(100)				//���x��1�i100�_���[�W�j
#define HIPDROP_POWER_LEVEL_2		(200)				//���x��2�i200�_���[�W�j
#define HIPDROP_POWER_LEVEL_3		(300)				//���x��3�i300�_���[�W�j
#define HIPDROP_POWER_LEVEL_MAX		(500)				//���x��MAX�i500�_���[�W�j

//����
#define ROT_WA	(0.75f * D3DX_PI)	//����
#define ROT_WD	(-0.75f * D3DX_PI)	//�E��
#define ROT_SA	(0.25f * D3DX_PI)	//����
#define ROT_SD	(-0.25f * D3DX_PI)	//�E��
#define ROT_W	(-1.0f * D3DX_PI)	//��
#define ROT_A	(0.5f * D3DX_PI)	//��
#define ROT_S	(0.0f * D3DX_PI)	//��
#define ROT_D	(-0.5f * D3DX_PI)	//�E

//�O���[�o���ϐ�
Player_HDR g_aPlayerHDR[MAX_USE_GAMEPAD];
int g_nIdxShadow_HDR = -1;
bool g_buse[MAX_USE_GAMEPAD] = { false };

//�v���g�^�C�v�錾
void ControllPlayer_HDR(int nPlayerNum);
void MovePlayer(int nPadNum);

void JumpPlayer_HDR(int nJumpPlayer);			//�W�����v�̐ݒ菈��
void HipDropPlayer_HDR(int nHipDropPlayer);		//�q�b�v�h���b�v�̐ݒ菈��
void HipSpinPlayer_HDR(int nHipSpinPlayer);		//�q�b�v�X�s���̏���

//�����ʒu����
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-225.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(-75.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(75.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(225.0f,8020.0f,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
};

//[�f�o�b�O�p]AI����������v���C���[�w��i�R���g���[���[���h�����Ă���΂����D��j
const bool c_aAIMove[MAX_USE_GAMEPAD] = { false,true,true,true };

//========================
//����������
//========================
void InitPlayer_HDR(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

												//�ϐ�������
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//�ϐ�������
		g_aPlayerHDR[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayerHDR[nCntPlayer].pos.y = MAX_BLOCK * COLLISION_SIZE_Y;	//��ԏ�̃u���b�N�̈ʒu�ɐݒ�
		g_aPlayerHDR[nCntPlayer].posOld = g_aPlayerHDR[nCntPlayer].pos;
		g_aPlayerHDR[nCntPlayer].move = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].moveV0 = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].rot = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].jumpTime = 0;
		g_aPlayerHDR[nCntPlayer].nHipDropWait = 0;
		g_aPlayerHDR[nCntPlayer].bJump = false;
		g_aPlayerHDR[nCntPlayer].bHipDrop = false;
		g_aPlayerHDR[nCntPlayer].bHipDropSpin = false;
		g_aPlayerHDR[nCntPlayer].bGoal = false;
		g_buse[nCntPlayer] = false;

		g_aPlayerHDR[nCntPlayer].animal = ANIMAL_WILDBOAR;

		g_aPlayerHDR[nCntPlayer].model = GetAnimal(g_aPlayerHDR[nCntPlayer].animal);
		g_aPlayerHDR[nCntPlayer].bUsePlayer = GetUseController_HDR(nCntPlayer);
	}

	//[�f�o�b�O]�R���g���[���[���ڑ�����Ă��Ȃ����1P�̂ݗL��������
	if (GetUseControllerNum_HDR() == 0)
	{
		g_aPlayerHDR[0].bUsePlayer = true;
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
	//�v���C���[�l�����J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//�W�����v���Ԃ𑝂₷
		g_aPlayerHDR[nCntPlayer].jumpTime++;

		if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
		{//�g�p���̂ݍs��

			//�ڑ�����Ă��邩�m�F���Đؒf����Ă�����v���C���[�������i��O�Ƃ��ăR���g���[���[���Ȃ����Ă��Ȃ��Ƃ��͖����j
			if (GetUseControllerNum_HDR() != 0)
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

			if (g_buse[nCntPlayer] == false)
			{//�S�[������
				g_aPlayerHDR[nCntPlayer].bGoal = true;
				UpdateRank(nCntPlayer);
				g_buse[nCntPlayer] = true;
			}
		}

		//�u���b�N�̓����蔻��i��U�S�[����͓����蔻��Ȃ��j
		if (g_aPlayerHDR[nCntPlayer].bGoal == false)
		{
			CollisionBlock(nCntPlayer);
		}
	}
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

				/*------------------------------------------------------------------
				�e�̕`��		Author:���V���� �Ό��D�n
				--------------------------------------------------------------------*/
				if (g_aPlayerHDR[nCntPlayer].pos.y >= 0.0f)
				{
					D3DXMATRIX	mtxShadow;		//�V���h�E�}�g���b�N�X
					D3DLIGHT9	light;			//���C�g���
					D3DXVECTOR4	posLight;		//���C�g�̈ʒu
					D3DXVECTOR3	pos, normal;	//���ʏ�̔C�ӂ̓_�A�@���x�N�g��
					D3DXPLANE	plane;			//���ʏ��

												//���C�g�̈ʒu��ݒ�
					pDevice->GetLight(0, &light);
					posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

					//���ʏ��𐶐�
					pos = ZERO_SET;
					normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					D3DXPlaneFromPointNormal(&plane, &pos, &normal);

					//�V���h�E�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtxShadow);

					//�V���h�E�}�g���b�N�X�̍쐬
					D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
					D3DXMatrixMultiply(&mtxShadow, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxShadow);

					//�V���h�E�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

					//�}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL *)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//�}�e���A���f�[�^����

																		//���F�ɐݒ�						//���Ȕ����𖳂���
						MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

						//�}�e���A���ݒ�
						pDevice->SetMaterial(&MatCopy);

						//�e�N�X�`���ݒ�
						pDevice->SetTexture(0, NULL);

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
			g_aPlayerHDR[nPadNum].rot.y = atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum));
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
			g_aPlayerHDR[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum));
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y�����̃X�e�B�b�N�����X���Ă���
		g_aPlayerHDR[nPadNum].rot.y = atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum));
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
	PlaySound(SOUND_LABEL_SE_JUMP);

	g_aPlayerHDR[nJumpPlayer].posOld.y = g_aPlayerHDR[nJumpPlayer].pos.y;
	g_aPlayerHDR[nJumpPlayer].moveV0.y = 7.7f;		//�ړ��ʐݒ�
	g_aPlayerHDR[nJumpPlayer].jumpTime = 0;			//�W�����v���ԃ��Z�b�g
	g_aPlayerHDR[nJumpPlayer].bJump = true;
}

//========================
//�q�b�v�h���b�v�̏���
//========================
void HipDropPlayer_HDR(int nHipDropPlayer)
{
	//�q�b�v�h���b�v�̃p���[���x���𑪒�
	g_aPlayerHDR[nHipDropPlayer].HipDropPower = g_aPlayerHDR[nHipDropPlayer].pos.y - g_aPlayerHDR[nHipDropPlayer].posOld.y;

	if (g_aPlayerHDR[nHipDropPlayer].HipDropPower >= HIPDROP_HEIGHT_LEVEL_MAX)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_MAX;
	}
	else if (g_aPlayerHDR[nHipDropPlayer].HipDropPower > HIPDROP_HEIGHT_LEVEL_3)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_3;
	}
	else if (g_aPlayerHDR[nHipDropPlayer].HipDropPower > HIPDROP_HEIGHT_LEVEL_2)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_2;
	}
	else if (g_aPlayerHDR[nHipDropPlayer].HipDropPower > HIPDROP_HEIGHT_LEVEL_1)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_LEVEL_1;
	}

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
//�擾����
//========================
Player_HDR *GetPlayer_HDR(void)
{
	return &g_aPlayerHDR[0];
}
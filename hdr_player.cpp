#include "main.h"
#include "HDRgame.h"
#include "HDR_player.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "sound.h"
#include "meshfield.h"
#include "block.h"

//�O���[�o���ϐ�
Player_HDR g_aPlayerHDR[MAX_USE_GAMEPAD];
int g_nIdxShadow_HDR = -1;

//�v���g�^�C�v�錾
void ControllKeyboardPlayer_HDR(int nPlayerNum);
void ControllGPadPlayer_HDR(int nPlayerNum);

void JumpPlayer_HDR(int nJumpPlayer);
void HipDropPlayer_HDR(int nHipDropPlayer);		//�q�b�v�h���b�v����

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
		g_aPlayerHDR[nCntPlayer].bJump = false;
		g_aPlayerHDR[nCntPlayer].bHipDrop = false;
		g_aPlayerHDR[nCntPlayer].bHipDropSpin = false;
		g_aPlayerHDR[nCntPlayer].nHipDropWait = 0;

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
			else
			{
				ControllKeyboardPlayer_HDR(nCntPlayer);
			}

			//�e�v���C���[�̑���
			ControllGPadPlayer_HDR(nCntPlayer);

		}

		////�g�p����Ă��邩�ɂ�����炸�s��
		//g_aPlayerHDR[nCntPlayer].move.y = -15;

		//�W�����v�ʐݒ�
		if (g_aPlayerHDR[nCntPlayer].bHipDrop == true)
		{

		}
		else
		{
			g_aPlayerHDR[nCntPlayer].move.y = g_aPlayerHDR[nCntPlayer].moveV0.y - (9.8f * g_aPlayerHDR[nCntPlayer].jumpTime / MAX_FPS);
		}

		//���ʂɈړ�
		g_aPlayerHDR[nCntPlayer].pos += g_aPlayerHDR[nCntPlayer].move;

		if (g_aPlayerHDR[nCntPlayer].pos.y < 0)
		{
			g_aPlayerHDR[nCntPlayer].pos.y = 0;
			g_aPlayerHDR[nCntPlayer].move.y = 0;
			g_aPlayerHDR[nCntPlayer].bJump = false;
			g_aPlayerHDR[nCntPlayer].bHipDrop = false;
		}

		CollisionBlock(nCntPlayer);
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
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

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
//�v���C���[�̃L�[�{�[�h����
//========================
void ControllKeyboardPlayer_HDR(int nPlayerNum)
{
	//�q�b�v�h���b�v���łȂ���Α���ł���
	if (g_aPlayerHDR[nPlayerNum].bHipDrop == false)
	{
		//�W�����v�E�q�b�v�h���b�v
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_aPlayerHDR[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayerHDR[nPlayerNum].bJump)
			{
				HipDropPlayer_HDR(nPlayerNum);		//�v���C���[�̃q�b�v�h���b�v����
			}
			else
			{
				JumpPlayer_HDR(nPlayerNum);			//�v���C���[�̃W�����v����
			}
		}
	}
}
//========================
//�v���C���[�̃Q�[���p�b�h����
//========================
void ControllGPadPlayer_HDR(int nPlayerNum)
{
	//�q�b�v�h���b�v���łȂ���Α���ł���
	if (g_aPlayerHDR[nPlayerNum].bHipDrop == false)
	{
		//�W�����v�E�q�b�v�h���b�v
		if (GetGamepadTrigger(nPlayerNum, XINPUT_GAMEPAD_A) == true && g_aPlayerHDR[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayerHDR[nPlayerNum].bJump)
			{
				HipDropPlayer_HDR(nPlayerNum);		//�v���C���[�̃q�b�v�h���b�v����
			}
			else
			{
				JumpPlayer_HDR(nPlayerNum);			//�v���C���[�̃W�����v����
			}
		}
	}
}
//========================
//�W�����v�̏���
//========================
void JumpPlayer_HDR(int nJumpPlayer)
{
	PlaySound(SOUND_LABEL_SE_JUMP);

	g_aPlayerHDR[nJumpPlayer].posOld.y = g_aPlayerHDR[nJumpPlayer].pos.y;
	g_aPlayerHDR[nJumpPlayer].moveV0.y = 7.7f;//�ړ��ʐݒ�
	g_aPlayerHDR[nJumpPlayer].jumpTime = 0;	//�W�����v���ԃ��Z�b�g
	g_aPlayerHDR[nJumpPlayer].bJump = true;
	/*g_aPlayerHDR[nJumpPlayer].stat = PLAYERSTAT_JUMP;*/
}
//========================
//�q�b�v�h���b�v�̏���
//========================
void HipDropPlayer_HDR(int nHipDropPlayer)
{
	PlaySound(SOUND_LABEL_SE_HIPDROP);

	g_aPlayerHDR[nHipDropPlayer].HipDropPower = g_aPlayerHDR[nHipDropPlayer].pos.y - g_aPlayerHDR[nHipDropPlayer].posOld.y;

	if (g_aPlayerHDR[nHipDropPlayer].HipDropPower >= 150)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = 10;
	}
	else if (g_aPlayerHDR[nHipDropPlayer].HipDropPower > 80)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = 4;
	}
	else
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = 1;
	}

	g_aPlayerHDR[nHipDropPlayer].move.y = -15.0f;		//�q�b�v�h���b�v�̍~�����x����
	g_aPlayerHDR[nHipDropPlayer].moveV0.y = -15.0f;		//�q�b�v�h���b�v�̍~�����x����
	g_aPlayerHDR[nHipDropPlayer].jumpTime = 0;						//�W�����v���ԃ��Z�b�g
	g_aPlayerHDR[nHipDropPlayer].bHipDrop = true;					//�q�b�v�h���b�v���Ă���
	g_aPlayerHDR[nHipDropPlayer].bHipDropSpin = true;				//�X�s�����Ă���
	//g_aPlayerHDR[nHipDropPlayer].stat = PLAYERSTAT_HIPDROP;
}
//========================
//�擾����
//========================
Player_HDR *GetPlayer_HDR(void)
{
	return &g_aPlayerHDR[0];
}
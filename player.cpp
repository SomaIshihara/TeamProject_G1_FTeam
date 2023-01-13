//==========================================
//
//�v���C���[�v���O����[player.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "player.h"
#include "model.h"
#include "input.h"
#include "wall.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>

//�}�N��
#define PLAYER_MOVE_SPEED	(3.0f)	//�v���C���[�ړ����x
#define DUMP_COEF			(0.4f)	//�����W��

//�_�b�V���֘A�}�N��

//#define ADD_GAUGE			(10.0f)	//�Q�[�W������
//#define MAX_GAUGE			(25.0f)	//�Q�[�W�ő�l
//#define GAUGE_BONUS			(15.0f)	//�{�[�i�X�Q�[�W


//����
#define ROT_WA	(-0.75f * D3DX_PI)	//����
#define ROT_WD	(0.75f * D3DX_PI)	//�E��
#define ROT_SA	(-0.25f * D3DX_PI)	//����
#define ROT_SD	(0.25f * D3DX_PI)	//�E��
#define ROT_W	(1.0f * D3DX_PI)	//��
#define ROT_A	(-0.5f * D3DX_PI)	//��
#define ROT_S	(0.0f * D3DX_PI)	//��
#define ROT_D	(0.5f * D3DX_PI)	//�E

//�v���g
//�Ȃ�

//�O���[�o���ϐ�
Player g_aPlayer[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;

//========================
//����������
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�ϐ�������
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = ZERO_SET;
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = ZERO_SET;
		g_aPlayer[nCntPlayer].rot = ZERO_SET;


		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].bUse = true;
	}
}

//========================
//�I������
//========================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//�v���C���[�̐�������������
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{//�擾�������Ȃ̂�NULL����邾����OK
			g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh = NULL;
			g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//========================
//�X�V����
//========================
void UpdatePlayer(void)
{
	float nGauge[MAX_USE_GAMEPAD];

	//�v���C���[�l�����J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			//���݂̈ʒu��O��̈ʒu�ɂ���
			g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

			//���f���ړ�
			if (GetKeyboardPress(DIK_W) == true)
			{
				if (GetKeyboardPress(DIK_A) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else if (GetKeyboardPress(DIK_D) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_W + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{
				if (GetKeyboardPress(DIK_A) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else if (GetKeyboardPress(DIK_D) == true)
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
				else
				{
					g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_S + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
					g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				}
			}
			else if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_A + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
				g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer[nCntPlayer].rot.y = -(float)fmod(GetCamera()->rot.y + ROT_D + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
				g_aPlayer[nCntPlayer].move.x = sinf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
				g_aPlayer[nCntPlayer].move.z = cosf(FIX_ROT((g_aPlayer[nCntPlayer].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			}

			//�ړ����@�i�_�b�V���j�����ė���
			{
				//nGauge[nCntPlayer] = g_aPlayer[nCntPlayer].movegauge / MAX_GAUGE;

				//if (GetKeyboardPress(DIK_SPACE) == true)
				//{//�X�y�[�X�L�[�͉����ꂽ

				//	g_aPlayer[nCntPlayer].movegauge += ADD_GAUGE;

				//	if (g_aPlayer[nCntPlayer].movegauge >= MAX_GAUGE)
				//	{
				//		g_aPlayer[nCntPlayer].movegauge = 0;
				//	}

				//}

				//if (GetKeyboardRelease(DIK_SPACE) == true)
				//{//SPACE�L�[�������ꂽ
				// //�i�s�����̐ݒ�

				//	if (nGauge[nCntPlayer] >= 0.9f)
				//	{
				//		g_aPlayer[nCntPlayer].movegauge += GAUGE_BONUS;
				//	}

				//	g_aPlayer[nCntPlayer].move.x += sinf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].movegauge;
				//	g_aPlayer[nCntPlayer].move.z += cosf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].movegauge;

				//	g_aPlayer[nCntPlayer].movegauge = 0;

				//}

			}

			//�{�^������ɉ����ăv���C���[�E�J�������_�E�����_�ړ�
			g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
			g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;

			//�����蔻���
			//��U�Ȃ�

			//�ړ��ʌ���
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			//�e�ʒu�ݒ�
			//��U�Ȃ�
		}
	}
}

//========================
//�`�揈��
//========================
void DrawPlayer(void)
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
		D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);

		//�ʒu���f
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);

		//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (g_aPlayer[nCntPlayer].model.aParts[nCntParts].bUse == true)
			{
				D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p
				D3DXMATRIX mtxParent;					//�e�̃}�g��

				//���ŃI�t�Z�b�g�����̂܂܎g��
				g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos = g_aPlayer[nCntPlayer].model.aParts[nCntParts].posOffset;
				g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot = g_aPlayer[nCntPlayer].model.aParts[nCntParts].rotOffset;

				//"���f����"���[���h�}�g���b�N�X������
				D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//�����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.y, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.x, g_aPlayer[nCntPlayer].model.aParts[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxRotModel);

				//�ʒu���f
				D3DXMatrixTranslation(&mtxTransModel, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.x, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.y, g_aPlayer[nCntPlayer].model.aParts[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxTransModel);

				//�p�[�c�̐e�}�g���ݒ�
				if (g_aPlayer[nCntPlayer].model.aParts[nCntParts].mIdxModelParent != -1)
				{
					mtxParent = g_aPlayer[nCntPlayer].model.aParts[g_aPlayer[nCntPlayer].model.aParts[nCntParts].mIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayer[nCntPlayer].mtxWorld;
				}

				//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxParent);

				//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld);

				//�}�e���A���f�[�^�ւ̃|�C���^�擾
				pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					//�}�e���A���ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���ݒ�
					pDevice->SetTexture(0, g_aPlayer[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

					//���f���`��
					g_aPlayer[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}
			}
		}
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�擾����
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
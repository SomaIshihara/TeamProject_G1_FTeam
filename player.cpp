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
#include "color.h"

//�}�N��
#define PLAYER_MOVE_SPEED	(20.0f)		//�v���C���[�ړ����x
#define PLAYER_POWER_MAX	(1.0f)		//�ړ��̋����̍ő�l
#define PLAYER_POWER_ADD	(0.01f)		//�ړ��̋����̑����l
#define DUMP_COEF			(0.04f)		//�����W��
#define DEBUG_PLAYER_MOVE_SPEED	(5.0f)	//[�f�o�b�O�p]���ʂɈړ�����Ƃ��̈ړ���
#define DECIMAL_PLACE		(1)			//�����_�扽�ʂ܂ňړ����Ă��邱�Ƃɂ��邩

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
void MovePlayer(Player *pPlayer);

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
		g_aPlayer[nCntPlayer].moveGauge = 0;


		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].bUse = false;
	}

	//1P�̂ݎg�p���Ă��邱�Ƃɂ���
	g_aPlayer[0].bUse = true;
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
	//�f�o�b�O�\��
	PrintDebugProc("[�p�����[�^]\n");

	//�v���C���[�l�����J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse == true)
		{
			//���݂̈ʒu��O��̈ʒu�ɂ���
			g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

			pow(10, DECIMAL_PLACE);
			

			//�ړ����@�i�_�b�V���j�����ė���
			if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
				&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
			{//�������ꓮ���Ă���Č���Ȃ���ˁi�قړ����Ă��Ȃ��j
				if (GetKeyboardPress(DIK_SPACE) == true)
				{//�X�y�[�X�L�[�͉����ꂽ
					g_aPlayer[nCntPlayer].moveGauge = fmodf(g_aPlayer[nCntPlayer].moveGauge + PLAYER_POWER_ADD, PLAYER_POWER_MAX + PLAYER_POWER_ADD);
				}
				else if (GetKeyboardRelease(DIK_SPACE) == true)
				{//SPACE�L�[�������ꂽ
					//�i�s�����̐ݒ�
					g_aPlayer[nCntPlayer].move.x = -sinf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;
					g_aPlayer[nCntPlayer].move.z = -cosf(g_aPlayer[nCntPlayer].rot.y) * g_aPlayer[nCntPlayer].moveGauge * PLAYER_MOVE_SPEED;

					g_aPlayer[nCntPlayer].moveGauge = 0;
				}
			}
			else
			{
				g_aPlayer[nCntPlayer].moveGauge = 0;
			}

			//�{�^������ɉ����ăv���C���[�E�J�������_�E�����_�ړ�
			g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
			g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;

			//�����蔻���
			//��U�Ȃ�

			//�ړ��ʌ���
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			//[�f�o�b�O�p]���ʂɈړ����鏈��
			MovePlayer(&g_aPlayer[nCntPlayer]);

			//�e�ʒu�ݒ�
			//��U�Ȃ�

			//�f�o�b�O�\���i�����\���j
			PrintDebugProc("[%d]:Power = %f\n", nCntPlayer, g_aPlayer[nCntPlayer].moveGauge);
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

				//���ŃI�t�Z�b�g�����̂܂܎g���i�A�j���[�V�����g���悤�ɂȂ���������āj
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

				/*------------------------------------------------------------------
				�e�̕`��		Author:���V����
				--------------------------------------------------------------------*/
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
				D3DXMatrixMultiply(&mtxShadow, &g_aPlayer[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxShadow);

				//�V���h�E�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL *)g_aPlayer[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//�}�e���A���f�[�^����

					//���F�ɐݒ�					//���Ȕ����𖳂���
					MatCopy.Diffuse = XCOL_BLACK;	MatCopy.Emissive = XCOL_BLACK;

					//�}�e���A���ݒ�
					pDevice->SetMaterial(&MatCopy);

					//�e�N�X�`���ݒ�
					pDevice->SetTexture(0, NULL);

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
//[�f�o�b�O�p]�v���C���[�̈ړ�����
//========================
void MovePlayer(Player *pPlayer)
{
	//���f���ړ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
			pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
		pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		pPlayer->rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
		pPlayer->move.x = sinf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		pPlayer->move.z = cosf(FIX_ROT((pPlayer->rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else
	{
		return;
	}

	//�{�^������ɉ����ăv���C���[�E�J�������_�E�����_�ړ�
	pPlayer->pos.x += pPlayer->move.x;
	pPlayer->pos.z += pPlayer->move.z;

	//�ړ��ʏ���
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================
//�擾����
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
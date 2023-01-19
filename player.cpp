//==========================================
//
//�v���C���[�v���O����[player.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "game.h"
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
#define PLAYER_JUMP_SPEED	(7.7f)		//�v���C���[�W�����v���x
#define ACCELERATION_GRAVITY (9.8f)		//�d�͉����x
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
void MovePlayer(int nPadNum);

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
		g_aPlayer[nCntPlayer].jumpTime = 0;


		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;

		g_aPlayer[nCntPlayer].model = GetModel(g_aPlayer[nCntPlayer].animal);
		g_aPlayer[nCntPlayer].bUsePlayer = GetUseController(nCntPlayer);
	}

	//[�f�o�b�O]�R���g���[���[���ڑ�����Ă��Ȃ����1P�̂ݗL��������
	if (GetUseControllerNum() == 0) 
	{
		g_aPlayer[0].bUsePlayer = true;
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
	//�f�o�b�O�\��
	PrintDebugProc("[�p�����[�^]\n");

	//�v���C���[�l�����J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUsePlayer == true)
		{
			//�ڑ�����Ă��邩�m�F���Đؒf����Ă�����v���C���[�������i��O�Ƃ��ăR���g���[���[���Ȃ����Ă��Ȃ��Ƃ��͖����j
			if (GetUseControllerNum() != 0)
			{
				g_aPlayer[nCntPlayer].bUsePlayer = GetUseController(nCntPlayer);
			}

			//���݂̈ʒu��O��̈ʒu�ɂ���
			g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

			//�W�����v���Ԃ𑝂₷
			g_aPlayer[nCntPlayer].jumpTime++;
			//�L�[�{�[�h���쎞�̓���
#if 1
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

			//�W�����v
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				g_aPlayer[nCntPlayer].move.y = PLAYER_JUMP_SPEED;//�ړ��ʐݒ�
			}
#endif
			//�Q�[���p�b�h�̑���
			//�ړ����@�i�_�b�V���j�����ė���
			if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
				&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
			{//�������ꓮ���Ă���Č���Ȃ���ˁi�قړ����Ă��Ȃ��j
				if (GetGamepadPress(nCntPlayer, XINPUT_GAMEPAD_X) == true)
				{//A�{�^���������ꂽ
					g_aPlayer[nCntPlayer].moveGauge = fmodf(g_aPlayer[nCntPlayer].moveGauge + PLAYER_POWER_ADD, PLAYER_POWER_MAX + PLAYER_POWER_ADD);
				}
				else if (GetGamepadRelease(nCntPlayer, XINPUT_GAMEPAD_X) == true)
				{//A�{�^���������ꂽ
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

			//�W�����v
			if (GetGamepadPress(nCntPlayer, XINPUT_GAMEPAD_A) == true)
			{
				g_aPlayer[nCntPlayer].move.y = PLAYER_JUMP_SPEED;//�ړ��ʐݒ�
				g_aPlayer[nCntPlayer].jumpTime = 0;	//�W�����v���ԃ��Z�b�g
			}

			//�{�^������ɉ����ăv���C���[�ړ�
			g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
			g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y - (ACCELERATION_GRAVITY * g_aPlayer[nCntPlayer].jumpTime / MAX_FPS);
			g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;

			//[��]��UY=0.0f�ɂȂ����璅�n
			if (g_aPlayer[nCntPlayer].pos.y <= 0.0f)
			{
				g_aPlayer[nCntPlayer].pos.y = 0.0f;
			}

			//�����蔻���
			//��U�Ȃ�

			//�ړ��ʌ���
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			//[�f�o�b�O�p]���ʂɈړ����鏈��
			MovePlayer(nCntPlayer);

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

					//���F�ɐݒ�						//���Ȕ����𖳂���
					MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

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
void MovePlayer(int nPadNum)
{
	//���f���ړ�
	//�Q�[���p�b�h��
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X�����̃X�e�B�b�N���X���Ă���
		if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y�����̃X�e�B�b�N���X���Ă���
			g_aPlayer[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y�����̃X�e�B�b�N�����X���Ă���
		g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	//�L�[�{�[�h��
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else
	{
		return;
	}

	//�{�^������ɉ����ăv���C���[�E�J�������_�E�����_�ړ�
	g_aPlayer[nPadNum].pos.x += g_aPlayer[nPadNum].move.x;
	g_aPlayer[nPadNum].pos.z += g_aPlayer[nPadNum].move.z;

	//�ړ��ʏ���
	g_aPlayer[nPadNum].move.x = 0.0f;
	g_aPlayer[nPadNum].move.z = 0.0f;
}
//========================
//�擾����
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
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
#include "score.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "Gauge.h"
#include "sound.h"
#include "effect.h"

//�}�N��
#define PLAYER_MOVE_SPEED	(20.0f)		//�v���C���[�ړ����x
#define PLAYER_JUMP_SPEED	(7.7f)		//�v���C���[�W�����v���x

#define PLAYER_HIPDROP_WAIT		(20)		//�q�b�v�h���b�v�́u�J�n�E���n�v�d������
#define PLAYER_BLOWING_POWER	(5.0f)	//�q�b�v�h���b�v���ꂽ���̈ړ���
#define PLAYER_HIPDROP_POWER	(-10.0f)	//�q�b�v�h���b�v����Ƃ��̈ړ���
#define ACCELERATION_GRAVITY (9.8f)		//�d�͉����x
#define PLAYER_WEIGHT		(50)		//����
#define PLAYER_POWER_ADD	(0.025f)		//�ړ��̋����̑����l
#define DUMP_COEF			(0.04f)		//�����W��
#define DEBUG_PLAYER_MOVE_SPEED	(5.0f)	//[�f�o�b�O�p]���ʂɈړ�����Ƃ��̈ړ���
#define DECIMAL_PLACE		(1)			//�����_�扽�ʂ܂ňړ����Ă��邱�Ƃɂ��邩
#define BF_RADIUS			(353.5f)	//�o�g���t�B�[���h�̔��a
#define DOWN_TIME			(200)		//�_�E������Ƃ��鎞��

//�ړ��ʊ֌W
#define ACCELERATION_CONS		(0.5f)		//�����萔�i1.0�őS���n���j
#define ACCELERATION_ITEMMAG	(1.5f)		//�A�C�e���������̋����{��
#define DEFANCE_CONS			(0.0f)		//�h��萔�i1.0�Ŋ��S�h��j
#define DEFANCE_ITEMADD			(0.3f)		//�A�C�e���������̋�����

#define TEST_SIZE_WIDTH		(30.0f)
#define TEST_SIZE_HEIGHT	(15.0f)
#define TEST_SIZE_DEPTH		(30.0f)

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
void RotPlayer(int nPadNum);		//MovePlayer ��rot.y �̌v�Z���������c���Ă��܂�

void ChargePlayer(int nChargePlayer);	//�`���[�W����
void DashPlayer(int nDashPlayer);		//�_�b�V������
void JumpPlayer(int nJumpPlayer);		//�W�����v����
void HipDropPlayer(int nHipDropPlayer);	//�q�b�v�h���b�v����

void CollisionPP(int nPlayerNum);	//�v���C���[���m�̏Փ˔���
void HipDropPP(int nPlayerNum);		//�q�b�v�h���b�v���̏Փ˔���
void DownPlayer(int nDownPlayerNum);	//�_�E�������v���C���[�̏���
void RespawnPlayer(int nRespawnPlayer);	//���X�|�[������

//�O���[�o���ϐ�
Player g_aPlayer[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;

//�����ʒu����
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(-50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
};

//========================
//����������
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�ϐ�������
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;
		g_aPlayer[nCntPlayer].move = ZERO_SET;
		g_aPlayer[nCntPlayer].rot = ZERO_SET;
		g_aPlayer[nCntPlayer].moveGauge = 0;
		g_aPlayer[nCntPlayer].jumpTime = 0;
		g_aPlayer[nCntPlayer].bJump = false;
		g_aPlayer[nCntPlayer].bHipDrop = false;
		g_aPlayer[nCntPlayer].nHipDropWait = 0;

		g_aPlayer[nCntPlayer].faceCollider[0] = D3DXVECTOR3(15.0f, 0.0f, 15.0f);
		g_aPlayer[nCntPlayer].faceCollider[1] = D3DXVECTOR3(-15.0f, 0.0f, 15.0f);

		g_aPlayer[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayer[nCntPlayer].nScore = 0;
		g_aPlayer[nCntPlayer].lastAtkPlayer = -1;
		g_aPlayer[nCntPlayer].nNumHitPlayer = -1;
		g_aPlayer[nCntPlayer].stat = PLAYERSTAT_WAIT;

		g_aPlayer[nCntPlayer].nATKItemTime = 0;
		g_aPlayer[nCntPlayer].nDEFItemTime = 0;

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
		g_aPlayer[nCntPlayer].lastAtkPlayer = -1;

		//���݂̈ʒu��O��̈ʒu�ɂ���
		g_aPlayer[nCntPlayer].posOld = g_aPlayer[nCntPlayer].pos;

		if (g_aPlayer[nCntPlayer].bUsePlayer == true)
		{
			//�ڑ�����Ă��邩�m�F���Đؒf����Ă�����v���C���[�������i��O�Ƃ��ăR���g���[���[���Ȃ����Ă��Ȃ��Ƃ��͖����j
			if (GetUseControllerNum() != 0)
			{
				g_aPlayer[nCntPlayer].bUsePlayer = GetUseController(nCntPlayer);
			}

			//�W�����v���Ԃ𑝂₷
			g_aPlayer[nCntPlayer].jumpTime++;
			
			//�q�b�v�h���b�v���łȂ���Α���ł���
			if (g_aPlayer[nCntPlayer].bHipDrop == false)
			{
				//�L�[�{�[�h���쎞�̓���
#if 1
				//�ړ����@�i�_�b�V���j�����ė���
				if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
					&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
				{//�������ꓮ���Ă���Č���Ȃ���ˁi�قړ����Ă��Ȃ��j
					if (GetKeyboardPress(DIK_SPACE) == true)
					{//�X�y�[�X�L�[�͉����ꂽ

						//�v���C���[�̃`���[�W����
						ChargePlayer(nCntPlayer);
					}
					else if (GetKeyboardRelease(DIK_SPACE) == true)
					{//SPACE�L�[�������ꂽ
						//�v���C���[�̃_�b�V������
						DashPlayer(nCntPlayer);
					}
				}
				else
				{
					g_aPlayer[nCntPlayer].moveGauge = 0;
				}

				//�W�����v�E�q�b�v�h���b�v
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					if (g_aPlayer[nCntPlayer].bJump)
					{
						HipDropPlayer(nCntPlayer);									//�v���C���[�̃q�b�v�h���b�v����
						g_aPlayer[nCntPlayer].nHipDropWait = PLAYER_HIPDROP_WAIT;	//�q�b�v�h���b�v�̊J�n�d����ݒ�
					}
					else
					{
						JumpPlayer(nCntPlayer);			//�v���C���[�̃W�����v����
					}
				}
#endif
				//�Q�[���p�b�h�̑���
				//�ړ����@�i�_�b�V���j�����ė���
				if ((int)(g_aPlayer[nCntPlayer].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
					&& (int)(g_aPlayer[nCntPlayer].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
				{//�������ꓮ���Ă���Č���Ȃ���ˁi�قړ����Ă��Ȃ��j
					if (GetGamepadPress(nCntPlayer, XINPUT_GAMEPAD_X) == true)
					{//X�{�^���������ꂽ					 
						//�v���C���[�̃`���[�W����
						ChargePlayer(nCntPlayer);
					}
					else if (GetGamepadRelease(nCntPlayer, XINPUT_GAMEPAD_X) == true)
					{//X�{�^���������ꂽ
						//�v���C���[�̃_�b�V������
						DashPlayer(nCntPlayer);
					}
				}
				else
				{
					g_aPlayer[nCntPlayer].moveGauge = 0;
				}

				//�W�����v�E�q�b�v�h���b�v
				if (GetGamepadTrigger(nCntPlayer, XINPUT_GAMEPAD_A) == true)
				{
					if (g_aPlayer[nCntPlayer].bJump)
					{
						HipDropPlayer(nCntPlayer);									//�v���C���[�̃q�b�v�h���b�v����
						g_aPlayer[nCntPlayer].nHipDropWait = PLAYER_HIPDROP_WAIT;	//�q�b�v�h���b�v�̊J�n�d����ݒ�
					}
					else
					{
						JumpPlayer(nCntPlayer);			//�v���C���[�̃W�����v����
					}
				}
				
				//[�f�o�b�O�p]���ʂɈړ����鏈��
#ifdef _DEBUG
				MovePlayer(nCntPlayer);
#endif
			}

			//�q�b�v�h���b�v��
			else
			{
				//�q�b�v�h���b�v�d�����Ԃ�����
				if (g_aPlayer[nCntPlayer].nHipDropWait > 0)
				{
					g_aPlayer[nCntPlayer].nHipDropWait--;		//�d�����Ԃ����炵�Ă���
				}
				else
				{
					//����������
					g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].moveV0.y;
				}
			}

			//�W�����v�ʐݒ�
			g_aPlayer[nCntPlayer].move.y = g_aPlayer[nCntPlayer].moveV0.y - (ACCELERATION_GRAVITY * g_aPlayer[nCntPlayer].jumpTime / MAX_FPS);

			//�ړ��オy<0�Ȃ痎���邩�ړ��ʏ���
			if (g_aPlayer[nCntPlayer].pos.y + g_aPlayer[nCntPlayer].move.y < 0.0f && g_aPlayer[nCntPlayer].stat != PLAYERSTAT_FALL)
			{
				float fLength = sqrtf(powf((g_aPlayer[nCntPlayer].pos.x + g_aPlayer[nCntPlayer].move.x), 2) +
					powf((g_aPlayer[nCntPlayer].pos.z + g_aPlayer[nCntPlayer].move.z), 2));

				if (fLength >= BF_RADIUS)
				{
					g_aPlayer[nCntPlayer].stat = PLAYERSTAT_FALL;
				}
				else
				{
					g_aPlayer[nCntPlayer].bJump = false;
					g_aPlayer[nCntPlayer].bHipDrop = false;
					g_aPlayer[nCntPlayer].moveV0.y = 0.0f;
					g_aPlayer[nCntPlayer].move.y = 0.0f;
					g_aPlayer[nCntPlayer].jumpTime = 0;
					g_aPlayer[nCntPlayer].pos.y = 0.0f;
				}
			}

			//������ς��鏈��
			RotPlayer(nCntPlayer);

			//�����蔻���
			CollisionPP(nCntPlayer);
			HipDropPP(nCntPlayer);

			//�e�ʒu�ݒ�
			//��U�Ȃ�

			if (g_aPlayer[nCntPlayer].stat == PLAYERSTAT_FALL && g_aPlayer[nCntPlayer].jumpTime >= DOWN_TIME)
			{
				DownPlayer(nCntPlayer);
			}

			//�f�o�b�O�\���i�����\���j
			PrintDebugProc("[%d]:Power = %f\n", nCntPlayer, g_aPlayer[nCntPlayer].moveGauge);
		}
	}

	//�ړ��ʂƏՓ˔�������ƂɈړ�����
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (true)
		{//������ꂽ����Γ����
			if (g_aPlayer[nCntPlayer].lastAtkPlayer == -1)
			{//�Ԃ����ĂȂ��܂��͈ړ��ʌ����ς�		
				//�q�b�v�h���b�v���Ă��炸�A�q�b�v�h���b�v�̍d�����ł��Ȃ�
				if (g_aPlayer[nCntPlayer].bHipDrop == false && g_aPlayer[nCntPlayer].nHipDropWait <= 0)
				{
					//���ʂɈړ�
					g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
					g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y;
					g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;
				}
			}
			else
			{//�Ԃ�����
				//�ړ��ʌ���
				D3DXVECTOR3 moveTmp1 = g_aPlayer[nCntPlayer].move;
				D3DXVECTOR3 moveTmp2 = g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].move;

				//�����ݒ�
				float fPowerConvertion1 = ACCELERATION_CONS * (g_aPlayer[nCntPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayer[nCntPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);

				float fPowerConvertion2 = ACCELERATION_CONS * (g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);

				g_aPlayer[nCntPlayer].move = moveTmp2 * fPowerConvertion1;
				g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].move = moveTmp1 * fPowerConvertion2;

				//�ړ��ʌ����ς݈����ɂ���
				g_aPlayer[nCntPlayer].lastAtkPlayer = -1;
				g_aPlayer[g_aPlayer[nCntPlayer].lastAtkPlayer].lastAtkPlayer = -1;

				//���ʂɈړ�
				g_aPlayer[nCntPlayer].pos.x += g_aPlayer[nCntPlayer].move.x;
				g_aPlayer[nCntPlayer].pos.y += g_aPlayer[nCntPlayer].move.y;
				g_aPlayer[nCntPlayer].pos.z += g_aPlayer[nCntPlayer].move.z;
			}

			//�ړ��ʌ���
			g_aPlayer[nCntPlayer].move.x += (0 - g_aPlayer[nCntPlayer].move.x) * DUMP_COEF;
			g_aPlayer[nCntPlayer].move.z += (0 - g_aPlayer[nCntPlayer].move.z) * DUMP_COEF;

			////[��]��UY=0.0f�ɂȂ����璅�n
			//if (g_aPlayer[nCntPlayer].pos.y <= 0.0f)
			//{
			//	g_aPlayer[nCntPlayer].pos.y = 0.0f;
			//}
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
				if (g_aPlayer[nCntPlayer].stat != PLAYERSTAT_FALL)
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
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�v���C���[�̃`���[�W����
//========================
void ChargePlayer(int nChargePlayer)
{
	g_aPlayer[nChargePlayer].moveGauge += PLAYER_POWER_ADD;

	if (g_aPlayer[nChargePlayer].moveGauge >= PLAYER_POWER_MAX)
	{
		g_aPlayer[nChargePlayer].moveGauge = PLAYER_POWER_MAX;
		StopSound(SOUND_LABEL_SE_ENERGY_00);
	}
	if (g_aPlayer[nChargePlayer].moveGauge <= PLAYER_POWER_ADD)
	{
		PlaySound(SOUND_LABEL_SE_ENERGY_00);
	}

	SetEffect(g_aPlayer[nChargePlayer].pos, nChargePlayer, EFFECTTYPE_CHARGE);
}

//========================
//�v���C���[�̃_�b�V������
//========================
void DashPlayer(int nDashPlayer)
{
	//�i�s�����̐ݒ�
	g_aPlayer[nDashPlayer].move.x = -sinf(g_aPlayer[nDashPlayer].rot.y) * g_aPlayer[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;
	g_aPlayer[nDashPlayer].move.z = -cosf(g_aPlayer[nDashPlayer].rot.y) * g_aPlayer[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;

	g_aPlayer[nDashPlayer].moveGauge = 0;
}

//========================
//�v���C���[�̃q�b�v�h���b�v����
//========================
void HipDropPlayer(int nHipDropPlayer)
{
	g_aPlayer[nHipDropPlayer].moveV0.y = PLAYER_HIPDROP_POWER;		//�q�b�v�h���b�v�̍~�����x����
	g_aPlayer[nHipDropPlayer].jumpTime = 0;							//�W�����v���ԃ��Z�b�g
	g_aPlayer[nHipDropPlayer].bHipDrop = true;						//�q�b�v�h���b�v���Ă���
}

//========================
//�v���C���[�̃W�����v����
//========================
void JumpPlayer(int nJumpPlayer)
{
	g_aPlayer[nJumpPlayer].moveV0.y = PLAYER_JUMP_SPEED;//�ړ��ʐݒ�
	g_aPlayer[nJumpPlayer].jumpTime = 0;	//�W�����v���ԃ��Z�b�g
	g_aPlayer[nJumpPlayer].bJump = true;
}

//========================
//�v���C���[���m�̏Փ˔��菈��
//========================
void CollisionPP(int nPlayerNum)
{
	//=pos0~pos3�̐���==================
	// pos3		pos2
	//	�E<-----�E		���:vecLine
	//	�b		��
	//	�b		�b
	//	��		�b
	//	�E----->�E
	// pos0		pos1
	//==================================

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMove[2];

	//2��2���_��2
	float fAreaARight[2], fAreaALeft[2], fAreaBRight[2], fAreaBLeft[2];
	float fAreaAUp[2], fAreaADown[2], fAreaBUp[2], fAreaBDown[2];

	//�����f�̈ʒu�l��
	D3DXVECTOR3 posTemp = g_aPlayer[nPlayerNum].pos + g_aPlayer[nPlayerNum].move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)// && g_aPlayer[nCntOtherPlayer].bUsePlayer == true
		{
			//�e���_���߂�
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//���_�ƒ��S�̋�����X��Z�ʁX�Ōv�Z����
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//���_�ƒ��S�̋��������߂�
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//���_�ƒ��S�̊p�x�����߂�
																	//0 - �v�Z�ŏo�����p�x + �I�u�W�F�N�g�̊p�x�� -PI ~ PI�ɏC��
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//�p�x�ɉ����Ē��_�̈ʒu�����炷
			pos0.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos0.y = 0.0f;
			pos0.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos0---------------------------------------------------------------------------------------------------------------------------

			//-pos1---------------------------------------------------------------------------------------------------------------------------
			//���_�ƒ��S�̋�����X��Z�ʁX�Ōv�Z����
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//���_�ƒ��S�̋��������߂�
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//���_�ƒ��S�̊p�x�����߂�
																	//0 + �v�Z�ŏo�����p�x + �I�u�W�F�N�g�̊p�x�� -PI ~ PI�ɏC��
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//�p�x�ɉ����Ē��_�̈ʒu�����炷
			pos1.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos1.y = 0.0f;
			pos1.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos1---------------------------------------------------------------------------------------------------------------------------

			//-pos2---------------------------------------------------------------------------------------------------------------------------
			//���_�ƒ��S�̋�����X��Z�ʁX�Ōv�Z����
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//���_�ƒ��S�̋��������߂�
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//���_�ƒ��S�̊p�x�����߂�
																	//PI - �v�Z�ŏo�����p�x + �I�u�W�F�N�g�̊p�x�� -PI ~ PI�ɏC��
			rot = FIX_ROT(D3DX_PI - fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//�p�x�ɉ����Ē��_�̈ʒu�����炷
			pos2.x = g_aPlayer[nCntOtherPlayer].pos.x - sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = g_aPlayer[nCntOtherPlayer].pos.z + cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//-pos3---------------------------------------------------------------------------------------------------------------------------
			//���_�ƒ��S�̋�����X��Z�ʁX�Ōv�Z����
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//���_�ƒ��S�̋��������߂�
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//���_�ƒ��S�̊p�x�����߂�
																	//-PI + �v�Z�ŏo�����p�x + �I�u�W�F�N�g�̊p�x�� -PI ~ PI�ɏC��
			rot = FIX_ROT(-D3DX_PI - fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//�p�x�ɉ����Ē��_�̈ʒu�����炷
			pos3.x = g_aPlayer[nCntOtherPlayer].pos.x - sinf(rot) * fLength;
			pos3.y = 0.0f;
			pos3.z = g_aPlayer[nCntOtherPlayer].pos.z + cosf(rot) * fLength;
			//-pos3---------------------------------------------------------------------------------------------------------------------------

			//�x�N�g�����߂�
			//move
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				vecMove[nCntCollision] = (posTemp + g_aPlayer[nPlayerNum].faceCollider[nCntCollision]) - (g_aPlayer[nPlayerNum].posOld + g_aPlayer[nPlayerNum].faceCollider[nCntCollision]);
			}

			//X
			//�E�����̌v�Z
			vecLineRight = pos1 - pos0;
			vecToPosRight = posTemp - pos0;
			vecToPosOldRight = g_aPlayer[nPlayerNum].posOld - pos0;

			//�������̌v�Z
			vecLineLeft = pos3 - pos2;
			vecToPosLeft = posTemp - pos2;
			vecToPosOldLeft = g_aPlayer[nPlayerNum].posOld - pos2;

			//Z
			//������̌v�Z
			vecLineUp = pos2 - pos1;
			vecToPosUp = posTemp - pos1;
			vecToPosOldUp = g_aPlayer[nPlayerNum].posOld - pos1;
			//�������̌v�Z
			vecLineDown = pos0 - pos3;
			vecToPosDown = posTemp - pos3;
			vecToPosOldDown = g_aPlayer[nPlayerNum].posOld - pos3;

			//�����蔻��{��
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				//X
				//�ʐϋ��߂�
				fAreaARight[nCntCollision] = TASUKIGAKE(vecToPosRight.x, vecToPosRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaALeft[nCntCollision] = TASUKIGAKE(vecToPosLeft.x, vecToPosLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBRight[nCntCollision] = TASUKIGAKE(vecLineRight.x, vecLineRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBLeft[nCntCollision] = TASUKIGAKE(vecLineLeft.x, vecLineLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//����AND�͈͓�
				if ((vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) <= 0.0f && (vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f)
				{
					if (fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] >= 0.0f && fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							//1.0f = pushback
							float fRate = fAreaARight[nCntCollision] / fAreaBRight[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos0.x + (vecLineRight.x * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos0.z + (vecLineRight.z * fRate) - cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) <= 0.0f && (vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f)
				{
					if (fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] >= 0.0f && fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							float fRate = fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos2.x + (vecLineLeft.x * fRate) + sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos2.z + (vecLineLeft.z * fRate) + cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
			
				//Z
				//�ʐϋ��߂�
				fAreaAUp[nCntCollision] = TASUKIGAKE(vecToPosUp.x, vecToPosUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaADown[nCntCollision] = TASUKIGAKE(vecToPosDown.x, vecToPosDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBUp[nCntCollision] = TASUKIGAKE(vecLineUp.x, vecLineUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBDown[nCntCollision] = TASUKIGAKE(vecLineDown.x, vecLineDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//����AND�͈͓�
				if ((vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) <= 0.0f && (vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f)
				{
					if (fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] >= 0.0f && fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							float fRate = fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos1.x + (vecLineUp.x * fRate) + cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos1.z + (vecLineUp.z * fRate) - sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) <= 0.0f && (vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f)
				{
					if (fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] >= 0.0f && fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayer[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayer[nCntOtherPlayer].pos.y + TEST_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayer[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayer[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayer[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
							}
							float fRate = fAreaADown[nCntCollision] / fAreaBDown[nCntCollision];
							g_aPlayer[nPlayerNum].pos.x = pos3.x + (vecLineDown.x * fRate) - cosf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayer[nPlayerNum].pos.z = pos3.z + (vecLineDown.z * fRate) + sinf(g_aPlayer[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
			}
		}
	}
}

//========================
//�q�b�v�h���b�v���̏Փˏ���
//========================
void HipDropPP(int nPlayerNum)
{
	//=pos0~pos2�̐���==================
	//
	//		�Eg_ap[nPNum].posOld
	//		��
	//		�Eg_ap[nPNum].pos
	// pos1		pos0
	//	�E<-----�E		���:vecLine
	//	�b
	//	�b
	//	��
	//�@�E
	// pos2
	//==================================

	D3DXVECTOR3 pos0, pos1, pos2;
	D3DXVECTOR3 vecLineX, vecToPosX, vecToPosOldX;
	D3DXVECTOR3 vecLineZ, vecToPosZ, vecToPosOldZ;
	D3DXVECTOR3 vecMove;
	float fAreaAX, fAreaBX, fAreaAZ, fAreaBZ;

	//�����f�̈ʒu�l��
	D3DXVECTOR3 posTemp = g_aPlayer[nPlayerNum].pos + g_aPlayer[nPlayerNum].move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)// && g_aPlayer[nCntOtherPlayer].bUsePlayer == true
		{
			//�e���_���߂�
			float fLengthX, fLengthZ;
			float fLength;
			float fAngle;
			float rot;

			//-pos0---------------------------------------------------------------------------------------------------------------------------
			//���_�ƒ��S�̋�����X��Z�ʁX�Ōv�Z����
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x + TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//���_�ƒ��S�̋��������߂�
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//���_�ƒ��S�̊p�x�����߂�
																	//0 - �v�Z�ŏo�����p�x + �I�u�W�F�N�g�̊p�x�� -PI ~ PI�ɏC��
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//�p�x�ɉ����Ē��_�̈ʒu�����炷
			pos0.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos0.y = 0.0f;
			pos0.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos0---------------------------------------------------------------------------------------------------------------------------

			//-pos1---------------------------------------------------------------------------------------------------------------------------
			//���_�ƒ��S�̋�����X��Z�ʁX�Ōv�Z����
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z + TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//���_�ƒ��S�̋��������߂�
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//���_�ƒ��S�̊p�x�����߂�
																	//0 + �v�Z�ŏo�����p�x + �I�u�W�F�N�g�̊p�x�� -PI ~ PI�ɏC��
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//�p�x�ɉ����Ē��_�̈ʒu�����炷
			pos1.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos1.y = 0.0f;
			pos1.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos1---------------------------------------------------------------------------------------------------------------------------

			//-pos2---------------------------------------------------------------------------------------------------------------------------
			//���_�ƒ��S�̋�����X��Z�ʁX�Ōv�Z����
			fLengthX = g_aPlayer[nCntOtherPlayer].pos.x - (g_aPlayer[nCntOtherPlayer].pos.x - TEST_SIZE_WIDTH);
			fLengthZ = g_aPlayer[nCntOtherPlayer].pos.z - (g_aPlayer[nCntOtherPlayer].pos.z - TEST_SIZE_DEPTH);

			fLength = sqrtf(powf(fLengthX, 2) + powf(fLengthZ, 2));	//���_�ƒ��S�̋��������߂�
			fAngle = atan2f(fLengthX * 2, fLengthZ * 2);			//���_�ƒ��S�̊p�x�����߂�
																	//0 + �v�Z�ŏo�����p�x + �I�u�W�F�N�g�̊p�x�� -PI ~ PI�ɏC��
			rot = FIX_ROT(-fAngle - g_aPlayer[nCntOtherPlayer].rot.y);

			//�p�x�ɉ����Ē��_�̈ʒu�����炷
			pos2.x = g_aPlayer[nCntOtherPlayer].pos.x + sinf(rot) * fLength;
			pos2.y = 0.0f;
			pos2.z = g_aPlayer[nCntOtherPlayer].pos.z - cosf(rot) * fLength;
			//-pos2---------------------------------------------------------------------------------------------------------------------------

			//�x�N�g�����߂�
			//move
			vecMove = g_aPlayer[nPlayerNum].pos - g_aPlayer[nPlayerNum].posOld;

			//X
			vecLineX = pos1 - pos0;
			vecToPosX = posTemp - pos0;
			vecToPosOldX = g_aPlayer[nPlayerNum].posOld - pos0;

			//Z
			vecLineZ = pos2 - pos1;
			vecToPosZ = posTemp - pos1;
			vecToPosOldZ = g_aPlayer[nPlayerNum].posOld - pos1;

			//�����蔻��{��
			//X
			//�ʐϋ��߂�
			fAreaAX = TASUKIGAKE(vecToPosX.x, vecToPosX.y, vecMove.x, vecMove.y);
			fAreaBX = TASUKIGAKE(vecLineX.x, vecLineX.y, vecMove.x, vecMove.y);
			fAreaAZ = TASUKIGAKE(vecToPosZ.z, vecToPosZ.y, vecMove.z, vecMove.y);
			fAreaBZ = TASUKIGAKE(vecLineZ.z, vecLineZ.y, vecMove.z, vecMove.y);
			//����AND�͈͓�
			if ((vecLineX.y * vecToPosX.x) - (vecLineX.x * vecToPosX.y) <= 0.0f && (vecLineX.y * vecToPosOldX.x) - (vecLineX.x * vecToPosOldX.y) >= 0.0f)
			{
				if (fAreaAX / fAreaBX >= 0.0f && fAreaAX / fAreaBX <= 1.0f)
				{//�Փ�
					if (fAreaAZ / fAreaBZ >= 0.0f && fAreaAZ / fAreaBZ <= 1.0f)
					{
						//�ړ��ʌv�Z
						float fAngleHipDrop = atan2f(g_aPlayer[nCntOtherPlayer].pos.x - g_aPlayer[nPlayerNum].pos.x,
							g_aPlayer[nCntOtherPlayer].pos.z - g_aPlayer[nPlayerNum].pos.z);
						g_aPlayer[nCntOtherPlayer].move.x = sinf(fAngleHipDrop) * PLAYER_BLOWING_POWER;
						g_aPlayer[nCntOtherPlayer].move.z = -cosf(fAngleHipDrop) * PLAYER_BLOWING_POWER;

						//�U�����ꂽ�����ɂ���
						g_aPlayer[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
					}
				}
			}
		}
	}
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
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y�����̃X�e�B�b�N�����X���Ă���
		g_aPlayer[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
	}
	//�L�[�{�[�h��
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayer[nPadNum].move.x = sinf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayer[nPadNum].move.z = cosf(FIX_ROT((g_aPlayer[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
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
//�v���C���[�̌����ύX����
//========================
void RotPlayer(int nPadNum)
{
	//���f���ړ�
	//�Q�[���p�b�h��
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X�����̃X�e�B�b�N���X���Ă���
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y�����̃X�e�B�b�N�����X���Ă���
		g_aPlayer[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
	}
	//�L�[�{�[�h��
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
		}
		else
		{
			g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_aPlayer[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
	}
	else
	{
		return;
	}
}

//========================
//�_�E�������v���C���[�̏���
//========================
void DownPlayer(int nDownPlayerNum)
{
	if (g_aPlayer[nDownPlayerNum].nNumHitPlayer != -1)
	{//���Ă��ė������ꍇ
		g_aPlayer[g_aPlayer[nDownPlayerNum].nNumHitPlayer].nScore++;
		AddScore(1, g_aPlayer[nDownPlayerNum].nNumHitPlayer);
	}
	else
	{//�����P�Ɏ��ł����ꍇ
		for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
		{
			if (nCntOtherPlayer != nDownPlayerNum)
			{
				g_aPlayer[nCntOtherPlayer].nScore++;
				AddScore(1, nCntOtherPlayer);
			}
		}
	}

	RespawnPlayer(nDownPlayerNum);
}

//========================
//�v���C���[�̃��X�|�[������
//========================
void RespawnPlayer(int nRespawnPlayer)
{
	//�e�X�e�[�^�X�Đݒ�
	g_aPlayer[nRespawnPlayer].pos = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
	g_aPlayer[nRespawnPlayer].posOld = g_aPlayer[nRespawnPlayer].pos;
	g_aPlayer[nRespawnPlayer].move = ZERO_SET;
	g_aPlayer[nRespawnPlayer].moveV0 = ZERO_SET;
	g_aPlayer[nRespawnPlayer].rot = ZERO_SET;
	g_aPlayer[nRespawnPlayer].moveGauge = 0;
	g_aPlayer[nRespawnPlayer].jumpTime = 0;
	g_aPlayer[nRespawnPlayer].bJump = true;

	g_aPlayer[nRespawnPlayer].lastAtkPlayer = -1;
	g_aPlayer[nRespawnPlayer].nNumHitPlayer = -1;
	g_aPlayer[nRespawnPlayer].stat = PLAYERSTAT_WAIT;
}

//========================
//�擾����
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
//==========================================
//
//�v���C���[�v���O����[player.cpp]
//Author:�Ό��D�n  ���V����
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "PvP_player.h"
#include "model.h"
#include "input.h"
#include "wall.h"
#include "score.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "gauge.h"
#include "sound.h"
#include "charge_cylinder.h"
#include "charge_effect.h"
#include "attack_effect.h"
#include "tremor_effect.h"
#include "item.h"
#include "meshfield.h"
#include "bonus.h"
#include "particle.h"
#include "comai.h"
#include "conversioninput.h"

//�}�N��
#define PLAYER_HIPDROP_WAIT		(15)		//�q�b�v�h���b�v�́u�J�n�E���n�v�d������
#define PLAYER_BLOWING_POWER	(5.0f)		//�q�b�v�h���b�v���ꂽ���̈ړ���
#define PLAYER_HIPDROP_POWER	(-15.0f)	//�q�b�v�h���b�v����Ƃ��̈ړ���
#define PLAYER_ROTATE_SPEED		(0.02f * D3DX_PI)	//��]���x
#define PLAYER_HIPSPIN_SPEED	(-0.5f)		//�q�b�v�h���b�v�X�s���̉�]�l
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//�q�b�v�h���b�v�X�s�������Ƃ��̂P��������Ƃ�l

//�ړ��ʊ֌W
#define PLAYER_MOVE_SPEED		(20.0f)		//�v���C���[�ړ����x
#define PLAYER_JUMP_SPEED		(7.7f)		//�v���C���[�W�����v���x
#define ACCELERATION_GRAVITY	(9.8f)		//�d�͉����x
#define PLAYER_WEIGHT			(50)		//����
#define PLAYER_POWER_ADD		(0.025f)	//�ړ��̋����̑����l
#define DUMP_COEF				(0.04f)		//�����W��
#define DEBUG_PLAYER_MOVE_SPEED	(5.0f)		//[�f�o�b�O�p]���ʂɈړ�����Ƃ��̈ړ���
#define DECIMAL_PLACE			(1)			//�����_�扽�ʂ܂ňړ����Ă��邱�Ƃɂ��邩
#define DOWN_HEIGHT				(-1200.0f)	//�_�E������Ƃ��鍂��
#define HIPDROP_RADIUS			(60.0f)		//�q�b�v�h���b�v����͈�
#define REBOUND_RATIO			(0.15f)		//����������̎����̕��̈ړ��ʊ���

//�A�C�e���֌W
#define ACCELERATION_CONS		(0.5f)		//�����萔�i1.0�őS���n���j
#define ACCELERATION_ITEMMAG	(1.5f)		//�A�C�e���������̋����{��
#define DEFANCE_CONS			(0.0f)		//�h��萔�i1.0�Ŋ��S�h��j
#define DEFANCE_ITEMADD			(0.3f)		//�A�C�e���������̋�����
#define INVINCIBLE_ATK			(1.0f)		//���G��Ԃ̎����̕ϊ�����
#define INVINCIBLE_DEF			(0.0f)		//���G��Ԃ̑���̕ϊ�����

//�S�[�X�g�����
#define GOAST_ALPHA				(0.25f)		//�s�����x
#define GOAST_FLASHSTART		(240)		//�_�ŊJ�n����c�莞��
#define GOAST_FLASHPULSE		(20)		//�_�ł̐؂�ւ�����

#define PLAYER_SIZE_WIDTH		(12.5f)
#define PLAYER_SIZE_HEIGHT		(15.0f)
#define PLAYER_SIZE_DEPTH		(20.0f)

//����
#define ROT_WA	(-0.75f * D3DX_PI)	//����
#define ROT_WD	(0.75f * D3DX_PI)	//�E��
#define ROT_SA	(-0.25f * D3DX_PI)	//����
#define ROT_SD	(0.25f * D3DX_PI)	//�E��
#define ROT_W	(1.0f * D3DX_PI)	//��
#define ROT_A	(-0.5f * D3DX_PI)	//��
#define ROT_S	(0.0f * D3DX_PI)	//��
#define ROT_D	(0.5f * D3DX_PI)	//�E

//�����蔻��͈͍\����
typedef struct
{
	D3DXVECTOR3 pos0, pos1, pos2, pos3;
} CollisionPos;

//�v���g
void ControllPlayer(int nPlayerNum);

void MovePlayer(int nPadNum);
void RotatePlayer(int nPadNum);				//MovePlayer ��rot.y �̌v�Z���������c���Ă��܂�

void ChargePlayer(int nChargePlayer);		//�`���[�W����
void DashPlayer(int nDashPlayer);			//�_�b�V������
void JumpPlayer(int nJumpPlayer);			//�W�����v����
void HipDropPlayer(int nHipDropPlayer);		//�q�b�v�h���b�v����

void CollisionPP(int nPlayerNum);			//�v���C���[���m�̏Փ˔���
void CollisionHipDropPP(int nPlayerNum);	//�q�b�v�h���b�v���̏Փ˔���
void GenerateCollision(int nPlayerNum, CollisionPos *pCollision);

void DownPlayer(int nDownPlayerNum);		//�_�E�������v���C���[�̏���
void RespawnPlayer(int nRespawnPlayer);		//���X�|�[������

void DecrementItemTime(int nPlayerNum);		//�A�C�e���J�E���g�����ׂČ��炷
void ItemStateParticle(int nPlayerNum);		//�A�C�e���p�[�e�B�N���\������

//�O���[�o���ϐ�
Player g_aPlayerPvP[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;
bool g_bDebugMove = false;	//[�f�o�b�O�p]���R�ɓ������邩�ǂ���
Player *g_pNotMove[MAX_USE_GAMEPAD];	//�ړ����Ă��Ȃ��v���C���[���m�p�|�C���^

//�����ʒu����
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,ROT_SA,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,50.0f) ,D3DXVECTOR3(0.0f,ROT_SD,0.0f) },
	{ D3DXVECTOR3(-50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,ROT_WA,0.0f) },
	{ D3DXVECTOR3(50.0f,0.0f,-50.0f) ,D3DXVECTOR3(0.0f,ROT_WD,0.0f) },
};

//[�f�o�b�O�p]AI����������v���C���[�w��i�R���g���[���[���h�����Ă���΂����D��j
const bool c_aAIMove[MAX_USE_GAMEPAD] = { false,true,true,true };

//========================
//����������
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�ϐ�������
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//�ϐ�������
		g_aPlayerPvP[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayerPvP[nCntPlayer].posOld = g_aPlayerPvP[nCntPlayer].pos;
		g_aPlayerPvP[nCntPlayer].move = ZERO_SET;
		g_aPlayerPvP[nCntPlayer].moveV0 = ZERO_SET;
		g_aPlayerPvP[nCntPlayer].rot = c_aPosRot[nCntPlayer][1];
		g_aPlayerPvP[nCntPlayer].moveGauge = 0;
		g_aPlayerPvP[nCntPlayer].jumpTime = 0;
		g_aPlayerPvP[nCntPlayer].bJump = false;
		g_aPlayerPvP[nCntPlayer].bHipDrop = false;
		g_aPlayerPvP[nCntPlayer].bHipDropSpin = false;
		g_aPlayerPvP[nCntPlayer].nHipDropWait = 0;
		g_aPlayerPvP[nCntPlayer].bNotMove = true;
		g_aPlayerPvP[nCntPlayer].nRespawnPosNum = nCntPlayer;

		g_aPlayerPvP[nCntPlayer].faceCollider[0] = D3DXVECTOR3(PLAYER_SIZE_WIDTH, 0.0f, PLAYER_SIZE_WIDTH);
		g_aPlayerPvP[nCntPlayer].faceCollider[1] = D3DXVECTOR3(-PLAYER_SIZE_WIDTH, 0.0f, PLAYER_SIZE_WIDTH);

		g_aPlayerPvP[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayerPvP[nCntPlayer].nScore = 0;
		g_aPlayerPvP[nCntPlayer].lastAtkPlayer = -1;
		g_aPlayerPvP[nCntPlayer].nNumHitPlayer = -1;
		g_aPlayerPvP[nCntPlayer].stat = PLAYERSTAT_WAIT;
		g_aPlayerPvP[nCntPlayer].nPlayerNum = nCntPlayer;

		g_aPlayerPvP[nCntPlayer].nATKItemTime = 0;
		g_aPlayerPvP[nCntPlayer].nDEFItemTime = 0;
		g_aPlayerPvP[nCntPlayer].nGhostItemTime = 0;
		g_aPlayerPvP[nCntPlayer].bInvincible = false;
		g_aPlayerPvP[nCntPlayer].nInvincibleTime = 0;

		g_aPlayerPvP[nCntPlayer].pAI = NULL;

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aPlayerPvP[nCntPlayer].animalInst[nCntParts] = {};
		}
		g_aPlayerPvP[nCntPlayer].bUsePlayer = GetUseController_PvP(nCntPlayer);

		g_pNotMove[nCntPlayer] = &g_aPlayerPvP[nCntPlayer];

		//�v���C���[�EAI�ݒ�
		if (GetUseController_PvP(nCntPlayer) == true)
		{//�v���C���[�͕��ʂɎg�p���Ă��邱�Ƃɂ���
			g_aPlayerPvP[nCntPlayer].bUsePlayer = true;
		}
		else if (c_aAIMove[nCntPlayer] == true)
		{//AI�͔]�݂��|�C���^������Ďg�p���Ă��邱�Ƃɂ���
			g_aPlayerPvP[nCntPlayer].pAI = GetAI();
			g_aPlayerPvP[nCntPlayer].bUsePlayer = true;
		}
	}

	//[�f�o�b�O]�R���g���[���[���ڑ�����Ă��Ȃ����1P�̂ݗL��������
	if (GetUseControllerNum_PvP() == 0) 
	{
		g_aPlayerPvP[0].bUsePlayer = true;
	}

	//���̑��ϐ�
	g_bDebugMove = false;
}

//========================
//�I������
//========================
void UninitPlayer(void)
{
	
}

//========================
//�X�V����
//========================
void UpdatePlayer(void)
{
	//[�f�o�b�O�p]���R�ړ��ݒ�
	if (GetKeyboardTrigger(DIK_F8))
	{
		g_bDebugMove = g_bDebugMove ? false : true;
	}

	//�v���C���[�l�����J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//�O��̏�񏉊���
		g_aPlayerPvP[nCntPlayer].lastAtkPlayer = -1;

		//���݂̈ʒu��O��̈ʒu�ɂ���
		g_aPlayerPvP[nCntPlayer].posOld = g_aPlayerPvP[nCntPlayer].pos;

		//�W�����v���Ԃ𑝂₷
		g_aPlayerPvP[nCntPlayer].jumpTime++;

		//�p�[�e�B�N���\��
		ItemStateParticle(nCntPlayer);

		//�A�C�e���������Ԍ��炷
		DecrementItemTime(nCntPlayer);

		if (g_aPlayerPvP[nCntPlayer].bUsePlayer == true)
		{//�g�p���̂ݍs��
			//AI����
			if (g_aPlayerPvP[nCntPlayer].pAI != NULL)
			{//AI
				//AI���R���g���[���[����
				SelectAIMove(&g_aPlayerPvP[nCntPlayer]);
			}
			else
			{//�v���C���[
				//�i���̕����Q�[���p�b�h������g�p����Ă��Ȃ���Ζ����j
				if (GetUseControllerNum_PvP() != 0)
				{
					g_aPlayerPvP[nCntPlayer].bUsePlayer = GetUseController_PvP(nCntPlayer);
				}
			}

			//���쏈��
			ControllPlayer(nCntPlayer);

			//�����蔻���
			if (g_aPlayerPvP[nCntPlayer].nGhostItemTime <= 0)
			{//�S�[�X�g����ԂłȂ����
				CollisionPP(nCntPlayer);
				CollisionIP(nCntPlayer);

				if (g_aPlayerPvP[nCntPlayer].stat == PLAYERSTAT_HIPDROP)
				{//�q�b�v�h���b�v���Ȃ�
					CollisionHipDropPP(nCntPlayer);
				}
			}
		}

		//�g�p����Ă��邩�ɂ�����炸�s��
		//�W�����v�ʐݒ�
		if (g_aPlayerPvP[nCntPlayer].bHipDrop == true)
		{
			g_aPlayerPvP[nCntPlayer].move.y = g_aPlayerPvP[nCntPlayer].moveV0.y;
		}
		else
		{
			g_aPlayerPvP[nCntPlayer].move.y = g_aPlayerPvP[nCntPlayer].moveV0.y - (ACCELERATION_GRAVITY * g_aPlayerPvP[nCntPlayer].jumpTime / MAX_FPS);
		}

		//�ړ��オy<0�ł���A���݂̈ʒu���A�t�B�[���h�ȏ�̍����ɂ���Ȃ�ړ��ʏ���
		if (g_aPlayerPvP[nCntPlayer].pos.y + g_aPlayerPvP[nCntPlayer].move.y < 0.0f && g_aPlayerPvP[nCntPlayer].pos.y >= 0.0f)
		{
			//���_�ʒu����̃v���C���[�̋������v�Z
			float fLength = sqrtf(powf((g_aPlayerPvP[nCntPlayer].pos.x + g_aPlayerPvP[nCntPlayer].move.x), 2) +
				powf((g_aPlayerPvP[nCntPlayer].pos.z + g_aPlayerPvP[nCntPlayer].move.z), 2));

			//���_�ʒu����̋������A�t�B�[���h�̔��a�ȉ��@�@�Ȃ�A�t�B�[���h�ɏ���Ă���
			if (fLength <= GetMeshField()->fRadius)
			{
				if (g_aPlayerPvP[nCntPlayer].bHipDrop == true)
				{//�q�b�v�h���b�v���Ă��Ȃ�G�t�F�N�g�o��
					SetTremorEffect(g_aPlayerPvP[nCntPlayer].pos);
					g_aPlayerPvP[nCntPlayer].bHipDrop = false;    //�q�b�v�h���b�v���ĂȂ�
				}
				g_aPlayerPvP[nCntPlayer].bJump = false;
				g_aPlayerPvP[nCntPlayer].moveV0.y = 0.0f;
				g_aPlayerPvP[nCntPlayer].move.y = 0.0f;
				g_aPlayerPvP[nCntPlayer].jumpTime = 0;
				g_aPlayerPvP[nCntPlayer].pos.y = 0.0f;
				g_aPlayerPvP[nCntPlayer].nHipDropWait = 0;
			}
			else
			{
				g_aPlayerPvP[nCntPlayer].stat = PLAYERSTAT_FALL;
			}
		}

		if (g_aPlayerPvP[nCntPlayer].pos.y <= DOWN_HEIGHT)
		{//�����؂���
			DownPlayer(nCntPlayer);
		}

		//�{�[�i�X�̓����蔻��
		CollisionBonus(g_aPlayerPvP[nCntPlayer].pos, nCntPlayer);
	}

	//�ړ��ʂƏՓ˔�������ƂɈړ�����
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//�ړ����m
		if (g_aPlayerPvP[nCntPlayer].bNotMove == true)
		{
			if (fabsf(g_aPlayerPvP[nCntPlayer].move.x) > 0.0f || fabsf(g_aPlayerPvP[nCntPlayer].move.z) > 0.0f)
			{
				g_aPlayerPvP[nCntPlayer].bNotMove = false;
			}
		}

		if (g_aPlayerPvP[nCntPlayer].lastAtkPlayer == -1)
		{//�Ԃ����ĂȂ��܂��͈ړ��ʌ����ς�		
			//�q�b�v�h���b�v�̍d�����ł͂Ȃ�
			if (g_aPlayerPvP[nCntPlayer].nHipDropWait <= 0)
			{
				//���ʂɈړ�
				g_aPlayerPvP[nCntPlayer].pos += g_aPlayerPvP[nCntPlayer].move;
			}
		}
		else
		{//�Ԃ�����
			//�ړ��ʌ���
			D3DXVECTOR3 moveTmp1 = g_aPlayerPvP[nCntPlayer].move;
			D3DXVECTOR3 moveTmp2 = g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].lastAtkPlayer].move;

			//�����ݒ�
			float fPowerConvertion1, fPowerConvertion2;
			if (g_aPlayerPvP[nCntPlayer].bInvincible == true)
			{//�䖳�G��(����0%,����100%)
				fPowerConvertion1 = INVINCIBLE_DEF;
				fPowerConvertion2 = INVINCIBLE_ATK;
			}
			else if (g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].lastAtkPlayer].bInvincible == true)
			{//���薳�G��(����100%,����0%)
				fPowerConvertion1 = INVINCIBLE_ATK;
				fPowerConvertion2 = INVINCIBLE_DEF;
			}
			else
			{//�ǂ������ނĂ�����Ȃ���
				fPowerConvertion1 = ACCELERATION_CONS * (g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].lastAtkPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayerPvP[nCntPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);

				fPowerConvertion2 = ACCELERATION_CONS * (g_aPlayerPvP[nCntPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMMAG : 1.0f) -
					DEFANCE_CONS + (g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].lastAtkPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);
			}

			g_aPlayerPvP[nCntPlayer].move = (moveTmp2 * fPowerConvertion1) + (moveTmp1 * (1.0f - fPowerConvertion2) * REBOUND_RATIO);
			g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].lastAtkPlayer].move = (moveTmp1 * fPowerConvertion2) + (moveTmp2 * (1.0f - fPowerConvertion1) * REBOUND_RATIO);

			//�ړ��ʌ����ς݈����ɂ���
			g_aPlayerPvP[nCntPlayer].lastAtkPlayer = -1;
			g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].lastAtkPlayer].lastAtkPlayer = -1;

			//���ʂɈړ�
			g_aPlayerPvP[nCntPlayer].pos += g_aPlayerPvP[nCntPlayer].move;
		}

		//�ړ��ʌ���
		g_aPlayerPvP[nCntPlayer].move.x += (0 - g_aPlayerPvP[nCntPlayer].move.x) * DUMP_COEF;
		g_aPlayerPvP[nCntPlayer].move.z += (0 - g_aPlayerPvP[nCntPlayer].move.z) * DUMP_COEF;
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
		Model useAnimal = GetAnimal(g_aPlayerPvP[nCntPlayer].animal);

		//"�v���C���[��"���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_aPlayerPvP[nCntPlayer].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerPvP[nCntPlayer].rot.y, g_aPlayerPvP[nCntPlayer].rot.x, g_aPlayerPvP[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].mtxWorld, &g_aPlayerPvP[nCntPlayer].mtxWorld, &mtxRot);

		//�ʒu���f
		D3DXMatrixTranslation(&mtxTrans, g_aPlayerPvP[nCntPlayer].pos.x, g_aPlayerPvP[nCntPlayer].pos.y, g_aPlayerPvP[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].mtxWorld, &g_aPlayerPvP[nCntPlayer].mtxWorld, &mtxTrans);

		//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerPvP[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (useAnimal.aParts[nCntParts].bUse == true)
			{
				D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p
				D3DXMATRIX mtxParent;					//�e�̃}�g��

				//�����V�d�l
				//���ŃI�t�Z�b�g�����̂܂܎g���i�A�j���[�V�����g���悤�ɂȂ���������āj
				g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].pos = useAnimal.aParts[nCntParts].posOffset;
				g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].rot = useAnimal.aParts[nCntParts].rotOffset;

				//"���f����"���[���h�}�g���b�N�X������
				D3DXMatrixIdentity(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld);

				//�����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].rot.y, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].rot.x, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

				//�ʒu���f
				D3DXMatrixTranslation(&mtxTransModel, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].pos.x, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].pos.y, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

				//�p�[�c�̐e�}�g���ݒ�
				if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
				{
					mtxParent = g_aPlayerPvP[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayerPvP[nCntPlayer].mtxWorld;
				}

				//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
				D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

				//"���f����"���[���h�}�g���b�N�X�ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld);

				//�}�e���A���f�[�^�ւ̃|�C���^�擾
				pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
				{
					//�S�[�X�g����Ԃ��l�������ύX�p�}�e���A���ϐ�
					D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

					//�S�[�X�g����ԂȂ甼�����Őݒ�
					if (g_aPlayerPvP[nCntPlayer].nGhostItemTime > GOAST_FLASHSTART)
					{
						matChange.Diffuse.a = GOAST_ALPHA;
					}
					else if (g_aPlayerPvP[nCntPlayer].nGhostItemTime > 0 && g_aPlayerPvP[nCntPlayer].nGhostItemTime % (GOAST_FLASHPULSE * 2) < GOAST_FLASHPULSE)
					{
						matChange.Diffuse.a = GOAST_ALPHA;
					}
					//�}�e���A���ݒ�
					pDevice->SetMaterial(&matChange);

					//�e�N�X�`���ݒ�
					pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

					//���f���`��
					useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				/*------------------------------------------------------------------
				�e�̕`��		Author:���V���� �Ό��D�n
				--------------------------------------------------------------------*/
				if (g_aPlayerPvP[nCntPlayer].pos.y >= 0.0f)
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
					D3DXMatrixMultiply(&mtxShadow, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxShadow);

					//�V���h�E�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

					//�}�e���A���f�[�^�ւ̃|�C���^���擾
					pMat = (D3DXMATERIAL *)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//�}�e���A���f�[�^����

						//���F�ɐݒ�						//���Ȕ����𖳂���
						MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

						//�}�e���A���ݒ�
						pDevice->SetMaterial(&MatCopy);

						//�e�N�X�`���ݒ�
						pDevice->SetTexture(0, NULL);

						//���f���`��
						useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
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
	g_aPlayerPvP[nChargePlayer].moveGauge += PLAYER_POWER_ADD;

	if (g_aPlayerPvP[nChargePlayer].moveGauge >= PLAYER_POWER_MAX)
	{
		g_aPlayerPvP[nChargePlayer].moveGauge = PLAYER_POWER_MAX;
		StopSound(SOUND_LABEL_SE_ENERGY_00);
	}
	if (g_aPlayerPvP[nChargePlayer].moveGauge <= PLAYER_POWER_ADD)
	{
		PlaySound(SOUND_LABEL_SE_ENERGY_00);
	}

	g_aPlayerPvP[nChargePlayer].stat = PLAYERSTAT_CHARGE;
	SetChargeEffect(g_aPlayerPvP[nChargePlayer].pos, nChargePlayer);
	SetChargeCylinder(g_aPlayerPvP[nChargePlayer].pos, nChargePlayer);
}

//========================
//�v���C���[�̃_�b�V������
//========================
void DashPlayer(int nDashPlayer)
{
	PlaySound(SOUND_LABEL_SE_GRASSDASH);

	//�i�s�����̐ݒ�
	g_aPlayerPvP[nDashPlayer].move.x = -sinf(g_aPlayerPvP[nDashPlayer].rot.y) * g_aPlayerPvP[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;
	g_aPlayerPvP[nDashPlayer].move.z = -cosf(g_aPlayerPvP[nDashPlayer].rot.y) * g_aPlayerPvP[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;

	g_aPlayerPvP[nDashPlayer].stat = PLAYERSTAT_DASH;
	g_aPlayerPvP[nDashPlayer].moveGauge = 0;
}

//========================
//�v���C���[�̃q�b�v�h���b�v����
//========================
void HipDropPlayer(int nHipDropPlayer)
{
	PlaySound(SOUND_LABEL_SE_HIPDROP);

	g_aPlayerPvP[nHipDropPlayer].moveV0.y = PLAYER_HIPDROP_POWER;		//�q�b�v�h���b�v�̍~�����x����
	g_aPlayerPvP[nHipDropPlayer].move.x = 0.0f;						//X�EZ�̈ړ��ʏ���
	g_aPlayerPvP[nHipDropPlayer].move.z = 0.0f;
	g_aPlayerPvP[nHipDropPlayer].moveGauge = 0.0f;
	g_aPlayerPvP[nHipDropPlayer].jumpTime = 0;							//�W�����v���ԃ��Z�b�g
	g_aPlayerPvP[nHipDropPlayer].bHipDrop = true;						//�q�b�v�h���b�v���Ă���
	g_aPlayerPvP[nHipDropPlayer].bHipDropSpin = true;					//�X�s�����Ă���
	g_aPlayerPvP[nHipDropPlayer].stat = PLAYERSTAT_HIPDROP;
}

//========================
//�v���C���[�̃W�����v����
//========================
void JumpPlayer(int nJumpPlayer)
{
	PlaySound(SOUND_LABEL_SE_JUMP);

	g_aPlayerPvP[nJumpPlayer].moveV0.y = PLAYER_JUMP_SPEED;//�ړ��ʐݒ�
	g_aPlayerPvP[nJumpPlayer].jumpTime = 0;	//�W�����v���ԃ��Z�b�g
	g_aPlayerPvP[nJumpPlayer].bJump = true;
	g_aPlayerPvP[nJumpPlayer].stat = PLAYERSTAT_JUMP;
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

	//���_
	CollisionPos collPos;

	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMove[2];

	//2��2���_��2
	float fAreaARight[2], fAreaALeft[2], fAreaBRight[2], fAreaBLeft[2];
	float fAreaAUp[2], fAreaADown[2], fAreaBUp[2], fAreaBDown[2];

	//�����f�̈ʒu�l��
	D3DXVECTOR3 posTemp = g_aPlayerPvP[nPlayerNum].pos + g_aPlayerPvP[nPlayerNum].move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)
		{
			//���_�ݒ�
			GenerateCollision(nCntOtherPlayer, &collPos);

			//�x�N�g�����߂�
			//move
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				vecMove[nCntCollision] = (posTemp + g_aPlayerPvP[nPlayerNum].faceCollider[nCntCollision]) - (g_aPlayerPvP[nPlayerNum].posOld + g_aPlayerPvP[nPlayerNum].faceCollider[nCntCollision]);
			}

			//X
			//�E�����̌v�Z
			vecLineRight = collPos.pos1 - collPos.pos0;
			vecToPosRight = posTemp - collPos.pos0;
			vecToPosOldRight = g_aPlayerPvP[nPlayerNum].posOld - collPos.pos0;

			//�������̌v�Z
			vecLineLeft = collPos.pos3 - collPos.pos2;
			vecToPosLeft = posTemp - collPos.pos2;
			vecToPosOldLeft = g_aPlayerPvP[nPlayerNum].posOld - collPos.pos2;

			//Z
			//������̌v�Z
			vecLineUp = collPos.pos2 - collPos.pos1;
			vecToPosUp = posTemp - collPos.pos1;
			vecToPosOldUp = g_aPlayerPvP[nPlayerNum].posOld - collPos.pos1;
			//�������̌v�Z
			vecLineDown = collPos.pos0 - collPos.pos3;
			vecToPosDown = posTemp - collPos.pos3;
			vecToPosOldDown = g_aPlayerPvP[nPlayerNum].posOld - collPos.pos3;

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
						if (posTemp.y >= g_aPlayerPvP[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayerPvP[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayerPvP[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayerPvP[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayerPvP[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayerPvP[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayerPvP[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							//1.0f = pushback
							float fRate = fAreaARight[nCntCollision] / fAreaBRight[nCntCollision];
							g_aPlayerPvP[nPlayerNum].pos.x = collPos.pos0.x + (vecLineRight.x * fRate) - sinf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayerPvP[nPlayerNum].pos.z = collPos.pos0.z + (vecLineRight.z * fRate) - cosf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) <= 0.0f && (vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f)
				{
					if (fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] >= 0.0f && fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayerPvP[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayerPvP[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayerPvP[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayerPvP[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayerPvP[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayerPvP[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayerPvP[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							float fRate = fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision];
							g_aPlayerPvP[nPlayerNum].pos.x = collPos.pos2.x + (vecLineLeft.x * fRate) + sinf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayerPvP[nPlayerNum].pos.z = collPos.pos2.z + (vecLineLeft.z * fRate) + cosf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
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
						if (posTemp.y >= g_aPlayerPvP[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayerPvP[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayerPvP[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayerPvP[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayerPvP[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayerPvP[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayerPvP[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							float fRate = fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision];
							g_aPlayerPvP[nPlayerNum].pos.x = collPos.pos1.x + (vecLineUp.x * fRate) + cosf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayerPvP[nPlayerNum].pos.z = collPos.pos1.z + (vecLineUp.z * fRate) - sinf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							break;
						}
					}
				}
				else if ((vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) <= 0.0f && (vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f)
				{
					if (fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] >= 0.0f && fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= g_aPlayerPvP[nCntOtherPlayer].pos.y && posTemp.y <= g_aPlayerPvP[nCntOtherPlayer].pos.y + PLAYER_SIZE_HEIGHT)
						{
							if (fabsf(g_aPlayerPvP[nPlayerNum].move.x) > 0.0f || fabsf(g_aPlayerPvP[nPlayerNum].move.z) > 0.0f)
							{//�����Ă�
								g_aPlayerPvP[nPlayerNum].lastAtkPlayer = nCntOtherPlayer;
								g_aPlayerPvP[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;

								SetAttackEffect(g_aPlayerPvP[nCntOtherPlayer].pos, nCntOtherPlayer);
							}
							float fRate = fAreaADown[nCntCollision] / fAreaBDown[nCntCollision];
							g_aPlayerPvP[nPlayerNum].pos.x = collPos.pos3.x + (vecLineDown.x * fRate) - cosf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
							g_aPlayerPvP[nPlayerNum].pos.z = collPos.pos3.z + (vecLineDown.z * fRate) + sinf(g_aPlayerPvP[nCntOtherPlayer].rot.y) / D3DX_PI * 1.0f;
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
void CollisionHipDropPP(int nPlayerNum)
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

	//���_
	CollisionPos collPos;

	D3DXVECTOR3 vecLineX, vecToPosX, vecToPosOldX;
	D3DXVECTOR3 vecLineZ, vecToPosZ, vecToPosOldZ;
	D3DXVECTOR3 vecMove;
	float fAreaAX, fAreaBX, fAreaAZ, fAreaBZ;

	//�����f�̈ʒu�l��
	D3DXVECTOR3 posTemp = g_aPlayerPvP[nPlayerNum].pos + g_aPlayerPvP[nPlayerNum].move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
	{
		if (nCntOtherPlayer != nPlayerNum)
		{
			//���_�ݒ�
			GenerateCollision(nCntOtherPlayer, &collPos);

			//�x�N�g�����߂�
			//move
			vecMove = posTemp - g_aPlayerPvP[nPlayerNum].posOld;

			//X
			vecLineX = collPos.pos1 - collPos.pos0;
			vecToPosX = posTemp - collPos.pos0;
			vecToPosOldX = g_aPlayerPvP[nPlayerNum].posOld - collPos.pos0;

			//Z
			vecLineZ = collPos.pos2 - collPos.pos1;
			vecToPosZ = posTemp - collPos.pos1;
			vecToPosOldZ = g_aPlayerPvP[nPlayerNum].posOld - collPos.pos1;

			//�����蔻��{��
			//X
			//�ʐϋ��߂�
			fAreaAX = TASUKIGAKE(vecToPosX.x, vecToPosX.y, vecMove.x, vecMove.y);
			fAreaBX = TASUKIGAKE(vecLineX.x, vecLineX.y, vecMove.x, vecMove.y);
			fAreaAZ = TASUKIGAKE(vecToPosZ.z, vecToPosZ.y, vecMove.z, vecMove.y);
			fAreaBZ = TASUKIGAKE(vecLineZ.z, vecLineZ.y, vecMove.z, vecMove.y);
			//����AND�͈͓�
			float fHeight = posTemp.y - g_aPlayerPvP[nCntOtherPlayer].pos.y;
			if (fHeight <= PLAYER_SIZE_HEIGHT)
			{
				float fRadius = PYTHAGORAS(g_aPlayerPvP[nCntOtherPlayer].pos.x - posTemp.x,
					g_aPlayerPvP[nCntOtherPlayer].pos.z - posTemp.z);

				if (fRadius <= HIPDROP_RADIUS)
				{
					//�ړ��ʌv�Z
					float fAngleHipDrop = atan2f(g_aPlayerPvP[nCntOtherPlayer].pos.x - posTemp.x,
						g_aPlayerPvP[nCntOtherPlayer].pos.z - posTemp.z);
					g_aPlayerPvP[nCntOtherPlayer].move.x = sinf(fAngleHipDrop) * PLAYER_BLOWING_POWER;
					g_aPlayerPvP[nCntOtherPlayer].move.z = -cosf(fAngleHipDrop) * PLAYER_BLOWING_POWER;

					//������Ɣ�΂�
					g_aPlayerPvP[nCntOtherPlayer].moveV0.y = PLAYER_BLOWING_POWER;
					g_aPlayerPvP[nCntOtherPlayer].jumpTime = 0;

					//�U�����ꂽ�����ɂ���
					g_aPlayerPvP[nCntOtherPlayer].nNumHitPlayer = nPlayerNum;
				}
			}
		}
	}
}

//========================
//�����蔻�萶��
//========================
void GenerateCollision(int nPlayerNum, CollisionPos *pCollision)
{
	//�e���_�����߂�̂ɕK�v�ȕϐ�
	D3DXMATRIX mtxWorld;	//��]�̃x�[�X�ƂȂ郏�[���h�}�g��
	D3DXMATRIX mtxRot;		//��]�s��
	D3DXMATRIX mtxTrans;	//���ׂĕϊ���̍s��
	D3DXVECTOR3 vtxTrans;	//�ϊ���̓_

	//-mtx----------------------------------------------------------------------------------------------------------------------------
	//��]�s������
	D3DXMatrixIdentity(&mtxWorld);

	//�������f
	D3DXMatrixRotationY(&mtxRot, FIX_ROT(g_aPlayerPvP[nPlayerNum].rot.y));
	D3DXMatrixMultiply(&mtxWorld, &mtxRot, &mtxWorld);

	//�ʒu���f
	mtxWorld._41 = g_aPlayerPvP[nPlayerNum].pos.x;
	mtxWorld._42 = 0.0f;
	mtxWorld._43 = g_aPlayerPvP[nPlayerNum].pos.z;

	//-mtx----------------------------------------------------------------------------------------------------------------------------
	
	//-pos0---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	D3DXVECTOR3 vtxPos = D3DXVECTOR3(
		- PLAYER_SIZE_WIDTH,
		0.0f,
		- PLAYER_SIZE_DEPTH);


	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	pCollision->pos0.x = vtxTrans.x;
	pCollision->pos0.y = vtxTrans.y;
	pCollision->pos0.z = vtxTrans.z;
	//-pos0---------------------------------------------------------------------------------------------------------------------------

	//-pos1---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	vtxPos = D3DXVECTOR3(
		+ PLAYER_SIZE_WIDTH,
		0.0f,
		- PLAYER_SIZE_DEPTH);

	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//�ϊ���̓_�̏ꏊ����
	pCollision->pos1.x = vtxTrans.x;
	pCollision->pos1.y = vtxTrans.y;
	pCollision->pos1.z = vtxTrans.z;
	//-pos1---------------------------------------------------------------------------------------------------------------------------

	//-pos2---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	vtxPos = D3DXVECTOR3(
		+ PLAYER_SIZE_WIDTH,
		0.0f,
		+ PLAYER_SIZE_DEPTH);

	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//�ϊ���̓_�̏ꏊ����
	pCollision->pos2.x = vtxTrans.x;
	pCollision->pos2.y = vtxTrans.y;
	pCollision->pos2.z = vtxTrans.z;
	//-pos2---------------------------------------------------------------------------------------------------------------------------

	//-pos3---------------------------------------------------------------------------------------------------------------------------
	//��]�s������Ƃɒ��_����]����
	//�x�[�X���R�s�[
	mtxTrans = mtxWorld;

	//0�x�̂Ƃ��̓_��u��
	vtxPos = D3DXVECTOR3(
		- PLAYER_SIZE_WIDTH,
		0.0f,
		+ PLAYER_SIZE_DEPTH);

	//��]�s��Ƃ������킹��
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//�ϊ���̓_�̏ꏊ����
	pCollision->pos3.x = vtxTrans.x;
	pCollision->pos3.y = vtxTrans.y;
	pCollision->pos3.z = vtxTrans.z;
	//-pos3---------------------------------------------------------------------------------------------------------------------------
}

//========================
//�v���C���[�̏���
//========================
void ControllPlayer(int nPlayerNum)
{
	//�q�b�v�h���b�v���łȂ���Α���ł���
	if (g_aPlayerPvP[nPlayerNum].bHipDrop == false)
	{
		if ((int)(g_aPlayerPvP[nPlayerNum].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
			&& (int)(g_aPlayerPvP[nPlayerNum].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
		{//�������ꓮ���Ă���Č���Ȃ���ˁi�قړ����Ă��Ȃ��j
			if (GetButton(nPlayerNum, INPUTTYPE_PRESS, BUTTON_X) == true)
			{//X�{�^���������ꂽ					 
			 //�v���C���[�̃`���[�W����
				ChargePlayer(nPlayerNum);
			}
			else
			{
				RotatePlayer(nPlayerNum);
				g_aPlayerPvP[nPlayerNum].stat = PLAYERSTAT_WAIT;
			}

			if (GetButton(nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X) == true)
			{//X�{�^���������ꂽ
			 //�v���C���[�̃_�b�V������
				DashPlayer(nPlayerNum);
			}
		}
		else
		{
			g_aPlayerPvP[nPlayerNum].moveGauge = 0;
		}

		//�W�����v�E�q�b�v�h���b�v
		if (GetButton(nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A) == true == true && g_aPlayerPvP[nPlayerNum].bHipDrop == false)
		{
			if (g_aPlayerPvP[nPlayerNum].bJump)
			{
				HipDropPlayer(nPlayerNum);									//�v���C���[�̃q�b�v�h���b�v����
				g_aPlayerPvP[nPlayerNum].nHipDropWait = PLAYER_HIPDROP_WAIT;	//�q�b�v�h���b�v�̊J�n�d����ݒ�
			}
			else
			{
				JumpPlayer(nPlayerNum);			//�v���C���[�̃W�����v����
			}

			MovePlayer(nPlayerNum);
		}

		////��]
		//if (GetButton(nPlayerNum, INPUTTYPE_PRESS, BUTTON_X) == false)
		//{//X�{�^����������Ă��Ȃ�
		//	RotatePlayer(nPlayerNum);
		//}
	}
	//�q�b�v�h���b�v��
	else
	{
		//�q�b�v�h���b�v�d�����Ԃ�����
		if (g_aPlayerPvP[nPlayerNum].nHipDropWait > 0)
		{
			g_aPlayerPvP[nPlayerNum].nHipDropWait--;		//�d�����Ԃ����炵�Ă���
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
			g_aPlayerPvP[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayerPvP[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayerPvP[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y�����̃X�e�B�b�N�����X���Ă���
		g_aPlayerPvP[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * DEBUG_PLAYER_MOVE_SPEED;
	}
	//�L�[�{�[�h��
	else if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
			g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_aPlayerPvP[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
		g_aPlayerPvP[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerPvP[nPadNum].rot.y + D3DX_PI))) * DEBUG_PLAYER_MOVE_SPEED;
	}
	else
	{
		return;
	}

	//�{�^������ɉ����ăv���C���[�E�J�������_�E�����_�ړ�
	g_aPlayerPvP[nPadNum].pos.x += g_aPlayerPvP[nPadNum].move.x;
	g_aPlayerPvP[nPadNum].pos.z += g_aPlayerPvP[nPadNum].move.z;

	//�ړ��ʏ���
	g_aPlayerPvP[nPadNum].move.x = 0.0f;
	g_aPlayerPvP[nPadNum].move.z = 0.0f;
}

//========================
//�v���C���[�̌����ύX����
//========================
void RotatePlayer(int nPadNum)
{
	if (!g_bDebugMove)
	{//�����[�X�p
	 //�Q�[���p�b�h��
		if (GetStick(nPadNum) == CONVSTICK_LEFT)
		{
			g_aPlayerPvP[nPadNum].rot.y -= PLAYER_ROTATE_SPEED;
		}
		else if(GetStick(nPadNum) == CONVSTICK_RIGHT)
		{
			g_aPlayerPvP[nPadNum].rot.y += PLAYER_ROTATE_SPEED;
		}
	}
	else
	{//�f�o�b�O�p
		//���f���ړ�
		//�Q�[���p�b�h��
		if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
		{//X�����̃X�e�B�b�N���X���Ă���
			g_aPlayerPvP[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
		}
		else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y�����̃X�e�B�b�N�����X���Ă���
			g_aPlayerPvP[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
		}
		//�L�[�{�[�h��
		else if (GetKeyboardPress(DIK_W) == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WA);
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_WD);
			}
			else
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_W);
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SA);
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_SD);
			}
			else
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_S);
			}
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_A);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_D);
		}
		else
		{
			return;
		}
	}
}

//========================
//�_�E�������v���C���[�̏���
//========================
void DownPlayer(int nDownPlayerNum)
{
	if (g_aPlayerPvP[nDownPlayerNum].nNumHitPlayer != -1)
	{//���Ă��ė������ꍇ
		g_aPlayerPvP[g_aPlayerPvP[nDownPlayerNum].nNumHitPlayer].nScore++;
		AddScore(1, g_aPlayerPvP[nDownPlayerNum].nNumHitPlayer);
	}
	else
	{//�����P�Ɏ��ł����ꍇ
		for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
		{
			if (nCntOtherPlayer != nDownPlayerNum)
			{
				g_aPlayerPvP[nCntOtherPlayer].nScore++;
				AddScore(1, nCntOtherPlayer);
			}
		}
	}

	PlaySound(SOUND_LABEL_SE_DROP);

	RespawnPlayer(nDownPlayerNum);
}

//========================
//�v���C���[�̃��X�|�[������
//========================
void RespawnPlayer(int nRespawnPlayer)
{
	//�e�X�e�[�^�X�Đݒ�
	//�ʒu�ݒ�i���Ԃ�Ȃ��悤�ɂ���j
	for (int nCntNUse = 0; nCntNUse < MAX_USE_GAMEPAD; nCntNUse++)
	{
		if (g_pNotMove[nCntNUse] == NULL || g_pNotMove[nCntNUse]->bNotMove == false)
		{
			//�O�Ƀ��X�|�[�������ʒu�ԍ��̃|�C���^��NULL�ɂ���
			if (g_pNotMove[g_aPlayerPvP[nRespawnPlayer].nRespawnPosNum] == &g_aPlayerPvP[nRespawnPlayer])
			{
				g_pNotMove[g_aPlayerPvP[nRespawnPlayer].nRespawnPosNum] = NULL;
			}			

			//�V�����ʒu������
			g_aPlayerPvP[nRespawnPlayer].nRespawnPosNum = nCntNUse;
			g_aPlayerPvP[nRespawnPlayer].pos = D3DXVECTOR3(c_aPosRot[nCntNUse][0].x, 70.0f, c_aPosRot[nCntNUse][0].z);
			//�p�x����������Ȃ炱���ɏ���

			//�����Ă��Ȃ����Ƃɂ���
			g_aPlayerPvP[nRespawnPlayer].bNotMove = true;

			//�Q�Ɨp�|�C���^���
			g_pNotMove[nCntNUse] = &g_aPlayerPvP[nRespawnPlayer];
			break;
		}
	}

	//���̑��ݒ�
	g_aPlayerPvP[nRespawnPlayer].posOld = g_aPlayerPvP[nRespawnPlayer].pos;
	g_aPlayerPvP[nRespawnPlayer].move = ZERO_SET;
	g_aPlayerPvP[nRespawnPlayer].moveV0 = ZERO_SET;
	g_aPlayerPvP[nRespawnPlayer].rot = ZERO_SET;
	g_aPlayerPvP[nRespawnPlayer].moveGauge = 0;
	g_aPlayerPvP[nRespawnPlayer].jumpTime = 0;
	g_aPlayerPvP[nRespawnPlayer].nHipDropWait = 0;
	g_aPlayerPvP[nRespawnPlayer].bJump = true;
	g_aPlayerPvP[nRespawnPlayer].bHipDrop = false;
	g_aPlayerPvP[nRespawnPlayer].bHipDropSpin = false;

	g_aPlayerPvP[nRespawnPlayer].lastAtkPlayer = -1;
	g_aPlayerPvP[nRespawnPlayer].nNumHitPlayer = -1;
	g_aPlayerPvP[nRespawnPlayer].stat = PLAYERSTAT_WAIT;

	g_aPlayerPvP[nRespawnPlayer].nATKItemTime = 0;
	g_aPlayerPvP[nRespawnPlayer].nDEFItemTime = 0;
	g_aPlayerPvP[nRespawnPlayer].nGhostItemTime = 0;
}

//========================
//�A�C�e���J�E���g���炷����
//========================
void DecrementItemTime(int nPlayerNum)
{
	g_aPlayerPvP[nPlayerNum].nATKItemTime--;
	g_aPlayerPvP[nPlayerNum].nDEFItemTime--;
	g_aPlayerPvP[nPlayerNum].nGhostItemTime--;
}

//========================
//�A�C�e���p�[�e�B�N���\������
//========================
void ItemStateParticle(int nPlayerNum)
{
	if (g_aPlayerPvP[nPlayerNum].nATKItemTime > 0)
	{
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 12.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_ATK);
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 7.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_ATK);
	}


	if (g_aPlayerPvP[nPlayerNum].nDEFItemTime > 0)
	{
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 5.0f, 15, PARTICLE_NORMAL, OBJECT_PLAYER_DEF);
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 7.0f, 15, PARTICLE_NORMAL, OBJECT_PLAYER_DEF);
	}


	if (g_aPlayerPvP[nPlayerNum].nGhostItemTime > 0)
	{
		//SetParticle(g_aPlayer[nPlayerNum].pos, 12.0f, 10, PARTICLE_NORMAL, OBJECT_PLAYER_GOAST);
		//SetParticle(g_aPlayer[nPlayerNum].pos, 12.0f, 10, PARTICLE_NORMAL, OBJECT_PLAYER_GOAST);
	}
}

//========================
//�擾����
//========================
Player *GetPlayer(void)
{
	return &g_aPlayerPvP[0];
}
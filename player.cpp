//==========================================
//
//�v���C���[�v���O����[player.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "wall.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>

//�}�N��
#define PLAYER_MOVE_SPEED	(7.0f)	//�v���C���[�ړ����x
#define DUMP_COEF			(0.4f)	//�����W��
#define BULLET_SPEED		(20.0f)	//�e�̑���

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

		g_aPlayer[nCntPlayer].nIdxShadow = -1;
		g_aPlayer[nCntPlayer].bUse = false;
	}
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
	
}

//========================
//�擾����
//========================
Player *GetPlayer(void)
{
	return &g_aPlayer[0];
}
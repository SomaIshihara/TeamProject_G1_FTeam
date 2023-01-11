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
Player g_player;
int g_nIdxShadow = -1;

//========================
//����������
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�ϐ�������
	g_player.pos = ZERO_SET;
	g_player.posOld = g_player.pos;
	g_player.move = ZERO_SET;
	g_player.rot = ZERO_SET;
	
	g_player.nNumModel = 0;

	g_nIdxShadow = -1;

	//�Ίp���̒����E�p�x
	//��U�Ȃ�
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
	//���݂̈ʒu��O��̈ʒu�ɂ���
	g_player.posOld = g_player.pos;

	//�J���������ɉ����ăv���C���[�̌�������
	g_player.rot.y = -(float)fmod(GetCamera()->rot.y + 1.0f * D3DX_PI + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;

	//���f���ړ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_WD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_W + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SA + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_SD + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_S + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
			g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
			g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_A + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
		g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_player.rot.y = -(float)fmod(GetCamera()->rot.y + ROT_D + D3DX_PI + (D3DX_PI * 2), D3DX_PI * 2) - D3DX_PI;
		g_player.move.x = sinf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_player.move.z = cosf(FIX_ROT((g_player.rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
	}
	else
	{
		
	}

	//�{�^������ɉ����ăv���C���[�E�J�������_�E�����_�ړ�
	g_player.pos.x += g_player.move.x;
	g_player.pos.z += g_player.move.z;

	//�����蔻���
	//��U�Ȃ�

	GetCamera()->posV.x += g_player.pos.x - g_player.posOld.x;
	GetCamera()->posV.z += g_player.pos.z - g_player.posOld.z;
	GetCamera()->posR.x += g_player.pos.x - g_player.posOld.x;
	GetCamera()->posR.z += g_player.pos.z - g_player.posOld.z;

	//�ړ��ʌ���
	g_player.move.x += (0 - g_player.move.x) * DUMP_COEF;
	g_player.move.z += (0 - g_player.move.z) * DUMP_COEF;

	//�e�ʒu�ݒ�
	//��U�Ȃ�
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
	return &g_player;
}
/*==========================================================================================================================================================

�Q�[�W�̏���[gauge.cpp]
Author:��{����

============================================================================================================================================================*/
#include "main.h"
#include "gauge.h"
#include "PvP_player.h"
#include "input.h"
#include "color.h"
#include "sound.h"

//�}�N����` 
#define NUM_GAUGE		(GAUGETYPE_MAX * MAX_USE_GAMEPAD)	//�Q�[�W�S�l���̑S�̐�

//�������֘A
#define MIN_COL			(0.0f)		//�F�̏����l

//���̃}�N����`
#define GAUGE_WIDTH		(200.0f)	//�Q�[�W���̏����l
#define GAUGE_HEIGHT	(50.0f)		//�����̍ŏ��l
#define MIN_DEPTH		(0.0f)		//���s���̍ŏ��l

//rhw�֘A
#define MAX_RHW		(1.0f)	//rhw�̍ő�l

//�F�֘A
#define USE_RED		(1.0f)	//�ԐF���g�p����
#define	NUSE_RED	(0.0f)	//�ԐF���g�p���Ȃ�
#define USE_GREEN	(1.0f)	//�ΐF���g�p����
#define NUSE_GREEN	(0.0f)	//�ΐF���g�p���Ȃ�
#define USE_BLUE	(1.0f)	//�F���g�p����
#define	NUSE_BLUE	(0.0f)	//�F���g�p���Ȃ�
#define	USE_ALPHA	(1.0f)	//�A���t�@�[�l

//�e�N�X�`���֘A
#define	MAX_TEX		(1.0f)	//�e�N�X�`���̍ő�l
#define MIN_TEX		(0.0f)	//�e�N�X�`���̍ŏ��l

//�X�V�֘A
#define MAX_NUM		(1.0f)	//�����鐔(�ő�l)
#define	UNQUAR_NUM	(0.9f)	//���鐔(�ő�l-10)
#define UNHALF_NUM  (0.75f)	//���鐔(�ő�l-25)
#define HALF_NUM	(0.5f)	//���鐔(�ő�l-50)
#define QUAR_NUM	(0.25f)	//���鐔(�ő�l-75)

//�O���[�o���ϐ�	
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGauge = NULL;					//�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureGauge[GAUGETYPE_MAX] = {};	//�e�N�X�`���ւ̃|�C���^(�Q�[�W��)
Gauge					g_Gauge[MAX_USE_GAMEPAD][GAUGETYPE_MAX];					//�Q�[�W���

const D3DXVECTOR3		g_Pos[MAX_USE_GAMEPAD] =				//�Q�[�W�̈ʒu
{
	D3DXVECTOR3(26.0f,286.0f,0.0f),
	D3DXVECTOR3(1054.0f,286.0f,0.0f),
	D3DXVECTOR3(26.0f,646.0f,0.0f),
	D3DXVECTOR3(1054.0f,646.0f,0.0f),
};

//����������
void InitGauge(void)
{
	//�|�C���^�[�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,			//==============
		"data\\TEXTURE\\GaugeBg.png",			//�Q�[�W�w�i
		&g_pTextureGauge[GAUGETYPE_BG]);		//===========

	D3DXCreateTextureFromFile(pDevice,			//==============
		"data\\TEXTURE\\Gauge.png",				//�Q�[�W�{��
		&g_pTextureGauge[GAUGETYPE_NORMAL]);	//===========

	D3DXCreateTextureFromFile(pDevice,			//==============
		"data\\TEXTURE\\GaugeFrame.png",		//�Q�[�W�̘g
		&g_pTextureGauge[GAUGETYPE_FRAME]);		//===========

												//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_GAUGE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	//�S�l�S���̃Q�[�W�̒��_��񏉊���
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		for (int nCntGauge = 0; nCntGauge < GAUGETYPE_MAX; nCntGauge++, pVtx += VTX_MAX)
		{//�Q�[�W��
			//�ʒu��������
			g_Gauge[nCntPlayer][nCntGauge].pos = g_Pos[nCntPlayer];

			//���_���W�̐ݒ�
			switch (nCntGauge)
			{
			case GAUGETYPE_NORMAL:
				pVtx[VTX_LE_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[VTX_RI_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[VTX_LE_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, GAUGE_HEIGHT, 0.0f);
				pVtx[VTX_RI_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, GAUGE_HEIGHT, 0.0f);
				break;
			default:
				pVtx[VTX_LE_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
				pVtx[VTX_RI_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(GAUGE_WIDTH, 0.0f, 0.0f);	//�E��
				pVtx[VTX_LE_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, GAUGE_HEIGHT, 0.0f);	//����
				pVtx[VTX_RI_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(GAUGE_WIDTH, GAUGE_HEIGHT, 0.0f);	//�E��
				break;
			}

			//rhw�̐ݒ�
			pVtx[VTX_LE_UP].rhw = MAX_RHW;
			pVtx[VTX_RI_UP].rhw = MAX_RHW;
			pVtx[VTX_LE_DO].rhw = MAX_RHW;
			pVtx[VTX_RI_DO].rhw = MAX_RHW;

			//���_�J���[�ݒ�
			pVtx[VTX_LE_UP].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);

			//�e�N�X�`�����̐ݒ�
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(MIN_TEX, MIN_TEX);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_TEX, MIN_TEX);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(MIN_TEX, MAX_TEX);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_TEX, MAX_TEX);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//�I������
void UninitGauge(void)
{
	for (int nCntGauge = 0; nCntGauge < GAUGETYPE_MAX; nCntGauge++)
	{//�Q�[�W��
	 //�e�N�X�`���̔j��
		if (g_pTextureGauge[nCntGauge] != NULL)
		{
			g_pTextureGauge[nCntGauge]->Release();
			g_pTextureGauge[nCntGauge] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//�X�V����
void UpdateGauge(void)
{
	Player *pPlayer = GetPlayer();		//�v���C���[���̎擾	

	VERTEX_2D *pVtx;					//�|�C���^��ݒ�

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	//�v���C���[�P�̃Q�[�W�Ƀ|�C���^�����킹��
	pVtx += VTX_MAX;

	for (int nCntGauge = 0; nCntGauge < MAX_USE_GAMEPAD; nCntGauge++, pPlayer++, pVtx += NUM_GAUGE)
	{//�Q�[�W��
	 //�v�Z(���̃Q�[�W�ʁ@/�@�ő�Q�[�W��)
		float Parcent = pPlayer->moveGauge / PLAYER_POWER_MAX;

		if (Parcent > UNQUAR_NUM)
		{//�ԐF
		 //���_�J���[�ݒ�
			pVtx[VTX_LE_UP].col = RGBA_RED;
			pVtx[VTX_RI_UP].col = RGBA_RED;
			pVtx[VTX_LE_DO].col = RGBA_RED;
			pVtx[VTX_RI_DO].col = RGBA_RED;
		}

		else if (Parcent > UNHALF_NUM)
		{//�I�����W�F
		 //���_�J���[�ݒ�
			pVtx[VTX_LE_UP].col = RGBA_ORANGE;
			pVtx[VTX_RI_UP].col = RGBA_ORANGE;
			pVtx[VTX_LE_DO].col = RGBA_ORANGE;
			pVtx[VTX_RI_DO].col = RGBA_ORANGE;
		}

		else if (Parcent > HALF_NUM)
		{//��
		 //���_�J���[�ݒ�
			pVtx[VTX_LE_UP].col = RGBA_BLUE;
			pVtx[VTX_RI_UP].col = RGBA_BLUE;
			pVtx[VTX_LE_DO].col = RGBA_BLUE;
			pVtx[VTX_RI_DO].col = RGBA_BLUE;
		}

		else if (Parcent > QUAR_NUM)
		{//���F
		 //���_�J���[�ݒ�
			pVtx[VTX_LE_UP].col = RGBA_RIGHTBLUE;
			pVtx[VTX_RI_UP].col = RGBA_RIGHTBLUE;
			pVtx[VTX_LE_DO].col = RGBA_RIGHTBLUE;
			pVtx[VTX_RI_DO].col = RGBA_RIGHTBLUE;
		}

		else
		{//���F
		 //���_�J���[�ݒ�
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}

		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos.x = g_Gauge[nCntGauge][1].pos.x + 0.0f;						//����
		pVtx[VTX_RI_UP].pos.x = g_Gauge[nCntGauge][1].pos.x + GAUGE_WIDTH * Parcent;	//�E��
		pVtx[VTX_LE_DO].pos.x = g_Gauge[nCntGauge][1].pos.x + 0.0f;						//����
		pVtx[VTX_RI_DO].pos.x = g_Gauge[nCntGauge][1].pos.x + GAUGE_WIDTH * Parcent;	//�E��
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//�`�揈��
void DrawGauge(void)
{
	//�|�C���^�[�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
	{//�Q�[�W��
	 //�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGauge[nCntGauge % GAUGETYPE_MAX]);

		//�|���S���̕`��	
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * VTX_MAX, 2);
	}
}
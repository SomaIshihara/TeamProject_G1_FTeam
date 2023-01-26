/*==========================================================================================================================================================

�Q�[�W�̏���[gauge.cpp]
Author:��{����

============================================================================================================================================================*/

#include "main.h"
#include "Gauge.h"
#include "player.h"
#include "input.h"
#include "color.h"

//�}�N����` 

//�������֘A
#define MIN_POS			(0.0f)		//�ʒu�̏����l
#define MIN_COL			(0.0f)		//�F�̏����l
#define GAUGE_WIDTH		(50.0f)		//�Q�[�W���̏����l
#define SMALL_WIDTH		(200.0f)	//���̏����l
#define SMALL_HEIGHT	(200.0f)	//�����̏����l

//�ʒu�֘A
#define MAX_WIDTH	(400.0f)	//���̍ő�l
#define MIN_WIDTH	(50.0f)		//���̍ŏ��l
#define MAX_HEIGHT	(680.0f)	//�����̍ő�l
#define MIN_HEIGHT	(650.0f)	//�����̍ŏ��l
#define MIN_DEPTH	(0.0f)		//���s���̍ŏ��l

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
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGauge = NULL;						//�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9      g_pTextureGauge[GAUGETYPE_MAX] = {};		//�e�N�X�`���ւ̃|�C���^(�Q�[�W��)
Gauge					g_Gauge[GAUGETYPE_MAX];						//�Q�[�W���		

//����������
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^�擾

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

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
	
	//�Q�[�W���̏���������
	for (int nCntGauge = 0; nCntGauge < GAUGETYPE_MAX; nCntGauge++)
	{
		g_Gauge[nCntGauge].pos = D3DXVECTOR3(MIN_POS, MIN_POS, MIN_POS);
		g_Gauge[nCntGauge].col = D3DXCOLOR(MIN_COL, MIN_COL, MIN_COL, MIN_COL);
		g_Gauge[nCntGauge].fGaugeWidth = GAUGE_WIDTH;
		g_Gauge[nCntGauge].fWidth = SMALL_WIDTH;
		g_Gauge[nCntGauge].fHeight = SMALL_HEIGHT;
		g_Gauge[nCntGauge].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_USE_GAMEPAD * VTX_MAX * GAUGETYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < GAUGETYPE_MAX; nCntGauge++, pVtx += VTX_MAX)
	{//�Q�[�W��
		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(MIN_WIDTH, MIN_HEIGHT, MIN_DEPTH);	//����
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(MAX_WIDTH, MIN_HEIGHT, MIN_DEPTH);	//�E��
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(MIN_WIDTH, MAX_HEIGHT, MIN_DEPTH);	//����
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(MAX_WIDTH, MAX_HEIGHT, MIN_DEPTH);	//�E��

		if (nCntGauge % GAUGETYPE_MAX == GAUGETYPE_NORMAL)
		{//�Q�[�W�{�̂��ǂݍ��܂ꂽ��
			//���_���W�̐ݒ�
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(GAUGE_WIDTH, MIN_HEIGHT, MIN_DEPTH);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(GAUGE_WIDTH, MIN_HEIGHT, MIN_DEPTH);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(GAUGE_WIDTH, MAX_HEIGHT, MIN_DEPTH);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(GAUGE_WIDTH, MAX_HEIGHT, MIN_DEPTH);
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

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	//
	pVtx += VTX_MAX;

	for (int nCntGauge = 0; nCntGauge < MAX_USE_GAMEPAD; nCntGauge++, pPlayer++, pVtx += (VTX_MAX * GAUGETYPE_MAX))
	{//�Q�[�W��
		//�v�Z(���̃Q�[�W�ʁ@/�@�ő�Q�[�W��)
		float Parcent = pPlayer->moveGauge / 2.25f;
		
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
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(GAUGE_WIDTH, MIN_HEIGHT, MIN_DEPTH);				//����
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(MAX_WIDTH * Parcent, MIN_HEIGHT, MIN_DEPTH);		//�E��
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(GAUGE_WIDTH, MAX_HEIGHT, MIN_DEPTH);				//����
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(MAX_WIDTH * Parcent, MAX_HEIGHT, MIN_DEPTH);		//�E��		
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//�`�揈��
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < GAUGETYPE_MAX; nCntGauge++)
	{//�Q�[�W��
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGauge[nCntGauge]);

		//�|���S���̕`��	
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
	}
}
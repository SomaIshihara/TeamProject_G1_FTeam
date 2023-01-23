/*==========================================================================================================================================================

�Q�[�W�̏���[gauge.cpp]
Author:��{����

============================================================================================================================================================*/

#include "main.h"
#include "Gauge.h"
#include "player.h"
#include "input.h"

//�}�N����`
#define NUM_GAUGE		(3)		//�Q�[�W�̐�
#define DISPLAY_ADD		(4)		//�\�����镪�������Z 
#define TOP_NUM			(12)	//���_��

//�������֘A
#define MIN_POS		(0.0f)
#define MIN_COL		(0.0f)
#define MIN_GA_WID	(200)
#define MIN_WIDTH	(200)
#define MIN_HEIGHT	(200)

//�ʒu�֘A

//rhw�֘A
#define MAX_RHW		(1.0f)	//rhw�̍ő�l

//�F�֘A
#define USE_RED		(1.0f)	//�ԐF���g�p����
#define	NUSE_RED	(0.0f)	//�ԐF���g�p���Ȃ�
#define USE_GREEN	(1.0f)	//�ΐF���g�p����
#define NUSE_GREEN	(0.0f)	//�ΐF���g�p���Ȃ�
#define USE_BLUE	(1.0f)	//�F���g�p����
#define	NUSE_BLUE	(0.0f)	//�F���g�p���Ȃ�
#define	USE_ALPHA	(1.0f)	//�A���t�@�[

//�e�N�X�`���֘A
#define	MAX_TEX		(1.0f)	//�e�N�X�`���̍ő�l
#define MIN_TEX		(0.0f)	//�e�N�X�`���̍ŏ��l
#define TEX_ZERO	(0)		//�e�N�X�`���̓ǂݍ���(0)
#define TEX_ONE		(1)		//�e�N�X�`���̓ǂݍ���(1)
#define TEX_TWO		(2)		//�e�N�X�`���̓ǂݍ���(2)

//�X�V�֘A
#define COUNT_ONE	(1)		//�J�E���g��1
#define MAX_NUM		(100)	//�����鐔(�ő�l)
#define UNHALF_NUM  (75)	//���鐔(�ő�l-25)
#define HALF_NUM	(50)	//���鐔(�ő�l�̔���)
#define QUAR_NUM	(25)	//���鐔(�ő�l��4����1)

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGauge = NULL;					//�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9      g_pTextureGauge[NUM_GAUGE] = {};		//�e�N�X�`���ւ̃|�C���^
Gauge g_Gauge[NUM_GAUGE];										//�Q�[�W���

//����������
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^�擾

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,		//==============
		"data\\TEXTURE\\GaugeBg.png",		//�Q�[�W�w�i
		&g_pTextureGauge[TEX_ZERO]);		//===========

	D3DXCreateTextureFromFile(pDevice,		//==============
		"data\\TEXTURE\\Gauge.png",			//�Q�[�W�{��
		&g_pTextureGauge[TEX_ONE]);			//===========

	D3DXCreateTextureFromFile(pDevice,		//==============
		"data\\TEXTURE\\GaugeFrame.png",	//�Q�[�W�̘g
		&g_pTextureGauge[TEX_TWO]);			//===========
	
	//�Q�[�W���̏���������
	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
	{
		g_Gauge[nCntGauge].pos = D3DXVECTOR3(MIN_POS, MIN_POS, MIN_POS);
		g_Gauge[nCntGauge].col = D3DXCOLOR(MIN_COL, MIN_COL, MIN_COL, MIN_COL);
		g_Gauge[nCntGauge].fGaugeWidth = MIN_GA_WID;
		g_Gauge[nCntGauge].fWidth = MIN_WIDTH;
		g_Gauge[nCntGauge].fHeight = MIN_HEIGHT;
		g_Gauge[nCntGauge].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * TOP_NUM * NUM_GAUGE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++, pVtx + DISPLAY_ADD)
	{
		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(50.0f, 650.0f, 0.0f);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(500.0f, 650.0f, 0.0f);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(50.0f, 710.0f, 0.0f);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(500.0f, 710.0f, 0.0f);
						
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
	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
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

	int Parcent;						//�Q�[�W�ʂ��i�[����ϐ�
	int MaxGauge = 100;					//�ő�Q�[�W��		

	for (int nCntGauge = 0; nCntGauge < MAX_USE_GAMEPAD; nCntGauge++,pPlayer++)
	{
		//�v�Z(���̃Q�[�W�ʁ@/�@�ő�Q�[�W��)
		Parcent = pPlayer->moveGauge / MaxGauge;

		if (nCntGauge == COUNT_ONE)
		{
			if (Parcent > 90)
			{//�ԐF
				
			}

			else if (Parcent > 75)
			{//�I�����W�F

			}

			else if (Parcent > 50)
			{//��

			}

			else if(Parcent > 25)
			{//���F

			}

			else
			{//���F

			}
		}
	}
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

	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGauge[nCntGauge]);

		//�|���S���̕`��	
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
	}
}
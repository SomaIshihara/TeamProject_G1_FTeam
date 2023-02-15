/*==========================================================================================================================================================

�^�C�g����ʏ���[title.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "title.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

#define NUM_TITLE_TEX	(3)									// �^�C�g���Ɏg���摜�̐�
#define TITLE_INFOFILE	"data/CSV/title.csv"				// �^�C�g���̏�񂪓������t�@�C����

#define TITLE_POS		(D3DXVECTOR3(NIL_F, NIL_F, NIL_F))	//�^�C�g���̈ʒu
#define TITLE_WIDTH		(510.0f)							//�^�C�g���̕�
#define TITLE_HEIGHT	(215.0f)							//�^�C�g���̍���

//�^�C�g���̏��\����
typedef struct
{
	D3DXVECTOR2 pos;		//�ʒu
	float		fWidth;		//��
	float		fHeight;	//����
	TITLE		type;		//���
}Title;

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;					//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9			g_pTextureTitle[NUM_TITLE_TEX] = {};	//�e�N�X�`���̃|�C���^
Title						g_Title[NUM_TITLE_TEX];					//�^�C�g���̏��
int							g_select;								//�I��ԍ�

//�^�C�g����ʂɎg�p����A�C�R�������̃p�X
const char *c_apTitleTexName[NUM_TITLE_TEX] = {
	"data/TEXTURE/title.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
};

//------------------------------------------------
//				�^�C�g���̏���������
//------------------------------------------------
void InitTitle(void)
{
	PlaySound(SOUND_LABEL_BGM_TITLE);

	//�f�o�C�X�ւ̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_TITLE_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void* *)&pVtx, 0);

	//�^�C�g���̏��ǂݍ���
	LoadTitle();

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++, pVtx += VTX_MAX)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apTitleTexName[nCntTitle], &g_pTextureTitle[nCntTitle]);

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x - g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y - g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x + g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y - g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x - g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y + g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x + g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y + g_Title[nCntTitle].fHeight, NIL_F);
		}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].rhw = RHW;
			pVtx[VTX_RI_UP].rhw = RHW;
			pVtx[VTX_LE_DO].rhw = RHW;
			pVtx[VTX_RI_DO].rhw = RHW;
		}

		//���_�J���[�̐ݒ�
		{
			if (nCntTitle >= 1)
			{
				pVtx[VTX_LE_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[VTX_RI_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[VTX_LE_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[VTX_RI_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
			else
			{
				pVtx[VTX_LE_UP].col = XCOL_WHITE;
				pVtx[VTX_RI_UP].col = XCOL_WHITE;
				pVtx[VTX_LE_DO].col = XCOL_WHITE;
				pVtx[VTX_RI_DO].col = XCOL_WHITE;
			}
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		g_Title[nCntTitle].type = (TITLE)nCntTitle;
	}

	g_select = TITLE_EXIT;

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//�^�C�g���̏��ǂݍ��ݏ���
void LoadTitle(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(TITLE_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);		//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
		{
			fscanf(pFile, "%s", &aData[0]);	//��s�ǂݍ���

			g_Title[nCntTitle].pos.x = fatof(strtok(&aData[0], ","));		// �\���ʒu������
			g_Title[nCntTitle].pos.y = fatof(strtok(NULL, ","));			// �\���ʒu������

			g_Title[nCntTitle].fWidth = fatof(strtok(NULL, ","));			// ��������
			g_Title[nCntTitle].fHeight = fatof(strtok(NULL, ","));			// ��������
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//------------------------------------------------
//				�^�C�g���̏I������
//------------------------------------------------
void UninitTitle(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	StopSound(SOUND_LABEL_BGM_TITLE);
}

//------------------------------------------------
//				�^�C�g���̍X�V����
//------------------------------------------------
void UpdateTitle(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void* *)&pVtx, 0);

	
		//
		DirecUpTitle(0);

		//
		DirecDownTitle(0);
	
	switch (g_select)
	{
	case TITLE_START:
		
		pVtx += g_select * VTX_MAX;

		pVtx[VTX_LE_UP].col = XCOL_WHITE;
		pVtx[VTX_RI_UP].col = XCOL_WHITE;
		pVtx[VTX_LE_DO].col = XCOL_WHITE;
		pVtx[VTX_RI_DO].col = XCOL_WHITE;

		pVtx += VTX_MAX;

		pVtx[VTX_LE_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_LE_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		break;

	case TITLE_EXIT:

		pVtx += g_select * VTX_MAX;

		pVtx[VTX_LE_UP].col = XCOL_WHITE;
		pVtx[VTX_RI_UP].col = XCOL_WHITE;
		pVtx[VTX_LE_DO].col = XCOL_WHITE;
		pVtx[VTX_RI_DO].col = XCOL_WHITE;

		pVtx -= VTX_MAX;

		pVtx[VTX_LE_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_UP].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_LE_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[VTX_RI_DO].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		break;
	}

	//Enter�������ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		switch (g_select)
		{
		case TITLE_START:

			//�Q�[���I����ʂɑJ��
			SetFade(MODE_SELECTGAME);

			break;

		case TITLE_EXIT:

			break;
		}
		
		//�^�C�g�����艹�Đ�
		PlaySound(SOUND_LABEL_SE_TITLE_DECIDE);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//------------------------------------------------
//				�^�C�g���̕`�揈��
//------------------------------------------------
void DrawTitle(void)
{
	//�f�o�C�X�̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * VTX_MAX, 2);
	}
}
//
void DirecUpTitle(int NumPad)
{
	if (GetKeyboardTrigger(DIK_UP) == true || GetGamepadTrigger(NumPad, XINPUT_GAMEPAD_DPAD_UP) == true)
	{
		switch (g_select)
		{
		case TITLE_START:

			g_select = TITLE_EXIT;

			break;

		case TITLE_EXIT:

			g_select = TITLE_START;

			break;
		}
	}
}
//
void DirecDownTitle(int NumPad)
{
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetGamepadTrigger(NumPad, XINPUT_GAMEPAD_DPAD_DOWN) == true)
	{
		switch (g_select)
		{
		case TITLE_START:

			g_select = TITLE_EXIT;

			break;

		case TITLE_EXIT:

			g_select = TITLE_START;

			break;
		}
	}
}
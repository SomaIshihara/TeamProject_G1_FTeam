//==========================================
//
//�v���C���[�I���v���O����[SelPlayer.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "SelPlayer.h"
#include "conversioninput.h"
#include "model.h"

//�}�N��
#define FRAME_USE_TEXTURE	(2)			//�^�C�v�g�Ŏg���e�N�X�`����
#define STR_USE_TEXTURE		(3)			//�^�C�v�����Ŏg���e�N�X�`����
#define FRAME_SIZE_WIDTH	(312.0f)	//�g�̃T�C�Y�i���j
#define FRAME_SIZE_HEIGHT	(108.0f)	//�g�̃T�C�Y�i�����j
#define STR_OFFSET_WIDTH	(37.0f)		//�g�ƕ����̈ʒu���炵�i���j
#define STR_OFFSET_HEIGHT	(29.0f)		//�g�ƕ����̈ʒu���炵�i�����j
#define STR_SIZE_WIDTH		(238.0f)	//�����̃T�C�Y�i���j
#define STR_SIZE_HEIGHT		(50.0f)		//�����̃T�C�Y�i�����j

//�O���[�o��
PLAYERTYPE g_playerType[MAX_USE_GAMEPAD];
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeFrame;					//�^�C�v�g�̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTypeFrame[FRAME_USE_TEXTURE];		//�^�C�v�g�̃e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeStr;						//�^�C�v�����̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTypeStr[STR_USE_TEXTURE];			//�^�C�v�����̃e�N�X�`���|�C���^

//�^�C�v�g�̃t�@�C���p�X
const char *c_apFilePathTypeFrame[FRAME_USE_TEXTURE] =
{
	"SelPlayer_Frame_01.png",
	"SelPlayer_Frame_02.png"
};

//�^�C�v�g�̈ʒu
const D3DXVECTOR3 c_aPosTypeFrame[MAX_USE_GAMEPAD] =
{
	D3DXVECTOR3(2.5f,550.0f,0.0f),
	D3DXVECTOR3(323.5f,550.0f,0.0f),
	D3DXVECTOR3(634.5f,550.0f,0.0f),
	D3DXVECTOR3(955.5f,550.0f,0.0f)
};

//�^�C�v�g�w�i�̐F
const D3DXCOLOR c_aColTypeFrameBG[MAX_USE_GAMEPAD] =
{
	D3DXCOLOR(1.0f,0.5f,0.5f,1.0f),
	D3DXCOLOR(0.24f,0.72f,0.92f,1.0f),
	D3DXCOLOR(0.72f,0.9f,0.73f,1.0f),
	D3DXCOLOR(1.0f,0.95f,0.67f,1.0f)
};

//�^�C�v�����̃t�@�C���p�X
const char *c_apFilePathTypeStr[STR_USE_TEXTURE] =
{
	"SelPlayer_Str_01.png",
	"SelPlayer_Str_02.png",
	"SelPlayer_Str_03.png"
};

//========================
//����������
//========================
void InitSelPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//�^�C�v�g
	//�e�N�X�`���ǂݍ���
	for (int nCntTypeFrame = 0; nCntTypeFrame < FRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeFrame[nCntTypeFrame],
			&g_pTextureTypeFrame[nCntTypeFrame]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD * FRAME_USE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeFrame,
		NULL);

	//�^�C�v����
	//�e�N�X�`���ǂݍ���
	for (int nCntTypeStr = 0; nCntTypeStr < STR_USE_TEXTURE; nCntTypeStr++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeFrame[nCntTypeStr],
			&g_pTextureTypeFrame[nCntTypeStr]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeFrame,
		NULL);


	//���_�ݒ�

	//�^�C�v�g
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * FRAME_USE_TEXTURE; nCntTypeFrame++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x + FRAME_SIZE_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y + FRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].x + FRAME_SIZE_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntTypeFrame / FRAME_USE_TEXTURE].y + FRAME_SIZE_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//���W�ϊ��p�W���ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		//���������i�w�i�j�Ȃ�F�ς���
		if (nCntTypeFrame / 2 == 0)
		{
			pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
			pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
			pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
			pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame / FRAME_USE_TEXTURE];
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffTypeFrame->Unlock();


	//����
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffTypeStr->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + STR_OFFSET_WIDTH;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + STR_OFFSET_HEIGHT;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + FRAME_SIZE_WIDTH - STR_OFFSET_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + STR_OFFSET_HEIGHT;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + STR_OFFSET_WIDTH;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + FRAME_SIZE_HEIGHT - STR_OFFSET_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + FRAME_SIZE_WIDTH - STR_OFFSET_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + FRAME_SIZE_HEIGHT - STR_OFFSET_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//���W�ϊ��p�W���ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffTypeStr->Unlock();
}

//========================
//�I������
//========================
void UninitSelPlayer(void)
{
	//�Q�[�W�g
	//�e�N�X�`���̔j��
	for (int nCntTypeFrame = 0; nCntTypeFrame < FRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		if (g_pTextureTypeFrame[nCntTypeFrame] != NULL)
		{
			g_pTextureTypeFrame[nCntTypeFrame]->Release();
			g_pTextureTypeFrame[nCntTypeFrame] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffTypeFrame != NULL)
	{
		g_pVtxbuffTypeFrame->Release();
		g_pVtxbuffTypeFrame = NULL;
	}


	//�A�C�R��
	//�e�N�X�`���̔j��
	for (int nCntTypeStr = 0; nCntTypeStr < FRAME_USE_TEXTURE; nCntTypeStr++)
	{
		if (g_pTextureTypeStr[nCntTypeStr] != NULL)
		{
			g_pTextureTypeStr[nCntTypeStr]->Release();
			g_pTextureTypeStr[nCntTypeStr] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffTypeStr != NULL)
	{
		g_pVtxbuffTypeStr->Release();
		g_pVtxbuffTypeStr = NULL;
	}
}

//========================
//�X�V����
//========================
void UpdateSelPlayer(void)
{
	
}

//========================
//�`�揈��
//========================
void DrawSelPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

												//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�^�C�v�g
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffTypeFrame, 0, sizeof(VERTEX_2D));

	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * FRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		//���������i�w�i�j�Ȃ���Z�����ɂ���
		if (nCntTypeFrame / 2 == 0)
		{//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureTypeFrame[nCntTypeFrame % FRAME_USE_TEXTURE]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);

		//���ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}


	//�A�C�R��
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffTypeStr, 0, sizeof(VERTEX_2D));

	for (int nCntTypeStr = 0; nCntTypeStr < MAX_USE_GAMEPAD; nCntTypeStr++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureTypeStr[g_playerType[nCntTypeStr]]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeStr, 2);
	}
}
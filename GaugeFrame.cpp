//==========================================
//
//�Q�[�W�ނ̘g�֌W�v���O����[GaugeFrame.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "GaugeFrame.h"
#include "pvp_game.h"
#include "input.h"
#include "model.h"
#include "item.h"
#include "PvP_player.h"

//�}�N��
#define GAUGE_FRAME_SIZE_WIDTH			(232)	//�v���C���[�̃|���S���T�C�Y�i���j
#define GAUGE_FRAME_SIZE_HEIGHT			(140)	//�v���C���[�̃|���S���T�C�Y�i�����j
#define ANIMALICON_SIZE_WIDTH			(64)	//�A�C�R���T�C�Y�i���j
#define ANIMALICON_SIZE_HEIGHT			(64)	//�A�C�R���T�C�Y�i�����j
#define BUFFICON_SIZE_WIDTH				(48)
#define BUFFICON_SIZE_HEIGHT			(48)
#define ANIMALICON_POS_OFFSET			(4)		//�A�C�R���̈ʒu���炵
#define BUFFICON_POS_OFFSET_WIDTH		(ANIMALICON_POS_OFFSET + ANIMALICON_SIZE_WIDTH + 8)
#define BUFFICON_POS_OFFSET_HEIGHT		(ANIMALICON_POS_OFFSET + 24)

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffGaugeFrame;					//�Q�[�W�g�̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureGaugeFrame[MAX_USE_GAMEPAD];		//�Q�[�W�g�̃e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffAnimalIcon;					//�����A�C�R���̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureAnimalIcon[ANIMAL_MAX];			//�����A�C�R���̃e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBuffIcon;						//�o�t�A�C�R���̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBuffIcon[ITEMTYPE_MAX];			//�o�t�A�C�R���̃e�N�X�`���|�C���^

//�t�@�C���p�X
const char* c_apFilePathGaugeFrame[MAX_USE_GAMEPAD] =
{
	"data\\TEXTURE\\PlayerFrame_01.png",
	"data\\TEXTURE\\PlayerFrame_02.png",
	"data\\TEXTURE\\PlayerFrame_03.png",
	"data\\TEXTURE\\PlayerFrame_04.png"
};

const char* c_apFilePathAnimalIcon[ANIMAL_MAX] =
{
	"data\\TEXTURE\\WildBoar_PlayerIcon.png"
};

const char* c_apFilePathBuffIcon[ITEMTYPE_MAX] = 
{
	"data\\TEXTURE\\Power.png",
	"data\\TEXTURE\\Shield.png",
	"data\\TEXTURE\\Ghost.png",
	"data\\TEXTURE\\Invincible.png"
};

const D3DXVECTOR3		c_aPosGaugeFrame[MAX_USE_GAMEPAD] =				//�Q�[�W�̈ʒu�i����уA�C�R���ʒu�j
{//�Q�l:X = gauge��X - 17.0f, Y = gauge��Y - 80.0f
	D3DXVECTOR3(10.0f,210.0f,0.0f),
	D3DXVECTOR3(SCREEN_WIDTH - GAUGE_FRAME_SIZE_WIDTH - 10.0f,210.0f,0.0f),
	D3DXVECTOR3(10.0f,SCREEN_HEIGHT - GAUGE_FRAME_SIZE_HEIGHT - 10.0f,0.0f),
	D3DXVECTOR3(SCREEN_WIDTH - GAUGE_FRAME_SIZE_WIDTH - 10.0f,SCREEN_HEIGHT - GAUGE_FRAME_SIZE_HEIGHT - 10.0f,0.0f),
};

//========================
//�v���C���[����������
//========================
void InitGaugeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�Q�[�W�g
	//�e�N�X�`���ǂݍ���
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathGaugeFrame[nCntGaugeFrame],
			&g_pTextureGaugeFrame[nCntGaugeFrame]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffGaugeFrame,
		NULL);


	//�����A�C�R��
	//�e�N�X�`���ǂݍ���
	for (int nCntIcon = 0; nCntIcon < ANIMAL_MAX; nCntIcon++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathAnimalIcon[nCntIcon],
			&g_pTextureAnimalIcon[nCntIcon]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffAnimalIcon,
		NULL);


	//�o�t�A�C�R��
	//�e�N�X�`���ǂݍ���
	for (int nCntIcon = 0; nCntIcon < ITEMTYPE_MAX; nCntIcon++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathBuffIcon[nCntIcon],
			&g_pTextureBuffIcon[nCntIcon]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBuffIcon,
		NULL);

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//�Q�[�W�g
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffGaugeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x;
		pVtx[0].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x + GAUGE_FRAME_SIZE_WIDTH;
		pVtx[1].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x;
		pVtx[2].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y + GAUGE_FRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x + GAUGE_FRAME_SIZE_WIDTH;
		pVtx[3].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y + GAUGE_FRAME_SIZE_HEIGHT;
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
	g_pVtxbuffGaugeFrame->Unlock();


	//�����A�C�R��
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffAnimalIcon->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_POS_OFFSET;
		pVtx[0].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_POS_OFFSET;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_SIZE_WIDTH + ANIMALICON_POS_OFFSET;
		pVtx[1].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_POS_OFFSET;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_POS_OFFSET;
		pVtx[2].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_SIZE_HEIGHT + ANIMALICON_POS_OFFSET;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_SIZE_WIDTH + ANIMALICON_POS_OFFSET;
		pVtx[3].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_SIZE_HEIGHT + ANIMALICON_POS_OFFSET;
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
	g_pVtxbuffAnimalIcon->Unlock();


	//�o�t�A�C�R��
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffBuffIcon->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[0].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_POS_OFFSET_HEIGHT;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_SIZE_WIDTH + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[1].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_POS_OFFSET_HEIGHT;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[2].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_SIZE_HEIGHT + BUFFICON_POS_OFFSET_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_SIZE_WIDTH + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[3].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_SIZE_HEIGHT + BUFFICON_POS_OFFSET_HEIGHT;
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
	g_pVtxbuffBuffIcon->Unlock();
}

//========================
//�v���C���[�I������
//========================
void UninitGaugeFrame(void)
{
	//�Q�[�W�g
	//�e�N�X�`���̔j��
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		if (g_pTextureGaugeFrame[nCntGaugeFrame] != NULL)
		{
			g_pTextureGaugeFrame[nCntGaugeFrame]->Release();
			g_pTextureGaugeFrame[nCntGaugeFrame] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffGaugeFrame != NULL)
	{
		g_pVtxbuffGaugeFrame->Release();
		g_pVtxbuffGaugeFrame = NULL;
	}


	//�����A�C�R��
	//�e�N�X�`���̔j��
	for (int nCntIcon = 0; nCntIcon < ANIMAL_MAX; nCntIcon++)
	{
		if (g_pTextureAnimalIcon[nCntIcon] != NULL)
		{
			g_pTextureAnimalIcon[nCntIcon]->Release();
			g_pTextureAnimalIcon[nCntIcon] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffAnimalIcon != NULL)
	{
		g_pVtxbuffAnimalIcon->Release();
		g_pVtxbuffAnimalIcon = NULL;
	}


	//�o�t�A�C�R��
	//�e�N�X�`���̔j��
	for (int nCntIcon = 0; nCntIcon < ITEMTYPE_MAX; nCntIcon++)
	{
		if (g_pTextureBuffIcon[nCntIcon] != NULL)
		{
			g_pTextureBuffIcon[nCntIcon]->Release();
			g_pTextureBuffIcon[nCntIcon] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffBuffIcon != NULL)
	{
		g_pVtxbuffBuffIcon->Release();
		g_pVtxbuffBuffIcon = NULL;
	}
}

//========================
//�v���C���[�X�V����
//========================
void UpdateGaugeFrame(void)
{
	//��
}

//========================
//�v���C���[�`�揈��
//========================
void DrawGaugeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	Player *pPlayer = GetPlayer();				//�v���C���[�擾

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�Q�[�W�g
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffGaugeFrame, 0, sizeof(VERTEX_2D));

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureGaugeFrame[nCntGaugeFrame]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGaugeFrame, 2);
	}


	//�����A�C�R��
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffAnimalIcon, 0, sizeof(VERTEX_2D));

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureAnimalIcon[0]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntIcon, 2);
	}


	//�o�t�A�C�R��
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffBuffIcon, 0, sizeof(VERTEX_2D));

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++, pPlayer++)
	{
		//�e�N�X�`���ݒ�
		if (pPlayer->nATKItemTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[0]);
		}
		else if (pPlayer->nDEFItemTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[1]);
		}
		else if (pPlayer->nGhostItemTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[2]);
		}
		else if (pPlayer->nInvincibleTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[3]);
		}
		else
		{
			continue;
		}

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntIcon, 2);
	}
}
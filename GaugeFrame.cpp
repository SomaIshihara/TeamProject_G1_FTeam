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

//�}�N��
#define GAUGE_FRAME_SIZE_WIDTH	(232)	//�v���C���[�̃|���S���T�C�Y�i���j
#define GAUGE_FRAME_SIZE_HEIGHT	(140)	//�v���C���[�̃|���S���T�C�Y�i�����j
#define ICON_SIZE_WIDTH			(64)	//�A�C�R���T�C�Y�i���j
#define ICON_SIZE_HEIGHT		(64)	//�A�C�R���T�C�Y�i�����j
#define ICON_POS_OFFSET			(4)		//�A�C�R���̈ʒu���炵

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffGaugeFrame;					//�Q�[�W�g�̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureGaugeFrame[MAX_USE_GAMEPAD];		//�Q�[�W�g�̃e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffIcon;							//�A�C�R���̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureIcon[ANIMAL_MAX];					//�A�C�R���̃e�N�X�`���|�C���^

//�t�@�C���p�X
const char* c_apFilePathGaugeFrame[MAX_USE_GAMEPAD] =
{
	"data\\TEXTURE\\PlayerFrame_01.png",
	"data\\TEXTURE\\PlayerFrame_02.png",
	"data\\TEXTURE\\PlayerFrame_03.png",
	"data\\TEXTURE\\PlayerFrame_04.png"
};

const char* c_apFilePathIcon[ANIMAL_MAX] =
{
	"data\\TEXTURE\\WildBoar_PlayerIcon.png"
};

const D3DXVECTOR3		c_aPosGaugeFrame[MAX_USE_GAMEPAD] =				//�Q�[�W�̈ʒu�i����уA�C�R���ʒu�j
{//�Q�l:X = gauge��X - 17.0f, Y = gauge��Y - 80.0f
	D3DXVECTOR3(83.0f,570.0f,0.0f),
	D3DXVECTOR3(383.0f,570.0f,0.0f),
	D3DXVECTOR3(683.0f,570.0f,0.0f),
	D3DXVECTOR3(983.0f,570.0f,0.0f),
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


	//�A�C�R��
	//�e�N�X�`���ǂݍ���
	for (int nCntIcon = 0; nCntIcon < ANIMAL_MAX; nCntIcon++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathIcon[nCntIcon],
			&g_pTextureIcon[nCntIcon]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffIcon,
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


	//�A�C�R��
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffIcon->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosGaugeFrame[nCntIcon].x + ICON_POS_OFFSET;
		pVtx[0].pos.y = c_aPosGaugeFrame[nCntIcon].y + ICON_POS_OFFSET;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosGaugeFrame[nCntIcon].x + ICON_SIZE_WIDTH + ICON_POS_OFFSET;
		pVtx[1].pos.y = c_aPosGaugeFrame[nCntIcon].y + ICON_POS_OFFSET;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosGaugeFrame[nCntIcon].x + ICON_POS_OFFSET;
		pVtx[2].pos.y = c_aPosGaugeFrame[nCntIcon].y + ICON_SIZE_HEIGHT + ICON_POS_OFFSET;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosGaugeFrame[nCntIcon].x + ICON_SIZE_WIDTH + ICON_POS_OFFSET;
		pVtx[3].pos.y = c_aPosGaugeFrame[nCntIcon].y + ICON_SIZE_HEIGHT + ICON_POS_OFFSET;
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
	g_pVtxbuffIcon->Unlock();
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


	//�A�C�R��
	//�e�N�X�`���̔j��
	for (int nCntIcon = 0; nCntIcon < ANIMAL_MAX; nCntIcon++)
	{
		if (g_pTextureIcon[nCntIcon] != NULL)
		{
			g_pTextureIcon[nCntIcon]->Release();
			g_pTextureIcon[nCntIcon] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffIcon != NULL)
	{
		g_pVtxbuffIcon->Release();
		g_pVtxbuffIcon = NULL;
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


	//�A�C�R��
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffIcon, 0, sizeof(VERTEX_2D));

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureIcon[0]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntIcon, 2);
	}
}
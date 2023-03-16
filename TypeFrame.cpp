//==========================================
//
//�v���C���[�I���̃^�C�v�g�v���O����[TypeFrame.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "TypeFrame.h"
#include "input.h"
#include "conversioninput.h"
#include "fade.h"
#include "select_game.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "color.h"
#include "sound.h"
#include "ai.h"

//�}�N��
#define TYPEFRAME_USE_TEXTURE	(3)			//�^�C�v�g�Ŏg���e�N�X�`����
#define TYPESTR_USE_TEXTURE		(3)			//�^�C�v�����Ŏg���e�N�X�`����
#define AIFRAME_USE_TEXTURE		(1)			//COM��Փx�g�Ŏg���e�N�X�`����
#define AISTR_USE_TEXTURE		(3)			//COM��Փx�����Ŏg���e�N�X�`����

#define TYPEFRAME_SIZE_WIDTH	(312.0f)	//�^�C�v�g�̃T�C�Y�i���j
#define TYPEFRAME_SIZE_HEIGHT	(108.0f)	//�^�C�v�g�̃T�C�Y�i�����j
#define TYPESTR_OFFSET_WIDTH	(37.0f)		//�^�C�v�g�ƕ����̈ʒu���炵�i���j
#define TYPESTR_OFFSET_HEIGHT	(29.0f)		//�^�C�v�g�ƕ����̈ʒu���炵�i�����j
#define TYPESTR_SIZE_WIDTH		(238.0f)	//�^�C�v�����̃T�C�Y�i���j
#define TYPESTR_SIZE_HEIGHT		(50.0f)		//�^�C�v�����̃T�C�Y�i�����j
#define AIFRAME_SIZE_WIDTH		(324.0f)	//COM��Փx�g�̃T�C�Y�i���j
#define AIFRAME_SIZE_HEIGHT		(144.0f)		//COM��Փx�g�̃T�C�Y�i�����j
#define AISTR_SIZE_WIDTH		(284.0f)	//COM��Փx�����̃T�C�Y�i���j
#define AISTR_SIZE_HEIGHT		(72.0f)		//COM��Փx�����̃T�C�Y�i�����j
#define AISTR_OFFSET_WIDTH		(20.0f)		//�^�C�v�g�ƕ����̈ʒu���炵�i���j
#define AISTR_OFFSET_HEIGHT		(60.0f)		//�^�C�v�g�ƕ����̈ʒu���炵�i�����j


#define INPUT_JUDGE			(STICK_MAX / 5)	//�X�e�B�b�N���͂������Ɣ��f����X�e�B�b�N�ړ���
#define MIN_PLAYER			(2)				//�v���C���[�̍Œ�l��

//�v���g�^�C�v�錾
bool CheckMinPlayer(void);

//�O���[�o��
PLAYERTYPE g_playerType[MAX_USE_GAMEPAD];

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeFrame;					//�^�C�v�g�̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTypeFrame[TYPEFRAME_USE_TEXTURE];	//�^�C�v�g�̃e�N�X�`���|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeStr;						//�^�C�v�����̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTypeStr[TYPESTR_USE_TEXTURE];		//�^�C�v�����̃e�N�X�`���|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffAIFrame;						//AI�g�̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureAIFrame[AIFRAME_USE_TEXTURE];		//AI�g�̃e�N�X�`���|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffAIStr;						//AI�����̒��_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureAIStr[AISTR_USE_TEXTURE];			//AI�����̃e�N�X�`���|�C���^
int g_nSelectNum = 0;

AIDIFF g_SelAIDiff = AIDIFF_NORMAL;

//�^�C�v�g�̃t�@�C���p�X
const char *c_apFilePathTypeFrame[TYPEFRAME_USE_TEXTURE] =
{
	"data\\TEXTURE\\SelPlayer_Frame_02.png",
	"data\\TEXTURE\\SelPlayer_Frame_01.png",
	"data\\TEXTURE\\SelPlayer_Frame_03.png"
};

//�^�C�v�����̃t�@�C���p�X
const char *c_apFilePathTypeStr[TYPESTR_USE_TEXTURE] =
{
	"data\\TEXTURE\\SelPlayer_Str_01.png",
	"data\\TEXTURE\\SelPlayer_Str_02.png",
	"data\\TEXTURE\\SelPlayer_Str_03.png"
};

//AI�g�̃t�@�C���p�X
const char *c_apFilePathAIFrame[AIFRAME_USE_TEXTURE] =
{
	"data\\TEXTURE\\AIDiff_Frame.png"
};

//AI�����̃t�@�C���p�X
const char *c_apFilePathAIStr[AISTR_USE_TEXTURE] =
{
	"data\\TEXTURE\\AIDiff_Str_01.png",
	"data\\TEXTURE\\AIDiff_Str_02.png",
	"data\\TEXTURE\\AIDiff_Str_03.png"
};

//�^�C�v�g�̈ʒu
const D3DXVECTOR3 c_aPosTypeFrame[MAX_USE_GAMEPAD] =
{
	D3DXVECTOR3(2.5f,550.0f,0.0f),
	D3DXVECTOR3(323.5f,550.0f,0.0f),
	D3DXVECTOR3(634.5f,550.0f,0.0f),
	D3DXVECTOR3(955.5f,550.0f,0.0f)
};

//AI�g�̈ʒu
const D3DXVECTOR3 c_PosAIFrame = D3DXVECTOR3(900.0f, 20.0f, 0.0f);

//�^�C�v�g�w�i�̐F
const D3DXCOLOR c_aColTypeFrameBG[MAX_USE_GAMEPAD] =
{
	D3DXCOLOR(1.0f,0.7f,0.7f,1.0f),
	D3DXCOLOR(0.24f,0.72f,0.92f,1.0f),
	D3DXCOLOR(0.72f,0.9f,0.73f,1.0f),
	D3DXCOLOR(1.0f,0.95f,0.67f,1.0f)
};

//========================
//����������
//========================
void InitTypeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//===============
	//�ϐ�������
	//===============
	for (int nCntType = 0; nCntType < MAX_USE_GAMEPAD; nCntType++)
	{
		g_playerType[nCntType] = PLAYERTYPE_PLAYER;
	}
	g_SelAIDiff = AIDIFF_NORMAL;
	g_nSelectNum = 0;

	//�R���g���[���[���Ȃ����Ă��Ȃ����AI�ɂ���
	for (int nCntAI = 1; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{
		if (GetUseGamepad(nCntAI) == false)
		{
			g_playerType[nCntAI] = PLAYERTYPE_COM;
		}
	}

	//===============
	//�e�N�X�`���E�o�b�t�@����
	//===============
	//�^�C�v�g
	//�e�N�X�`���ǂݍ���
	for (int nCntTypeFrame = 0; nCntTypeFrame < TYPEFRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeFrame[nCntTypeFrame],
			&g_pTextureTypeFrame[nCntTypeFrame]);
	}

	//���_�o�b�t�@�̐���(+1�͑I�𒆂̘g�p�j
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_USE_GAMEPAD * (TYPEFRAME_USE_TEXTURE - 1) + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeFrame,
		NULL);

	//�^�C�v����
	//�e�N�X�`���ǂݍ���
	for (int nCntTypeStr = 0; nCntTypeStr < TYPESTR_USE_TEXTURE; nCntTypeStr++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeStr[nCntTypeStr],
			&g_pTextureTypeStr[nCntTypeStr]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeStr,
		NULL);

	//AI�g
	//�e�N�X�`���ǂݍ���
	for (int nCntAIFrame = 0; nCntAIFrame < AIFRAME_USE_TEXTURE; nCntAIFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathAIFrame[nCntAIFrame],
			&g_pTextureAIFrame[nCntAIFrame]);
	}

	//���_�o�b�t�@�̐���(+1�͑I�𒆂̘g�p�j
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffAIFrame,
		NULL);

	//AI����
	//�e�N�X�`���ǂݍ���
	for (int nCntAIStr = 0; nCntAIStr < AISTR_USE_TEXTURE; nCntAIStr++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathAIStr[nCntAIStr],
			&g_pTextureAIStr[nCntAIStr]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffAIStr,
		NULL);


	//���_�ݒ�

	//�^�C�v�g
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	//�ʒu�Œ�g
	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * (TYPEFRAME_USE_TEXTURE - 1); nCntTypeFrame++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x + TYPEFRAME_SIZE_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y + TYPEFRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x + TYPEFRAME_SIZE_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y + TYPEFRAME_SIZE_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//���W�ϊ��p�W���ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		//���������i�w�i�j�Ȃ�F�ς���
		if (nCntTypeFrame % 2 == 0)
		{
			//���_�J���[
			if (g_playerType[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)] == PLAYERTYPE_PLAYER)
			{//�v���C���[�Ȃ�F�t����
				pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
				pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
				pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
				pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
			}
			else
			{//COM�E�g�p���Ȃ��Ȃ�D�F
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
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

	//�ʒu�Ϙg
	//���_���W�̐ݒ�
	pVtx[0].pos.x = c_aPosTypeFrame[0].x;
	pVtx[0].pos.y = c_aPosTypeFrame[0].y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_aPosTypeFrame[0].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_aPosTypeFrame[0].y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_aPosTypeFrame[0].x;
	pVtx[2].pos.y = c_aPosTypeFrame[0].y + TYPEFRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_aPosTypeFrame[0].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_aPosTypeFrame[0].y + TYPEFRAME_SIZE_HEIGHT;
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

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffTypeFrame->Unlock();


	//����
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffTypeStr->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPESTR_OFFSET_WIDTH;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPESTR_OFFSET_HEIGHT;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPEFRAME_SIZE_WIDTH - TYPESTR_OFFSET_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPESTR_OFFSET_HEIGHT;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPESTR_OFFSET_WIDTH;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPEFRAME_SIZE_HEIGHT - TYPESTR_OFFSET_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPEFRAME_SIZE_WIDTH - TYPESTR_OFFSET_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPEFRAME_SIZE_HEIGHT - TYPESTR_OFFSET_HEIGHT;
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

	//AI�g
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffAIFrame->Lock(0, 0, (void **)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = c_PosAIFrame.x;
	pVtx[0].pos.y = c_PosAIFrame.y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_PosAIFrame.x + AIFRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_PosAIFrame.y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_PosAIFrame.x;
	pVtx[2].pos.y = c_PosAIFrame.y + AIFRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_PosAIFrame.x + AIFRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_PosAIFrame.y + AIFRAME_SIZE_HEIGHT;
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

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffAIFrame->Unlock();


	//AI����
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffAIStr->Lock(0, 0, (void **)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = c_PosAIFrame.x + AISTR_OFFSET_WIDTH;
	pVtx[0].pos.y = c_PosAIFrame.y + AISTR_OFFSET_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_PosAIFrame.x + AISTR_SIZE_WIDTH + AISTR_OFFSET_WIDTH;
	pVtx[1].pos.y = c_PosAIFrame.y + AISTR_OFFSET_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_PosAIFrame.x + AISTR_OFFSET_WIDTH;
	pVtx[2].pos.y = c_PosAIFrame.y + AISTR_SIZE_HEIGHT + AISTR_OFFSET_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_PosAIFrame.x + AISTR_SIZE_WIDTH + AISTR_OFFSET_WIDTH;
	pVtx[3].pos.y = c_PosAIFrame.y + AISTR_SIZE_HEIGHT + AISTR_OFFSET_HEIGHT;
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

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffAIStr->Unlock();
}

//========================
//�I������
//========================
void UninitTypeFrame(void)
{
	//�^�C�v�g
	//�e�N�X�`���̔j��
	for (int nCntTypeFrame = 0; nCntTypeFrame < TYPEFRAME_USE_TEXTURE; nCntTypeFrame++)
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


	//�^�C�v����
	//�e�N�X�`���̔j��
	for (int nCntTypeStr = 0; nCntTypeStr < TYPEFRAME_USE_TEXTURE; nCntTypeStr++)
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

	//AI�g
	//�e�N�X�`���̔j��
	for (int nCntAIFrame = 0; nCntAIFrame < AIFRAME_USE_TEXTURE; nCntAIFrame++)
	{
		if (g_pTextureAIFrame[nCntAIFrame] != NULL)
		{
			g_pTextureAIFrame[nCntAIFrame]->Release();
			g_pTextureAIFrame[nCntAIFrame] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffAIFrame != NULL)
	{
		g_pVtxbuffAIFrame->Release();
		g_pVtxbuffAIFrame = NULL;
	}


	//AI����
	//�e�N�X�`���̔j��
	for (int nCntAIStr = 0; nCntAIStr < AISTR_USE_TEXTURE; nCntAIStr++)
	{
		if (g_pTextureAIStr[nCntAIStr] != NULL)
		{
			g_pTextureAIStr[nCntAIStr]->Release();
			g_pTextureAIStr[nCntAIStr] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffAIStr != NULL)
	{
		g_pVtxbuffAIStr->Release();
		g_pVtxbuffAIStr = NULL;
	}
}

//========================
//�X�V����
//========================
void UpdateTypeFrame(void)
{
	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//�v���C���[�I��
	if (GetUseGamepad(0) == true)
	{
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_A) == true || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{//���̃v���C���[
			g_nSelectNum = (g_nSelectNum + 1) % MAX_USE_GAMEPAD;
		}
		else if (GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{//�߂�
			g_nSelectNum = (g_nSelectNum + (MAX_USE_GAMEPAD - 1)) % MAX_USE_GAMEPAD;
		}

		//�^�C�v�I��
		if (GetStick(0, INPUTTYPE_TRIGGER).y == CONVSTICK_DOWN || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{//���ɓ|����
			//�������񌸂炷
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);

			//�Œ�l���������܂��̓v���C���[�g�p���Ă��Ȃ��i1P�����j�Ȃ�������
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
		}
		else if (GetStick(0, INPUTTYPE_TRIGGER).y == CONVSTICK_UP || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_UP) == true)
		{//��ɓ|����
			//�������񌸂炷
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);

			//�Œ�l���������܂��̓v���C���[�g�p���Ă��Ȃ��i1P�����j�Ȃ�������
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
		}

		//AI�ݒ�
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_LEFT_THUMB) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + (AIDIFF_VIEW -1)) % AIDIFF_VIEW);
		}
		else if (GetGamepadTrigger(0, XINPUT_GAMEPAD_RIGHT_THUMB) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + 1) % AIDIFF_VIEW);
		}

		//����
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_START) == true)
		{
			//�^�C�g�����艹�Đ�
			PlaySoundSE(SOUND_LABEL_SE_TITLE_DECIDE, 0);
			if (GetSelGameMode() == SelGameMode_PVP)
			{
				SetFade(MODE_PvPGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_PvP(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
			else if (GetSelGameMode() == SelGameMode_HDR)
			{
				SetFade(MODE_RaceGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_HDR(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
		}
		//�߂�
		else if (GetGamepadTrigger(0, XINPUT_GAMEPAD_B) == true)
		{
			SetFade(MODE_SELECTGAME);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_A) == true)
		{//���ɓ|����
			g_nSelectNum = (g_nSelectNum + (MAX_USE_GAMEPAD - 1)) % MAX_USE_GAMEPAD;
		}
		else if (GetKeyboardTrigger(DIK_D) == true)
		{//�E�ɓ|����
			g_nSelectNum = (g_nSelectNum + 1) % MAX_USE_GAMEPAD;
		}

		//�^�C�v�I��
		if (GetKeyboardTrigger(DIK_UP) == true)
		{//���ɓ|����
			//�������񌸂炷
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);

			//�Œ�l���������܂��̓v���C���[�g�p���Ă��Ȃ��i1P�����j�Ȃ�������
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
		}
		else if (GetKeyboardTrigger(DIK_DOWN) == true)
		{//��ɓ|����
			 //�������񌸂炷
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);

			//�Œ�l���������܂��̓v���C���[�g�p���Ă��Ȃ��i1P�����j�Ȃ�������
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
		}

		//AI�ݒ�
		if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + (AIDIFF_VIEW - 1)) % AIDIFF_VIEW);
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + 1) % AIDIFF_VIEW);
		}

		//����
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//�^�C�g�����艹�Đ�
			PlaySoundSE(SOUND_LABEL_SE_TITLE_DECIDE, 0);
			if (GetSelGameMode() == SelGameMode_PVP)
			{
				SetFade(MODE_PvPGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_PvP(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
			else if (GetSelGameMode() == SelGameMode_HDR)
			{
				SetFade(MODE_RaceGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_HDR(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
		}
		//�߂�
		else if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
		{
			SetFade(MODE_SELECTGAME);
		}
	}

	//�^�C�v�g���_�ݒ�
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD; nCntTypeFrame++, pVtx += 8)	//�����̂ݕύX�̂���8���炵�Ă���
	{
		//���_�J���[
		if (g_playerType[nCntTypeFrame] == PLAYERTYPE_PLAYER)
		{//�v���C���[�Ȃ�F�t����
			pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame];
		}
		else
		{//COM�E�g�p���Ȃ��Ȃ�D�F
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}

	//�ʒu�ϕ�
	//���_���W�̐ݒ�
	pVtx[0].pos.x = c_aPosTypeFrame[g_nSelectNum].x;
	pVtx[0].pos.y = c_aPosTypeFrame[g_nSelectNum].y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_aPosTypeFrame[g_nSelectNum].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_aPosTypeFrame[g_nSelectNum].y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_aPosTypeFrame[g_nSelectNum].x;
	pVtx[2].pos.y = c_aPosTypeFrame[g_nSelectNum].y + TYPEFRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_aPosTypeFrame[g_nSelectNum].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_aPosTypeFrame[g_nSelectNum].y + TYPEFRAME_SIZE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffTypeFrame->Unlock();
}

//========================
//�`�揈��
//========================
void DrawTypeFrame(void)
{
	int nCntTypeFrame;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�^�C�v�g
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffTypeFrame, 0, sizeof(VERTEX_2D));

	for (nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * (TYPEFRAME_USE_TEXTURE - 1); nCntTypeFrame++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureTypeFrame[nCntTypeFrame % (TYPEFRAME_USE_TEXTURE - 1)]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);
	}

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureTypeFrame[2]);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);


	//�^�C�v����
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffTypeStr, 0, sizeof(VERTEX_2D));

	for (int nCntTypeStr = 0; nCntTypeStr < MAX_USE_GAMEPAD; nCntTypeStr++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureTypeStr[g_playerType[nCntTypeStr]]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeStr, 2);
	}

	//AI�g
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffAIFrame, 0, sizeof(VERTEX_2D));

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureAIFrame[0]);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	//AI����
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffAIStr, 0, sizeof(VERTEX_2D));

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureAIStr[g_SelAIDiff]);

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//�v���C���[���Œ�l���������Ȃ����m�F���鏈��
//========================
bool CheckMinPlayer(void)
{
	int nUsePlayer = 0;

	//���݂̎g�p�l���m�F
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_playerType[nCntPlayer] != PLAYERTYPE_NONE)
		{
			nUsePlayer++;
		}
	}

	//�����Ȃ��ꍇ�͌��炵�Ă�OK
	return nUsePlayer >= MIN_PLAYER ? true : false;
}

//========================
//�v���C���[�g�p�擾����
//========================
bool GetUsePlayer(int nPlayerNum)
{
	return g_playerType[nPlayerNum] != PLAYERTYPE_NONE ? true : false;
}

//========================
//AI��Փx�擾����
//========================
AIDIFF GetAIDiff(void)
{
	return g_SelAIDiff;
}
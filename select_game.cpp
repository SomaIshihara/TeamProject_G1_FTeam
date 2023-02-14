/*==========================================================================================================================================================

//�Q�[�����j���[�̑I������[select_game.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "select_game.h"

//�}�N����`
#define SELECTGAMEMODE_NUM_TEX		(1)								/*�Q�[���I���̃e�N�X�`����*/

//�O���[�o���ϐ��錾
SelectGameMode				g_SelectGameMode;				/*�I�����ꂽ�Q�[�����[�h*/
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffSelectGame = NULL;			/*���_�o�b�t�@�ւ̃|�C���^*/
LPDIRECT3DTEXTURE9			g_pTextureSelectGame[SELECTGAMEMODE_NUM_TEX] = {};	/*�e�N�X�`���̃|�C���^*/

//�Q�[���I���̃e�N�X�`���p�X
const char *c_apSelectGameModeTex[SELECTGAMEMODE_NUM_TEX] = {
	"data\\TEXTURE\\selectgame.png",		/*�Q�[���I��w�i*/
};

//------------------------------------------------
//				�Q�[���I���̏���������
//------------------------------------------------
void InitSelectGame(void)
{
	//�f�o�C�X�ւ̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * SELECTGAMEMODE_NUM_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSelectGame, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectGame->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntSelect = 0; nCntSelect < SELECTGAMEMODE_NUM_TEX; nCntSelect++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apSelectGameModeTex[nCntSelect], &g_pTextureSelectGame[nCntSelect]);

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(			0.0f, 0.0f, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(	SCREEN_WIDTH, 0.0f, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(			0.0f, SCREEN_HEIGHT, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(	SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
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
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffSelectGame->Unlock();
}

//------------------------------------------------
//				�Q�[���I���̏I������
//------------------------------------------------
void UninitSelectGame(void)
{
	//�e�N�X�`���j��
	for (int nCntTex = 0; nCntTex < SELECTGAMEMODE_NUM_TEX; nCntTex++)
	{
		if (g_pTextureSelectGame[nCntTex] != NULL)
		{
			g_pTextureSelectGame[nCntTex]->Release();
			g_pTextureSelectGame[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�j��
	if (g_pVtxBuffSelectGame != NULL)
	{
		g_pVtxBuffSelectGame->Release();
		g_pVtxBuffSelectGame = NULL;
	}
}

//------------------------------------------------
//				�Q�[���I���̍X�V����
//------------------------------------------------
void UpdateSelectGame(void)
{
	//�Q�[�����[�h�I��
	DecideGameMode();
}

//�Q�[�����[�h�I��
void DecideGameMode(void)
{
	//Enter�������ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		//�`���[�g���A����ʂɑJ��
		SetFade(MODE_GAME);

		//�^�C�g�����艹�Đ�
		PlaySound(SOUND_LABEL_SE_TITLE_DECIDE);
	}
}

//------------------------------------------------
//				�Q�[���I���̕`�揈��
//------------------------------------------------
void DrawSelectGame(void)
{
	//�f�o�C�X�̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSelectGame, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntSelect = 0; nCntSelect < SELECTGAMEMODE_NUM_TEX; nCntSelect++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSelectGame[nCntSelect]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelect * VTX_MAX, 2);
	}
}
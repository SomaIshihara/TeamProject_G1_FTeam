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
#define SELECTGAME_INFO		"data\\CSV\\SelectGame.csv"						/*�Q�[���I���̏��*/

//�O���[�o���ϐ��錾
SelectGame					g_SelectGame[SelectGameMenu_MAX];				/*�Q�[���I���̏��*/
SelectGameMenu				g_SelectGameMenu;								/*�I�����ꂽ�Q�[�����[�h*/
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffSelectGame = NULL;					/*���_�o�b�t�@�ւ̃|�C���^*/
LPDIRECT3DTEXTURE9			g_pTextureSelectGame[SelectGameMenu_MAX] = {};	/*�e�N�X�`���̃|�C���^*/

//�Q�[���I���̃e�N�X�`���p�X
const char *c_apSelectGameModeTex[SelectGameMenu_MAX] = {
	"data\\TEXTURE\\BG_selectgame.png",		/*�Q�[���I��w�i*/
	"data\\TEXTURE\\UI_PVP.png",			/*UI_PVP*/
	"data\\TEXTURE\\UI_HipDropRace.png",	/*UI_�q�b�v�h���b�v���[�X*/
};

//------------------------------------------------
//				�Q�[���I���̏���������
//------------------------------------------------
void InitSelectGame(void)
{
	//�I�����j���[���̏�����
	LoadSelectGame();
	g_SelectGameMenu = SelectGameMenu_PVP;

	//�f�o�C�X�ւ̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * SelectGameMenu_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSelectGame, NULL);

	//�e�N�X�`���ǂݍ���
	for (int nCntTex = 0; nCntTex < SelectGameMenu_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apSelectGameModeTex[nCntTex], &g_pTextureSelectGame[nCntTex]);
	}

	//���_���ݒ�
	SetVertexSelectGame();
}

//�Q�[���I���̏��ǂݍ��ݏ���
void LoadSelectGame(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(SELECTGAME_INFO, "r");

	//�t�@�C����ǂݍ��߂�
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);		//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntSelect = 0; nCntSelect < SelectGameMenu_MAX; nCntSelect++)
		{
			fscanf(pFile, "%s", &aData[0]);	//��s�ǂݍ���

			g_SelectGame[nCntSelect].pos.x = fatof(strtok(&aData[0], ","));		// �\���ʒu������
			g_SelectGame[nCntSelect].pos.y = fatof(strtok(NULL, ","));			// �\���ʒu������

			g_SelectGame[nCntSelect].fWidth = fatof(strtok(NULL, ","));			// ��������
			g_SelectGame[nCntSelect].fHeight = fatof(strtok(NULL, ","));		// ��������
			g_SelectGame[nCntSelect].bChoice = false;							// �I������Ă��Ȃ�
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//�Q�[���I�����j���[�̒��_���ݒ菈��
void SetVertexSelectGame(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectGame->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntSelect = 0; nCntSelect < SelectGameMenu_MAX; nCntSelect++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x - g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y - g_SelectGame[nCntSelect].fHeight, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x + g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y - g_SelectGame[nCntSelect].fHeight, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x - g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y + g_SelectGame[nCntSelect].fHeight, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_SelectGame[nCntSelect].pos.x + g_SelectGame[nCntSelect].fWidth, g_SelectGame[nCntSelect].pos.y + g_SelectGame[nCntSelect].fHeight, NIL_F);
		}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].rhw = RHW;
			pVtx[VTX_RI_UP].rhw = RHW;
			pVtx[VTX_LE_DO].rhw = RHW;
			pVtx[VTX_RI_DO].rhw = RHW;
		}

		//���_�J���[�̐ݒ�
		//�J�E���^�[���I������Ă��郁�j���[�ԍ� �܂��� �w�i�Ɠ���
		if (nCntSelect == g_SelectGameMenu || nCntSelect == SelectGameMenu_BG)
		{
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}
		else
		{
			pVtx[VTX_LE_UP].col = XCOL_NO_SELECT;
			pVtx[VTX_RI_UP].col = XCOL_NO_SELECT;
			pVtx[VTX_LE_DO].col = XCOL_NO_SELECT;
			pVtx[VTX_RI_DO].col = XCOL_NO_SELECT;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += VTX_MAX;
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
	for (int nCntTex = 0; nCntTex < SelectGameMenu_MAX; nCntTex++)
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
	//�Q�[�����[�h�̍��ڈړ�
	ChooseGameMode();

	//���_���ݒ�
	SetVertexSelectGame();

	//�Q�[�����[�h�I��
	DecideGameMode();
}

//�Q�[�����[�h��I��
void ChooseGameMode(void)
{
	//�E��󂪉����ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RIGHT) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		switch (g_SelectGameMenu)
		{
			//PVP���I������Ă���ꍇ  �������̓o�O�ŁA���ɂ��I������Ă��Ȃ������ꍇ
		case SelectGameMenu_PVP:
		default:
			g_SelectGameMenu = SelectGameMenu_HipDropRace;	//�I������Ă��郁�j���[���u�q�b�v�h���b�v���[�X�v��
			break;

			//�q�b�v�h���b�v���[�X���I������Ă���ꍇ
		case SelectGameMenu_HipDropRace:
			g_SelectGameMenu = SelectGameMenu_PVP;	//�I������Ă��郁�j���[���uPVP�v��
			break;
		}
	}

	//�E��󂪉����ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_LEFT) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		switch (g_SelectGameMenu)
		{
			//PVP���I������Ă���ꍇ  �������̓o�O�ŁA���ɂ��I������Ă��Ȃ������ꍇ
		case SelectGameMenu_PVP:
		default:
			g_SelectGameMenu = SelectGameMenu_HipDropRace;	//�I������Ă��郁�j���[���u�q�b�v�h���b�v���[�X�v��
			break;

			//�q�b�v�h���b�v���[�X���I������Ă���ꍇ
		case SelectGameMenu_HipDropRace:
			g_SelectGameMenu = SelectGameMenu_PVP;	//�I������Ă��郁�j���[���uPVP�v��
			break;
		}
	}
}

//�Q�[�����[�h�I��
void DecideGameMode(void)
{
	//Enter�������ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		//�`���[�g���A����ʂɑJ��
		SetFade(MODE_PvPGAME);

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

	for (int nCntSelect = 0; nCntSelect < SelectGameMenu_MAX; nCntSelect++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSelectGame[nCntSelect]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelect * VTX_MAX, 2);
	}
}
//==========================================
//
//�|�[�Y�v���O����[pause.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "PvP_player.h"
#include "HDRgame.h"
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "color.h"
#include "sound.h"
#include "conversioninput.h"

//****************************//
//		�@ �}�N����`		  //
//****************************//

//�e�N�X�`���̏��
#define NUM_TEXTURE				(7)									//�e�N�X�`���̍ő吔

//�e���W
#define CENTER_POS_MENU			(D3DXVECTOR3(640.0f,360.0f,0.0f))	//���j���[�̒��S�̈ʒu
#define CENTER_POS_TEXT			(D3DXVECTOR3(640.0f,440.0f,0.0f))	//�e�L�X�g�̒��S�̈ʒu
#define CENTER_POS_DIRE_UP		(D3DXVECTOR3(640.0f,340.0f,0.0f))	//�{�^���̒��S�̈ʒu
#define CENTER_POS_DIRE_DOWN	(D3DXVECTOR3(640.0f,540.0f,0.0f))	//�{�^���̒��S�̈ʒu

//��ʂ̏��
#define HALF_WIDTH				(640.0f)							//�����̉���
#define HALF_HEIGHT				(360.0f)							//�����̍���

//�e�L�X�g�̏��
#define TEXT_WIDTH				(200.0f)							//�e�L�X�g�̉���
#define TEXT_HEIGHT				(140.0f)								//�e�L�X�g�̍���

//�{�^���̏��
#define NUM_BUTTON				(2)									//�{�^���̐�
#define COUNT_BUTTON			(15)								//�{�^���̃J�E���g
#define DIRE_WIDTH_UP			(40.0f)								//��{�^���̕�
#define DIRE_HEIGHT_UP			(40.0f)								//��{�^���̍���
#define DIRE_WIDTH_DOWN			(-40.0f)							//���{�^���̕�
#define DIRE_HEIGHT_DOWN		(-40.0f)							//���{�^���̍���

//****************************//
//		�e�N�X�`���̃p�X	  //
//****************************//
const char *c_pFileNamePause[] =
{
	"data/TEXTURE/pause.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
	"data/TEXTURE/modeselect.png",
	"data/TEXTURE/quit.png",
	"data/TEXTURE/sankaku.png",
	"data/TEXTURE/sankaku.png"
};

//****************************//
//		  �eUI�̏��	      //
//****************************//

//�eUI�̍��W
const D3DXVECTOR3 g_CenterPos[] = {
	CENTER_POS_MENU,								//�|�[�Y��ʂ̈ʒu
	CENTER_POS_TEXT,								//�R���e�j���[�̈ʒu
	CENTER_POS_TEXT,								//���g���C�̈ʒu
	CENTER_POS_TEXT,								//���g���C�̈ʒu
	CENTER_POS_TEXT,								//�I���̈ʒu
	CENTER_POS_DIRE_UP,								//��{�^���̈ʒu
	CENTER_POS_DIRE_DOWN							//���{�^���̈ʒu
};

//�eUI�̕�
const float g_Width[] = {
	HALF_WIDTH,										//�|�[�Y��ʂ̕�
	TEXT_WIDTH,										//�R���e�j���[�̕�
	TEXT_WIDTH,										//���g���C�̕�
	TEXT_WIDTH,										//���g���C�̕�
	TEXT_WIDTH,										//�I���̕�
	DIRE_WIDTH_UP,									//��{�^���̕�
	DIRE_WIDTH_DOWN									//���{�^���̕�
};

//�eUI�̍���
const float g_Height[] = {
	HALF_HEIGHT,									//�|�[�Y��ʂ̍���
	TEXT_HEIGHT,									//�R���e�j���[�̍���
	TEXT_HEIGHT,									//���g���C�̍���
	TEXT_HEIGHT,									//���g���C�̍���
	TEXT_HEIGHT,									//�I���̍���
	DIRE_HEIGHT_UP,									//��{�^���̍���
	DIRE_HEIGHT_DOWN								//���{�^���̍���
};

//****************************//
//	    �v���g�^�C�v�錾      //
//****************************//
void SwitchPause(void);

//****************************//
//	   �O���[�o���ϐ��錾     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//���_�o�b�t�@�ւ̃|�C���^
PAUSE g_Pause;												//�|�[�Y�̏��
Pause g_aPause[NUM_TEXTURE];								//�|�[�Y�̏��
int g_nCnt[NUM_BUTTON];										//�J�E���g�p�ϐ�
bool g_ButtonPush[NUM_BUTTON];								//�{�^���������ꂽ���ǂ���
int g_GamePad;
bool g_bDisconnectPause = false;
MODE g_gamemode;

//=================================
//�|�[�Y�̏���������
//=================================
void InitPause(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_TEXTURE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_pFileNamePause[nCnt], &g_pTexturePause[nCnt]);
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//�R���e�j���[�ɐݒ�
	g_Pause = PAUSE_CONTINUE;

	//�ϐ�������
	g_bDisconnectPause = false;

	//�����{�^���ݒ�
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		//�{�^���������ĂȂ���Ԃɂ���
		g_ButtonPush[nCnt] = false;

		//�{�^���̃J�E���g��ݒ�
		g_nCnt[nCnt] = COUNT_BUTTON;
	}

	//�e���ǂݍ���
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++,pVtx += VTX_MAX)
	{
		//�ʒu�̐ݒ�
		g_aPause[nCnt].pos = g_CenterPos[nCnt];

		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x - g_Width[nCnt], g_aPause[nCnt].pos.y - g_Height[nCnt], NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x + g_Width[nCnt], g_aPause[nCnt].pos.y - g_Height[nCnt], NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x - g_Width[nCnt], g_aPause[nCnt].pos.y + g_Height[nCnt], NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x + g_Width[nCnt], g_aPause[nCnt].pos.y + g_Height[nCnt], NIL_F);

		//rhw�̐ݒ�
		pVtx[VTX_LE_UP].rhw = RHW;
		pVtx[VTX_RI_UP].rhw = RHW;
		pVtx[VTX_LE_DO].rhw = RHW;
		pVtx[VTX_RI_DO].rhw = RHW;

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//�e�N�X�`���̍��W
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Unlock();
}
//=================================
//�|�[�Y�̏I������
//=================================
void UninitPause(void) 
{
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//=================================
//�|�[�Y�̍X�V����
//=================================
void UpdatePause(MODE mode)
{
	g_gamemode = mode;

	//�|�[�Y�̗v�����ؒf���ꂽ���̂ł���i�S���ɑ��쌠�j
	if (g_bDisconnectPause == true)
	{
		for (int nCntPad = 0; nCntPad < MAX_USE_GAMEPAD; nCntPad++)
		{
			//�|�[�Y�̏�����I������
			SelectUpPause(nCntPad);

			//�|�[�Y�̉������I������
			SelectDownPause(nCntPad);

			//�|�[�Y��UI�̏�ԕω�����
			UIStatePause();

			if (GetGamepadTrigger(nCntPad, XINPUT_GAMEPAD_START) == true)
			{//START�{�^���i�|�[�Y�����j
				g_Pause = PAUSE_CONTINUE;
				SwitchPause();
			}
			if (GetGamepadTrigger(nCntPad, XINPUT_GAMEPAD_A) == true)
			{//A�{�^���i�I���j
				SwitchPause();
			}
		}
	}
	else
	{//�Ӑ}���ă|�[�Y�����i���쌠�͉������l�j
	 //�|�[�Y�̏�����I������
		SelectUpPause(g_GamePad);

		//�|�[�Y�̉������I������
		SelectDownPause(g_GamePad);

		//�|�[�Y��UI�̏�ԕω�����
		UIStatePause();

		if (GetGamepadTrigger(g_GamePad, XINPUT_GAMEPAD_START) == true)
		{//START�{�^���i�|�[�Y�����j
			g_Pause = PAUSE_CONTINUE;
			SwitchPause();
		}
		if (GetGamepadTrigger(g_GamePad, XINPUT_GAMEPAD_A) == true)
		{//A�{�^���i�I���j
			SwitchPause();
		}
	}

	//�L�[�{�[�h�̑���͎󂯕t����
	if (GetKeyboardTrigger(DIK_P) == true)
	{//P�L�[�i�|�[�Y�����j
		g_Pause = PAUSE_CONTINUE;
		SwitchPause();
	}
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//ENTER�L�[�i�I���j
		SwitchPause();
	}
}
//=================================
//�|�[�Y�̕`�揈��
//=================================
void DrawPause(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * VTX_MAX, 2);
	}
}
//=================================
//�|�[�Y�̏�����I������
//=================================
void SelectUpPause(int nPadNum)
{
	//�L�[�{�[�h�̏�����L�[�@���@�Q�[���p�b�h�́@�\����{�^���@�������ꂽ
	if (GetKeyboardTrigger(DIK_UP) == true || GetGamepadTrigger(nPadNum,XINPUT_GAMEPAD_DPAD_UP) == true || GetStick(nPadNum,INPUTTYPE_TRIGGER).y == CONVSTICK_UP)
	{
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_SELECTION, nPadNum);

		//�{�^�����g�p���Ă����Ԃɂ���
		g_ButtonPush[0] = true;

		//�J�E���g�̍Đݒ�
		g_nCnt[0] = COUNT_BUTTON;

		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += VTX_MAX * 5;

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = XCOL_YELLOW;
		pVtx[VTX_RI_UP].col = XCOL_YELLOW;
		pVtx[VTX_LE_DO].col = XCOL_YELLOW;
		pVtx[VTX_RI_DO].col = XCOL_YELLOW;

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffPause->Unlock();

		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			//�I���{�^���ɐݒ�
			g_Pause = PAUSE_QUIT;

			break;

		case PAUSE_RETRY:

			//������{�^���ɐݒ�
			g_Pause = PAUSE_CONTINUE;

			break;

		case PAUSE_MODESELECT:

			//���g���C�{�^���ɐݒ�
			g_Pause = PAUSE_RETRY;

			break;

		case PAUSE_QUIT:

			//���[�h�I���{�^���ɐݒ�
			g_Pause = PAUSE_MODESELECT;

			break;
		}
	}
}
//=================================
//�|�[�Y�̉������I������
//=================================
void SelectDownPause(int nPadNum)
{
	//�L�[�{�[�h�̉������L�[�@���@�Q�[���p�b�h�́@�\�����{�^���@�������ꂽ
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetGamepadTrigger(nPadNum, XINPUT_GAMEPAD_DPAD_DOWN) == true || GetStick(nPadNum, INPUTTYPE_TRIGGER).y == CONVSTICK_DOWN)
	{
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_SELECTION, nPadNum);

		//�{�^�����g�p���Ă����Ԃɂ���
		g_ButtonPush[1] = true;

		//�J�E���g�̍Đݒ�
		g_nCnt[1] = COUNT_BUTTON;

		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += VTX_MAX * 6;

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = XCOL_YELLOW;
		pVtx[VTX_RI_UP].col = XCOL_YELLOW;
		pVtx[VTX_LE_DO].col = XCOL_YELLOW;
		pVtx[VTX_RI_DO].col = XCOL_YELLOW;

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffPause->Unlock();

		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			//���g���C�{�^���ɐݒ�
			g_Pause = PAUSE_RETRY;

			break;

		case PAUSE_RETRY:

			//���[�h�I���{�^���ɐݒ�
			g_Pause = PAUSE_MODESELECT;

			break;

		case PAUSE_MODESELECT:

			//�I���{�^���ɐݒ�
			g_Pause = PAUSE_QUIT;

			break;

		case PAUSE_QUIT:

			//������{�^���ɐݒ�
			g_Pause = PAUSE_CONTINUE;

			break;
		}
	}
}
//=================================
//�|�[�Y��UI�̏�ԕω�����
//=================================
void UIStatePause(void)
{
	//�F�̕ύX�̃J�E���g�_�E��
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		if (g_ButtonPush[nCnt] == true)
		{
			g_nCnt[nCnt]--;
		}
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//�I�����ڂ̕\���Ɩ��̐F�ύX
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++, pVtx += VTX_MAX)
	{
		if (g_Pause == nCnt)
		{
			//���_�J���[�̐ݒ�
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}
		else if (nCnt != 0 && nCnt <= 4)
		{
			//���_�J���[�̐ݒ�
			pVtx[VTX_LE_UP].col = XCOL_NONE;
			pVtx[VTX_RI_UP].col = XCOL_NONE;
			pVtx[VTX_LE_DO].col = XCOL_NONE;
			pVtx[VTX_RI_DO].col = XCOL_NONE;
		}
		else if (nCnt >= 5)
		{
			if (g_nCnt[0] <= 0 && nCnt == 5)
			{
				//���ɐݒ�
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//�{�^�����g�p���Ă��Ȃ���Ԃɂ���
				g_ButtonPush[0] = false;

				//�J�E���g�̍Đݒ�
				g_nCnt[0] = COUNT_BUTTON;
			}
			else if (g_nCnt[1] <= 0 && nCnt == 6)
			{
				//���ɐݒ�
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//�{�^�����g�p���Ă��Ȃ���Ԃɂ���
				g_ButtonPush[1] = false;

				//�J�E���g�̍Đݒ�
				g_nCnt[1] = COUNT_BUTTON;
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Unlock();
}
//=================================
//�|�[�Y�̐ݒ菈��
//=================================
void SetPause(PAUSE Pause)
{
	g_Pause = Pause;
}
//=================================
//�|�[�Y�̃Q�[���p�b�h�ݒ菈��
//=================================
void SetPadPause(bool bDisconect, int GamePad)
{
	g_GamePad = GamePad;
	g_bDisconnectPause = bDisconect;
}

//=================================
//�|�[�Y�̎擾
//=================================
PAUSE *GetPause(void)
{
	return &g_Pause;
}

//=================================
//�|�[�Y�I���̕���
//Author:�Ό��D�n
//=================================
void SwitchPause(void)
{
	switch (g_Pause)
	{
	case PAUSE_CONTINUE:
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_DECISION, 0);

		//�Ċm�F�������ǃR���g���[���[�̐������ĂȂ�
		if (CheckUseController_PvP(CHECKMODE_DISCONNOPAUSE) == true)
		{
			//�x�����b�Z�[�W

			if (/*���Ⴂ���ł���*/false)
			{
				break;
			}
		}
		RestartSound(SOUND_LABEL_BGM_GAME_PVP);
		CheckUseController_PvP(CHECKMODE_REMOVE);
		SetEnablePause_PvP(false);
		SetEnablePause_HDR(false);
		break;

	case PAUSE_RETRY:
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_TRANSITION, 0);

		switch (g_gamemode)
		{
		case MODE_PvPGAME:

				//���[�h�ݒ�i�Q�[����ʂɑJ��)
				SetFade(MODE_PvPGAME);

				break;

		case MODE_RaceGAME:

			//���[�h�ݒ�i�Q�[����ʂɑJ��)
			SetFade(MODE_RaceGAME);

			break;
		}

		break;

	case PAUSE_MODESELECT:

		//�Q�[���I����ʂɑJ��
		SetFade(MODE_SELECTGAME);

		break;

	case PAUSE_QUIT:
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_TRANSITION, 0);

		//�^�C�g����ʂɑJ��
		SetFade(MODE_TITLE);
		break;
	}
}
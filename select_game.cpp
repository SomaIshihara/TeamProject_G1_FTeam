/*==========================================================================================================================================================

//�Q�[�����j���[�̑I������[select_game.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "fade.h"
#include "conversioninput.h"
#include "sound.h"
#include "select_game.h"

//�}�N����`
#define SELECTGAME_INFO		"data\\CSV\\SelectGame.csv"						/*�Q�[���I���̏��*/

//�O���[�o���ϐ��錾
SelectGame					g_SelectGame[SelGameMode_MAX];				/*�Q�[���I���̏��*/
SelGameMode					g_SelGameMode = SelGameMode_PVP;			/*�I�����ꂽ�Q�[�����[�h*/
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffSelectGame = NULL;				/*���_�o�b�t�@�ւ̃|�C���^*/
LPDIRECT3DTEXTURE9			g_pTextureSelectGame[SelGameMode_MAX] = {};	/*�e�N�X�`���̃|�C���^*/

//�Q�[���I���̃e�N�X�`���p�X
const char *c_apSelGameModeTex[SelGameMode_MAX] = {
	"data\\TEXTURE\\PVP_GameTitle.png",		/*PVP�̃^�C�g��*/
	"data\\TEXTURE\\HDR_GameTitle.png",		/*�q�b�v�h���b�v���[�X�̃^�C�g��*/
};

//------------------------------------------------
//				�Q�[���I���̏���������
//------------------------------------------------
void InitSelectGame(void)
{
	//�I�����j���[���̏�����
	LoadSelectGame();
	g_SelGameMode = SelGameMode_PVP;

	//�f�o�C�X�ւ̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * SelGameMode_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSelectGame, NULL);

	//�e�N�X�`���ǂݍ���
	for (int nCntTex = 0; nCntTex < SelGameMode_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apSelGameModeTex[nCntTex], &g_pTextureSelectGame[nCntTex]);
	}

	//���_���ݒ�
	SetVertexSelectGame();
}

//�Q�[���I���̏��ǂݍ��ݏ���
void LoadSelectGame(void)
{
	FILE *pFile;
	char aData[MAX_CHAR];		//�ǂݍ��񂾕������i�[

	pFile = fopen(SELECTGAME_INFO, "r");

	//�t�@�C����ǂݍ��߂�
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);		//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntSelect = 0; nCntSelect < SelGameMode_MAX; nCntSelect++)
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
	//���I������Ă��郂�[�h���擾
	int nSelect = g_SelGameMode + 1;

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSelectGame->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntSelUI = 0; nCntSelUI < SelGameMode_MAX; nCntSelUI++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_SelectGame[nCntSelUI].pos.x - g_SelectGame[nCntSelUI].fWidth, g_SelectGame[nCntSelUI].pos.y - g_SelectGame[nCntSelUI].fHeight, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_SelectGame[nCntSelUI].pos.x + g_SelectGame[nCntSelUI].fWidth, g_SelectGame[nCntSelUI].pos.y - g_SelectGame[nCntSelUI].fHeight, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_SelectGame[nCntSelUI].pos.x - g_SelectGame[nCntSelUI].fWidth, g_SelectGame[nCntSelUI].pos.y + g_SelectGame[nCntSelUI].fHeight, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_SelectGame[nCntSelUI].pos.x + g_SelectGame[nCntSelUI].fWidth, g_SelectGame[nCntSelUI].pos.y + g_SelectGame[nCntSelUI].fHeight, NIL_F);
		}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].rhw = RHW;
			pVtx[VTX_RI_UP].rhw = RHW;
			pVtx[VTX_LE_DO].rhw = RHW;
			pVtx[VTX_RI_DO].rhw = RHW;
		}

		//���_�J���[�̐ݒ�
		//�J�E���^�[���I������Ă��郁�j���[�ԍ��Ɠ���
		if (g_SelGameMode == nCntSelUI)
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

		//�|�C���^���S�i�߂�
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
	for (int nCntTex = 0; nCntTex < SelGameMode_MAX; nCntTex++)
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
	//���ݑI�΂�Ă��郂�[�h���擾
	int SelectMode = g_SelGameMode;

	//�E��󂪉����ꂽ�@�������́@�Q�[���p�b�h�̉E�\���L�[�@�������ꂽ�@�������́@���X�e�B�b�N���E�ɓ|���ꂽ
	if ((GetKeyboardTrigger(DIK_RIGHT) || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_RIGHT) || GetStick(0, INPUTTYPE_TRIGGER).x == CONVSTICK_RIGHT ))
	{
		//���̃��[�h��I��
		SelectMode++;
	}

	//����󂪉����ꂽ�@�������́@�Q�[���p�b�h�̍��\���L�[�������ꂽ
	if ((GetKeyboardTrigger(DIK_LEFT) || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_LEFT) || GetStick(0, INPUTTYPE_TRIGGER).x == CONVSTICK_LEFT))
	{
		//�O�̃��[�h��I��
		SelectMode--;
	}

	//���[�h����
	g_SelGameMode = (SelGameMode)(abs(SelectMode) % SelGameMode_MAX);
}

//�Q�[�����[�h�I��
void DecideGameMode(void)
{
	//Enter�������ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		switch (g_SelGameMode)
		{
			//PVP���I������Ă���ꍇ  �������̓o�O�ŁA���ɂ��I������Ă��Ȃ������ꍇ
		case SelGameMode_PVP:

			g_SelGameMode = SelGameMode_PVP;	//PVP�Q�[����I��

			break;

			//�q�b�v�h���b�v���[�X���I������Ă���ꍇ
		case SelGameMode_HDR:

			g_SelGameMode = SelGameMode_HDR;	//�q�b�v�h���b�v���[�X�Q�[����I��

			break;
		}

		SetFade(MODE_TUTORIAL);					//�`���[�g���A����ʂɑJ��

		//�^�C�g�����艹�Đ�
		PlaySoundSE(SOUND_LABEL_SE_TITLE_DECIDE, 0);
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

	for (int nCntSelect = 0; nCntSelect < SelGameMode_MAX; nCntSelect++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSelectGame[nCntSelect]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSelect * VTX_MAX, 2);
	}
}

//�I�����ꂽ�Q�[�����[�h���擾
SelGameMode GetSelGameMode(void)
{
	return g_SelGameMode;
}
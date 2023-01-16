//==========================================
//
//�|�[�Y�v���O����[meshfield.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "player.h"
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "color.h"

//�}�N����`
#define NUM_TEXTURE				(6)									//�|�[�Y�摜�̃t�@�C����
#define NUM_BUTTON				(2)									//�{�^���̐�
#define COUNT_BUTTON			(15)								//�{�^���̃J�E���g
#define HALF_WIDTH				(640.0f)							//�����̉���
#define HALF_HEIGHT				(360.0f)							//�����̍���
#define UI_WIDTH				(200.0f)							//UI�̉���
#define UI_HEIGHT				(70.0f)								//UI�̍���
#define DIRE_WIDTH_UP			(40.0f)								//����̕�
#define DIRE_HEIGHT_UP			(40.0f)								//����̍���
#define DIRE_WIDTH_DOWN			(-40.0f)							//�����̕�
#define DIRE_HEIGHT_DOWN		(-40.0f)							//�����̍���
#define CENTER_POS_MENU			(D3DXVECTOR3(640.0f,360.0f,0.0f))	//���j���[�̒��S�̈ʒu
#define CENTER_POS_UI			(D3DXVECTOR3(640.0f,440.0f,0.0f))	//UI�̒��S�̈ʒu
#define CENTER_POS_DIRE_UP		(D3DXVECTOR3(640.0f,340.0f,0.0f))	//���̒��S�̈ʒu
#define CENTER_POS_DIRE_DOWN	(D3DXVECTOR3(640.0f,540.0f,0.0f))	//���̒��S�̈ʒu

//�t�@�C���p�X
const char *c_pFileNamePause[] =
{
	"data/TEXTURE/pause.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
	"data/TEXTURE/quit.png",
	"data/TEXTURE/sankaku.png",
	"data/TEXTURE/sankaku.png"
};

//�|�[�Y��ʂ�UI�̕�
const D3DXVECTOR3 g_CenterPos[] = {
	CENTER_POS_MENU,								//�|�[�Y��ʂ̈ʒu
	CENTER_POS_UI,									//�R���e�j���[�̈ʒu
	CENTER_POS_UI,									//���g���C�̈ʒu
	CENTER_POS_UI,									//�I���̈ʒu
	CENTER_POS_DIRE_UP,
	CENTER_POS_DIRE_DOWN
};

//�|�[�Y��ʂ�UI�̕�
const float g_Width[] = {
	HALF_WIDTH,										//�|�[�Y��ʂ̕�
	UI_WIDTH,										//�R���e�j���[�̕�
	UI_WIDTH,										//���g���C�̕�
	UI_WIDTH,										//�I���̕�
	DIRE_WIDTH_UP,
	DIRE_WIDTH_DOWN
};

//�|�[�Y��ʂ�UI�̍���
const float g_Height[] = {
	HALF_HEIGHT,									//�|�[�Y��ʂ̕�
	UI_HEIGHT,										//�R���e�j���[�̕�
	UI_HEIGHT,										//���g���C�̕�
	UI_HEIGHT,										//�I���̕�
	DIRE_HEIGHT_UP,
	DIRE_HEIGHT_DOWN
};

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//���_�o�b�t�@�ւ̃|�C���^
PAUSE g_Pause;
Pause g_aPause[NUM_TEXTURE];
int g_nCnt[NUM_BUTTON];
bool g_ButtonUse[NUM_BUTTON];

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

	//�����{�^���J�E���g�ݒ�
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		g_nCnt[nCnt] = COUNT_BUTTON;
	}

	//�e���ǂݍ���
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++,pVtx +=4)
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
void UpdatePause(void)
{
	//�|�[�Y�̉������I������
	SelectDownPause();
	
	//�|�[�Y�̏�����I������
	SelectUpPause();

	//�F�̕ύX�̃J�E���g�_�E��
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		if (g_ButtonUse[nCnt] == true)
		{
			g_nCnt[nCnt]--;
		}
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//�I�����ڂ̕\���Ɩ��̐F�ύX
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++, pVtx += 4)
	{
		if (g_Pause == nCnt)
		{
			//���_�J���[�̐ݒ�
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}
		else if(nCnt != 0 && nCnt <= 3)
		{
			//���_�J���[�̐ݒ�
			pVtx[VTX_LE_UP].col = XCOL_NONE;
			pVtx[VTX_RI_UP].col = XCOL_NONE;
			pVtx[VTX_LE_DO].col = XCOL_NONE;
			pVtx[VTX_RI_DO].col = XCOL_NONE;
		}
		else if (nCnt >= 4)
		{
			if (g_nCnt[1] <= 0 && nCnt == 4)
			{
				//���ɐݒ�
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//�{�^�����g�p���Ă��Ȃ���Ԃɂ���
				g_ButtonUse[1] = false;

				//�J�E���g�̍Đݒ�
				g_nCnt[1] = COUNT_BUTTON;
			}
			else if (g_nCnt[0] <= 0 && nCnt == 5)
			{
				//���ɐݒ�
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//�{�^�����g�p���Ă��Ȃ���Ԃɂ���
				g_ButtonUse[0] = false;

				//�J�E���g�̍Đݒ�
				g_nCnt[0] = COUNT_BUTTON;
			}
		}
	}
	
	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Unlock();

	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			break;

		case PAUSE_RETRY:
			//���[�h�ݒ�i�Q�[����ʂɑJ��)
			SetFade(MODE_GAME);
			break;

		case PAUSE_QUIT:
			//�^�C�g����ʂɑJ��
			SetFade(MODE_TITLE);
			break;
		}
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
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}
//=================================
//�|�[�Y�̉������I������
//=================================
void SelectDownPause(void)
{
	//�L�[�{�[�h�̉������L�[�@���@�Q�[���p�b�h�́@�\�����{�^���@�������ꂽ
	if (GetKeyboardTrigger(DIK_DOWN) == true)
	{
		//�{�^�����g�p���Ă����Ԃɂ���
		g_ButtonUse[0] = true;

		//�J�E���g�̍Đݒ�
		g_nCnt[0] = COUNT_BUTTON;

		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += 20;

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
//�|�[�Y�̏�����I������
//=================================
void SelectUpPause(void)
{
	//�L�[�{�[�h�̏�����L�[�@���@�Q�[���p�b�h�́@�\����{�^���@�������ꂽ
	if (GetKeyboardTrigger(DIK_UP) == true)
	{
		//�{�^�����g�p���Ă����Ԃɂ���
		g_ButtonUse[1] = true;

		//�J�E���g�̍Đݒ�
		g_nCnt[1] = COUNT_BUTTON;

		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += 16;

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

		case PAUSE_QUIT:

			//���g���C�{�^���ɐݒ�
			g_Pause = PAUSE_RETRY;

			break;
		}
	}
}
//=================================
//�|�[�Y�̐ݒ菈��
//=================================
void SetPause(PAUSE Pause)
{
	g_Pause = Pause;
}
//=================================
//�|�[�Y�̎擾
//=================================
PAUSE *GetPause(void)
{
	return &g_Pause;
}
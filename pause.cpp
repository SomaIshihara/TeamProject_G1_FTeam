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
#define NUM_TEXTURE		(4)		//�|�[�Y�摜�̃t�@�C����

//�t�@�C���p�X
const char *c_pFileNamePause[] =
{
	"data/TEXTURE/pause.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
	"data/TEXTURE/quit.got"
};

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTexturePause = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//���_�o�b�t�@�ւ̃|�C���^
PAUSE g_Pause;

//=================================
//�|�[�Y�̏���������
//=================================
void InitPause(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//�`���[�g���A�����̏�����

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_pFileNamePause[nCnt], &g_pTexturePause);
	}

	//���_���W�̐ݒ�
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, NIL_F, NIL_F);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(NIL_F, SCREEN_HEIGHT, NIL_F);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);

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

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Unlock();
}

//=================================
//�|�[�Y�̏I������
//=================================
void UninitPause(void) 
{
	//�e�N�X�`���̔j��
	if (g_pTexturePause != NULL)
	{
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
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

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePause);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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
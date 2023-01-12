/*==========================================================================================================================================================

�`���[�g���A������[tutorial.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "color.h"
//#include "sound.h"

//�}�N����`
#define TUTORIAL_TEX_NAME		"data/TEXTURE/tutorial.png"		//�`���[�g���A���摜�̃t�@�C����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureTutorial = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	//���_�o�b�t�@�ւ̃|�C���^

//------------------------------------------------
//				�`���[�g���A������������
//------------------------------------------------
void InitTutorial(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void* *)&pVtx, 0);

	//�`���[�g���A�����̏�����

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEX_NAME, &g_pTextureTutorial);
	
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
	g_pVtxBuffTutorial->Unlock();
}

//------------------------------------------------
//				�`���[�g���A���I������
//------------------------------------------------
void UninitTutorial(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//�^�C�g��BGM��~
	//StopSound(SOUND_LABEL_TITLEBGM);
}

//------------------------------------------------
//				�`���[�g���A���X�V����
//------------------------------------------------
void UpdateTutorial(void)
{
	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if (GetKeyboardTrigger(DIK_RETURN) == true)	
	{
		//���[�h�ݒ�i�Q�[����ʂɑJ��)
		SetFade(MODE_GAME);
	}
}

//------------------------------------------------
//				�`���[�g���A���`�揈��
//------------------------------------------------
void DrawTutorial(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
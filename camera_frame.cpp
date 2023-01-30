/*==========================================================================================================================

�J���������ɂ�鋫�ڂ̘g�\������[camera_frame.cpp]
Author:���V����

==========================================================================================================================*/
#include "camera_frame.h"
#include "color.h"
#include "input.h"

//�}�N����`
#define	FRAME_TEX_PASS		"data\\TEXTURE\\CameraFrame.png"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureCameraFrame = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCameraFrame = NULL;		//���_�o�b�t�@�ւ̃|�C���^

//==============================================================
//��ʕ����̘g�̏���������
//===============================================================
void InitCameraFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, FRAME_TEX_PASS, &g_pTextureCameraFrame);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCameraFrame, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCameraFrame->Lock(0, 0, (void* *)&pVtx, 0);

	//���_���W�̐ݒ�
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
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
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;
	}

	//�e�N�X�`���̍��W
	{
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCameraFrame->Unlock();
}

//==============================================================
//��ʕ����̘g�̏I������
//===============================================================
void UninitCameraFrame(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCameraFrame != NULL)
	{
		g_pTextureCameraFrame->Release();
		g_pTextureCameraFrame = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCameraFrame != NULL)
	{
		g_pVtxBuffCameraFrame->Release();
		g_pVtxBuffCameraFrame = NULL;
	}
}

//==============================================================
//��ʕ����̘g�̕`�揈��
//===============================================================
void DrawCameraFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCameraFrame, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCameraFrame);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
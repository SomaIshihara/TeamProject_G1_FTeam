/*==========================================================================================================================

�J���������ɂ�鋫�ڂ̘g�\������[camera_frame.cpp]
Author:���V����

==========================================================================================================================*/
#include "camera_frame.h"
#include "color.h"
#include "input.h"

#define FRAME_LINE_SIZE		(4.0f)		//�g���̐��̕�

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCameraFrame = NULL;		//���_�o�b�t�@�ւ̃|�C���^
bool					g_bUseFrame[UseFrame_MAX];			//�e�t���[�����g�p���邩�ǂ���
const bool c_aUseFrame[NumCamera_MAX][UseFrame_MAX]=
{
	{ false ,false ,false ,false },	//�g���S��OFF
	{ false ,true ,false ,false },	//�����c���݂̂�ON
	{ true ,false ,false ,false },	//���������݂̂�ON
	{ true ,true ,false ,false },	//�����̏\������ON
	{ false ,true ,true ,true }		//�c�����ׂ�ON
};

//==============================================================
//��ʕ����̘g�̏���������
//===============================================================
void InitCameraFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * UseFrame_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCameraFrame, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCameraFrame->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntFrame = 0; nCntFrame < UseFrame_MAX; nCntFrame++, pVtx += VTX_MAX)
	{
		//�g���g���Ă���
		g_bUseFrame[nCntFrame] = false;

		//���_���W�̐ݒ�

		switch (nCntFrame)
		{
		case UseFrame_WIDTH://�����̏ꍇ

			//��ʂ̉����̔����̈ʒu�𒆐S�Ƃ��āA���̃T�C�Y���@���ɕ��򂳂���
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
			break;

		case UseFrame_HEIGHT://�c���̏ꍇ

			//��ʂ̍����̔����𒆐S�Ƃ��āA���̃T�C�Y���@�c�ɕ��򂳂���
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			break;

		case UseFrame_HEIGHT4_1://�c��4����1�Ԗڂ̏ꍇ

			//��ʂ̍�����4����1�𒆐S�Ƃ��āA���̃T�C�Y���@�c�ɕ��򂳂���
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) - FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) + FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			break;

		case UseFrame_HEIGHT4_3://�c��4����3�Ԗڂ̏ꍇ

			//��ʂ̍�����4����1�𒆐S�Ƃ��āA���̃T�C�Y���@�c�ɕ��򂳂���
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 - FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 + FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			break;
		}

		////�����̏ꍇ
		//if (nCntFrame == UseFrame_WIDTH)
		//{
		//	//��ʂ̉����̔����̈ʒu�𒆐S�Ƃ��āA���̃T�C�Y���@���ɕ��򂳂���
		//	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
		//	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
		//	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
		//	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
		//}

		////�c���̏ꍇ
		//else if (nCntFrame == UseFrame_HEIGHT)
		//{
		//	//��ʂ̍����̔����𒆐S�Ƃ��āA���̃T�C�Y���@�c�ɕ��򂳂���
		//	pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, 0.0f, 0.0f);
		//	pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, 0.0f, 0.0f);
		//	pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
		//	pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
		//}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].rhw = RHW;
			pVtx[VTX_RI_UP].rhw = RHW;
			pVtx[VTX_LE_DO].rhw = RHW;
			pVtx[VTX_RI_DO].rhw = RHW;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[VTX_LE_UP].col = RGBA_BLACK;
			pVtx[VTX_RI_UP].col = RGBA_BLACK;
			pVtx[VTX_LE_DO].col = RGBA_BLACK;
			pVtx[VTX_RI_DO].col = RGBA_BLACK;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCameraFrame->Unlock();
}

//==============================================================
//��ʕ����̘g�̏I������
//===============================================================
void UninitCameraFrame(void)
{
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
	pDevice->SetTexture(0, NULL);

	for (int nCntFrame = 0; nCntFrame < UseFrame_MAX; nCntFrame++)
	{
		//���̃t���[�����g���Ă���
		if (g_bUseFrame[nCntFrame] == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFrame * VTX_MAX, 2);
		}
	}
}

//�g�p����g��ݒ�
void SetUseFrame(NumCamera type)
{
	for (int nCntCamera = 0; nCntCamera < UseFrame_MAX; nCntCamera++)
	{
		g_bUseFrame[nCntCamera] = c_aUseFrame[type][nCntCamera];
	}

}
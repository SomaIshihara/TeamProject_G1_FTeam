/*==========================================================================================================================================================

���U���g��ʏ���[result.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "resultPlayer.h"

#define NUM_RESULT			(1)		//���U���g�摜�̎��

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;			//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9			g_pTextureResult[NUM_RESULT] = {};	//�e�N�X�`���̃|�C���^

//************************************************
//				���U���g�̏���������
//************************************************
void InitResult(void)
{
	//�f�o�C�X�ւ̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_RESULT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void* *)&pVtx, 0);

	//�e���̏�����
	for (int nCntRuselt = 0; nCntRuselt < NUM_RESULT; nCntRuselt++, pVtx += VTX_MAX)
	{
		//�e�N�X�`���̓ǂݍ���


		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(0.0f, 0.0f, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
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
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffResult->Unlock();

	//===============================================
	//���U���g�p�I�u�W�F�N�g�̏����������֐��Ăяo��
	//===============================================
	InitResultObject();
}

//================================================
//		���U���g�p�I�u�W�F�N�g�̏���������
//================================================
void InitResultObject(void)
{
	InitPlayer_RESULT();		//�v���C���[�̏���������
}

//************************************************
//				���U���g�̏I������
//************************************************
void UninitResult(void)
{
	//�e�N�X�`��(�Q��)�̔j��
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//===============================================
	//���U���g�p�I�u�W�F�N�g�̏I�������֐��Ăяo��
	//===============================================
	UninitResultObject();
}

//================================================
//		���U���g�p�I�u�W�F�N�g�̏I������
//================================================
void UninitResultObject(void)
{
	UninitPlayer_RESULT();		//�v���C���[�̏I������
}

//************************************************
//				���U���g�̍X�V����
//************************************************
void UpdateResult(void)
{
	//===============================================
	//���U���g�p�I�u�W�F�N�g�̍X�V�����֐��Ăяo��
	//===============================================
	UpdateResultObject();
}

//================================================
//		���U���g�p�I�u�W�F�N�g�̍X�V����
//================================================
void UpdateResultObject(void)
{
	UpdatePlayer_RESULT();		//�v���C���[�̍X�V����
}

//************************************************
//				���U���g�̕`�揈��
//************************************************
void DrawResult(void)
{
	//�f�o�C�X�̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//===============================================
	//���U���g�p�I�u�W�F�N�g�̕`�揈���֐��Ăяo��
	//===============================================
	DrawResultObject();
}

//================================================
//		���U���g�p�I�u�W�F�N�g�̕`�揈��
//================================================
void DrawResultObject(void)
{
	DrawPlayer_RESULT();		//�v���C���[�̕`�揈��
}
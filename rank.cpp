//==========================================
//
//�����N�v���O����[rank.cpp]
//Author:�������P
//
//==========================================
#include "rank.h"
#include "color.h"
#include "fade.h"

#define POS_XY (150)

//****************************//
//	   �O���[�o���ϐ��錾     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTextureRank = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;	//���_�o�b�t�@�ւ̃|�C���^
RANK g_Rank;							//�|�[�Y�̏��
Rank g_aRank[MAX_RANK];							//�|�[�Y�̏��

//=================================
//�����N�̏���������
//=================================
void InitRank(void)
{

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * MAX_RANK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRank, NULL);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rank.png", &g_pTextureRank);
	
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	//�e���ǂݍ���
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		g_aRank[nCnt].pos = D3DXVECTOR3(160.0f + nCnt * 320, 600.0f, 0.0f);
		g_aRank[nCnt].buse = false;
	}

	g_Rank = RANK_1ST;

	//�e���ǂݍ���
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++, pVtx += VTX_MAX)
	{
		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x - POS_XY, g_aRank[nCnt].pos.y - POS_XY, NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x + POS_XY, g_aRank[nCnt].pos.y - POS_XY, NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x - POS_XY, g_aRank[nCnt].pos.y + POS_XY, NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_aRank[nCnt].pos.x + POS_XY, g_aRank[nCnt].pos.y + POS_XY, NIL_F);

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
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(0.25f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffRank->Unlock();
}
//=================================
//�����N�̏I������
//=================================
void UninitRank(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureRank != NULL)
	{
		g_pTextureRank->Release();
		g_pTextureRank = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}
//=================================
//�����N�̍X�V����
//=================================
void UpdateRank(int nPlayer)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++, pVtx += VTX_MAX)
	{
		if (nCnt == nPlayer)
		{
			g_aRank[nCnt].buse = true;

			//�e�N�X�`���̍��W
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f + g_Rank * 0.25f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(0.25f + g_Rank * 0.25f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f + g_Rank * 0.25f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(0.25f + g_Rank * 0.25f, 1.0f);

			g_Rank = (RANK)(g_Rank + 1);

		/*	if (g_Rank == RANK_MAX)
			{
				SetFade(MODE_RESULT);
			}*/
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffRank->Unlock();
}
//=================================
//�|�[�Y�̕`�揈��
//=================================
void DrawRank(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (g_aRank[nCnt].buse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRank);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * VTX_MAX, 2);
		}
	}
}
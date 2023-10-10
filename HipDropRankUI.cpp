/*==========================================================================================================================================================

�q�b�v�h���b�v�̃����NUI����[HipDropRankUI.h]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "hdr_player.h"
#include "HipDropRankUI.h"
#include "color.h"
#include "input.h"

//�}�N����`
#define RANK_UI_NUM			(4)				// �S�l��
#define RANK_UI_LIFE		(80)			// ����
#define RANK_UI_WIDTH		(130.0f)		// ��
#define RANK_UI_HEIGHT		(50.0f)			// ����
#define RANK_UI_TEX_HEIGHT	(0.2f)			// �e�N�X�`���̍���
#define RANK_UI_TEX			"data/TEXTURE/HipDropRank.png"	//�q�b�v�h���b�v�ʂ�UI�̃e�N�X�`����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureHipDropRankUI = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHipDropRankUI = NULL;	//���_�o�b�t�@�ւ̃|�C���^
HDRankUI				g_HDRankUI[RANK_UI_NUM];		//UI���

//�����NUI�̈ʒu���
const D3DXVECTOR3 c_RankUIPos[RANK_UI_NUM] = {
	D3DXVECTOR3(160.0f, 80.0f, 0.0f),
	D3DXVECTOR3(480.0f, 80.0f, 0.0f),
	D3DXVECTOR3(800.0f, 80.0f, 0.0f),
	D3DXVECTOR3(1120.0f,80.0f, 0.0f)
};

//------------------------------------------
//		�q�b�v�h���b�v�ʂ�UI����������
//------------------------------------------
void InitHipDropRankUI(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, RANK_UI_TEX, &g_pTextureHipDropRankUI);

	//��񏉊���
	for (int nCntRankUI = 0; nCntRankUI < RANK_UI_NUM; nCntRankUI++)
	{
		g_HDRankUI[nCntRankUI].pos = c_RankUIPos[nCntRankUI];	// �ʒu������
		g_HDRankUI[nCntRankUI].nLife = RANK_UI_LIFE;			// ����������
		g_HDRankUI[nCntRankUI].fWidth = RANK_UI_WIDTH;			// ��������
		g_HDRankUI[nCntRankUI].fHeight = RANK_UI_HEIGHT;		// ����������
		g_HDRankUI[nCntRankUI].bDisp = false;					// �\�����邩�ۂ�
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * RANK_UI_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHipDropRankUI, NULL);

	//���_���̏���������
	InitHipDropRankUIVertex();
}

//------------------------------------------
//	�q�b�v�h���b�v�ʂ�UI���_��񏉊�������
//------------------------------------------
void InitHipDropRankUIVertex(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHipDropRankUI->Lock(0, 0, (void* *)&pVtx, 0);

	//UI�̃|�C���^���擾
	HDRankUI *pUI = &g_HDRankUI[0];

	for (int nCntVtx = 0; nCntVtx < RANK_UI_NUM; nCntVtx++, pVtx += VTX_MAX, pUI++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pUI->pos.x - pUI->fWidth, pUI->pos.y - pUI->fHeight, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(pUI->pos.x + pUI->fWidth, pUI->pos.y - pUI->fHeight, NIL_F);
		pVtx[2].pos = D3DXVECTOR3(pUI->pos.x - pUI->fWidth, pUI->pos.y + pUI->fHeight, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(pUI->pos.x + pUI->fWidth, pUI->pos.y + pUI->fHeight, NIL_F);

		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = RHW;			//rhw�̐ݒ�
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = RGBA_WHITE;		//���_�J���[�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx);	//�e�N�X�`���̍��W
		pVtx[1].tex = D3DXVECTOR2(1.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx);	//�e�N�X�`���̍��W
		pVtx[2].tex = D3DXVECTOR2(0.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx + RANK_UI_TEX_HEIGHT);	//�e�N�X�`���̍��W
		pVtx[3].tex = D3DXVECTOR2(1.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx + RANK_UI_TEX_HEIGHT);	//�e�N�X�`���̍��W
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffHipDropRankUI->Unlock();
}

//------------------------------------------
//		�q�b�v�h���b�v�ʂ�UI�I������
//------------------------------------------
void UninitHipDropRankUI(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHipDropRankUI != NULL)
	{
		g_pTextureHipDropRankUI->Release();
		g_pTextureHipDropRankUI = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHipDropRankUI != NULL)
	{
		g_pVtxBuffHipDropRankUI->Release();
		g_pVtxBuffHipDropRankUI = NULL;
	}
}

//------------------------------------------
//		�q�b�v�h���b�v�ʂ�UI�X�V����
//------------------------------------------
void UpdateHipDropRankUI(void)
{
	for (int nCntRankUI = 0; nCntRankUI < RANK_UI_NUM; nCntRankUI++)
	{
		//�\������Ă���
		if (g_HDRankUI[nCntRankUI].bDisp)
		{
			//���������炷
			g_HDRankUI[nCntRankUI].nLife--;

			//�������s����
			if (g_HDRankUI[nCntRankUI].nLife <= 0)
			{
				//��\���ɂ���
				g_HDRankUI[nCntRankUI].bDisp = false;
			}
		}
	}
}

//------------------------------------------
//		�q�b�v�h���b�v�ʂ�UI�`�揈��
//------------------------------------------
void DrawHipDropRankUI(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHipDropRankUI, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureHipDropRankUI);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntRankUI = 0; nCntRankUI < RANK_UI_NUM; nCntRankUI++)
	{
		//UI���g���Ă���
		if (g_HDRankUI[nCntRankUI].bDisp)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRankUI * VTX_MAX, 2);
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//------------------------------------------
//		�q�b�v�h���b�v�ʂ�UI�ݒ菈��
//------------------------------------------
void SetRankUI(int nNumUI, int nTexHeight)
{
	g_HDRankUI[nNumUI].nLife = RANK_UI_LIFE;	/*�����ݒ�*/
	g_HDRankUI[nNumUI].bDisp = true;			/*�\������*/

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHipDropRankUI->Lock(0, 0, (void* *)&pVtx, 0);

	//�|�C���^�����炷
	pVtx += nNumUI * VTX_MAX;

	//�e�N�X�`���̂x���W�̐ݒ�
	pVtx[0].tex.y = pVtx[1].tex.y = RANK_UI_TEX_HEIGHT * (float)nTexHeight;							/*�e�N�X�`���̏��*/
	pVtx[2].tex.y = pVtx[3].tex.y = RANK_UI_TEX_HEIGHT * (float)nTexHeight + RANK_UI_TEX_HEIGHT;	/*�e�N�X�`���̒��*/

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffHipDropRankUI->Unlock();
}
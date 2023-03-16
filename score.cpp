//============================================================================================
//
//�X�R�A�̏���[score.cpp]
//Author:�������P
//
//============================================================================================

#include"main.h"
#include"color.h"
#include"input.h"
#include"score.h"

//****************************//
//		�@ �}�N����`		  //
//****************************//
#define NUM_SCORE		(4)		//�X�R�A�̐�
#define NUM_PLASE		(2)		//�X�R�A����

#define SCORE_MINUS_X		(230.0f)	//�X�R�A�̃}�C�i�XX���W
#define SCORE_PLUS_X		(270.0f)	//�X�R�A�̃v���XX���W
#define SCORE_WIDTH			(30.0f)		//�X�R�A�̕�
#define SCORE_HEIGHT		(50.0f)		//�X�R�A�̍���
#define SCORE_INTERVAL_X	(300.0f)	//�X�R�A���m�̊Ԋu

#define SCORE_MINUS_Y		(600.0f)	//�X�R�A�̃}�C�i�XY���W
#define SCORE_PLUS_Y		(650.0f)	//�X�R�A�̃v���XX���W

//******************************//
//		�O���[�o���ϐ��錾		//
//******************************//
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
int g_aScore[NUM_SCORE];

const D3DXVECTOR3		g_PosScore[MAX_USE_GAMEPAD] =				//�Q�[�W�̈ʒu
{
	D3DXVECTOR3(166.0f,236.0f,0.0f),
	D3DXVECTOR3(1194.0f,236.0f,0.0f),
	D3DXVECTOR3(166.0f,596.0f,0.0f),
	D3DXVECTOR3(1194.0f,596.0f,0.0f),
};

//===================================================
//�X�R�A�̏���������
//===================================================
void InitScore(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Number.png",
		&g_pTextureScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_PLASE * NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLASE * NUM_SCORE; nCntScore++, pVtx += VTX_MAX)
	{
		pVtx[0].pos = pVtx[1].pos = pVtx[2].pos = pVtx[3].pos = ZERO_SET;

		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = RGBA_WHITE;

		pVtx[0].tex = D3DXVECTOR2(NIL_F, NIL_F);
		pVtx[1].tex = D3DXVECTOR2(0.1f , NIL_F);
		pVtx[2].tex = D3DXVECTOR2(NIL_F, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f , 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}
//================================================
//�X�R�A�I������
//=================================================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
//=========================================================
//�X�R�A�X�V����
//=========================================================
void UpdateScore(void)
{
	
}
//======================================================
//�X�R�A�`�揈��
//======================================================
void DrawScore(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = 0; nCntScore < NUM_PLASE * NUM_SCORE; nCntScore++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * VTX_MAX, 2);
	}
}
//==================================================
//�X�R�A�̃Z�b�g����
//==================================================
void SetScore(int nScore, int nPlayer)
{
	//�e�N�X�`���̕���ۑ�
	int aTexU[NUM_SCORE][NUM_PLASE];

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//�X�R�A�̐ݒ�
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++)
	{
		g_aScore[nCntPlayer] = nScore;

		aTexU[nCntPlayer][0] = g_aScore[nCntPlayer] % 100 / 10;	//�\�̈ʂ��Z�o
		aTexU[nCntPlayer][1] = g_aScore[nCntPlayer] % 10;		//��̈ʂ��Z�o

		for (int nCntScore = 0; nCntScore < NUM_PLASE; nCntScore++, pVtx += VTX_MAX)
		{
			pVtx[0].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + (nCntScore * SCORE_WIDTH), g_PosScore[nCntPlayer].y, NIL_F);
			pVtx[1].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + ((nCntScore + 1) * SCORE_WIDTH), g_PosScore[nCntPlayer].y, NIL_F);
			pVtx[2].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + (nCntScore * SCORE_WIDTH), g_PosScore[nCntPlayer].y + SCORE_HEIGHT, NIL_F);
			pVtx[3].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + ((nCntScore + 1) * SCORE_WIDTH), g_PosScore[nCntPlayer].y + SCORE_HEIGHT, NIL_F);

			pVtx[0].tex = D3DXVECTOR2(NIL_F + (aTexU[nCntPlayer][nCntScore] * 0.1f), NIL_F);
			pVtx[1].tex = D3DXVECTOR2(0.1f  + (aTexU[nCntPlayer][nCntScore] * 0.1f), NIL_F);
			pVtx[2].tex = D3DXVECTOR2(NIL_F + (aTexU[nCntPlayer][nCntScore] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f  + (aTexU[nCntPlayer][nCntScore] * 0.1f), 1.0f);
		}
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Unlock();
}
//==============================================================
//�X�R�A�̉��Z����
//===============================================================
void AddScore(int nValue, int nPlayer)
{
	int aTexU[NUM_SCORE][NUM_PLASE];
	int nCntScore;

	VERTEX_2D *pVtx;

	g_aScore[nPlayer] += nValue;

	aTexU[nPlayer][0] = g_aScore[nPlayer] % 100 / 10;
	aTexU[nPlayer][1] = g_aScore[nPlayer] % 10;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//�Ώۂ̌��܂Ń|�C���^���X�L�b�v
	pVtx += nPlayer * (VTX_MAX * NUM_PLASE);

	for (nCntScore = 0; nCntScore < NUM_PLASE; nCntScore++, pVtx += VTX_MAX)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nPlayer][nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nPlayer][nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nPlayer][nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nPlayer][nCntScore] * 0.1f), 1.0f);
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Unlock();
}
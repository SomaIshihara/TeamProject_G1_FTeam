//==========================================
//
//�����N�v���O����[rank.cpp]
//Author:�������P  ���V����
//
//==========================================
#include "rank.h"
#include "color.h"
#include "fade.h"
#include "HDRgame.h"
#include "hdr_player.h"
#include "debugproc.h"

//�}�N����`
#define POS_X			(200)
#define POS_Y			(100)
#define RANK_TEX_WIDTH	(0.25f)	//�e�N�X�`���̉�����
#define FADE_COUNTER	(120)	//�I���܂ł̎���

//****************************//
//	   �O���[�o���ϐ��錾     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTextureRank = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;	//���_�o�b�t�@�ւ̃|�C���^
int			g_nFadeCounter;		//�t�F�[�h����܂ł̃J�E���^�[
int			g_nUpperRank;		//���ʂ��グ���
RANKING		g_nRanking;			//���݂̏���
Rank		g_aRank[MAX_RANK];	//���ʂt�h���

//=================================
//�����N�̏���������
//=================================
void InitRank(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &g_pTextureRank);
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * MAX_RANK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRank, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	//�e���ǂݍ���
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++, pVtx += VTX_MAX)
	{
		g_aRank[nCntRank].pos = D3DXVECTOR3(150.0f + nCntRank * 320, 600.0f, 0.0f);
		g_aRank[nCntRank].bDisp = false;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x - POS_X, g_aRank[nCntRank].pos.y - POS_Y, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x + POS_X, g_aRank[nCntRank].pos.y - POS_Y, NIL_F);
		pVtx[2].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x - POS_X, g_aRank[nCntRank].pos.y + POS_Y, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x + POS_X, g_aRank[nCntRank].pos.y + POS_Y, NIL_F);

		//���_�J���[�ERHW�E�e�N�X�`�����W�ݒ�
		for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
		{
			pVtx[nCntVtx].rhw = RHW;					//rhw�̐ݒ�
			pVtx[nCntVtx].col = RGBA_WHITE;				//���_�J���[�̐ݒ�
			pVtx[nCntVtx].tex.x = 0.0f;					//�e�N�X�`���̇]���W�i���ʐݒ菈���ŁA�e�N�X�`�����W��ݒ肷��̂ŁA�����l�͂O
			pVtx[nCntVtx].tex.y = (float)(nCntVtx / 2);	//�e�N�X�`���̂x���W�i�J�E���^�[�� 0�E1 ��0.0���@�@2�E3��1.0f
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffRank->Unlock();

	g_nRanking = RANKING_1ST;
	g_nUpperRank = 0;
	g_nFadeCounter = FADE_COUNTER;
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
void UpdateRank(void)
{
	//���ʂ��グ��
	UpperRank();

	//���ʂ��v���C�l���ɒB����
	if (g_nRanking >= JoinPlayer())
	{
		//�I�����Ԍ���
		g_nFadeCounter--;

		//�I������
		if (g_nFadeCounter <= 0)
		{
			//���U���g�ֈڍs
			SetFade(MODE_RESULT);
		}
	}
}

//�Q���l���𑪒�
int JoinPlayer(void)
{
	//�Q�����Ă���v���C���[�l�����i�[
	int nJoinNumPlayer = GetUseControllerNum_HDR();

	//�Q���҂����Ȃ�
	if (nJoinNumPlayer == 0)
	{
		//�L�[�{�[�h���g�p���Ă��邱�Ƃ�z�肵�A�Q���҂���l�͂��邱�Ƃɂ���
		nJoinNumPlayer++;
	}

	for (int nCntJoin = nJoinNumPlayer; nCntJoin < MAX_USE_GAMEPAD; nCntJoin++)
	{
		//AI���g�p����Ă���
		if (GetPlayer_HDR()[nCntJoin].bUseAI)
		{
			//�Q���l���𑝂₷
			nJoinNumPlayer++;
		}
	}

	PrintDebugProc("�Q���l���F%d\n", nJoinNumPlayer);

	//�Q���l����Ԃ�
	return nJoinNumPlayer;
}

//���ʂ��グ��
void UpperRank(void)
{
	for (g_nUpperRank; 0 < g_nUpperRank; g_nUpperRank--)
	{
		//���ʂ��グ��
		g_nRanking = (RANKING)(g_nRanking + 1);

		//���ʂ��S�ʂ𒴂��Ă��܂���
		if (g_nRanking > RANKING_MAX)
		{
			g_nRanking = RANKING_MAX;	//�����L���O���ŉ��ʂɂƂǂ߂�
			break;						//�������~�߂�
		}
	}
}

//=================================
//���ʂ̕`�揈��
//=================================
void DrawRank(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (g_aRank[nCntRank].bDisp == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRank);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * VTX_MAX, 2);
		}
	}
}

//�����L���O�ݒ�
int SetRank(int nCntPlayer)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	//�Ώۂ̒��_�܂Ń|�C���^�����炷
	pVtx += VTX_MAX * nCntPlayer;

	//�e�N�X�`���̍��W
	pVtx[0].tex.x = pVtx[2].tex.x = g_nRanking * RANK_TEX_WIDTH;					//�e�N�X�`���̍���
	pVtx[1].tex.x = pVtx[3].tex.x = g_nRanking * RANK_TEX_WIDTH + RANK_TEX_WIDTH;	//�e�N�X�`���̉E��

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffRank->Unlock();

	g_aRank[nCntPlayer].bDisp = true;	//�\������
	g_nUpperRank++;						//���ʂ����̍X�V�����ŏグ��

	return g_nRanking;	//���݂̏��ʂ�Ԃ�
}
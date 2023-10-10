/*==========================================================================================================================================================

�v���C���[�̈ʒu�v���r���[����[preview.h]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "hdr_player.h"
#include "debugproc.h"
#include "preview.h"
#include "block.h"
#include "color.h"
#include "input.h"

typedef enum
{
	PREVIEW_UI_COURSE = 0,
	PREVIEW_UI_PLAYER,
	PREVIEW_UI_MAX
}PREVIEW_UI;

//�}�N����`
#define NUM_PREVIEW			(4)		//�v���r���[��
#define ALL_PREBIEW			NUM_PREVIEW * PREVIEW_UI_MAX	//�S�̃v���r���[��
#define PREVIEW_TEX_COURSE	"data/TEXTURE/HDR_Course.png"	//�R�[�XUI
#define PREVIEW_TEX_PLAYER	"data/TEXTURE/PlayerUI000.png"	//�v���C���[UI
#define PREVIEW_HOR_SEP		(SCREEN_WIDTH / 4)	//�c�S����
#define PREVIEW_POS_Y		(570.0f)			//�z�u���鍂��

#define PREVIEW_COURSE_MARGIN_WIDTH	(70.0f)		//�R�[�XUI�̍��ɂ��炷�]��
#define PREVIEW_COURSE_WIDTH		(60.0f)		//�R�[�XUI�̕�
#define PREVIEW_COURSE_HEIGHT		(370.0f)	//�R�[�XUI�̍���

#define PREVIEW_PLAYER_UI_FIX_Y			(359.0f)//�v���C���[UI�̏�������
#define PREVIEW_PLAYER_MARGIN_WIDTH		(34.0f)	//�v���C���[UI�̍��ɂ��炷�]��
#define PREVIEW_PLAYER_MARGIN_HEIGHT	(5.0f)	//�v���C���[UI�̏�ɂ��炷�]���i�R�[�X��UI�̉������������Ă���̂ŁA���̒����p

#define PREVIEW_PLAYER_SIZE			(20.0f)		//�v���C���[UI�̃T�C�Y�i�����`
#define PREVIEW_PLAYER_TEX_WIDTH	(0.25f)		//�v���C���[UI�̃e�N�X�`�����i�S����

#define PLAYER_START_HEIGHT	(MAX_BLOCK * COLLISION_SIZE_Y)	//�v���C���[�̃X�^�[�g�n�_


//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPreview = NULL;				//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTexturePreview[PREVIEW_UI_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
Preview					g_Preview[NUM_PREVIEW];					//UI���

//--------------------------------------------
//			�ʒu�v���r���[����������
//--------------------------------------------
void InitPreview(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PREVIEW_TEX_COURSE, &g_pTexturePreview[PREVIEW_UI_COURSE]);
	D3DXCreateTextureFromFile(pDevice, PREVIEW_TEX_PLAYER, &g_pTexturePreview[PREVIEW_UI_PLAYER]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * ALL_PREBIEW, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPreview, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPreview->Lock(0, 0, (void* *)&pVtx, 0);

	//�v���r���[���̃|�C���^�擾
	Preview *pPreview = &g_Preview[0];

	//���_���̐ݒ�
	for (int nCntVtx = 0; nCntVtx < NUM_PREVIEW; nCntVtx++, pVtx += VTX_MAX * PREVIEW_UI_MAX, pPreview++)
	{
		// �ʒu������
		pPreview->pos = D3DXVECTOR3(
			(float)(PREVIEW_HOR_SEP + PREVIEW_HOR_SEP * nCntVtx), //��ʂ��c�ɂS�������āA���̕����_���獶�ɏ������炷
			PREVIEW_POS_Y, 0.0f);
		pPreview->fPlayerUIHeight = PREVIEW_PLAYER_UI_FIX_Y;	//�v���C���[UI�̕\�����鍂��
		pPreview->fHeightPer = 1.0f;							//�v���C���[UI�̕\�����鍂���̃p�[�Z���e�[�W

		/*******************************************************************
							�R�[�X�ƃv���C���[UI�𓯎��ɐݒ�
			MEMO:�R�[�X�̃|�C���^�ԍ��O�`�R�@�v���C���[�̃|�C���^�ԍ��S�`�V
		*********************************************************************/
		//=================================================================
		//			�R�[�X�̒��_���W�E�e�N�X�`�����W�̐ݒ�
		//=================================================================
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y - PREVIEW_COURSE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y - PREVIEW_COURSE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y, 0.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//=================================================================
		//			�v���C���[UI�̒��_���W�E�e�N�X�`�����W�̐ݒ�
		//=================================================================
		//���_���W�̐ݒ�
		pVtx[4].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_SIZE * 2.0f - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_SIZE * 2.0f - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);

		//�e�N�X�`���̕�
		float aTexU = PREVIEW_PLAYER_TEX_WIDTH * nCntVtx;

		//�e�N�X�`�����W
		pVtx[4].tex = D3DXVECTOR2(aTexU, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(aTexU + PREVIEW_PLAYER_TEX_WIDTH, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(aTexU, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(aTexU + PREVIEW_PLAYER_TEX_WIDTH, 1.0f);

		//=================================================================
		//	������ RHW�E���_�J���[�͐ݒ肪�����Ȃ̂ŁA�ꏏ�ɐݒ�
		//=================================================================
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = pVtx[4].rhw = pVtx[5].rhw = pVtx[6].rhw = pVtx[7].rhw = RHW;		//rhw�̐ݒ�
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = pVtx[4].col = pVtx[5].col = pVtx[6].col = pVtx[7].col = XCOL_WHITE;	//���_�J���[�̐ݒ�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPreview->Unlock();
}

//------------------------------------------
//			�v���r���[�I������
//------------------------------------------
void UninitPreview(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < PREVIEW_UI_MAX; nCntTex++)
	{
		if (g_pTexturePreview[nCntTex] != NULL)
		{
			g_pTexturePreview[nCntTex]->Release();
			g_pTexturePreview[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPreview != NULL)
	{
		g_pVtxBuffPreview->Release();
		g_pVtxBuffPreview = NULL;
	}
}

//--------------------------------------
//			�v���r���[�X�V����
//--------------------------------------
void UpdatePreview(void)
{
	//�v���r���[���̃|�C���^���擾
	Preview *pPreview = &g_Preview[0];

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPreview->Lock(0, 0, (void* *)&pVtx, 0);

	//��ڂ̃R�[�X�̒��_�o�b�t�@���΂�
	pVtx += VTX_MAX;

	for (int nCntPreview = 0; nCntPreview < NUM_PREVIEW; nCntPreview++, pVtx += VTX_MAX * PREVIEW_UI_MAX, pPreview++)
	{
		//�v���C���[UI�̕\�����鍂�����Z�o
		FixPlayerUIHeight(nCntPreview);

		//���_���W�̐ݒ�
		float fUp = pVtx[0].pos.y = pVtx[1].pos.y = (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_SIZE * 2.0f - PREVIEW_PLAYER_MARGIN_HEIGHT;	//�v���C���[UI�̏��
		float fDo = pVtx[2].pos.y = pVtx[3].pos.y = (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_MARGIN_HEIGHT;								//�v���C���[UI�̒��

		PrintDebugProc("��ӂ̈ʒu�F%f    ��ӂ̈ʒu�F%f\n", fUp, fDo);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPreview->Unlock();
}

//�v���C���[UI�̕\�����鍂�����C��
void FixPlayerUIHeight(int nNumPreview)
{
	//�v���C���[�̌��݂̍����̊��������߂�
	float fPercent = GetPlayer_HDR()[nNumPreview].pos.y / PLAYER_START_HEIGHT;
	g_Preview[nNumPreview].fPlayerUIHeight = fPercent * PREVIEW_PLAYER_UI_FIX_Y;

	PrintDebugProc("%dP�̊����F%f  �ő�̍����F%f  ���݂̍����F%f    ", nNumPreview + 1, fPercent, PREVIEW_PLAYER_UI_FIX_Y, g_Preview[nNumPreview].fPlayerUIHeight);
}

//--------------------------------------
//			�v���r���[�`�揈��
//--------------------------------------
void DrawPreview(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player_HDR *pPlayer = GetPlayer_HDR();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPreview, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntPreview = 0; nCntPreview < ALL_PREBIEW; nCntPreview++)
	{
		if ((pPlayer + (nCntPreview / PREVIEW_UI_MAX))->bGoal == false)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePreview[nCntPreview % EVENPARITY]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPreview * VTX_MAX, 2);
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
/*==========================================================================================================================================================

�`���[�g���A������[tutorial.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "select_game.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "color.h"
#include "sound.h"

#define TEX_SIZE		(1.0f)	//�e�N�X�`���̃T�C�Y
#define TURN_PAGE_SPEED	(4.0f)	//�߂���X�s�[�h

//�`���[�g���A���̃e�N�X�`���̎�ށ@�񋓌^
typedef enum
{
	TutTexType_HowToPlay = 0,	//�������
	TutTexType_PlayRulu,		//���[������
	TutTexType_MAX
}TutTexType;

//�`���[�g���A���̍\����
typedef struct
{
	float	fWidth;		//���S�ʒu����E�[�܂ł̕�
	float	fTurnSpeed;	//�߂���X�s�[�h
	bool	bTurn;		//�߂���ǂ����i�ŏ��͂߂����Ă��Ȃ�
}Tutorial;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureTutorial[TutTexType_MAX] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Tutorial				g_Tutorial[TutTexType_MAX];				//�`���[�g���A���̏��

//�t�@�C���p�X
const char* c_aFilePathTutorial[SelGameMode_MAX][TutTexType_MAX] =
{
	{ "data/TEXTURE/Tutorial001.png", "data/TEXTURE/Tutorial002.png" },
	{ "data/TEXTURE/Tutorial001.png", "data/TEXTURE/Tutorial002.png" },
};

//--------------------------------------------
//			�`���[�g���A������������
//--------------------------------------------
void InitTutorial(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TutTexType_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTut = 0; nCntTut < TutTexType_MAX; nCntTut++)
	{
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_aFilePathTutorial[GetSelGameMode()][nCntTut], &g_pTextureTutorial[nCntTut]);

		g_Tutorial[nCntTut].fWidth = (float)SCREEN_WIDTH;//�`���[�g���A���̒��_���W�̕���������
		g_Tutorial[nCntTut].fTurnSpeed = 0.0f;			 //�߂���X�s�[�h������
		g_Tutorial[nCntTut].bTurn = true;				 //�߂����Ă��Ȃ�

		//========================
		//		���_���ݒ�
		//========================
		SetVertexInfoTut(nCntTut);
	}

	//�`���[�g���A��BGM�Đ�
	PlaySoundBGM(SOUND_LABEL_BGM_TUTORIAL);
}

//------------------------------------------------
//				�`���[�g���A���I������
//------------------------------------------------
void UninitTutorial(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < TutTexType_MAX; nCntTexture++)
	{
		if (g_pTextureTutorial[nCntTexture] != NULL)
		{
			g_pTextureTutorial[nCntTexture]->Release();
			g_pTextureTutorial[nCntTexture] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//�^�C�g��BGM��~
	StopSoundBGM(SOUND_LABEL_BGM_TUTORIAL);
}

//------------------------------------------------
//				�`���[�g���A���X�V����
//------------------------------------------------
void UpdateTutorial(void)
{
	//�y�[�W�����ɂ߂���
	if (GetKeyboardTrigger(DIK_LEFT))
	{
		g_Tutorial[0].bTurn = false;
	}

	//�y�[�W���E�ɂ߂���
	else if (GetKeyboardTrigger(DIK_RIGHT))
	{
		g_Tutorial[0].bTurn = true;
	}

	//�P�y�[�W�ڂ݂̂��߂��鏈��
	TurnPageTutorial();

	//���_���i�P���ڂ̂݁j�ݒ�
	SetVertexInfoTut(0);

	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		//���[�h�ݒ�i�Q�[����ʂɑJ��)
		SetFade(MODE_SELECTPLAYER);
	}
}

//�P�y�[�W�ڂ݂̂��߂��鏈��
void TurnPageTutorial(void)
{
	//�ꖇ�ڂ̃|�C���^���擾
	Tutorial *pTut = &g_Tutorial[0];

	//�E�ɂ߂���
	if (pTut->bTurn)
	{
		//�߂���X�s�[�h���グ��
		pTut->fTurnSpeed += TURN_PAGE_SPEED;
	}

	//���ɂ߂���
	else
	{
		//�߂���X�s�[�h���グ��
		pTut->fTurnSpeed -= TURN_PAGE_SPEED;
	}

	//�߂���
	pTut->fWidth += pTut->fTurnSpeed;

	//�����A��ʂ̕��𒴂���
	if (pTut->fWidth >= SCREEN_WIDTH)
	{
		pTut->fWidth = SCREEN_WIDTH;//������ʂ̕��ɖ߂�
		pTut->fTurnSpeed = 0.0f;	//�߂���X�s�[�h���O��
	}

	//�����A��ʂ̕��𒴂���
	if (pTut->fWidth <= 0.0f)
	{
		pTut->fWidth = 0.0f;//������ʂ̕��ɖ߂�
		pTut->fTurnSpeed = 0.0f;	//�߂���X�s�[�h���O��
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

	//�ꖇ�ڂ���ɂȂ�悤�Ɍ�납��\��
	for (int nCntTex = TutTexType_MAX - 1; 0 <= nCntTex; nCntTex--)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorial[nCntTex]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * VTX_MAX, 2);
	}
}

//------------------------------------------------
//				���_���ݒ菈��
//------------------------------------------------
void SetVertexInfoTut(int nCntTut)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void* *)&pVtx, 0);

	//�|�C���^��ݒ�
	pVtx += nCntTut * VTX_MAX;

	//========================
	//		���_���ݒ�
	//========================
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = g_Tutorial[nCntTut].fWidth * (nCntVtx % EVENPARITY);
		pVtx[nCntVtx].pos.y = SCREEN_HEIGHT * (nCntVtx / EVENPARITY);
		pVtx[nCntVtx].pos.z = 0.0f;

		pVtx[nCntVtx].tex.x = TEX_SIZE * (nCntVtx % EVENPARITY);
		pVtx[nCntVtx].tex.y = TEX_SIZE * (nCntVtx / EVENPARITY);

		D3DXVECTOR2 pos = D3DXVECTOR2(pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y);
		D3DXVECTOR2 tex = pVtx[nCntVtx].tex;

		pVtx[nCntVtx].rhw = RHW;
		pVtx[nCntVtx].col = RGBA_WHITE;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTutorial->Unlock();
}
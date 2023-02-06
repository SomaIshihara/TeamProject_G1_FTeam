//==========================================
//
//�^�C�}�[�v���O����[timer.cpp]
//Author:�ѓc�F��
//
//==========================================

#include "main.h"
#include "game.h"
#include "timer.h"
#include "player.h"
#include "camera.h"
#include "fade.h"
#include "color.h"
#include "bonus.h"
#include "time.h"

//�}�N����`
#define NUM_PLACE  (2)								 //�X�R�A�̌���

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//���_�o�b�t�@�ւ̃|�C���^
TIME g_aTime;


D3DXMATRIX mtxWorldTime;

int BonusLimit;
bool bBonusUse;

//===============================
//�^�C���̏������̏��� 
//===============================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Number.png",
		&g_pTextureTime);

	
		g_aTime.pos = ZERO_SET;//�ʒu������
		g_aTime.nCounter = 0;
		SetTimerDejit();		//�����ݒ�

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * VTX_MAX * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += VTX_MAX)
	{

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(TIMER_MINUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_MINUS_Y, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(TIMER_PLUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_MINUS_Y, NIL_F);
		pVtx[2].pos = D3DXVECTOR3(TIMER_MINUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_PLUS_Y, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(TIMER_PLUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_PLUS_Y, NIL_F);
			
		//rhw�̐ݒ�
		pVtx[VTX_LE_UP].rhw = RHW;
		pVtx[VTX_RI_UP].rhw = RHW;
		pVtx[VTX_LE_DO].rhw = RHW;
		pVtx[VTX_RI_DO].rhw = RHW;

		//���_�J���[�̐ݒ�
		pVtx[0].col = RGBA_ORANGE;
		pVtx[1].col = RGBA_ORANGE;
		pVtx[2].col = RGBA_ORANGE;
		pVtx[3].col = RGBA_ORANGE;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���ݎ��Ԃ̎擾
	srand((unsigned int)time(0));

	//�{�[�i�X�̃��~�b�g�������_���ɐݒ�
	BonusLimit = rand() % 900;

	//�{�[�i�X�̎g�p�ł���悤�ɂ���
	bBonusUse = false;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//===============================
//�^�C���̏I������
//===============================
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//===============================
//�^�C���̍X�V����
//===============================
void UpdateTime(void)
{
	//�{�[�i�X�̏o������
	if (g_aTime.nTime <= 40)
	{
		BonusLimit--;

		if (bBonusUse == false)
		{
			if (BonusLimit <= 0)
			{
				//�{�[�i�X�̃Z�b�g����
				SetBonus();

				//�{�[�i�X���g�p�ł��Ȃ��悤�ɂ���
				bBonusUse = true;
			}
		}
	}

	g_aTime.nCounter++;

	if ((g_aTime.nCounter % 60) == 0)
	{//��莞�Ԍo��
		g_aTime.nCounter = 0;		//�J�E���^�[�������l�ɖ߂�

		AddTime(1);
	}

}

//===============================
//�^�C���̕`�揈��
//===============================
void DrawTime(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * VTX_MAX, 2);
	}

}


//===============================
//�^�C���̐ݒ菈��
//===============================
void SetTime(int nTime)
{
	int nCntTime;
	VERTEX_2D * pVtx;

	g_aTime.nTime = nTime;

	//�^�C���̕ۑ�
	g_aTime.nTemp = g_aTime.nTime;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += VTX_MAX)
	{
		int aTexU = g_aTime.nTime % g_aTime.nDejit / (g_aTime.nDejit / 10);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 1.0f);

		g_aTime.nDejit /= 10;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();

	//�����Đݒ�
	SetTimerDejit();
}

//===============================
//�^�C���̌��Z����
//===============================
void AddTime(int nValue)
{
	/*if (GetGameState() == GAMESTATE_NORMAL )
	{
*/
		int nCntTime;
		VERTEX_2D * pVtx;

		g_aTime.nTime -= nValue;

		//�������Ԃ�0�ɂȂ�����Q�[���I�[�o�[
		if (g_aTime.nTime == 0)
		{
			SetFade(MODE_TITLE);
		}

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += VTX_MAX)
		{
			int aTexU = g_aTime.nTime % g_aTime.nDejit / (g_aTime.nDejit / 10);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(aTexU * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 1.0f);

			g_aTime.nDejit /= 10;
		}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTime->Unlock();

		//�����Đݒ�
		SetTimerDejit();
}

//===============================
//�^�C���̌����ݒ�
//===============================
void SetTimerDejit(void)
{
	g_aTime.nDejit = pow(10, NUM_PLACE);
}
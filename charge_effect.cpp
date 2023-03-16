//==========================================
//
//�G�t�F�N�g�v���O����[charge_effect.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "charge_effect.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//�e�N�X�`���̏��
#define NUM_CHARGE_EFFECT				(4)			//�e�N�X�`���̍ő�\����

#define CHARGE_EFFECT_SIZE				(80.0f)		//�G�t�F�N�g�̃T�C�Y
#define EFFECT_CHARGE_MOVE				(3.5f)		//�G�t�F�N�g�̃`���[�W�^�C�v�̕ω���

//�`���[�W�G�t�F�N�g�̃e�N�X�`���p�X
#define	CHARGE_EFFECT_TEX_PASS		"data\\TEXTURE\\charge_effect002.png"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeEffect = NULL;			//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureChargeEffect = NULL;			//�e�N�X�`���̃|�C���^
D3DXMATRIX				mtxWorldChargeEffect;					//���[���h�}�g���b�N�X
ChargeEffect			g_ChargeEffect[NUM_CHARGE_EFFECT];		//�G�t�F�N�g�̏��

//=================================
//�G�t�F�N�g�̏���������
//=================================
void InitChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾	

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, CHARGE_EFFECT_TEX_PASS, &g_pTextureChargeEffect);	
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_CHARGE_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeEffect, NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_ChargeEffect[nCntEffect].pos = ZERO_SET;				//�ʒu������
		g_ChargeEffect[nCntEffect].fSize = CHARGE_EFFECT_SIZE;	//�T�C�Y������
		g_ChargeEffect[nCntEffect].fAlpha = 1.0f;				//�����x������
		g_ChargeEffect[nCntEffect].bUse = false;				//�g���Ă��Ȃ���Ԃ�
		g_ChargeEffect[nCntEffect].bUseCharge = false;			//�g���Ă��Ȃ���Ԃ�

		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_ChargeEffect[nCntEffect].fSize, 0.0f, +g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_ChargeEffect[nCntEffect].fSize, 0.0f, +g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_ChargeEffect[nCntEffect].fSize, 0.0f, -g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_ChargeEffect[nCntEffect].fSize, 0.0f, -g_ChargeEffect[nCntEffect].fSize);

		//nor(�@��)�̐ݒ�
		pVtx[VTX_LE_UP].nor = pVtx[VTX_RI_UP].nor = 
		pVtx[VTX_LE_DO].nor = pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = 
		pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//�e�N�X�`�����_���W�̐ݒ�
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffChargeEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̏I������
//=================================
void UninitChargeEffect(void)
{	
	//�e�N�X�`���̔j��
	if (g_pTextureChargeEffect != NULL)
	{
		g_pTextureChargeEffect->Release();
		g_pTextureChargeEffect = NULL;
	}	

	//�o�b�t�@�̔j��
	if (g_pVtxBuffChargeEffect != NULL)
	{
		g_pVtxBuffChargeEffect->Release();
		g_pVtxBuffChargeEffect = NULL;
	}
}

//=================================
//�G�t�F�N�g�̍X�V����
//=================================
void UpdateChargeEffect(void)
{
	//�G�t�F�N�g�̈ʒu��ݒ�
	SetChargeEffectPos();

	//�G�t�F�N�g�̃T�C�Y�X�V  (���_���W�̍X�V������̂ŁA����Update�֐��̍Ōオ�]�܂���)
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		//�G�t�F�N�g���g���Ă���
		if (g_ChargeEffect[nCntEffect].bUse)
		{
			//�T�C�Y���X�V����
			UpdateChargeEffectSize(nCntEffect);
		}
	}
}

//�G�t�F�N�g�̃T�C�Y�X�V
void UpdateChargeEffectSize(int nEffect)
{
	//�G�t�F�N�g�̑傫�����g��
	g_ChargeEffect[nEffect].fSize += EFFECT_CHARGE_MOVE;

	//�G�t�F�N�g�̑傫�����[���ɂȂ���
	if (g_ChargeEffect[nEffect].fSize >= CHARGE_EFFECT_SIZE)
	{
		//�G�t�F�N�g�{���̑傫���ɒ���
		g_ChargeEffect[nEffect].fSize = CHARGE_EFFECT_SIZE;
		g_ChargeEffect[nEffect].bUse = false;
	}

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffChargeEffect->Lock(0, 0, (void* *)&pVtx, 0);

	//�|�C���^�[�����킹��
	pVtx += VTX_MAX * nEffect;

	//�v���C���[�̃Q�[�W�ʂ��i�[
	float Gauge = GetPlayer()[nEffect].moveGauge;

	//���_���W�̐ݒ�
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_ChargeEffect[nEffect].fSize, 0.0f, +g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_ChargeEffect[nEffect].fSize, 0.0f, +g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_ChargeEffect[nEffect].fSize, 0.0f, -g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_ChargeEffect[nEffect].fSize, 0.0f, -g_ChargeEffect[nEffect].fSize);

	//�G�t�F�N�g�F�ݒ�
	pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = 
	pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f * Gauge, 1.0f - Gauge * 0.3f, 1.0f - Gauge, g_ChargeEffect[nEffect].fAlpha);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffChargeEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̕`�揈��
//=================================
void DrawChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX  mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffChargeEffect, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureChargeEffect);
	
	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		//�G�t�F�N�g���g���Ă���
		if (g_ChargeEffect[nCntEffect].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldChargeEffect);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_ChargeEffect[nCntEffect].pos.x, g_ChargeEffect[nCntEffect].pos.y, g_ChargeEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&mtxWorldChargeEffect, &mtxWorldChargeEffect, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldChargeEffect);

			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, VTX_MAX * nCntEffect, 2);
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//�G�t�F�N�g�̈ʒu�ݒ�
void SetChargeEffectPos()
{
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		//�Ώۂ̃G�t�F�N�g���g���Ă���
		if (g_ChargeEffect[nCntEffect].bUse == true)
		{
			//�G�t�F�N�g�̈ʒu���v���C���[�̈ʒu�ɂ���
			g_ChargeEffect[nCntEffect].pos = GetPlayer()[nCntEffect].pos;
		}
	}
}

//�G�t�F�N�g�̐ݒ菈��
void SetChargeEffect(D3DXVECTOR3 pos, int nCntType)
{
	//�Ώۂ̃G�t�F�N�g���g���Ă��Ȃ�
	if (g_ChargeEffect[nCntType].bUse == false)
	{
		g_ChargeEffect[nCntType].fSize = 0.0f;		//�T�C�Y��������
		g_ChargeEffect[nCntType].fAlpha = 1.0f;		//�����x
		g_ChargeEffect[nCntType].bUse = true;		//�g���Ă����Ԃ�
	}
}
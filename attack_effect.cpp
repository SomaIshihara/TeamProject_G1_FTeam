//==========================================
//
//�G�t�F�N�g�v���O����[charge_effect.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "game.h"
#include "attack_effect.h"
#include "color.h"
#include "player.h"
#include "input.h"

//�e�N�X�`���̏��
#define NUM_ATTACK_EFFECT				(4)			//�e�N�X�`���̍ő�\����

#define ATTACK_EFFECT_SIZE				(50.0f)		//�G�t�F�N�g�̃T�C�Y
#define ATTACK_EFFECT_MAX_SIZE			(150.0f)	//�G�t�F�N�g�̍ő�T�C�Y
#define EFFECT_ATTACK_MOVE				(3.0f)		//�G�t�F�N�g�̃A�^�b�N�^�C�v�̕ω���
#define EFFECT_CLEAACCEL				(0.1f)		//�G�t�F�N�g�̓��������x

//�}�N����`
#define	CHARGE_EFFECT_TEX_PASS		"data\\TEXTURE\\AttackEffect.png"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackEffect = NULL;				//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureAttackEffect = NULL;	//�e�N�X�`���̃|�C���^
D3DXMATRIX				mtxWorldAttackEffect;							//���[���h�}�g���b�N�X
AttackEffect			g_AttackEffect[NUM_ATTACK_EFFECT];					//�G�t�F�N�g�̏��

//=================================
//�G�t�F�N�g�̏���������
//=================================
void InitAttackEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		CHARGE_EFFECT_TEX_PASS,
		&g_pTextureAttackEffect);
	
	
	//�G�t�F�N�g�̈ʒu��ݒ�
	SetAttackEffectPos();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VTX_MAX * NUM_ATTACK_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffAttackEffect, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffAttackEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		//g_AttackEffect[nCntEffect].nType = EFFECTTYPE_CHARGE;	//��ޏ�����
		g_AttackEffect[nCntEffect].nCntLoop = 0;				//���[�v�񐔏�����
		g_AttackEffect[nCntEffect].fSize = ATTACK_EFFECT_SIZE;		//�T�C�Y������
		g_AttackEffect[nCntEffect].fAlpha = 1.0f;				//�����x������
		g_AttackEffect[nCntEffect].fResize = 0.0f;
		g_AttackEffect[nCntEffect].bUse = false;				//�g���Ă��Ȃ���Ԃ�
	

		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_AttackEffect[nCntEffect].fSize, 0.0f, +g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_AttackEffect[nCntEffect].fSize, 0.0f, +g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_AttackEffect[nCntEffect].fSize, 0.0f, -g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_AttackEffect[nCntEffect].fSize, 0.0f, -g_AttackEffect[nCntEffect].fSize);

		//nor(�@��)�̐ݒ�
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = RGBA_RED;
		pVtx[VTX_RI_UP].col = RGBA_RED;
		pVtx[VTX_LE_DO].col = RGBA_RED;
		pVtx[VTX_RI_DO].col = RGBA_RED;

		//�e�N�X�`�����_���W�̐ݒ�
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffAttackEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̏I������
//=================================
void UninitAttackEffect(void)
{
	
	//�e�N�X�`���̔j��
	if (g_pTextureAttackEffect != NULL)
	{
		g_pTextureAttackEffect->Release();
		g_pTextureAttackEffect = NULL;
	}
	

	//�o�b�t�@�̔j��
	if (g_pVtxBuffAttackEffect != NULL)
	{
		g_pVtxBuffAttackEffect->Release();
		g_pVtxBuffAttackEffect = NULL;
	}
}

//=================================
//�G�t�F�N�g�̍X�V����
//=================================
void UpdateAttackEffect(void)
{
	if (GetKeyboardPress(DIK_X) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
		{
			SetAttackEffect(g_AttackEffect[nCntEffect].pos, nCntEffect);
		}
	}
	
	//�G�t�F�N�g�̈ʒu��ݒ�
	SetAttackEffectPos();

	//�G�t�F�N�g�̃T�C�Y�X�V  (���_���W�̍X�V������̂ŁA����Update�֐��̍Ōオ�]�܂���)
	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		UpdateAttackEffectSize(nCntEffect);
	}
}

//�G�t�F�N�g�̃T�C�Y�X�V
void UpdateAttackEffectSize(int nEffect)
{

	if (g_AttackEffect[nEffect].fSize <= ATTACK_EFFECT_MAX_SIZE)
	{
		g_AttackEffect[nEffect].fResize += EFFECT_ATTACK_MOVE;

		//�G�t�F�N�g�̑傫�����g��
		g_AttackEffect[nEffect].fSize += g_AttackEffect[nEffect].fResize;

	}
	
	//�G�t�F�N�g�̑傫�����K��l�ɂȂ���
	if (g_AttackEffect[nEffect].fSize >= ATTACK_EFFECT_SIZE)
	{
		g_AttackEffect[nEffect].fAlpha -= EFFECT_CLEAACCEL;

		if (g_AttackEffect[nEffect].fAlpha <= 0.0f)
		{
			//�G�t�F�N�g�{���̑傫���ɒ���
			g_AttackEffect[nEffect].fSize = ATTACK_EFFECT_SIZE;
			g_AttackEffect[nEffect].bUse = false;
		}

	}

	VERTEX_3D *pVtx;							//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffAttackEffect->Lock(0, 0, (void**)&pVtx, 0);

	//�|�C���^�[�����킹��
	pVtx += VTX_MAX * nEffect;

	//���_���W�̐ݒ�
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_AttackEffect[nEffect].fSize, 0.0f, +g_AttackEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_AttackEffect[nEffect].fSize, 0.0f, +g_AttackEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_AttackEffect[nEffect].fSize, 0.0f, -g_AttackEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_AttackEffect[nEffect].fSize, 0.0f, -g_AttackEffect[nEffect].fSize);

	pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);
	pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);
	pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);
	pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAttackEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̕`�揈��
//=================================
void DrawAttackEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		if (g_AttackEffect[nCntEffect].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldAttackEffect);

			//Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

			//�r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_AttackEffect[nCntEffect].pos.x, g_AttackEffect[nCntEffect].pos.y, g_AttackEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&mtxWorldAttackEffect, &mtxWorldAttackEffect, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldAttackEffect);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffAttackEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureAttackEffect);

			//�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, VTX_MAX * nCntEffect, 2);

			//Z�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
		}
	}
}

//�G�t�F�N�g�̈ʒu�ݒ�
void SetAttackEffectPos()
{
	Player *pPlayer = GetPlayer();

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++, pPlayer++)
	{
		//�Ώۂ̃G�t�F�N�g���g���Ă���
		if (g_AttackEffect[nCntEffect].bUse == true)
		{
			//�G�t�F�N�g�̈ʒu���v���C���[�̈ʒu�ɂ���
			g_AttackEffect[nCntEffect].pos = pPlayer->pos;
		}

	}
}

//�G�t�F�N�g�̐ݒ菈��
void SetAttackEffect(D3DXVECTOR3 pos, int nCntType)
{
	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		//�Ώۂ̃G�t�F�N�g���g���Ă��Ȃ�
		if (g_AttackEffect[nCntType].bUse == false)
		{
			g_AttackEffect[nCntType].fSize = 0.0f;		//�T�C�Y��������
			
			g_AttackEffect[nCntType].nCntLoop = 0;		//���[�v�񐔏�����
			g_AttackEffect[nCntType].fAlpha = 1.0f;		//�����x
			g_AttackEffect[nCntType].fResize = 0.0f;	//�T�C�Y�ύX�l
			g_AttackEffect[nCntType].bUse = true;		//�g���Ă����Ԃ�
			//break;
		}
	}
}
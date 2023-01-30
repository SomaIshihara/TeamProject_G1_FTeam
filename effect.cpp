//==========================================
//
//�|�[�Y�v���O����[effect.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "game.h"
#include "effect.h"
#include "color.h"
#include "player.h"
#include "input.h"

//�e�N�X�`���̏��
#define NUM_EFFECT				(4)			//�e�N�X�`���̍ő吔
#define MAX_EFFECT				(128)		//�e�N�X�`���ő吔

#define EFFECT_SIZE				(80.0f)		//�G�t�F�N�g�̃T�C�Y
#define EFFECT_CHARGE_MOVE		(3.5f)		//�G�t�F�N�g�̃`���[�W�^�C�v�̕ω���
#define EFFECT_ATTACK_MOVE		(8.0f)		//�G�t�F�N�g�̃A�^�b�N�^�C�v�̕ω���

//�e�N�X�`���̃p�X��
const char *c_pFileNameEffect[EFFECTTYPE_MAX] =
{
	"data\\TEXTURE\\charge_effect002.png",
	"data\\TEXTURE\\AttackEffect.png",
};

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;				//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureEffect[EFFECTTYPE_MAX] = {};	//�e�N�X�`���̃|�C���^
D3DXMATRIX				mtxWorldEffect;							//���[���h�}�g���b�N�X
Effect					g_Effect[NUM_EFFECT];					//�G�t�F�N�g�̏��

//=================================
//�G�t�F�N�g�̏���������
//=================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_pFileNameEffect[nCntEffect],
			&g_pTextureEffect[nCntEffect]);
	}
	
	//�G�t�F�N�g�̈ʒu��ݒ�
	SetEffectPos();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * VTX_MAX * MAX_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_Effect[nCntEffect].nType = EFFECTTYPE_CHARGE;	//��ޏ�����
		g_Effect[nCntEffect].nCntLoop = 0;				//���[�v�񐔏�����
		g_Effect[nCntEffect].fSize = EFFECT_SIZE;		//�T�C�Y������
		g_Effect[nCntEffect].fAlpha = 1.0f;				//�����x������
		g_Effect[nCntEffect].bUse = false;				//�g���Ă��Ȃ���Ԃ�
		g_Effect[nCntEffect].bUseCharge = false;		//�g���Ă��Ȃ���Ԃ�

		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fSize, 0.0f, +g_Effect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fSize, 0.0f, +g_Effect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fSize, 0.0f, -g_Effect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fSize, 0.0f, -g_Effect[nCntEffect].fSize);

		//nor(�@��)�̐ݒ�
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//�e�N�X�`�����_���W�̐ݒ�
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̏I������
//=================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureEffect[nCntEffect] != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=================================
//�G�t�F�N�g�̍X�V����
//=================================
void UpdateEffect(void)
{
	if (GetKeyboardPress(DIK_X) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
		{
			SetEffect(g_Effect[nCntEffect].pos ,nCntEffect, EFFECTTYPE_ATTACK);
		}
	}

	if (GetKeyboardPress(DIK_M) == true)
	{
		for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
		{
			SetEffect(g_Effect[nCntEffect].pos, nCntEffect, EFFECTTYPE_CHARGE);
		}
	}
	
	//�G�t�F�N�g�̈ʒu��ݒ�
	SetEffectPos();

	//�G�t�F�N�g�̃T�C�Y�X�V  (���_���W�̍X�V������̂ŁA����Update�֐��̍Ōオ�]�܂���)
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		UpdateEffectSize(nCntEffect);
	}
}

//�G�t�F�N�g�̃T�C�Y�X�V
void UpdateEffectSize(int nEffect)
{
	//�G�t�F�N�g�̎�ނɂ��T�C�Y�̍X�V
	switch (g_Effect[nEffect].nType)
	{
	case EFFECTTYPE_CHARGE:
	{
		//�G�t�F�N�g�̑傫�����g��
		g_Effect[nEffect].fSize += EFFECT_CHARGE_MOVE;

		//�G�t�F�N�g�̑傫�����[���ɂȂ���
		if (g_Effect[nEffect].fSize >= EFFECT_SIZE)
		{
			//�G�t�F�N�g�{���̑傫���ɒ���
			g_Effect[nEffect].fSize = EFFECT_SIZE;
			//g_Effect[nEffect].nCntLoop++;		//���[�v�񐔉��Z
			g_Effect[nEffect].bUse = false;

	/*		if (g_Effect[nEffect].nCntLoop >= 5)
			{
				
				g_Effect[nEffect].nCntLoop = 0;
			}*/
		}
	}
	break;

	case EFFECTTYPE_ATTACK:
	{

		//�G�t�F�N�g�̑傫�����g��
		g_Effect[nEffect].fSize += EFFECT_ATTACK_MOVE;

		//�G�t�F�N�g�̑傫�����{���̑傫���𒴂���
		if (EFFECT_SIZE <= g_Effect[nEffect].fSize)
		{
			g_Effect[nEffect].fAlpha -= 0.1f;

			if (g_Effect[nEffect].fAlpha <= 0.0f)
			{
				//�g���Ă��Ȃ���Ԃ�
				g_Effect[nEffect].bUse = false;
			}
			
			g_Effect[nEffect].nCntLoop = 0;				//���[�v�񐔏�����
		}
	}
	}

	VERTEX_3D *pVtx;							//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//�|�C���^�[�����킹��
	pVtx += VTX_MAX * nEffect;

	//���_���W�̐ݒ�
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Effect[nEffect].fSize, 0.0f, +g_Effect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Effect[nEffect].fSize, 0.0f, +g_Effect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Effect[nEffect].fSize, 0.0f, -g_Effect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Effect[nEffect].fSize, 0.0f, -g_Effect[nEffect].fSize);

	pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);
	pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);
	pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);
	pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̕`�揈��
//=================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		if (g_Effect[nCntEffect].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldEffect);

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
			D3DXMatrixTranslation(&mtxTrans, g_Effect[nCntEffect].pos.x, g_Effect[nCntEffect].pos.y, g_Effect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldEffect);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect[g_Effect[nCntEffect].nType]);

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
void SetEffectPos(void)
{
	Player *pPlayer = GetPlayer();

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++, pPlayer++)
	{
		//�Ώۂ̃G�t�F�N�g���g���Ă���
		if (g_Effect[nCntEffect].bUse == true)
		{
			//�G�t�F�N�g�̈ʒu���v���C���[�̈ʒu�ɂ���
			g_Effect[nCntEffect].pos = pPlayer->pos;
		}
		//�G�t�F�N�g�̃^�C�v���`���[�W�Ȃ�A�Ǐ]����
		if (g_Effect[nCntEffect].nType == EFFECTTYPE_CHARGE)
		{
			
		}
	}
}

//�G�t�F�N�g�̐ݒ菈��
void SetEffect(D3DXVECTOR3 pos, int nCntType, EFFECTTYPE type)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�Ώۂ̃G�t�F�N�g���g���Ă��Ȃ�
		if (g_Effect[nCntEffect].bUse == false)
		{
			//��ނ��`���[�W
			if (type == EFFECTTYPE_CHARGE)
			{
				g_Effect[nCntEffect].fSize = 0.0f;		//�T�C�Y��������
			}

			//��ނ������������̂���
			else if (type == EFFECTTYPE_ATTACK)
			{
				g_Effect[nCntEffect].fSize = 0.0f;		//�T�C�Y��������
			}

			g_Effect[nCntEffect].nType = type;		//��ސݒ�
			g_Effect[nCntEffect].nCntLoop = 0;		//���[�v�񐔏�����
			g_Effect[nCntEffect].fAlpha = 1.0f;
			g_Effect[nCntEffect].bUse = true;		//�g���Ă����Ԃ�
		}
	}
}
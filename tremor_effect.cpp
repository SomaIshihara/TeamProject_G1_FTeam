//==========================================
//
//�G�t�F�N�g�v���O����[tremor_effect.cpp]
//Author:�Ό��D�n�i�x�[�X�F�ѓc�F��j
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "tremor_effect.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//�e�N�X�`���̏��
#define MAX_TREMOR_EFFECT				(4)			//�e�N�X�`���̍ő�\����

#define TREMOR_EFFECT_SIZE				(50.0f)		//�G�t�F�N�g�̃T�C�Y
#define EFFECT_TREMOR_MOVE				(2.5f)		//�G�t�F�N�g�̃`���[�W�^�C�v�̕ω���

//�}�N����`
#define	TREMOR_EFFECT_TEX_PASS		"data\\TEXTURE\\charge_effect001.png"
#define TREMOR_FADEOUT_TIME				(15)		//�t�F�[�h�A�E�g�J�n���ԁi�t���[���P�ʁj

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTremorEffect = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureTremorEffect = NULL;		//�e�N�X�`���̃|�C���^
D3DXMATRIX				mtxWorldTremorEffect;				//���[���h�}�g���b�N�X
TremorEffect			g_TremorEffect[MAX_TREMOR_EFFECT];	//�G�t�F�N�g�̏��

//=================================
//�G�t�F�N�g�̏���������
//=================================
void InitTremorEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;							//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TREMOR_EFFECT_TEX_PASS, &g_pTextureTremorEffect);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_TREMOR_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,D3DPOOL_MANAGED, &g_pVtxBuffTremorEffect, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTremorEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_TremorEffect[nCntEffect].nCounter = 0;				//�o�ߎ��ԏ�����
		g_TremorEffect[nCntEffect].fSize = TREMOR_EFFECT_SIZE;	//�T�C�Y������
		g_TremorEffect[nCntEffect].fAlpha = 1.0f;				//�����x������
		g_TremorEffect[nCntEffect].bUse = false;				//�g���Ă��Ȃ���Ԃ�

		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_TremorEffect[nCntEffect].fSize, 0.0f, +g_TremorEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_TremorEffect[nCntEffect].fSize, 0.0f, +g_TremorEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_TremorEffect[nCntEffect].fSize, 0.0f, -g_TremorEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_TremorEffect[nCntEffect].fSize, 0.0f, -g_TremorEffect[nCntEffect].fSize);

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

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTremorEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̏I������
//=================================
void UninitTremorEffect(void)
{	
	//�e�N�X�`���̔j��
	if (g_pTextureTremorEffect != NULL)
	{
		g_pTextureTremorEffect->Release();
		g_pTextureTremorEffect = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffTremorEffect != NULL)
	{
		g_pVtxBuffTremorEffect->Release();
		g_pVtxBuffTremorEffect = NULL;
	}
}

//=================================
//�G�t�F�N�g�̍X�V����
//=================================
void UpdateTremorEffect(void)
{
	//�G�t�F�N�g����
	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++)
	{
		if (g_TremorEffect[nCntEffect].bUse == true)
		{
			//�J�E���g���₷
			g_TremorEffect[nCntEffect].nCounter++;

			//�G�t�F�N�g�̃T�C�Y�X�V
			UpdateTremorEffectSize(nCntEffect);

			//���ԉ߂��������
			if (g_TremorEffect[nCntEffect].nCounter > TREMOR_EFFECT_TIME)
			{
				g_TremorEffect[nCntEffect].bUse = false;
			}

			//�t�F�[�h�A�E�g�J�n�����瓧���x���炷
			else if (g_TremorEffect[nCntEffect].nCounter > TREMOR_FADEOUT_TIME)
			{
				g_TremorEffect[nCntEffect].fAlpha = 1.0f - (float)(g_TremorEffect[nCntEffect].nCounter - TREMOR_FADEOUT_TIME) / (TREMOR_EFFECT_TIME - TREMOR_FADEOUT_TIME);
			}
		}
	}
}

//�G�t�F�N�g�̃T�C�Y�X�V
void UpdateTremorEffectSize(int nEffect)
{
	//�G�t�F�N�g�̑傫�����g��
	g_TremorEffect[nEffect].fSize += EFFECT_TREMOR_MOVE;

	//�G�t�F�N�g�̑傫�����[���ɂȂ���
	if (g_TremorEffect[nEffect].fSize >= TREMOR_EFFECT_SIZE)
	{
		//�G�t�F�N�g�{���̑傫���ɒ���
		g_TremorEffect[nEffect].fSize = TREMOR_EFFECT_SIZE;
	}

	VERTEX_3D *pVtx;							//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTremorEffect->Lock(0, 0, (void**)&pVtx, 0);

	//�|�C���^�[�����킹��
	pVtx += VTX_MAX * nEffect;

	//���_���W�̐ݒ�
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_TremorEffect[nEffect].fSize, 0.0f, +g_TremorEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_TremorEffect[nEffect].fSize, 0.0f, +g_TremorEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_TremorEffect[nEffect].fSize, 0.0f, -g_TremorEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_TremorEffect[nEffect].fSize, 0.0f, -g_TremorEffect[nEffect].fSize);

	//���_�J���[�̐ݒ�		�G�t�F�N�g�̃T�C�Y���K��ʂ𒴂��Ă���΁A���X�ɓ����ɂȂ��Ă���
	pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col =
	pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TremorEffect[nEffect].fAlpha);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTremorEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̕`�揈��
//=================================
void DrawTremorEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++)
	{
		if (g_TremorEffect[nCntEffect].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorldTremorEffect);

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
			D3DXMatrixTranslation(&mtxTrans, g_TremorEffect[nCntEffect].pos.x, g_TremorEffect[nCntEffect].pos.y, g_TremorEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&mtxWorldTremorEffect, &mtxWorldTremorEffect, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldTremorEffect);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffTremorEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTremorEffect);

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

//�G�t�F�N�g�̐ݒ菈��
void SetTremorEffect(D3DXVECTOR3 pos)
{
	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++)
	{
		//�Ώۂ̃G�t�F�N�g���g���Ă��Ȃ�
		if (g_TremorEffect[nCntEffect].bUse == false)
		{
			g_TremorEffect[nCntEffect].pos = pos;			//�ʒu�ݒ�
			g_TremorEffect[nCntEffect].fSize = 0.0f;		//�T�C�Y��������			
			g_TremorEffect[nCntEffect].nCounter = 0;		//���[�v�񐔏�����
			g_TremorEffect[nCntEffect].fAlpha = 1.0f;		//�����x
			g_TremorEffect[nCntEffect].bUse = true;			//�g���Ă����Ԃ�
			break;
		}
	}
}
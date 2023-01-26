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

//�e�N�X�`���̏��
#define NUM_EFFECT				(1)									//�e�N�X�`���̍ő吔

const char *c_pFileNameEffect[] =
{
	"data\\TEXTURE\\charge_effect001.png"
};

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffect[NUM_EFFECT] = {};			//�e�N�X�`���̃|�C���^
Effect g_Effect[NUM_EFFECT];

D3DXMATRIX mtxWorldEffect;

//=================================
//�G�t�F�N�g�̏���������
//=================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCntEffect=0;nCntEffect<NUM_EFFECT; nCntEffect++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_pFileNameEffect[nCntEffect],
			&g_pTextureEffect[nCntEffect]);
	}
	
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		g_Effect[nCntEffect].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++, pVtx+=4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Effect[nCntEffect].pos.x - 40.0f, g_Effect[nCntEffect].pos.y + 40.0f, 20.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Effect[nCntEffect].pos.x + 40.0f, g_Effect[nCntEffect].pos.y + 40.0f, -20.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Effect[nCntEffect].pos.x - 40.0f, g_Effect[nCntEffect].pos.y - 40.0f, 20.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Effect[nCntEffect].pos.x + 40.0f, g_Effect[nCntEffect].pos.y - 40.0f, -20.0f);

		//nor(�@��)�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = RGBA_RED;
		pVtx[1].col = RGBA_RED;
		pVtx[2].col = RGBA_RED;
		pVtx[3].col = RGBA_RED;

		//�e�N�X�`�����_���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffEffect->Unlock();
}

//=================================
//�G�t�F�N�g�̏I������
//=================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
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
	Player pPlayer = *GetPlayer();
	

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		g_Effect[nCntEffect].pos= pPlayer.pos;
	}

}

//=================================
//�G�t�F�N�g�̕`�揈��
//=================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//�v�Z�p�}�g���b�N�X

											//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
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

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&mtxWorldEffect, NULL, &mtxView);//�t�s������߂�
															  /*g_Billboard.mtxWorld._12 = 0.0f;
															  g_Billboard.mtxWorld._21 = 0.0f;*/
		mtxWorldEffect._41 = 0.0f;
		mtxWorldEffect._42 = 0.0f;
		mtxWorldEffect._43 = 0.0f;

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
		pDevice->SetTexture(0, g_pTextureEffect[nCntEffect]);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect, 2);

		//Z�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


		//�A���t�@�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	}
}
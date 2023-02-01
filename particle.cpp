#include"main.h"
#include"particle.h"

//**********************************************************************
//�O���[�o���ϐ�
//**********************************************************************
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;			//�e�N�X�`���o�b�t�@
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffParticle = NULL;			//���_�o�b�t�@
D3DXMATRIX				g_mtxWorldParticle;					//���[���h�}�g���b�N�X
Particle				g_aParticle[MAX_PARTICLE];			//�p�[�e�B�N���̏��

//======================================================================
//�p�[�e�B�N���̏���������
//======================================================================
void InitParticle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_3D *pVtx;

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.jpg",
		&g_pTextureParticle);

	//�G�t�F�N�g�̃f�[�^�̏�����
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
	{
		pVtx[0].pos = D3DXVECTOR3(-4.0f,  4.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3( 4.0f,  4.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-4.0f, -4.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3( 4.0f, -4.0f, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffParticle->Unlock();
}
//==============================================================
//�p�[�e�B�N���̏I������
//==============================================================
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureParticle != NULL)
	{
	g_pTextureParticle->Release();
	g_pTextureParticle = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
	g_pVtxBuffParticle->Release();
	g_pVtxBuffParticle = NULL;
	}
}
//==============================================================
//�p�[�e�B�N���̍X�V����
//==============================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			g_aParticle[nCntParticle].nLife--;

			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;
			g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;

			//******************************************************************//
			//						�p�[�e�B�N���̏�ԕω�						//
			//******************************************************************//

			switch (g_aParticle[nCntParticle].nType)
			{

				//�ʏ�̃p�[�e�B�N��
			case PARTICLE_NORMAL:

				pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

				g_aParticle[nCntParticle].move.x += (0.0f - g_aParticle[nCntParticle].move.x) * 0.1f;
				g_aParticle[nCntParticle].move.y += (0.0f - g_aParticle[nCntParticle].move.y) * 0.1f;
				g_aParticle[nCntParticle].move.z += (0.0f - g_aParticle[nCntParticle].move.z) * 0.1f;

				pVtx[0].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nLifeTemp));
				pVtx[1].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nLifeTemp));
				pVtx[2].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nLifeTemp));
				pVtx[3].col = D3DXCOLOR(1.0f, 0.6f, 0.0f, ((float)g_aParticle[nCntParticle].nLife / (float)g_aParticle[nCntParticle].nLifeTemp));

				break;
			}
			
			//�p�[�e�B�N���̔j��
			if (g_aParticle[nCntParticle].nLife == 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffParticle->Unlock();
}
//======================================================================
//�p�[�e�B�N���̕`�揈��
//======================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();  //�f�o�C�X�̎擾
	D3DXMATRIX		  mtxTrans;				  //�v�Z�p�}�g���b�N�X
	D3DXMATRIX		  mtxView;				  //�r���[�}�g���b�N�X�擾�p

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���_�o�b�t�@�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldParticle);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����Ɍ�����
			D3DXMatrixInverse(&g_mtxWorldParticle, NULL, &mtxView);		//�t�s������߂�
			g_mtxWorldParticle._41 = 0.0f;
			g_mtxWorldParticle._42 = 0.0f;
			g_mtxWorldParticle._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureParticle);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
//======================================================================
//�p�[�e�B�N���̃Z�b�g����
//======================================================================
void SetParticle(D3DXVECTOR3 pos, float fRadius, int nLife, int nType)
{
	VERTEX_3D *pVtx;
	int ParticleCount = 0;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			//******************************************************************//
			//						�p�[�e�B�N���̏�Ԑݒ�						//
			//******************************************************************//

			switch (nType)
			{
				//�ʏ�̃p�[�e�B�N��
			case PARTICLE_NORMAL:

				//�ʒu�̐ݒ�
				g_aParticle[nCntParticle].pos = pos;

				//�ړ��ʂ̐ݒ�
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;

				//���a�̐ݒ�
				g_aParticle[nCntParticle].fRadius = fRadius;

				//�����̐ݒ�
				g_aParticle[nCntParticle].nLife = (rand() % 40 - 20) + nLife;

				//�����̐��l�ۑ�
				g_aParticle[nCntParticle].nLifeTemp = g_aParticle[nCntParticle].nLife;

				//��ނ̐ݒ�
				g_aParticle[nCntParticle].nType = nType;

				//�p�[�e�B�N�����g���Ă����Ԃɂ���
				g_aParticle[nCntParticle].bUse = true;

				ParticleCount++;

				break;

			}
		}

		if (nType == PARTICLE_NORMAL && ParticleCount == NORMAL_PARTICLE)
		{
			break;
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffParticle->Unlock();
}
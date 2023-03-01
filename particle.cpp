#include"main.h"
#include"particle.h"
#include"bonus.h"
#include"PvP_Player.h"

//**********************************************************************
//�}�N����`
//**********************************************************************
#define PARTICLE_DIRECTION_MOVE	(10.0f)		//��������^�C�v�̃p�[�e�B�N���̎��k��
#define PARTICLE_TEX_PASS		"data/TEXTURE/effect000.jpg"

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

	//�|�C���^���擾
	VERTEX_3D *pVtx;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEX_PASS, &g_pTextureParticle);

	//�G�t�F�N�g�̃f�[�^�̏�����
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = 
		g_aParticle[nCntParticle].move = ZERO_SET;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_PARTICLE,	D3DUSAGE_WRITEONLY,	FVF_VERTEX_3D, D3DPOOL_MANAGED,&g_pVtxBuffParticle,	NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += VTX_MAX)
	{
		//�ʒu�̐ݒ�
		pVtx[VTX_LE_UP].pos = pVtx[VTX_RI_UP].pos = pVtx[VTX_LE_DO].pos = pVtx[VTX_RI_DO].pos = ZERO_SET;

		//�@���̐ݒ�
		pVtx[VTX_LE_UP].nor = pVtx[VTX_RI_UP].nor = pVtx[VTX_LE_DO].nor = pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//�F�̐ݒ�
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffParticle->Unlock();
}
//======================================================================
//�p�[�e�B�N���̏I������
//======================================================================
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
//======================================================================
//�p�[�e�B�N���̍X�V����
//======================================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += VTX_MAX)
	{
		//�g�p����Ă���Ƃ�
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//�p�[�e�B�N���̏�Ԑݒ�
			StateParticle(nCntParticle, pVtx);
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

	//���_�o�b�t�@�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureParticle);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	
	

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].nObject != OBJECT_PLAYER_GHOST)
		{//�A���t�@�u�����f�B���O�����Z�����ɐݒ�i�S�[�X�g�ȊO�j
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else
		{//�A���t�@�u�����f�B���O�����Z�����ɐݒ�i�S�[�X�g�j
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		
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

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, 2);
		}
	}

	//���ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
//======================================================================
//�p�[�e�B�N���̃Z�b�g����
//======================================================================
void SetParticle(D3DXVECTOR3 pos, float fRadius, int nLife, PARTICLE nType, PARTICLE_OBJECT nObject)
{
	int ParticleCount = 0;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//�p�[�e�B�N�����g���Ă��Ȃ�
		if (g_aParticle[nCntParticle].bUse == false)
		{
			//�ʒu�̐ݒ�
			g_aParticle[nCntParticle].pos = pos;

			//���a�̐ݒ�
			g_aParticle[nCntParticle].fRadius = fRadius;

			//�����̐��l�ۑ�
			g_aParticle[nCntParticle].nLifeTemp = g_aParticle[nCntParticle].nLife;

			//��ނ̐ݒ�
			g_aParticle[nCntParticle].nType = nType;

			//�I�u�W�F�N�g��ނ̐ݒ�
			g_aParticle[nCntParticle].nObject = nObject;

			switch (nType)
			{
				//�ʏ�̃p�[�e�B�N��
			case PARTICLE_NORMAL:

				//�ړ��ʂ̐ݒ�
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				break;

				//�z���̃p�[�e�B�N��
			case PARTICLE_ACSORPTION:

				g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].pos.x + sinf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;
				g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].pos.y + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;
				g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].pos.z + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;

				//�p�[�e�B�N���̈ړ������ݒ�
				DirectionParticle(nCntParticle);

				break;

				//�z���̃p�[�e�B�N���i�S�[�X�g�j
			case PARTICLE_ACSORPTION_GHOST:

				g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].pos.x + sinf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *1.0f;
				g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].pos.y + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *1.0f;
				g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].pos.z + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *1.0f;

				//�p�[�e�B�N���̈ړ������ݒ�
				DirectionParticle(nCntParticle);

				break;
			}

			//�����̐ݒ�
			g_aParticle[nCntParticle].nLife = (rand() % 30 - 15) + nLife;

			//�����ʃJ�E���g
			ParticleCount++;

			//�p�[�e�B�N�����g���Ă����Ԃɂ���
			g_aParticle[nCntParticle].bUse = true;

			//��ޕʔ����ʂɂ����鏈���I������
			if (nType == PARTICLE_NORMAL && ParticleCount == NORMAL_PARTICLE)
			{
				break;
			}
			else if (nType == PARTICLE_ACSORPTION && ParticleCount == ACSORPTION_PARTICLE)
			{
				break;
			}
			else if (nType == PARTICLE_ACSORPTION_GHOST && ParticleCount == ACSORPTION_PARTICLE)
			{
				break;
			}
		}
	}
}
//======================================================================
//�p�[�e�B�N���̏�Ԑݒ�
//======================================================================
void StateParticle(int nCount,VERTEX_3D *pVtx)
{
	//�{�[�i�X�̗͎̑擾
	int BonusLife = GetBonus().nLife;

	//�����̌���
	g_aParticle[nCount].nLife--;

	//�p�[�e�B�N���̔j��
	if (g_aParticle[nCount].nLife <= 0)
	{
		g_aParticle[nCount].bUse = false;
		return;//�������I����
	}

	//���W�̐ݒ�
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aParticle[nCount].fRadius, +g_aParticle[nCount].fRadius, 0.0f);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aParticle[nCount].fRadius, +g_aParticle[nCount].fRadius, 0.0f);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aParticle[nCount].fRadius, -g_aParticle[nCount].fRadius, 0.0f);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aParticle[nCount].fRadius, -g_aParticle[nCount].fRadius, 0.0f);

	//�ړ��ʂ̐ݒ�
	g_aParticle[nCount].pos.x += g_aParticle[nCount].move.x;
	g_aParticle[nCount].pos.y += g_aParticle[nCount].move.y;
	g_aParticle[nCount].pos.z += g_aParticle[nCount].move.z;

	//*************************************//
	//�p�[�e�B�N���̎�ޕ� �ړ��ʂ̒�������//
	//*************************************//
	switch (g_aParticle[nCount].nType)
	{
		//******************//
		//==================//
		//�ʏ�̃p�[�e�B�N��//
		//==================//
		//******************//
		case PARTICLE_NORMAL:
		{
			g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.3f;
			g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.3f;
			g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.3f;
		}
		break;

		//******************//
		//==================//
		//�z���̃p�[�e�B�N��//
		//==================//
		//******************//
		case PARTICLE_ACSORPTION:
		{
			g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.2f;
			g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.2f;
			g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.2f;
		}
		break;

		case PARTICLE_ACSORPTION_GHOST:
		{
			g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.5f;
			g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.5f;
			g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.5f;
		}
		break;
	}

	//�p�[�e�B�N���̎����̊������Z�o
	float LifeParcent = (float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp;

	//*********************************//
	//�p�[�e�B�N���̎������ �F��������//
	//*********************************//
	switch (g_aParticle[nCount].nObject)
	{
		//=======================================
		//		�v���C���[�̃o�t �p�[�e�B�N��
		//=======================================
		//�A�^�b�N����
	case OBJECT_PLAYER_ATK:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.1f);
		break;
		
		//�K�[�h����
	case OBJECT_PLAYER_DEF:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
		break;

		//�S�[�X�g��
	case OBJECT_PLAYER_GHOST:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.15f);
		break;

	case OBJECT_PLAYER_INVINCIBLE:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 0.75f, 0.0f, 0.1f);
		break;

		//�{�[�i�X�p�[�e�B�N��
	case OBJECT_BONUS:
		if (BonusLife == 3)
		{
			pVtx[VTX_LE_UP].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(0.6f, 0.2f, 0.4f, LifeParcent);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(0.4f, 0.2f, 0.6f, LifeParcent);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.2f, 0.8f, LifeParcent);
		}
		else if (BonusLife == 2)
		{
			pVtx[VTX_LE_UP].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(0.6f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(0.4f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, LifeParcent);
		}
		else
		{
			pVtx[VTX_LE_UP].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(0.6f, 0.4f, 0.2f, LifeParcent);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(0.4f, 0.6f, 0.2f, LifeParcent);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.8f, 0.2f, LifeParcent);
		}
		break;
	}
}
//======================================================================
//�p�[�e�B�N���̈ړ������ݒ�
//======================================================================
void DirectionParticle(int nCount)
{
	//������̍��W�ۑ��p
	D3DXVECTOR3 OrnerPos;

	switch (g_aParticle[nCount].nObject)
	{
		//�v���C���[�p�[�e�B�N��
	case OBJECT_PLAYER_ATK:
	case OBJECT_PLAYER_DEF:
	case OBJECT_PLAYER_GHOST:
		OrnerPos = GetPlayer()->pos;
		break;

		//�{�[�i�X�p�[�e�B�N��
	case OBJECT_BONUS:
		OrnerPos = GetBonus().pos;
		break;
	}

	//�����傪�E�ɂ���
	if (g_aParticle[nCount].pos.x < OrnerPos.x)
	{
		//�E�i�v���X�j�����ւ̈ړ��ʂ���
		g_aParticle[nCount].move.x = PARTICLE_DIRECTION_MOVE;
	}
	//�����傪���ɂ���
	else if (OrnerPos.x < g_aParticle[nCount].pos.x)
	{
		//���i�}�C�i�X�j�����ւ̈ړ��ʂ���
		g_aParticle[nCount].move.x = -PARTICLE_DIRECTION_MOVE;
	}

	//�����傪���ɂ���
	if (OrnerPos.z > g_aParticle[nCount].pos.z)
	{
		//���i�v���X�j�����ւ̈ړ��ʂ���
		g_aParticle[nCount].move.z = PARTICLE_DIRECTION_MOVE;
	}
	//�����傪��O�ɂ���
	else if (OrnerPos.z < g_aParticle[nCount].pos.z)
	{
		//�O�i�}�C�i�X�j�����ւ̈ړ��ʂ���
		g_aParticle[nCount].move.z = -PARTICLE_DIRECTION_MOVE;
	}

	//�����傪���ɂ���
	if (g_aParticle[nCount].pos.y < OrnerPos.y)
	{
		//��i�v���X�j�����ւ̈ړ��ʂ���
		g_aParticle[nCount].move.y = PARTICLE_DIRECTION_MOVE;
	}
	//�����傪��ɂ���
	else if (OrnerPos.y < g_aParticle[nCount].pos.y)
	{
		//���i�}�C�i�X�j�����ւ̈ړ��ʂ���
		g_aParticle[nCount].move.y = -PARTICLE_DIRECTION_MOVE;
	}
}
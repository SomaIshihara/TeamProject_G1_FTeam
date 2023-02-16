//========================================
// 
// EFF:�Ռ��g[00] �̊֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** eff_shock-wave_00.cpp ***
//========================================
#include "main.h"
#include "color.h"
#include "eff_shock-wave_00.h"

//****************************************
// �}�N����`
//****************************************
#define EFF_SHOCKWAVE_00_MAX		(16)	// EFF:�Ռ��g[00] �̍ő吔
#define EFF_SHOCKWAVE_00_DIVISION	(16)	// EFF:�Ռ��g[00] �̕�����

// EFF:�Ռ��g[00] �̖@���x�N�g��
#define EFF_SHOCKWAVE_00_NOR	D3DXVECTOR3(0.0f,1.0f,0.0f)

//****************************************
// �\���̂̒�`
//****************************************
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	bool		bUse;			// �g�p����Ă��邩�t���O
	int			nLife;			// ����
	int			nLifeMax;		// �����̍ő�l
	float		fRadius;		// ���a
	float		fThickness;		// ����
	float		fEdgeHeight;	// ���̍���
	Color		inCol;			// �����̐F
	Color		outCol;			// �O���̐F
}Eff_shockWave_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// EFF:�Ռ��g[00] �̃p�����[�^�[����������
void InitParameterEff_shockWave_00(Eff_shockWave_00 *pEff);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_shockWave_00	// EFF:�Ռ��g[00] �̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
D3DXMATRIX				g_mtxWorldEff_shockWave_00;	// EFF:�Ռ��g[00] �̃��[���h�}�g���b�N�X
Eff_shockWave_00		g_aEff_shockWave_00			// EFF:�Ռ��g[00] �̏��
						[EFF_SHOCKWAVE_00_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterEff_shockWave_00�֐� - EFF:�Ռ��g[00] �̃p�����[�^�[���������� -
//========================================
void InitParameterEff_shockWave_00(Eff_shockWave_00 *pEff) 
{
	pEff->pos			= ZERO_SET;		// �ʒu
	pEff->rot			= ZERO_SET;		// ����
	pEff->bUse			= false;		// �g�p����Ă��邩�t���O
	pEff->nLife			= 0;			// ����
	pEff->nLifeMax		= 0;			// �����̍ő�l
	pEff->fRadius		= 0.0f;			// ���a
	pEff->fThickness	= 0.0f;			// ����
	pEff->fEdgeHeight	= 0.0f;			// ���̍���
	pEff->inCol			= {0,0,0,0};	// �F
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitEff_shockWave_00�֐� - EFF:�Ռ��g[00] �̏��������� -
//========================================
void InitEff_shockWave_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((EFF_SHOCKWAVE_00_DIVISION + 1) * 2) * EFF_SHOCKWAVE_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_shockWave_00,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_shockWave_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_shockWave_00	*pEff	// EFF:�Ռ��g[00] �̏��\����
						= g_aEff_shockWave_00;

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++,pEff++)
	{
		// �p�����[�^�[�̏���������
		InitParameterEff_shockWave_00(pEff);

		for (int nCntVtx = 0; nCntVtx < (EFF_SHOCKWAVE_00_DIVISION + 1) * 2; nCntVtx++, pVtx++)
		{
			pVtx->nor = EFF_SHOCKWAVE_00_NOR;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEff_shockWave_00->Unlock();
}

//========================================
// UninitEff_shockWave_00�֐� - EFF:�Ռ��g[00] �̏I������ -
//========================================
void UninitEff_shockWave_00(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEff_shockWave_00 != NULL)
	{
		g_pVtxBuffEff_shockWave_00->Release();
		g_pVtxBuffEff_shockWave_00 = NULL;
	}
}

//========================================
// UpdateEff_shockWave_00�֐� - EFF:�Ռ��g[00] �̍X�V���� -
//========================================
void UpdateEff_shockWave_00(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_shockWave_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_shockWave_00	*pEff	// EFF:�Ռ��g[00] �̏��̃|�C���^
						= g_aEff_shockWave_00;

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++, pEff++, pVtx += ((EFF_SHOCKWAVE_00_DIVISION + 1) * 2))
	{
		if (!pEff->bUse)
		{// �g�p����Ă����ԂłȂ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}
		else if (--pEff->nLife <= 0)
		{// ���������Z��������0�ȉ��̎��A
			pEff->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���
			continue;			// �J��Ԃ�������܂�Ԃ�
		}

		for (int nCntDiv = 0; nCntDiv < EFF_SHOCKWAVE_00_DIVISION + 1; nCntDiv++)
		{
			float fSetAngle = nCntDiv * (D3DX_PI / (float)EFF_SHOCKWAVE_00_DIVISION) * 2;
			int   nStartVtx = nCntDiv * 2;
			float fLifeRate = (float)pEff->nLife / (float)pEff->nLifeMax;
			float fRadius	= pEff->fRadius * (1.0f - fLifeRate);

			// �����̒��_���W��ݒ�
			{
				float fDistance = fRadius - pEff->fThickness;
				if (fDistance <= 0.0f) 
				{// ������0�ȉ��̎��A0�ɐ���
					fDistance = 0.0f;
				}
				pVtx[nStartVtx].pos.x = sinf(fSetAngle) * (fDistance);
				pVtx[nStartVtx].pos.z = cosf(fSetAngle) * (fDistance);
			}
			// �O���̒��_���W��ݒ�
			pVtx[nStartVtx + 1].pos.x = sinf(fSetAngle) * fRadius;
			pVtx[nStartVtx + 1].pos.z = cosf(fSetAngle) * fRadius;
			pVtx[nStartVtx + 1].pos.y = pEff->fEdgeHeight;

			// �F��ݒ�
			pVtx[nStartVtx].col =
				D3DCOLOR_RGBA(
					pEff->inCol.r,
					pEff->inCol.g,
					pEff->inCol.b,
					(int)(pEff->inCol.a * fLifeRate));
			pVtx[nStartVtx + 1].col =
				D3DCOLOR_RGBA(
					pEff->outCol.r,
					pEff->outCol.g,
					pEff->outCol.b,
					(int)(pEff->outCol.a * fLifeRate));
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEff_shockWave_00->Unlock();
}

//========================================
// DrawEff_shockWave_00�֐� - EFF:�Ռ��g[00] �̕`�揈�� -
//========================================
void DrawEff_shockWave_00(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX		  mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	Eff_shockWave_00  *pEff = g_aEff_shockWave_00;	// EFF:�Ռ��g[00] �̏��\����
		
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEff_shockWave_00, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//���ʃJ�����O��ON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���C�e�B���O���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 40);

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++, pEff++)
	{
		if ((!pEff->bUse) || (pEff->nLife == pEff->nLifeMax)) 
		{// �g�p����Ă��Ȃ���� or �������ő�l�ɓ��������A
			continue;	// ������܂�Ԃ�
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldEff_shockWave_00);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pEff->rot.y, pEff->rot.x, pEff->rot.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shockWave_00, &g_mtxWorldEff_shockWave_00, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pEff->pos.x, pEff->pos.y, pEff->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldEff_shockWave_00, &g_mtxWorldEff_shockWave_00, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEff_shockWave_00);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((EFF_SHOCKWAVE_00_DIVISION + 1) * 2) * nCntEff, (EFF_SHOCKWAVE_00_DIVISION * 2));
	}

	//���C�e�B���O���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 40);

	//���ʂ̃J�����O���[�h�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//========================================
// SetEff_shockWave_00�֐� - EFF:�Ռ��g[00] �̐ݒ菈�� -
//========================================
void SetEff_shockWave_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fThickness, float fEdgeHeight,Color inCol, Color outCol, int nLife)
{
	Eff_shockWave_00 *pEff	// EFF:�Ռ��g[00] �̏��̃|�C���^
					 = g_aEff_shockWave_00;

	for (int nCntEff = 0; nCntEff < EFF_SHOCKWAVE_00_MAX; nCntEff++, pEff++)
	{
		if (pEff->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// ������܂�Ԃ�
			continue;
		}

		pEff->pos = pos;					// �ʒu����
		pEff->rot = rot;					// ��������
		pEff->fRadius = fRadius;			// ���a����
		pEff->fThickness = fThickness;		// ��������
		pEff->fEdgeHeight = fEdgeHeight;	// ���̍�������
		pEff->inCol = inCol;				// �����̐F����
		pEff->outCol = outCol;				// �O���̐F����
		pEff->nLife	=						// ��������
		pEff->nLifeMax = nLife;				// �����̍ő�l����
		pEff->bUse = true;					// �g�p����Ă����Ԃɂ���

		// �J��Ԃ������𔲂���
		break;
	}
}
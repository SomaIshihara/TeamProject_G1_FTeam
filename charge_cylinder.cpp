//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.cpp]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "charge_cylinder.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//�}�N����`
#define CHARGECYLINDER_HEIGHT				(20.0f)						// �V�����_�[�̍���
#define CHARGECYLINDER_SPLIT				(32)						// �V�����_�[�̕�����
#define CHARGECYLINDER_TEX_RESOLUTION		(3.0f)						// �V�����_�[�̃e�N�X�`���̕�
#define NUM_CHARGE_CYLINDER					(4)							// �V�����_�[�̐�
#define CHARGECYLINDER_ALL_VTX		(CHARGECYLINDER_SPLIT * 2 + 2)		// �V�����_�[�̍ő咸�_�E�C���f�b�N�X��
#define CHARGE_MAX_RADIUS					(40.0f)						// �ő唼�a
#define CHARGE_SPREAD_SPEED					(3.5f)						// ���a������	SPREAD �� �L����
#define CHARGE_TOPPART_SPREAD				(1.3f)						// �㕔�̕��������̍L����{��
#define CHARGE_HEIGHT_EXTEND				(2.0f)						// �������L�яk�݂���X�s�[�h
#define CHARGE_TEX_PASS				"data/TEXTURE/ChargeCylinder.png"	// �`���[�W�V�����_�[�̃e�N�X�`���p�X

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureChargeCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeCylinder = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffChargeCylinder = NULL;
D3DXMATRIX				g_mtxWorldChargeCylinder;	//���[���h�}�g���b�N�X
ChargeCylinder			g_ChargeCylinder[NUM_CHARGE_CYLINDER];

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̏���������
//--------------------------------------------------------------------------------------------------------
void InitChargeCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, CHARGE_TEX_PASS, &g_pTextureChargeCylinder);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * CHARGECYLINDER_ALL_VTX * NUM_CHARGE_CYLINDER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeCylinder, NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * CHARGECYLINDER_ALL_VTX * NUM_CHARGE_CYLINDER, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffChargeCylinder, NULL);

	//�`���[�W�V�����_�[�̏��̏�����
	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		g_ChargeCylinder[nCntCylinder].pos = ZERO_SET;
		g_ChargeCylinder[nCntCylinder].rot = ZERO_SET;
		g_ChargeCylinder[nCntCylinder].fRadius = 0.0f;
		g_ChargeCylinder[nCntCylinder].nPlayerNo = 0;
		g_ChargeCylinder[nCntCylinder].bUse = false;

		//���_���̐ݒ菈��
		SetChargeCylinderVertex(nCntCylinder);

		//�C���f�b�N�X�ԍ��̐ݒ菈��
		SetChargeCylinderIndex(nCntCylinder);
	}
}

//�f�ʂ̒��_���̐ݒ菈��
void SetChargeCylinderVertex(int nCntCylinder)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void* *)&pVtx, 0);

	//�|�C���^�[�����炷
	pVtx += CHARGECYLINDER_ALL_VTX * nCntCylinder;

	ChargeCylinder *pCylinder = &g_ChargeCylinder[nCntCylinder];
	float Rot = D3DX_PI;	//Y���̊p�x
	float LapSepRot = ONE_LAP / CHARGECYLINDER_SPLIT;		//�P���̕������������߂�
	float fGauge = GetPlayer()[pCylinder->nPlayerNo].moveGauge;		//�v���C���[�̃Q�[�W�ʂ��i�[
	float TexSep_U = CHARGECYLINDER_TEX_RESOLUTION / CHARGECYLINDER_SPLIT;//�e�N�X�`���̉��̕����̊��������߂�
	D3DXCOLOR col = D3DXCOLOR(1.0f * fGauge, 1.0f - fGauge * 0.3f, 1.0f - fGauge, 1.0f);//�v���C���[�̃Q�[�W�ʂɉ������O���f�[�V����

	//���_���W�̐ݒ�
	for (int nCntVtx = 0; nCntVtx <= CHARGECYLINDER_SPLIT; nCntVtx++)
	{
		float	VtxPos_X = sinf(Rot) * pCylinder->fRadius,	//�w���W
				VtxPos_Z = cosf(Rot) * pCylinder->fRadius;	//�y���W
		int		nNumTopVtx = CHARGECYLINDER_SPLIT + nCntVtx + 1;	//�Ώۂ̒��_�̏㕔�̒��_�ԍ�

		//���_�ʒu�Ɠ��������̒��_���W��ݒ�
		pVtx[nCntVtx].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//��Őݒ肵�����_���W�̐^��̒��_���W��ݒ�
		pVtx[nNumTopVtx].pos = D3DXVECTOR3(VtxPos_X * CHARGE_TOPPART_SPREAD, CHARGECYLINDER_HEIGHT, VtxPos_Z * CHARGE_TOPPART_SPREAD);

		//�P�������Ƃ��̒��_���W
		if (nCntVtx == CHARGECYLINDER_SPLIT)
		{
			pVtx[nCntVtx].pos = pVtx[0].pos;							//�ŏ��̒��_���W����
			pVtx[nNumTopVtx].pos = pVtx[CHARGECYLINDER_SPLIT + 1].pos;	//�ŏ��̏㕔�̒��_���W����
		}

		//�@���x�N�g���̐ݒ�
		pVtx[nCntVtx].nor = 
		pVtx[nNumTopVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = 
		pVtx[nNumTopVtx].col = col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 1.0f);
		pVtx[nNumTopVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 0.0f);

		//�p�x���@�S�̂̊p�x���������Ŋ������������A����
		Rot -= LapSepRot;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffChargeCylinder->Unlock();
}

//�f�ʂ̃C���f�b�N�X�ԍ��̐ݒ菈��
void SetChargeCylinderIndex(int nCntCylinder)
{
	WORD *pIdx;
	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffChargeCylinder->Lock(0, 0, (void* *)&pIdx, 0);

	pIdx += CHARGECYLINDER_ALL_VTX * nCntCylinder;			//�|�C���^�[�����炷
	int nBottomNumber = ODDPARITY + CHARGECYLINDER_SPLIT;	//�V�����_�[�����̊���_�ԍ�

	//�C���f�b�N�X�ԍ��f�[�^�̐ݒ�
	for (int nCntIdx = 0; nCntIdx < CHARGECYLINDER_ALL_VTX; nCntIdx++)
	{
		//�J�E���^�[�������̏ꍇ�A�㔼�ȍ~�̌v�Z�� �� �̓������[���ɂȂ�	��̏ꍇ�́A�V�����_�[�����̊���_�ԍ������Z�����
		pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + (nBottomNumber * (nCntIdx % EVENPARITY));
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffChargeCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̏I������
//--------------------------------------------------------------------------------------------------------
void UninitChargeCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureChargeCylinder != NULL)
	{
		g_pTextureChargeCylinder->Release();
		g_pTextureChargeCylinder = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffChargeCylinder != NULL)
	{
		g_pVtxBuffChargeCylinder->Release();
		g_pVtxBuffChargeCylinder = NULL;
	}

	//�C���f�b�N�X�̔j��
	if (g_pIdxBuffChargeCylinder != NULL)
	{
		g_pIdxBuffChargeCylinder->Release();
		g_pIdxBuffChargeCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̍X�V����
//--------------------------------------------------------------------------------------------------------
void UpdateChargeCylinder(void)
{
	//�V�����_�[�̃|�C���^���擾
	ChargeCylinder *pCylinder = &g_ChargeCylinder[0];

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++, pCylinder++)
	{
		//�`���[�W�V�����_�[���g���Ă���
		if (pCylinder->bUse == true)
		{
			//�v���C���[�̈ʒu�ɒu��
			SetChargeCylinderPos(pCylinder);

			//�V�����_�[�̍L���鏈��
			SpreadChargeCylinder(pCylinder);

			//�V�����_�[�̒��_���̐ݒ菈��
			SetChargeCylinderVertex(nCntCylinder);
		}
	}
}

// �v���C���[�̈ʒu�ɐݒ肷��
void SetChargeCylinderPos(ChargeCylinder *pCylinder)
{
	//�V�����_�[�̈ʒu���Ăяo�����v���C���[�Ɠ����ɂ���
	pCylinder->pos = GetPlayer()[pCylinder->nPlayerNo].pos;
}

// �V�����_�[�̍L���鏈��
void SpreadChargeCylinder(ChargeCylinder *pCylinder)
{
	//���a���L����
	pCylinder->fRadius += CHARGE_SPREAD_SPEED;

	//�ő唼�a�ɓ��B����
	if (pCylinder->fRadius >= CHARGE_MAX_RADIUS)
	{
		//�g��Ȃ�����
		pCylinder->bUse = false;
		pCylinder->fRadius = 0.0f;
	}
}

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̕`�揈��
//--------------------------------------------------------------------------------------------------------
void DrawChargeCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffChargeCylinder, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffChargeCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureChargeCylinder);

	//���ʃJ�����O��ON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//�V�����_�[���g���Ă���
		if (g_ChargeCylinder[nCntCylinder].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldChargeCylinder);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ChargeCylinder[nCntCylinder].rot.y, g_ChargeCylinder[nCntCylinder].rot.x, g_ChargeCylinder[nCntCylinder].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_ChargeCylinder[nCntCylinder].pos.x, g_ChargeCylinder[nCntCylinder].pos.y, g_ChargeCylinder[nCntCylinder].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldChargeCylinder);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, CHARGECYLINDER_ALL_VTX, 0, CHARGECYLINDER_ALL_VTX);
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//�ʏ�J�����O�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

// �V�����_�[�̐ݒ菈��
void SetChargeCylinder(D3DXVECTOR3 pos, int nPlayerNo)
{
	//�V�����_�[�̃|�C���^���擾
	ChargeCylinder *pCylinder = &g_ChargeCylinder[0];

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++, pCylinder++)
	{
		//�ΏۃV�����_�[���g���Ă��Ȃ�
		if (pCylinder->bUse == false)
		{
			pCylinder->pos = pos;				// �ʒu���f
			pCylinder->rot = ZERO_SET;			// ����������
			pCylinder->fRadius = 0.0f;			// ���a������
			pCylinder->nPlayerNo = nPlayerNo;	// �v���C���[�ԍ����L��
			pCylinder->bUse = true;				// �g�p����
			break;
		}
	}
}
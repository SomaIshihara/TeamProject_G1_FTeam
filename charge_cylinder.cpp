//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.h]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "charge_cylinder.h"
#include "color.h"

//�}�N����`
#define NUM_CHARGE_CYLINDER			(4)		//�V�����_�[��

//�f�ʏ��̍\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	D3DXMATRIX		mtxWorld;	//���[���h�}�g���b�N�X
	float			fRadius;	//���a�̑傫��
	bool			bUse;		//�g���Ă��邩�ǂ���
}ChargeCylinder;

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
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureChargeCylinder);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//�`���[�W�V�����_�[�̏�����
		g_ChargeCylinder[nCntCylinder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ChargeCylinder[nCntCylinder].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ChargeCylinder[nCntCylinder].bUse = false;

		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeCylinder, NULL);

		//���_���̐ݒ菈��
		SetChargeCylinderVertex(nCntCylinder);

		//�C���f�b�N�X�o�b�t�@�̐���
		pDevice->CreateIndexBuffer(sizeof(WORD) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffChargeCylinder, NULL);

		//�C���f�b�N�X�ԍ��̐ݒ菈��
		SetChargeCylinderIndex();
	}
}

//�f�ʂ̒��_���̐ݒ菈��
void SetChargeCylinderVertex(int nCntCylinder)
{
	//���a��ݒ�
	g_ChargeCylinder[nCntCylinder].fRadius = CHARGESYLINDER_WIDTH;

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void**)&pVtx, 0);

	float Rot = D3DX_PI;	//Y���̊p�x

							//���_���W�̐ݒ�
	for (int nCntChargeCylinder = 0; nCntChargeCylinder <= CHARGESYLINDER_SPLIT; nCntChargeCylinder++)
	{
		float	VtxPos_X = sinf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius,					//�w���W
			VtxPos_Z = cosf(Rot) * g_ChargeCylinder[nCntCylinder].fRadius;					//�y���W
		int		nNumBottomVtx = CHARGESYLINDER_SPLIT + nCntChargeCylinder + 1;	//�Ώۂ̒��_�̐^���̒��_�ԍ�

																		//���_�ʒu�Ɠ��������̒��_���W��ݒ�
		pVtx[nCntChargeCylinder].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//��Őݒ肵�����_���W�̐^���̒��_���W��ݒ�
		pVtx[nNumBottomVtx].pos = D3DXVECTOR3(VtxPos_X, CHARGESYLINDER_HEIGHT, VtxPos_Z);

		//�P�������Ƃ��̒��_���W
		if (nCntChargeCylinder == CHARGESYLINDER_SPLIT)
		{
			pVtx[nCntChargeCylinder].pos = pVtx[0].pos;						//�ŏ��̒��_���W����
			pVtx[nNumBottomVtx].pos = pVtx[CHARGESYLINDER_SPLIT + 1].pos;	//�ŏ��̑����̒��_���W����
		}

		//�@���x�N�g���̐ݒ�
		pVtx[nCntChargeCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nNumBottomVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntChargeCylinder].col = XCOL_WHITE;
		pVtx[nNumBottomVtx].col = XCOL_WHITE;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntChargeCylinder].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 0.0f);
		pVtx[nNumBottomVtx].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 1.0f);

		//�p�x���@�S�̂̊p�x���������Ŋ������������A����
		Rot -= ONE_LAP / CHARGESYLINDER_SPLIT;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffChargeCylinder->Unlock();
}

//�f�ʂ̃C���f�b�N�X�ԍ��̐ݒ菈��
void SetChargeCylinderIndex(void)
{
	WORD*pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffChargeCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntChargeCylinder = 0; nCntChargeCylinder < CHARGESYLINDER_SPLIT * 2 + 2; nCntChargeCylinder++)
	{
		if (nCntChargeCylinder % EVENPARITY == NOPARITY)
		{
			pIdx[nCntChargeCylinder] = nCntChargeCylinder / EVENPARITY;
		}
		if (nCntChargeCylinder % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntChargeCylinder] = (nCntChargeCylinder / EVENPARITY) + ODDPARITY + CHARGESYLINDER_SPLIT;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffChargeCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̏I������
//--------------------------------------------------------------------------------------------------------
void UninitChargeCylinder(void)
{
	//�C���f�b�N�X�̔j��
	if (g_pIdxBuffChargeCylinder != NULL)
	{
		g_pIdxBuffChargeCylinder->Release();
		g_pIdxBuffChargeCylinder = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureChargeCylinder != NULL)
	{
		g_pTextureChargeCylinder->Release();
		g_pTextureChargeCylinder = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffChargeCylinder != NULL)
	{
		g_pVtxBuffChargeCylinder->Release();
		g_pVtxBuffChargeCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̍X�V����
//--------------------------------------------------------------------------------------------------------
void UpdateChargeCylinder(void)
{

}

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̕`�揈��
//--------------------------------------------------------------------------------------------------------
void DrawChargeCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldChargeCylinder);

	for (int nCntCylinder = 0; nCntCylinder < NUM_CHARGE_CYLINDER; nCntCylinder++)
	{
		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ChargeCylinder[nCntCylinder].rot.y, g_ChargeCylinder[nCntCylinder].rot.x, g_ChargeCylinder[nCntCylinder].rot.z);
		D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_ChargeCylinder[nCntCylinder].pos.x, g_ChargeCylinder[nCntCylinder].pos.y, g_ChargeCylinder[nCntCylinder].pos.z);
		D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldChargeCylinder);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffChargeCylinder, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffChargeCylinder);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureChargeCylinder);

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (CHARGESYLINDER_SPLIT * 2 + 2), 0, (CHARGESYLINDER_SPLIT * 2 + 2));
	}
}
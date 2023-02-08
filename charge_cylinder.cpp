//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃`���[�W�V�����_�[[chargecylinder.h]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "charge_cylinder.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureChargeCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeCylinder = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffChargeCylinder = NULL;
D3DXVECTOR3				g_posChargeCylinder;
D3DXVECTOR3				g_rotChargeCylinder;
D3DXMATRIX				g_mtxWorldChargeCylinder;	//���[���h�}�g���b�N�X

//--------------------------------------------------------------------------------------------------------
//�`���[�W�V�����_�[�̏���������
//--------------------------------------------------------------------------------------------------------
void InitChargeCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureChargeCylinder);

	//�`���[�W�V�����_�[�̏�����
	g_posChargeCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotChargeCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeCylinder, NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffChargeCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntChargeCylinder = 0; nCntChargeCylinder < CHARGESYLINDER_SPLIT + 1; nCntChargeCylinder++)
	{
		pVtx[nCntChargeCylinder].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) * CHARGESYLINDER_WIDTH,
			CHARGESYLINDER_HEIGHT,
			cosf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) * CHARGESYLINDER_WIDTH);

		pVtx[CHARGESYLINDER_SPLIT + nCntChargeCylinder + 1].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) *CHARGESYLINDER_WIDTH,
			0.0f,
			cosf(D3DX_PI * (1.0f - (2.0f / CHARGESYLINDER_SPLIT * nCntChargeCylinder))) * CHARGESYLINDER_WIDTH);

		if (nCntChargeCylinder == CHARGESYLINDER_SPLIT)
		{
			pVtx[nCntChargeCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *CHARGESYLINDER_WIDTH,
				CHARGESYLINDER_HEIGHT,
				cosf(D3DX_PI * 1.0f) * CHARGESYLINDER_WIDTH);

			pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *CHARGESYLINDER_WIDTH,
				0.0f,
				cosf(D3DX_PI * 1.0f) * CHARGESYLINDER_WIDTH);
		}

		//�@���x�N�g���̐ݒ�
		pVtx[nCntChargeCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntChargeCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntChargeCylinder].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 0.0f);
		pVtx[CHARGESYLINDER_SPLIT + 1 + nCntChargeCylinder].tex = D3DXVECTOR2(nCntChargeCylinder * (CHARGESYLINDER_TEX_RESOLUTION / CHARGESYLINDER_SPLIT), 1.0f);
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffChargeCylinder->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (CHARGESYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffChargeCylinder, NULL);

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

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotChargeCylinder.y, g_rotChargeCylinder.x, g_rotChargeCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldChargeCylinder, &g_mtxWorldChargeCylinder, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posChargeCylinder.x, g_posChargeCylinder.y, g_posChargeCylinder.z);
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
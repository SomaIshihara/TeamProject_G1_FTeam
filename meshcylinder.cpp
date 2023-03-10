//--------------------------------------------------------------------------------------------------------
//
//���b�V���V�����_�[�̒n�ʂ̒f��[meshcylinder.cpp]
//Author:�e�n�z�l
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshcylinder.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;
D3DXVECTOR3 g_posMeshCylinder;
D3DXVECTOR3 g_rotMeshCylinder;
D3DXMATRIX g_mtxWorldMeshCylinder;	//���[���h�}�g���b�N�X

//--------------------------------------------------------------------------------------------------------
//�|���S���̏���������
//--------------------------------------------------------------------------------------------------------
void InitMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky002.png", &g_pTextureMeshCylinder);

	//�|���S���̏�����
	g_posMeshCylinder = 
	g_rotMeshCylinder = ZERO_SET;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHSYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshCylinder, NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MESHSYLINDER_SPLIT + 1; nCntMeshCylinder++)
	{
		pVtx[nCntMeshCylinder].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) *MESHSYLINDER_WIDTH,
			MESHSYLINDER_HEIGHT,
			cosf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) * MESHSYLINDER_WIDTH);

		pVtx[MESHSYLINDER_SPLIT + nCntMeshCylinder + 1].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) *MESHSYLINDER_WIDTH,
			0.0f,
			cosf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) * MESHSYLINDER_WIDTH);

		if (nCntMeshCylinder == MESHSYLINDER_SPLIT)
		{
			pVtx[nCntMeshCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *MESHSYLINDER_WIDTH,
				MESHSYLINDER_HEIGHT,
				cosf(D3DX_PI * 1.0f) * MESHSYLINDER_WIDTH);

			pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *MESHSYLINDER_WIDTH,
				0.0f,
				cosf(D3DX_PI * 1.0f) * MESHSYLINDER_WIDTH);
		}

		//�@���x�N�g���̐ݒ�
		pVtx[nCntMeshCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntMeshCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntMeshCylinder].tex = D3DXVECTOR2(nCntMeshCylinder * (MESHSYLINDER_TEX_RESOLUTION / MESHSYLINDER_SPLIT), 0.0f);
		pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].tex = D3DXVECTOR2(nCntMeshCylinder * (MESHSYLINDER_TEX_RESOLUTION / MESHSYLINDER_SPLIT), 1.0f);
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshCylinder->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MESHSYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshCylinder, NULL);

	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MESHSYLINDER_SPLIT * 2 + 2; nCntMeshCylinder++)
	{
		if (nCntMeshCylinder % EVENPARITY == NOPARITY)
		{
			pIdx[nCntMeshCylinder] = nCntMeshCylinder / EVENPARITY;
		}
		if (nCntMeshCylinder % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntMeshCylinder] = (nCntMeshCylinder / EVENPARITY) + ODDPARITY + MESHSYLINDER_SPLIT;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//�|���S���̏I������
//--------------------------------------------------------------------------------------------------------
void UninitMeshCylinder(void)
{
	//�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//�|���S���̍X�V����
//--------------------------------------------------------------------------------------------------------
void UpdateMeshCylinder(void)
{

}

//--------------------------------------------------------------------------------------------------------
//�|���S���̕`�揈��
//--------------------------------------------------------------------------------------------------------
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshCylinder);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshCylinder.y, g_rotMeshCylinder.x, g_rotMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshCylinder.x, g_posMeshCylinder.y, g_posMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCylinder);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (MESHSYLINDER_SPLIT * 2 + 2), 0, (MESHSYLINDER_SPLIT * 2 + 2));
}
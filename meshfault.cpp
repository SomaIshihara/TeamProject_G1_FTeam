//--------------------------------------------------------------------------------------------------------
//
//���b�V���̒f�ʐ}[meshfault.h]
//Author:�e�n�z�l	���V����
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshfault.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureMeshFault = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshFault = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshFault = NULL;
D3DXVECTOR3				g_posMeshFault;
D3DXVECTOR3				g_rotMeshFault;
D3DXMATRIX				g_mtxWorldMeshFault;	//���[���h�}�g���b�N�X

//--------------------------------------------------------------------------------------------------------
//�f�ʂ̏���������
//--------------------------------------------------------------------------------------------------------
void InitMeshFault(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureMeshFault);

	//�|���S���̏�����
	g_posMeshFault = ZERO_SET;
	g_rotMeshFault = ZERO_SET;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshFault, NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMeshFault->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntMeshFault = 0; nCntMeshFault < MESHFAULT_SPLIT + 1; nCntMeshFault++)
	{
		float	VtxPos_X = sinf(D3DX_PI * (1.0f - (2.0f / MESHFAULT_SPLIT * nCntMeshFault))) * MESHFAULT_WIDTH,
				VtxPos_Z = cosf(D3DX_PI * (1.0f - (2.0f / MESHFAULT_SPLIT * nCntMeshFault))) * MESHFAULT_WIDTH;

		//���_�ʒu�Ɠ��������̒��_���W��ݒ�
		pVtx[nCntMeshFault].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//��Őݒ肵�����_���W�̐^���̒��_���W��ݒ�
		pVtx[MESHFAULT_SPLIT + nCntMeshFault + 1].pos = D3DXVECTOR3(VtxPos_X, MESHFAULT_BOTTOM, VtxPos_Z);

		if (nCntMeshFault == MESHFAULT_SPLIT)
		{
			pVtx[nCntMeshFault].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH,
				MESHFAULT_BOTTOM,
				cosf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH);

			pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH,
				0.0f,
				cosf(D3DX_PI * 1.0f) * MESHFAULT_WIDTH);
		}

		//�@���x�N�g���̐ݒ�
		pVtx[nCntMeshFault].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntMeshFault].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntMeshFault].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 0.0f);
		pVtx[MESHFAULT_SPLIT + 1 + nCntMeshFault].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 1.0f);
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshFault->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshFault, NULL);

	WORD*pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffMeshFault->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntMeshFault = 0; nCntMeshFault < MESHFAULT_SPLIT * 2 + 2; nCntMeshFault++)
	{
		if (nCntMeshFault % EVENPARITY == NOPARITY)
		{
			pIdx[nCntMeshFault] = nCntMeshFault / EVENPARITY;
		}
		if (nCntMeshFault % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntMeshFault] = (nCntMeshFault / EVENPARITY) + ODDPARITY + MESHFAULT_SPLIT;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshFault->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//�f�ʂ̏I������
//--------------------------------------------------------------------------------------------------------
void UninitMeshFault(void)
{
	//�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshFault != NULL)
	{
		g_pIdxBuffMeshFault->Release();
		g_pIdxBuffMeshFault = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureMeshFault != NULL)
	{
		g_pTextureMeshFault->Release();
		g_pTextureMeshFault = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffMeshFault != NULL)
	{
		g_pVtxBuffMeshFault->Release();
		g_pVtxBuffMeshFault = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//�f�ʂ̍X�V����
//--------------------------------------------------------------------------------------------------------
void UpdateMeshFault(void)
{

}

//--------------------------------------------------------------------------------------------------------
//�f�ʂ̕`�揈��
//--------------------------------------------------------------------------------------------------------
void DrawMeshFault(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshFault);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshFault.y, g_rotMeshFault.x, g_rotMeshFault.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshFault, &g_mtxWorldMeshFault, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshFault.x, g_posMeshFault.y, g_posMeshFault.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshFault, &g_mtxWorldMeshFault, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshFault);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshFault, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshFault);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshFault);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (MESHFAULT_SPLIT * 2 + 2), 0, (MESHFAULT_SPLIT * 2 + 2));
}
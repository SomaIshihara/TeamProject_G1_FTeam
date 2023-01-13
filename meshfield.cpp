//==========================================
//
//���b�V���t�B�[���h�v���O����[meshfield.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "player.h"
#include "meshfield.h"

//�}�N����`       �t�@�C���ǂݍ��݂ɕς���\������
#define BESIDE					(2)						//���̕�����
#define VERTICAL				(2)						//�c�̕�����
#define BESIDE_LENGTH			(250.0f)				//���̒���
#define VERTICAL_LENGTH			(250.0f)				//�c�̒���
#define MAX_FIELD				(1)					//���b�V���t�B�[���h�ő吔

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureMeshfield = NULL;			//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXMATRIX g_mtxWorldMeshfield;							//���[���h�}�g���b�N�X

MESHFIELD g_MeshField[MAX_FIELD];

//====================================================================
//���b�V���t�B�[���h�̏���������
//====================================================================
void InitMeshfield(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾	

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	WORD *pIdx;					//�C���f�b�N�X���ւ̃|�C���^


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field001.jpg",
		&g_pTextureMeshfield);

	//�\���̂�������
	for (int nCntfield = 0; nCntfield < MAX_FIELD; nCntfield++)
	{
		g_MeshField[nCntfield].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshField[nCntfield].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshField[nCntfield].fLength_X = BESIDE_LENGTH;
		g_MeshField[nCntfield].fLength_Z = VERTICAL_LENGTH;
		g_MeshField[nCntfield].nBlock_X = BESIDE;
		g_MeshField[nCntfield].nBlock_Z = VERTICAL;
	}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_MeshField[0].nBlock_X + 1)*(g_MeshField[0].nBlock_Z + 1),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield, NULL);


	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_MeshField[0].nBlock_X + 1) * 2 * g_MeshField[0].nBlock_Z +
		(g_MeshField[0].nBlock_Z - 1) * 2),
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField, NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntVtxZ = 0; nCntVtxZ < g_MeshField[0].nBlock_X + 1; nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < g_MeshField[0].nBlock_Z + 1; nCntVtxX++)
		{
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].pos
				= D3DXVECTOR3(
					(nCntVtxX - g_MeshField[0].nBlock_X / 2) * g_MeshField[0].fLength_X,
					0.0f,
					(nCntVtxZ - g_MeshField[0].nBlock_Z / 2) * g_MeshField[0].fLength_Z);

			//�@���̐ݒ�
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����_���W�̐ݒ�
			pVtx[nCntVtxZ*(g_MeshField[0].nBlock_X + 1) + nCntVtxX].tex = D3DXVECTOR2(1.0f*nCntVtxX, 1.0f*nCntVtxZ);
		}
	}

	g_pVtxBuffMeshfield->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�

	int nCntIdx = 0;

	for (int nCntDepth = 0; nCntDepth < g_MeshField[0].nBlock_X; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < g_MeshField[0].nBlock_Z + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((g_MeshField[0].nBlock_Z + 1) * (nCntDepth + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((g_MeshField[0].nBlock_Z + 1) * nCntDepth);
			nCntIdx += 2;
		}
		if (nCntDepth < g_MeshField[0].nBlock_X - 1)
		{
			pIdx[nCntIdx] = ((g_MeshField[0].nBlock_Z + 1) * (nCntDepth + 1)) - 1;
			pIdx[nCntIdx + 1] = (g_MeshField[0].nBlock_Z + 1) * (nCntDepth + 2);
			nCntIdx += 2;
		}
	}

	g_pIdxBuffMeshField->Unlock();
}

//====================================================================
//���b�V���t�B�[���h�̏I������
//====================================================================
void UninitMeshfield(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//====================================================================
//���b�V���t�B�[���h�̍X�V����
//====================================================================
void UpdateMeshfield(void)
{

}

//====================================================================
//���b�V���t�B�[���h�̕`�揈��
//====================================================================
void DrawMeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	for (int nCntfield = 0; nCntfield < MAX_FIELD; nCntfield++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldMeshfield);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField[nCntfield].rot.y, g_MeshField[nCntfield].rot.x, g_MeshField[nCntfield].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_MeshField[nCntfield].pos.x, g_MeshField[nCntfield].pos.y, g_MeshField[nCntfield].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshField);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMeshfield);

		//�`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
			(g_MeshField[0].nBlock_X + 1)*(g_MeshField[0].nBlock_Z + 1),
			0,
			g_MeshField[0].nBlock_X*g_MeshField[0].nBlock_Z * 2 + (g_MeshField[0].nBlock_Z - 1) * 4);
	}
}
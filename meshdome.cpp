//--------------------------------------------------------------------------------------------------------
//
//���b�V���h�[���̉����E�ޗ�[meshdome.cpp]
//Author:���V����
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshdome.h"
#include "color.h"

//�}�N��
#define NUM_MESHDOME				(1)			//���b�V���h�[���̐�
#define MESHDOME_WIDTH				(2000.0f)	//�w�i�̍L��
#define MESHDOME_HEIGHT				(1000.0f)	//�w�i�̍���
#define MESHDOME_TEX_RESOLUTION		(3.0f)		//�w�i�̉𑜓x
#define MESHDOME_SPLIT				(64)		//�w�i�̒��_��
#define MESHDOME_SEPALATE			(2)			//�w�i�̏c�̕�����
#define MESHDOME_NUM_OVERLAP		(2)			//�ŏ��ƍŌ�̒��_���d�Ȃ鐔
//#define MESHDOME_ALL_VERTEX			(MESHDOME_SPLIT * MESHDOME_SEPALATE + MESHDOME_NUM_OVERLAP)	//�S�̂̒��_��  �{���̃}�N��
#define MESHDOME_ALL_VERTEX			(65)	//�S�̂̒��_��		�����p�̃}�N��

//���b�V���h�[���̍\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	float			fRadius;	//���a
}MeshDome;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureMeshDome = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshDome = NULL;
D3DXMATRIX				g_mtxWorldMeshDome;
MeshDome				g_MeshDome;

//--------------------------------------------------------------------------------------------------------
//���b�V���h�[���̏���������
//--------------------------------------------------------------------------------------------------------
void InitMeshDome(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�ݒ�p�����x�N�g��
	D3DXVECTOR3 aVecDir;

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureMeshDome);

	//�h�[�����̏�����
	g_MeshDome.pos = ZERO_SET;
	g_MeshDome.rot = ZERO_SET;
	g_MeshDome.fRadius = MESHDOME_WIDTH;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshDome, NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	/*for (int nCntMeshDome = 0; nCntMeshDome < NUM_MESHDOME; nCntMeshDome++, pVtx += MESHDOME_ALL_VERTEX)
	{
		float fAngle = D3DX_PI;	//�p�x

		//���_���W�̐ݒ�
		for (int nCntVtx = 0; nCntVtx <= MESHDOME_SPLIT; nCntVtx++)
		{
			//�Ώۂ̒��_���W�ԍ��ƁA�����ȊO���������������_�ԍ�
			int nDissimilar = MESHDOME_SPLIT + nCntVtx + 1;

			pVtx[nCntVtx].pos = D3DXVECTOR3(
				sinf(fAngle) * g_MeshDome.fRadius,
				MESHDOME_HEIGHT,
				cosf(fAngle) * g_MeshDome.fRadius);

			pVtx[nDissimilar].pos = D3DXVECTOR3(
				sinf(fAngle) * g_MeshDome.fRadius,
				0.0f,
				cosf(fAngle) * g_MeshDome.fRadius);

			//�Ō�̒��_�́A�ŏ��̒��_�Ɠ����ɂ���
			if (nCntVtx == MESHDOME_SPLIT)
			{
				pVtx[nCntVtx].pos = D3DXVECTOR3(
					sinf(D3DX_PI) * g_MeshDome.fRadius,
					MESHDOME_HEIGHT,
					cosf(D3DX_PI) * g_MeshDome.fRadius);

				pVtx[nDissimilar].pos = D3DXVECTOR3(
					sinf(D3DX_PI) * g_MeshDome.fRadius,
					0.0f,
					cosf(D3DX_PI) * g_MeshDome.fRadius);
			}

			//�@���x�N�g���̐ݒ�
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nDissimilar].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCntVtx].col = XCOL_WHITE;
			pVtx[nDissimilar].col = XCOL_WHITE;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * (MESHDOME_TEX_RESOLUTION / MESHDOME_SPLIT), 0.0f);
			pVtx[nDissimilar].tex = D3DXVECTOR2(nCntVtx * (MESHDOME_TEX_RESOLUTION / MESHDOME_SPLIT), 1.0f);

			fAngle -= (D3DX_PI * 2.0f) / MESHDOME_SPLIT;
		}
	}*/

	int nNumVtx = 0;		//���_�ԍ�
	//�V�ʂ̏o�����蒸�_�̐ݒ�
	pVtx[nNumVtx++].pos.x = g_MeshDome.pos.x;
	pVtx[nNumVtx++].pos.y = g_MeshDome.pos.y + MESHDOME_HEIGHT;
	pVtx[nNumVtx++].pos.z = g_MeshDome.pos.z;

	//���̕������@�]�@�V�ʂ̏o������̂P���_�@�񐔕� for������
	for (int nCntDevideY = 0; nCntDevideY < MESHDOME_SPLIT - 1; nCntDevideY++)
	{

	}

	//�x�N�g���𐳋K������
	D3DXVec3Normalize(&aVecDir, &aVecDir);

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshDome->Unlock();

	//--------------------------------------
	//		�C���f�b�N�X�o�b�t�@�̐���
	//--------------------------------------
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshDome, NULL);

	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�
	/*for (int nCntIdx = 0; nCntIdx < MESHDOME_ALL_VERTEX; nCntIdx++)
	{
		if (nCntIdx % EVENPARITY == NOPARITY)
		{
			pIdx[nCntIdx] = nCntIdx / EVENPARITY;
		}
		if (nCntIdx % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + ODDPARITY + MESHDOME_SPLIT;
		}
	}*/
	
	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshDome->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//���b�V���h�[���̏I������
//--------------------------------------------------------------------------------------------------------
void UninitMeshDome(void)
{
	//�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshDome != NULL)
	{
		g_pIdxBuffMeshDome->Release();
		g_pIdxBuffMeshDome = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//�o�b�t�@�̔j��
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//���b�V���h�[���̍X�V����
//--------------------------------------------------------------------------------------------------------
void UpdateMeshDome(void)
{

}

//--------------------------------------------------------------------------------------------------------
//���b�V���h�[���̕`�揈��
//--------------------------------------------------------------------------------------------------------
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshDome);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshDome);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshDome);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHDOME_ALL_VERTEX, 0, MESHDOME_ALL_VERTEX);
}
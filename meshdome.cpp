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
#define NUM_MESHDOME				(1)					//���b�V���h�[���̐�
#define MESHDOME_RADIS				(200.0f)			//�w�i�̔��a
#define MESHDOME_TEX_RESOLUTION		(3.0f)				//�w�i�̉𑜓x
#define MESHDOME_SPLIT				(8)				//�w�i�̒��_��
#define MESHDOME_SEPALATE			(8)					//�w�i�̏c�̕�����
#define MESHDOME_NUM_OVERLAP		(2)					//�ŏ��ƍŌ�̒��_���d�Ȃ鐔
#define MESHDOME_ALL_VERTEX			((MESHDOME_SPLIT * (MESHDOME_SEPALATE - 1)) + MESHDOME_NUM_OVERLAP)	//�S�̂̒��_��
#define CREATE_SQUARE_NEED_TRIANGLE	(2)					//�l�p�`�����̂ɕK�v�ȎO�p�`�̐�
#define TRIANGLE					(3)					//�R�p�`�ɕK�v�Ȓ��_
#define SQUARE_NUM_VERTEX			(6)					//�l�p�`�̒��Ɋ܂܂�钸�_�̐�(�d�����܂�)
#define ONE_LAP						(D3DX_PI * 2.0f)	//�P�����̊p�x
#define MESHDOME_ALL_VERTEX			(19)				//�S�̂̒��_��
#define MESHDOME_ALL_INDEX			(19)				//�S�̂̃C���f�b�N�X��

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

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky001.png", &g_pTextureMeshDome);

	//�h�[�����̏�����
	g_MeshDome.pos = ZERO_SET;
	g_MeshDome.rot = ZERO_SET;
	g_MeshDome.fRadius = MESHDOME_WIDTH;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshDome, NULL);

	//--------------------------------------
	//			���_���̐ݒ�
	//--------------------------------------
	SetMeshDomeVertexBuffer();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_ALL_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshDome, NULL);

	//--------------------------------------
	//		�C���f�b�N�X�o�b�t�@�̐���
	//--------------------------------------
	SetMeshDomeIndexBuffer();
}

//���_���W�̐ݒ�
void SetMeshDomeVertexBuffer(void)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	int nNumVtx = 0;			//���_�ԍ�
	float yRadian = 0.0f;		//�c�ŕ����������̉��P������Y���W�����܂�p�x
	D3DXVECTOR3 pos[MESHDOME_ALL_VERTEX];
	
	//�V�ʂ̏o�����蒸�_�̐ݒ�
	pos[nNumVtx] = D3DXVECTOR3(g_MeshDome.pos.x, g_MeshDome.pos.y + MESHDOME_RADIS, g_MeshDome.pos.z);
	pVtx[nNumVtx].pos = D3DXVECTOR3(g_MeshDome.pos.x, g_MeshDome.pos.y + MESHDOME_RADIS, g_MeshDome.pos.z);
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
	nNumVtx++;
	
	//�Q�Ԗڂ̒��_����A���̕������@�]�@��ʂ̏o������̂P���_�@�񐔕� for������
	for (int nCntDevideY = nNumVtx; nCntDevideY < 3 ; nCntDevideY++)
	{
		int nStrage = nNumVtx;

		//---------------------------------------------------------------------------------------------------------
		//	MEMO�F�c�ɕ������鐔���g���Č��_���牡�P�����̍����܂ł̊p�x�v�Z (�������ŕ����_��������̂ŁA3.14
		//---------------------------------------------------------------------------------------------------------
		yRadian += D3DX_PI / MESHDOME_SEPALATE;

		float TempLen = fabsf(sinf(yRadian) * g_MeshDome.fRadius);	//X�EZ�̔��a
		float rot_Y = D3DX_PI;										//Y���̊p�x
		float Height_Y = cosf(yRadian) * g_MeshDome.fRadius;		//Y�̍���

		for (int nCntDevideX = 0; nCntDevideX < MESHDOME_SPLIT; nCntDevideX++)
		{//���P�����̒��_���W��ݒ�

			pos[nNumVtx] = D3DXVECTOR3(
				sinf(rot_Y) * TempLen,		//X�̈ʒu
				Height_Y,					//Y�̈ʒu
				cosf(rot_Y) * TempLen);		//Z�̈ʒu

			pVtx[nNumVtx].pos = g_MeshDome.pos + pos[nNumVtx];

			pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nNumVtx].col = XCOL_WHITE;
			pVtx[nNumVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
			nNumVtx++;

			//�p�x���@�S�̂̊p�x���������Ŋ������������A����
			rot_Y -= ONE_LAP / MESHDOME_SPLIT;
		}

		pVtx[nNumVtx].pos = pVtx[nStrage].pos;
		pVtx[nNumVtx].nor = pVtx[nStrage].nor;
		pVtx[nNumVtx].col = pVtx[nStrage].col;
		pVtx[nNumVtx].tex = pVtx[nStrage].tex;
		nNumVtx++;
	}


	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshDome->Unlock();
}

//�C���f�b�N�X�ԍ��̐ݒ�
void SetMeshDomeIndexBuffer(void)
{
	WORD *pIdx;
	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	int nNumIdx = 0;		/*�C���f�b�N�X�ԍ�*/

	int offsetIndex = 0;	/*�V�ʂ̏o������̊O�ɍL���钸�_�̊�ԍ��i��{�I�ɒ��S���_�̎��Ɏw�肳��钸�_�ԍ��j*/

	//			�^�_
	//		  �^�V�ʁ_
	//		�^�ԍ��ݒ�_
	for (nNumIdx = 0; nNumIdx < 10; nNumIdx++)
	{
		pIdx[nNumIdx] = nNumIdx;
	}



	//			�^�_
	//		  �^�V�ʁ_
	//		�^�ԍ��ݒ�_			MESHDOME_SEPALATE * TRIANGLE	���������̎O�p�`���쐬���邽�߂ɁA�O�p�`�ɕK�v�Ȓ��_��3����Z����
	/*for (int nCntIdx = 0; nCntIdx < MESHDOME_SEPALATE * TRIANGLE; nCntIdx++)
	{
		switch (nCntIdx % 3)
		{
		case 0:		//�V�ʂ̂ł��ς�ԍ�
			pIdx[nNumIdx++] = 0;
			break;

		case 1:		//�Q�Ԗڂ̒��_�ԍ�
			pIdx[nNumIdx++] = 1 + offsetIndex;
			break;

		case 2:		//�Ō�̒��_�ԍ�
			int LapIndex = 2 + offsetIndex++;

			// �C���f�b�N�X�ԍ����P�������Ƃ��A����3�����Z�q���@True�ƂȂ�A�P��Ԃ�
			LapIndex = MESHDOME_SEPALATE < LapIndex ? pIdx[1] : LapIndex;
			
			//�R�p�`�Ō�̃C���f�b�N�X�ԍ�����
			pIdx[nNumIdx++] = LapIndex;
			break;
		}
	}*/
	
	//	�^----------------�_
	// <	���ʔԍ��ݒ�	>
	//	�_----------------�^
	int StartIndex = pIdx[1];	/*�J�nIndex�ԍ�*/
	int SideIndexLen = MESHDOME_SEPALATE * (MESHDOME_SPLIT - MESHDOME_NUM_OVERLAP) * 2 * TRIANGLE;	/*�V�ʁE��ʂ�������Index�v�f��*/
	int LapDiv = MESHDOME_SEPALATE * 2 * TRIANGLE;	/*�P�����K�v��Index��*/
	int LapLastIndex = 0,							/*���[�v���g�p����Ō��Index*/
		LapBoobyIndex = 0;							/*���[�v���g�p����Ōォ��2�Ԗڂ�Index*/
	int CreateSquareCnt = 0;						/*�쐬�����l�p�`�̐�*/

	for (int nCntSideIdx = 0; nCntSideIdx < SideIndexLen; nCntSideIdx++)
	{
		// ����̒��_���𒴂�����X�V(������܂�)
		if (nCntSideIdx % LapDiv == 0)
		{
			LapLastIndex = StartIndex;
			LapBoobyIndex = StartIndex + MESHDOME_SEPALATE;
			CreateSquareCnt++;
		}

		switch (nCntSideIdx % 6)
		{
		case 0:
		case 3:
		{
			pIdx[nNumIdx++] = StartIndex;
		}
		break;

		case 1:
		{
			pIdx[nNumIdx++] = StartIndex + MESHDOME_SEPALATE;
		}
		break;

		case 2:
		case 4:
		{
			if (nCntSideIdx > 0
				&& (nCntSideIdx % (LapDiv * CreateSquareCnt - 2) == 0 || nCntSideIdx % (LapDiv * CreateSquareCnt - 4) == 0))
			{
				// 1�������Ƃ��̃��[�v����
				// ����|���S���̍Ōォ��2�Ԗڂ�Index
				pIdx[nNumIdx++] = LapBoobyIndex;
			}
			else
			{
				pIdx[nNumIdx++] = StartIndex + MESHDOME_SEPALATE + 1;
			}
		}
		break;

		case 5:
		{
			if (0 < nCntSideIdx && nCntSideIdx % (LapDiv * CreateSquareCnt - 1) == 0)
			{
				// 1�������Ƃ��̃��[�v����
				// ����|���S���̍Ō��Index
				pIdx[nNumIdx++] = LapLastIndex;
			}
			else
			{
				pIdx[nNumIdx++] = StartIndex + 1;
			}

			// �J�nIndex�̍X�V 
			StartIndex++;
		}
		break;
		}
	}

	//		�_��ʔԍ��^
	//		  �_�ݒ�^
	//			�_�^
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		pIdx[nCnt] = nCnt;
	}


	//�Ō�̒��_�ԍ��i��ʂ̐撸�_)
	offsetIndex = MESHDOME_ALL_VERTEX - 1;
	int LoopIndex = offsetIndex;

	/*for (int i = MESHDOME_SEPALATE * TRIANGLE - 1; i >= 0; i--)
	{
		if (i % 3 == 0)
		{
			// ��ʂ̐撸�_  
			pIdx[nNumIdx++] = MESHDOME_ALL_VERTEX - 1;
			offsetIndex++;
		}
		else if (i % 3 == 1)
		{
			pIdx[nNumIdx++] = offsetIndex;
		}
		else if (i % 3 == 2)
		{
			//��ʂ̐撸�_�̎��ɑ������_�ԍ����A�撸�_�̔ԍ��ȏ�ɂȂ�����A��ʂ����Ƃ��ɐ錾�������[�v�ԍ��ɒu�������āA1������������
			pIdx[nNumIdx++] = MESHDOME_ALL_VERTEX - 1 <= 1 + offsetIndex ? MESHDOME_ALL_VERTEX - 1 : 1 + offsetIndex;
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

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, 10, 0, 8);		//�t�^�`��
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 16, MESHDOME_ALL_VERTEX + 1, 16);	//���ʕ`��
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHDOME_ALL_VERTEX, 0, MESHDOME_ALL_VERTEX);
	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
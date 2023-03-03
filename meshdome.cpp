//--------------------------------------------------------------------------------------------------------
//
//���b�V���h�[���̉����E�ޗ�[meshdome.cpp]
//Author:���V����
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshdome.h"
#include "color.h"
#include "input.h"

//�}�N��
#define NUM_MESHDOME				(1)						//���b�V���h�[���̐�
#define MESHDOME_TEX_WIDTH			(1.0f)					//�w�i�̉𑜓x
#define MESHDOME_TEX_HEIGHT			(1.0f)					//�w�i�̉𑜓x
#define MESHDOME_SPLIT				(16)					//�w�i�̉��̕�����		���͐����Ɉ����āA���E�ɕ�����
#define MESHDOME_SEPALATE			(16)					//�w�i�̏c�̕�����		���͐����Ɉ����āA�㉺�ɕ�����
#define MESHDOME_NUM_OVERLAP		(2)						//�ŏ��ƍŌ�̒��_���d�Ȃ鐔

//���_�E�C���f�b�N�X�̐��@�}�N��
#define MESHDOME_ALL_VERTEX			(MESHDOME_SPLIT * (MESHDOME_SEPALATE - 1) + MESHDOME_NUM_OVERLAP + (MESHDOME_SEPALATE - 1))	//�S�̂̒��_��
#define MESHDOME_COVER_INDEX		(MESHDOME_SPLIT + MESHDOME_NUM_OVERLAP)					//�t�^�̃C���f�b�N�X��
#define MESHDOME_SIDE_INDEX			((MESHDOME_SPLIT + 1) * (MESHDOME_SEPALATE - 2) * 2)	//���ʂ̃C���f�b�N�X��  1������̂ɕK�v�Ȑ� �~ ���ʂ����̃C���f�b�N�X �~ �y�A�ŃC���f�b�N�X��ݒ肷�鐔�i�Q�j

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
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffCoverMeshDome = NULL,		//�t�^�̃C���f�b�N�X�ԍ�
						g_pIdxBuffSideMeshDome = NULL,		//���ʂ̃C���f�b�N�X�ԍ�
						g_pIdxBuffBottomMeshDome = NULL;	//��ʂ̃C���f�b�N�X�ԍ�
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
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky000.png", &g_pTextureMeshDome);

	//�h�[�����̏�����
	g_MeshDome.pos = ZERO_SET;
	g_MeshDome.rot = ZERO_SET;
	g_MeshDome.fRadius = MESHDOME_RADIUS;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshDome, NULL);

	//--------------------------------------
	//			���_���̐ݒ�
	//--------------------------------------
	SetMeshDomeVertexBuffer();

	//--------------------------------------
	//		�C���f�b�N�X�o�b�t�@�̐���
	//--------------------------------------
	//�t�^�̃C���f�b�N�X�o�b�t�@
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_COVER_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffCoverMeshDome, NULL);
	
	//���ʂ̃C���f�b�N�X�o�b�t�@
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_SIDE_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffSideMeshDome, NULL);

	//��ʂ̃C���f�b�N�X�o�b�t�@
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_COVER_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffBottomMeshDome, NULL);

	//--------------------------------------
	//		�C���f�b�N�X�o�b�t�@�̐ݒ�
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
	
	//�V�ʂ̏o�����蒸�_�̐ݒ�
	pVtx[nNumVtx].pos = D3DXVECTOR3(0.0f, MESHDOME_RADIUS, 0.0f);
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
	nNumVtx++;
	
	//�Q�Ԗڂ̒��_����A���̕������@�]�@��ʂ̏o������̂P���_�@�񐔕� for������
	for (int nCntDevideY = nNumVtx; nCntDevideY < MESHDOME_SEPALATE; nCntDevideY++)
	{
		//---------------------------------------------------------------------------------------------------------
		//	MEMO�F�c�ɕ������鐔���g���Č��_���牡�P�����̍����܂ł̊p�x�v�Z (�������ŕ����_��������̂ŁA3.14
		//---------------------------------------------------------------------------------------------------------
		yRadian += D3DX_PI / MESHDOME_SEPALATE;

		float TempLen = fabsf(sinf(yRadian) * g_MeshDome.fRadius);	//X�EZ�̔��a
		float rot_Y = D3DX_PI;										//Y���̊p�x
		float Height_Y = cosf(yRadian) * g_MeshDome.fRadius;		//Y�̍���
		float aTexV = (MESHDOME_TEX_HEIGHT / (MESHDOME_SEPALATE + 2)) * nCntDevideY;	//�e�N�X�`���̍���

		int nStrageVtx = nNumVtx;		//�P������Ƃ��Ɏg�p���邽�߁A�P���̊J�n�ԍ����i�[

		for (int nCntDevideX = 0; nCntDevideX < MESHDOME_SPLIT; nCntDevideX++)
		{//���P�����̒��_���W��ݒ�
			
			pVtx[nNumVtx].pos = 
				D3DXVECTOR3(
				sinf(rot_Y) * TempLen,		//X�̈ʒu
				Height_Y,					//Y�̈ʒu
				cosf(rot_Y) * TempLen);		//Z�̈ʒu

			pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nNumVtx].col = XCOL_WHITE;

			float aTexU = (MESHDOME_TEX_WIDTH / (MESHDOME_SPLIT + 1)) * nCntDevideX;		//�e�N�X�`���̕�

			pVtx[nNumVtx++].tex = D3DXVECTOR2(aTexU, aTexV);
			
			//�p�x���@�S�̂̊p�x���������Ŋ������������A����
			rot_Y -= ONE_LAP / MESHDOME_SPLIT;
		}

		//�P�������邽�߁A�P���̊J�n���_������
		pVtx[nNumVtx].pos = pVtx[nStrageVtx].pos;
		pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nNumVtx].col = XCOL_WHITE;
		pVtx[nNumVtx++].tex = D3DXVECTOR2(1.0f, aTexV);
	}

	//��ʂ̏o�����蒸�_�̐ݒ�
	pVtx[nNumVtx].pos = D3DXVECTOR3(0.0f, -MESHDOME_RADIUS, 0.0f);
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.5f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshDome->Unlock();
}

//�C���f�b�N�X�ԍ��̐ݒ�
void SetMeshDomeIndexBuffer(void)
{
	WORD *pIdx;		//�C���f�b�N�X�̃|�C���^

	//			�^�_
	//		  �^�V�ʁ_		//�u- 1�v�͓V�ʂ̏o���������������
	//		�^�ԍ��ݒ�_	�V��̂ł��ς�����������J�E���^�[����
		//�t�^�̃C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffCoverMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�ԍ�
	int nNumIdx = 0;

	for (nNumIdx = 0; nNumIdx <= MESHDOME_COVER_INDEX - 1; nNumIdx++)
	{
		//���̌v�Z���ł́A�@���傤��1������Ƃ��ɓ�����1�ɂȂ�@�@����ȊO�ł́A�J�E���^�[���������@�̗]�肪�ԍ��Ƃ��Đݒ肳���
		//pIdx[nNumIdx] = (nNumIdx / (MESHDOME_COVER_INDEX - 1)) + (nNumIdx % (MESHDOME_COVER_INDEX - 1));
		pIdx[nNumIdx] = nNumIdx;
	}

	//�V�ʂ̃C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffCoverMeshDome->Unlock();

	//	�^----------------�_
	// <	���ʔԍ��ݒ�	>
	//	�_----------------�^
	//�C���f�b�N�X�ԍ�������
	nNumIdx = 0;

	//���ʂ̃C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffSideMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntHeight = 1; nCntHeight < MESHDOME_SEPALATE - 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < MESHDOME_SPLIT + 1; nCntWidth++)
		{
			int  Over = (nCntWidth + 1) + ((nCntHeight - 1) * (MESHDOME_SPLIT + 1)),
				Under = (nCntWidth + 1) + ((nCntHeight - 0) * (MESHDOME_SPLIT + 1));

			pIdx[nNumIdx++] = (nCntWidth + 1) + ((nCntHeight - 1) * (MESHDOME_SPLIT + 1));
			pIdx[nNumIdx++] = (nCntWidth + 1) + ((nCntHeight - 0) * (MESHDOME_SPLIT + 1));
		}
	}

	//���ʂ̃C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffSideMeshDome->Unlock();

	//		�_��ʔԍ��^
	//		  �_�ݒ�^
	//			�_�^
	//�t�^�̃C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffBottomMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	nNumIdx = 0;								//�C���f�b�N�X�ԍ�������
	int nBottomLap = MESHDOME_COVER_INDEX - 1;	//��ʂ̂P�����̐�
	int BottomVtx = MESHDOME_ALL_VERTEX - 1;	//��ʔԍ�

	for (int nCntIdx = 0; nCntIdx <= MESHDOME_COVER_INDEX; nCntIdx++)
	{
		//--------------------------------------------------
		//�S�̂̒��_������t�Z���Ĕԍ�������o��
		//
		//MEMO : �J�E���^�[���[���̏ꍇ		=>	��ʔԍ�����������
		//		 �J�E���^�[���P�E�Ō�̏ꍇ	=>	�����ԍ����������� (���R�́A�Ō�̏ꍇ�@�v�Z���̓������@�u�P ���܂� �O�v �ɂȂ邽��
		//		 ����ȊO�̏ꍇ				=>	�u��ʂ̔ԍ� - (�J�E���^�[ �� ��ʂ̂P�����̐�  �̗]��)�v  �ŎZ�o���ꂽ�ԍ�����������
		//--------------------------------------------------
		pIdx[nNumIdx++] = BottomVtx - nCntIdx;
	}

	//��ʂ̃C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffBottomMeshDome->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//���b�V���h�[���̏I������
//--------------------------------------------------------------------------------------------------------
void UninitMeshDome(void)
{
	//�t�^�̃C���f�b�N�X�̔j��
	if (g_pIdxBuffCoverMeshDome != NULL)
	{
		g_pIdxBuffCoverMeshDome->Release();
		g_pIdxBuffCoverMeshDome = NULL;
	}

	//���ʂ̃C���f�b�N�X�̔j��
	if (g_pIdxBuffSideMeshDome != NULL)
	{
		g_pIdxBuffSideMeshDome->Release();
		g_pIdxBuffSideMeshDome = NULL;
	}

	//��̃C���f�b�N�X�̔j��
	if (g_pIdxBuffBottomMeshDome != NULL)
	{
		g_pIdxBuffBottomMeshDome->Release();
		g_pIdxBuffBottomMeshDome = NULL;
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

	//���ʃJ�����O��ON
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//-----------------------------------------------------------
	//�t�^�̃C���f�b�N�X��ݒ肵�A�`�悷��
	//-----------------------------------------------------------
	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffCoverMeshDome);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHDOME_COVER_INDEX, 0, MESHDOME_SPLIT);

	//-----------------------------------------------------------
	//���ʂ̃C���f�b�N�X��ݒ肵�A�`�悷��
	//-----------------------------------------------------------
	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffSideMeshDome);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHDOME_SIDE_INDEX, 0, MESHDOME_SPLIT * 2 * (MESHDOME_SEPALATE - 1) - 6);

	//-----------------------------------------------------------
	//��ʂ̃C���f�b�N�X��ݒ肵�A�`�悷��
	//-----------------------------------------------------------
	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffBottomMeshDome);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHDOME_COVER_INDEX, 0, MESHDOME_SPLIT);

	//���ʂ̃J�����O���[�h�ɂ���
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
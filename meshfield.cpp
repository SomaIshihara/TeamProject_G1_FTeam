//==========================================
//
//���b�V���t�B�[���h�v���O����[meshfield.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "PvP_player.h"
#include "meshfield.h"
#include "file.h"
#include "color.h"
#include "input.h"
#include "timer.h"

//�}�N����`
#define MESHFIELD_SPLIT			(64)					//������
#define CENTER_AND_ONELAP		(2)						//���S�_�ƂP���ŏd�Ȃ�_
#define MESHFIELD_ALL_VERTEX	(MESHFIELD_SPLIT + 1)	//���ӂ̕����n�_�ƒ��S
#define MESHFIELD_ALL_INDEX		(MESHFIELD_SPLIT + 2)	//���ӂ̕����n�_�ƒ��S�E�P���ŏd�Ȃ�_
#define MESHFIELD_MIN_RADIUS	(250.0f)				//�ŏ����a
#define MESHFIELD_SHRINK_SPEED	(0.1f)					//�k�܂�X�s�[�h
#define MESHFIELD_SHRINK_TIME	(70)					//�k�܂�n�߂鎞��

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//���_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureMeshfield = NULL;		//�e�N�X�`���̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldMeshfield;			//���[���h�}�g���b�N�X
MESHFIELD				g_MeshField;

//====================================================================
//���b�V���t�B�[���h�̏���������
//====================================================================
void InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshfield, NULL);

	//���_���ݒ�
	SetFieldVertex();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_ALL_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,	&g_pIdxBuffMeshField, NULL);

	//�C���f�b�N�X����ݒ�
	SetFieldIndex();
}

//���b�V���t�B�[���h�̒��_����ݒ�
void SetFieldVertex(void)
{
	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ԍ�
	int nNumVtx = 0;

	float Rot = D3DX_PI;	//�p�x

	pVtx[nNumVtx].pos = ZERO_SET;				//���S���W���
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//�@��
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.5f, 0.5f);
	nNumVtx++;

	//���ӂ̒��_���̐ݒ�
	for (int nCntVtx = 0; nCntVtx < MESHFIELD_SPLIT; nCntVtx++, nNumVtx++)
	{
		pVtx[nNumVtx].pos = D3DXVECTOR3(
			sinf(Rot) * g_MeshField.fRadius,	//X�̈ʒu
			0.0f,								//Y�̈ʒu
			cosf(Rot) * g_MeshField.fRadius);	//Z�̈ʒu

		pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nNumVtx].col = XCOL_WHITE;

		float aTexU = 0.5f - 0.5f * sinf(Rot);	//�e�N�X�`����
		float aTexV = 0.5f - 0.5f * cosf(Rot);	//�e�N�X�`������

		pVtx[nNumVtx].tex = D3DXVECTOR2(aTexU, aTexV);

		//�p�x���@�S�̂̊p�x���������Ŋ������������A����
		Rot -= ONE_LAP / MESHFIELD_SPLIT;
	}

	//���o�b�t�@���A�����b�N
	g_pVtxBuffMeshfield->Unlock();
}

//���b�V���t�B�[���h�̃C���f�b�N�X����ݒ�
void SetFieldIndex(void)
{
	//�C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nNumIdx = 0; nNumIdx < MESHFIELD_ALL_INDEX; nNumIdx++)
	{
		//���̌v�Z���ł́A�@���傤��1������Ƃ��ɓ�����1�ɂȂ�@�@����ȊO�ł́A�J�E���^�[���������@�̗]�肪�ԍ��Ƃ��Đݒ肳���
		pIdx[nNumIdx] = (nNumIdx / (MESHFIELD_ALL_INDEX - 1)) + (nNumIdx % (MESHFIELD_ALL_INDEX - 1));
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

	//�C���f�b�N�X�o�b�t�@�̔j��
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
	//PvP�̃Q�[�����[�h�������A�k��������
	if (GetMode() == MODE_PvPGAME)
	{
		//��莞�Ԍ㔼�a���k�߂�
		if (GetTime() <= MESHFIELD_SHRINK_TIME)
		{//���a���k�߂�
			g_MeshField.fRadius -= MESHFIELD_SHRINK_SPEED;
		}

		//�ŏ����a�����������Ȃ���
		if (g_MeshField.fRadius < MESHFIELD_MIN_RADIUS)
		{
			//�ŏ����a�ɒ���
			g_MeshField.fRadius = MESHFIELD_MIN_RADIUS;
		}

		//���_���W�ݒ�
		SetFieldVertex();
	}
}

//====================================================================
//���b�V���t�B�[���h�̕`�揈��
//====================================================================
void DrawMeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���ʃJ�����O��ON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);

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
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHFIELD_ALL_VERTEX, 0, MESHFIELD_SPLIT);

	//���ʂ̃J�����O���[�h�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//====================================================================
//���b�V���t�B�[���h�̐ݒ菈��
//Author:�Ό��D�n
//Memo:���b�V���t�B�[���h�����g�p����Ȃ�file.cpp�̍\���ς��Ȃ���Ȃ��̂Ō�����
//====================================================================
void SetMeshField(int nTexNum, MESHFIELD mf)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\grassland001.png", &g_pTextureMeshfield)))
	{
	}

	//�����
	g_MeshField = mf;
}

//====================================================================
//���b�V���t�B�[���h�ɏ�������ǂ����̔���
//Author:���V����
//Memo:�t�B�[���h�͓����蔻��Ƃ������u���v�C���[�W�����������̂ŁA�ϐ�����  Land (���)  �ɂ��Ă��܂�
//���݁A�v���C���[�̍X�V�����̊֌W�ŏ������s���Ă���܂���
//====================================================================
bool LandMeshfield(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	//���݁E�O��̈ʒu�ƃt�B�[���h�̒��S�ʒu�̍����i�[
	D3DXVECTOR2 PosDiff;

	PosDiff.x = powf(g_MeshField.pos.x - pPos->x, 2.0f);	//���݂̂w���W�̍��̂Q��
	PosDiff.y = powf(g_MeshField.pos.z - pPos->z, 2.0f);	//���݂̂y���W�̍��̂Q��	�ϐ���  "�x" �����ǁA�y���W�̍������܂� 

	float fLength = sqrtf(PosDiff.x + PosDiff.y);	//���݂̂w�E�y�̒������擾

	//���S�ʒu����̋������t�B�[���h�̔��a�ȉ�������
	if (fLength <= g_MeshField.fRadius)
	{
		if (g_MeshField.pos.y <= pPosOld->y &&	//�O��̓t�B�[���h�����ɋ��āA
			g_MeshField.pos.y >= pPos->y)		//���݂̓t�B�[���h���牺�ɂ���
		{
			pPos->y = g_MeshField.pos.y;	//�t�B�[���h�̍����ɖ߂�
			return true;					//����������Ԃ��ďI��			
		}
	}

	//����Ă��Ȃ�����
	return false;
}

//���b�V���t�B�[���h�̏��擾
MESHFIELD *GetMeshField(void)
{
	return &g_MeshField;
}
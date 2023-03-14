//--------------------------------------------------------------------------------------------------------
//
//���b�V���̃��U���g�p�w�i[chargecylinder.cpp]
//Author:���V�����@�ѓc�F��
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "resultCylinder.h"
#include "color.h"

//�}�N����`
#define RESULTCYLINDER_HEIGHT				(1000.0f)					// �V�����_�[�̍���
#define RESULTCYLINDER_SPLIT				(32)						// �V�����_�[�̕�����
#define RESULTCYLINDER_TEX_RESOLUTION		(1.0f)						// �V�����_�[�̃e�N�X�`���̕�
#define NUM_RESULT_CYLINDER					(1)							// �V�����_�[�̐�
#define RESULTCYLINDER_ALL_VTX		(RESULTCYLINDER_SPLIT * 2 + 2)		// �V�����_�[�̍ő咸�_�E�C���f�b�N�X��
#define RESULT_MAX_RADIUS					(400.0f)					// �ő唼�a

//���U���g�p�w�i���̍\����
typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����
	float			fRadius;	// ���a�̑傫��
	float			fHeight;	// ����
}ResultCylinder;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureResultCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultCylinder = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffResultCylinder = NULL;
D3DXMATRIX				g_mtxWorldResultCylinder;	//���[���h�}�g���b�N�X
ResultCylinder			g_ResultCylinder[NUM_RESULT_CYLINDER];

//--------------------------------------------------------------------------------------------------------
//���U���g�p�w�i�̏���������
//--------------------------------------------------------------------------------------------------------
void InitResultCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky002.png", &g_pTextureResultCylinder);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * RESULTCYLINDER_ALL_VTX * NUM_RESULT_CYLINDER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffResultCylinder, NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * RESULTCYLINDER_ALL_VTX * NUM_RESULT_CYLINDER, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffResultCylinder, NULL);

	//���U���g�p�w�i�̏��̏�����
	for (int nCntCylinder = 0; nCntCylinder < NUM_RESULT_CYLINDER; nCntCylinder++)
	{
		g_ResultCylinder[nCntCylinder].pos = ZERO_SET;
		g_ResultCylinder[nCntCylinder].rot = ZERO_SET;
		g_ResultCylinder[nCntCylinder].fRadius = RESULT_MAX_RADIUS;
		g_ResultCylinder[nCntCylinder].fHeight = RESULTCYLINDER_HEIGHT;

		//���_���̐ݒ菈��
		SetResultCylinderVertex(nCntCylinder);

		//�C���f�b�N�X�ԍ��̐ݒ菈��
		SetResultCylinderIndex(nCntCylinder);
	}
}

//�f�ʂ̒��_���̐ݒ菈��
void SetResultCylinderVertex(int nCntCylinder)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffResultCylinder->Lock(0, 0, (void* *)&pVtx, 0);

	//�|�C���^�[�����炷
	pVtx += RESULTCYLINDER_ALL_VTX * nCntCylinder;

	float Rot = D3DX_PI;	//Y���̊p�x	
	float LapSepRot = ONE_LAP / RESULTCYLINDER_SPLIT;						//�P���̕������������߂�
	float TexSep_U = RESULTCYLINDER_TEX_RESOLUTION / RESULTCYLINDER_SPLIT;	//�e�N�X�`���̉��̕����̊��������߂�

	//���_���W�̐ݒ�
	for (int nCntVtx = 0; nCntVtx <= RESULTCYLINDER_SPLIT; nCntVtx++)
	{
		float	VtxPos_X = sinf(Rot) * g_ResultCylinder[nCntCylinder].fRadius,	//�w���W
				VtxPos_Z = cosf(Rot) * g_ResultCylinder[nCntCylinder].fRadius;	//�y���W
		int		nNumTopVtx = RESULTCYLINDER_SPLIT + nCntVtx + 1;		//�Ώۂ̒��_�̏㕔�̒��_�ԍ�

																		//���_�ʒu�Ɠ��������̒��_���W��ݒ�
		pVtx[nCntVtx].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//��Őݒ肵�����_���W�̐^��̒��_���W��ݒ�
		pVtx[nNumTopVtx].pos = D3DXVECTOR3(VtxPos_X, g_ResultCylinder[nCntCylinder].fHeight, VtxPos_Z);

		//�P�������Ƃ��̒��_���W
		if (nCntVtx == RESULTCYLINDER_SPLIT)
		{
			pVtx[nCntVtx].pos = pVtx[0].pos;							//�ŏ��̒��_���W����
			pVtx[nNumTopVtx].pos = pVtx[RESULTCYLINDER_SPLIT + 1].pos;	//�ŏ��̏㕔�̒��_���W����
		}

		//�@���x�N�g���̐ݒ�
		pVtx[nCntVtx].nor =
			pVtx[nNumTopVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntVtx].col = pVtx[nNumTopVtx].col = XCOL_WHITE;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 1.0f);
		pVtx[nNumTopVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 0.0f);

		//�p�x���@�S�̂̊p�x���������Ŋ������������A����
		Rot -= LapSepRot;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffResultCylinder->Unlock();
}

//�f�ʂ̃C���f�b�N�X�ԍ��̐ݒ菈��
void SetResultCylinderIndex(int nCntCylinder)
{
	WORD *pIdx;
	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	g_pIdxBuffResultCylinder->Lock(0, 0, (void* *)&pIdx, 0);

	pIdx += RESULTCYLINDER_ALL_VTX * nCntCylinder;			//�|�C���^�[�����炷
	int nBottomNumber = ODDPARITY + RESULTCYLINDER_SPLIT;	//�V�����_�[�����̊���_�ԍ�

															//�C���f�b�N�X�ԍ��f�[�^�̐ݒ�
	for (int nCntIdx = 0; nCntIdx < RESULTCYLINDER_ALL_VTX; nCntIdx++)
	{
		//�J�E���^�[�������̏ꍇ�A�㔼�ȍ~�̌v�Z�� �� �̓������[���ɂȂ�	��̏ꍇ�́A�V�����_�[�����̊���_�ԍ������Z�����
		pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + (nBottomNumber * (nCntIdx % EVENPARITY));
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffResultCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//���U���g�p�w�i�̏I������
//--------------------------------------------------------------------------------------------------------
void UninitResultCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureResultCylinder != NULL)
	{
		g_pTextureResultCylinder->Release();
		g_pTextureResultCylinder = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultCylinder != NULL)
	{
		g_pVtxBuffResultCylinder->Release();
		g_pVtxBuffResultCylinder = NULL;
	}

	//�C���f�b�N�X�̔j��
	if (g_pIdxBuffResultCylinder != NULL)
	{
		g_pIdxBuffResultCylinder->Release();
		g_pIdxBuffResultCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//���U���g�p�w�i�̍X�V����
//--------------------------------------------------------------------------------------------------------
void UpdateResultCylinder(void)
{

}

//--------------------------------------------------------------------------------------------------------
//���U���g�p�w�i�̕`�揈��
//--------------------------------------------------------------------------------------------------------
void DrawResultCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultCylinder, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffResultCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultCylinder);

	//���ʃJ�����O��ON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntCylinder = 0; nCntCylinder < NUM_RESULT_CYLINDER; nCntCylinder++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldResultCylinder);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ResultCylinder[nCntCylinder].rot.y, g_ResultCylinder[nCntCylinder].rot.x, g_ResultCylinder[nCntCylinder].rot.z);
		D3DXMatrixMultiply(&g_mtxWorldResultCylinder, &g_mtxWorldResultCylinder, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_ResultCylinder[nCntCylinder].pos.x, g_ResultCylinder[nCntCylinder].pos.y, g_ResultCylinder[nCntCylinder].pos.z);
		D3DXMatrixMultiply(&g_mtxWorldResultCylinder, &g_mtxWorldResultCylinder, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldResultCylinder);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, RESULTCYLINDER_ALL_VTX, 0, RESULTCYLINDER_ALL_VTX);
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//�ʏ�J�����O�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
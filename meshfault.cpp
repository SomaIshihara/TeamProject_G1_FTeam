//--------------------------------------------------------------------------------------------------------
//
//���b�V���̒f�ʐ}[meshfault.h]
//Author:�e�n�z�l�@���V�����@�Ό��D�n
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshfault.h"
#include "meshfield.h"
#include "color.h"
#include "file.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureMeshFault = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshFault = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshFault = NULL;
Fault					g_Fault;

//--------------------------------------------------------------------------------------------------------
//�f�ʂ̏���������
//--------------------------------------------------------------------------------------------------------
void InitMeshFault(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fault002.png", &g_pTextureMeshFault);

	//�f�ʏ�񏉊����̏�����
	g_Fault.pos = ZERO_SET;
	g_Fault.rot = ZERO_SET;

	//�f�ʏ��̎擾�i���f���I���W�i���t�@�C���ǂݍ��݌�ɍs�����Ɓj
	g_Fault = GetFaultData();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshFault, NULL);

	//���_���̐ݒ菈��
	SetMeshFaultVertex();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshFault, NULL);

	//�C���f�b�N�X�ԍ��̐ݒ菈��
	SetMeshFaultIndex();
}

//�f�ʂ̒��_���̐ݒ菈��
void SetMeshFaultVertex(void)
{
	//���a��ݒ�
	g_Fault.fRadius = GetMeshField()->fRadius;

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�̃��b�N
	g_pVtxBuffMeshFault->Lock(0, 0, (void**)&pVtx, 0);

	float Rot = D3DX_PI;	//Y���̊p�x

	//���_���W�̐ݒ�
	for (int nCntMeshFault = 0; nCntMeshFault <= MESHFAULT_SPLIT; nCntMeshFault++)
	{
		float	VtxPos_X = sinf(Rot) * g_Fault.fRadius,					//�w���W
				VtxPos_Z = cosf(Rot) * g_Fault.fRadius;					//�y���W
		int		nNumBottomVtx = MESHFAULT_SPLIT + nCntMeshFault + 1;	//�Ώۂ̒��_�̐^���̒��_�ԍ�

		//���_�ʒu�Ɠ��������̒��_���W��ݒ�
		pVtx[nCntMeshFault].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//��Őݒ肵�����_���W�̐^���̒��_���W��ݒ�
		pVtx[nNumBottomVtx].pos = D3DXVECTOR3(VtxPos_X, MESHFAULT_BOTTOM, VtxPos_Z);

		//�P�������Ƃ��̒��_���W
		if (nCntMeshFault == MESHFAULT_SPLIT)
		{
			pVtx[nCntMeshFault].pos = pVtx[0].pos;						//�ŏ��̒��_���W����
			pVtx[nNumBottomVtx].pos = pVtx[MESHFAULT_SPLIT + 1].pos;	//�ŏ��̑����̒��_���W����
		}

		//�@���x�N�g���̐ݒ�
		pVtx[nCntMeshFault].nor =
		pVtx[nNumBottomVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCntMeshFault].col = 
		pVtx[nNumBottomVtx].col = XCOL_WHITE;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntMeshFault].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 0.0f);
		pVtx[nNumBottomVtx].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 1.0f);

		//�p�x���@�S�̂̊p�x���������Ŋ������������A����
		Rot -= ONE_LAP / MESHFAULT_SPLIT;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffMeshFault->Unlock();
}

//�f�ʂ̃C���f�b�N�X�ԍ��̐ݒ菈��
void SetMeshFaultIndex(void)
{
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
	//���_���̐ݒ菈��
	SetMeshFaultVertex();
}

//--------------------------------------------------------------------------------------------------------
//�f�ʂ̕`�揈��
//--------------------------------------------------------------------------------------------------------
void DrawMeshFault(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Fault.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Fault.rot.y, g_Fault.rot.x, g_Fault.rot.z);
	D3DXMatrixMultiply(&g_Fault.mtxWorld, &g_Fault.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Fault.pos.x, g_Fault.pos.y, g_Fault.pos.z);
	D3DXMatrixMultiply(&g_Fault.mtxWorld, &g_Fault.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Fault.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshFault, 0, sizeof(VERTEX_3D));

	//���ʃJ�����O��ON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshFault);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshFault);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (MESHFAULT_SPLIT * 2 + 2), 0, (MESHFAULT_SPLIT * 2 + 2));

	//���ʂ̃J�����O���[�h�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
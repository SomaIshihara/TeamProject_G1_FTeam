/*==========================================================================================================================================================

�\���䏈��[VictoryStand.h]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "VictoryStand.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "sound.h"

#define VICTORYSTAND_TEX	(10)	// �\����̃e�N�X�`���̍ő吔
#define NUM_VICTORYSTAND	(1)		// �\����̑���
#define VICTORYSTAND_XFILE	"data\\MODEL\\victorystand\\Stand_No4.x"	// �\����̃t�@�C����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9			g_pTextureVictoryStand[VICTORYSTAND_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH					g_pMeshVictoryStand[NUM_VICTORYSTAND];			// ���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX					g_mtxWorldVictoryStand;							// ���[���h�}�g���b�N�X
LPD3DXBUFFER				g_pBuffMatVictoryStand[NUM_VICTORYSTAND];		// �}�e���A���ւ̃|�C���^
DWORD						g_dwNumMatVictoryStand[NUM_VICTORYSTAND];		// �}�e���A���̐�
VictoryStand				g_VictoryStand[NUM_VICTORYSTAND];				// �\����̏��

//----------------------------------------------------
//				�\����̏���������
//----------------------------------------------------
void InitVictoryStand(void)
{
	//�\����̏�񏉊���
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//��܂̕\����̃|�C���^�擾
		VictoryStand *pVicStand = &g_VictoryStand[nCntVicStd];

		pVicStand->pos = ZERO_SET;		// �ʒu������
		pVicStand->rot = ZERO_SET;		// ����������
		pVicStand->fLandheight = 0.0f;	// ���n�ʒu������
		pVicStand->bUse = true;		// �g���Ă��Ȃ��悤�ɂ���
		g_pMeshVictoryStand[nCntVicStd] = NULL;		// ���b�V��������
		g_pBuffMatVictoryStand[nCntVicStd] = NULL;	// �}�e���A���ւ̃|�C���^������
		g_dwNumMatVictoryStand[nCntVicStd] = NULL;	// �}�e���A���̐�������

		//X�t�@�C���̓ǂݍ��� and �e�N�X�`���ݒ�
		InitVicStdMeshMatrix(nCntVicStd);

		//�ŏ��E�ő�ʒu�ݒ�
		SearchVictoryStand_Land_Pos(nCntVicStd, &pVicStand->fLandheight);
	}
}

//���b�V����}�e���A���̏������E�ݒ菈��
void InitVicStdMeshMatrix(int nCntMetrix)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX(VICTORYSTAND_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatVictoryStand[nCntMetrix], NULL, &g_dwNumMatVictoryStand[nCntMetrix], &g_pMeshVictoryStand[nCntMetrix]);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL *pMat ;
	pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nCntMetrix]->GetBufferPointer();

	//�\����̃e�N�X�`���ݒ�
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nCntMetrix]; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureVictoryStand[nCntMat]);
		}
	}
}

//�����蔻��Ɏg�p����ŏ��E�ő�l�ݒ菈��
void SearchVictoryStand_Land_Pos(int nCntVicStd, float *pfLandHeight)
{
	int		nNumVtx;		//���_��
	DWORD	dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

							//���_�����擾
	nNumVtx = g_pMeshVictoryStand[nCntVicStd]->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshVictoryStand[nCntVicStd]->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshVictoryStand[nCntVicStd]->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	//���_���W�̑��
	D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

	//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
	pVtxBuff += dwSizeFVF;

								//�����蔻��Ɏg�����_�̍ŏ��E�ő�ʒu���擾
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���ݕ\����̎擾�������_���W�̂x���W���A�ϐ��ɑ������Ă��钅�n�ʒu��荂����΁A�㏑������
		*pfLandHeight = *pfLandHeight < vtx.y ? vtx.y : *pfLandHeight;

		//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;

		//���_���W�̑��
		vtx = *(D3DXVECTOR3 *)pVtxBuff;
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshVictoryStand[nCntVicStd]->UnlockVertexBuffer();
}

//----------------------------------------------
//				�\����̏I������
//----------------------------------------------
void UninitVictoryStand(void)
{
	//���b�V���ƃ}�e���A���̔j��
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//���b�V���j��
		if (g_pMeshVictoryStand[nCntVicStd] != NULL)
		{
			g_pMeshVictoryStand[nCntVicStd]->Release();
			g_pMeshVictoryStand[nCntVicStd] = NULL;
		}

		//�}�e���A���j��
		if (g_pBuffMatVictoryStand[nCntVicStd] != NULL)
		{
			g_pBuffMatVictoryStand[nCntVicStd]->Release();
			g_pBuffMatVictoryStand[nCntVicStd] = NULL;
		}
	}

	//�e�N�X�`���j��
	for (int nCntTex = 0; nCntTex < VICTORYSTAND_TEX; nCntTex++)
	{
		if (g_pTextureVictoryStand[nCntTex] != NULL)
		{
			g_pTextureVictoryStand[nCntTex]->Release();
			g_pTextureVictoryStand[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------
//				�\����̍X�V����
//----------------------------------------------
void UpdateVictoryStand(void)
{

}

//----------------------------------------------------
//					�\����̕`�揈��
//----------------------------------------------------
void DrawVictoryStand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//�g���Ă���
		if (g_VictoryStand[nCntVicStd].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldVictoryStand);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_VictoryStand[nCntVicStd].rot.y, g_VictoryStand[nCntVicStd].rot.x, g_VictoryStand[nCntVicStd].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_VictoryStand[nCntVicStd].pos.x, g_VictoryStand[nCntVicStd].pos.y, g_VictoryStand[nCntVicStd].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldVictoryStand);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nCntVicStd]->GetBufferPointer();

			//�}�e���A���ݒ�
			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nCntVicStd]; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureVictoryStand[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_pMeshVictoryStand[nCntVicStd]->DrawSubset(nCntMat);
			}

			//�}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//�\����̓����蔻��
bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove)
{
	bool bCollision = false;		//�����������ǂ����̔���

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{

	}

	//�����Ԃ�
	return bCollision;
}

//----------------------------------------------------
//					�\����̐ݒ菈��
//----------------------------------------------------
void SetVictoryStand(D3DXVECTOR3 Setpos)
{
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//�\���䂪�g���Ă��Ȃ�
		if (g_VictoryStand[nCntVicStd].bUse == false)
		{
			g_VictoryStand[nCntVicStd].pos = Setpos;		//�ʒu�ݒ�
			g_VictoryStand[nCntVicStd].rot = ZERO_SET;	//�����ݒ�

			g_VictoryStand[nCntVicStd].bUse = true;		//�g��
			break;//�����𔲂���
		}
	}
}
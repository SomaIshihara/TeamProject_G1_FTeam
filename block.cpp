//==========================================
//
//�A�C�e���v���O����[Block.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "Block.h"
#include "HDR_player.h"

//�}�N����`
#define MAX_PLAYER			(4)			//�ő吔
#define MAX_BLOCK_TEX		(10)		//�e�N�X�`���̍ő吔
#define MAX_POS				(5)			//���X�|�[���ʒu�ő吔
#define INIT_POS_Y			(200.0f)	//������Y�ʒu
#define INIT_POS_XZ			(200.0f)	//�����̊O�ʒu
#define COLLISION_SIZE_XZ	(30.0f)		//�c���̓����蔻��T�C�Y
#define BLOCK_LIFE			(1)			//�̗�

//�O���[�o���ϐ��錾     
LPDIRECT3DTEXTURE9		g_pTextureBlock[MAX_BLOCK_TEX] = {};	//�e�N�X�`���ւɃ|�C���^
LPD3DXMESH				g_pMeshBlock = NULL;					//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatBlock = NULL;					//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatBlock = 0;					//�}�e���A���̐�
D3DXMATRIX				g_mtxWorldBlock;						//���[���h�}�g���b�N�X
Block					g_Block[MAX_BLOCK * MAX_PLAYER];						//�u���b�N�̏��

//========================
//����������
//========================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATERIAL	  *pMat;					//�}�e���A���ւ̃|�C���^

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX({ "data/MODEL/block/block.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBlock,
		NULL,
		&g_dwNumMatBlock,
		&g_pMeshBlock);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatBlock->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBlock; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���@

		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureBlock[nCntMat]);
		}
	}

	//�����ݒ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		int nCount = 0;

		for (int nCntBlock = 0 + (100 * nCntPlayer); nCntBlock < MAX_BLOCK * MAX_PLAYER; nCntBlock++)
		{
			g_Block[nCntBlock].pos = D3DXVECTOR3(225.0f - 150 * nCntPlayer, 20.0f + nCount * 80, 0.0f);
			g_Block[nCntBlock].fWidth = COLLISION_SIZE_XZ;
			g_Block[nCntBlock].buse = false;
			g_Block[nCntBlock].nLife = BLOCK_LIFE;
			nCount++;

			if (nCount == 100)
			{
				break;
			}
		}
	}

	SetBlock();
}

//========================
//�I������
//========================
void UninitBlock(void)
{
	//���b�V���̔j��
	if (g_pMeshBlock != NULL)
	{
		g_pMeshBlock->Release();
		g_pMeshBlock = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatBlock != NULL)
	{
		g_pBuffMatBlock->Release();
		g_pBuffMatBlock = NULL;
	}

	//�}�e���A���̔j��
	for (int nCntTex = 0; nCntTex < MAX_BLOCK_TEX; nCntTex++)
	{
		if (g_pTextureBlock[nCntTex] != NULL)
		{
			g_pTextureBlock[nCntTex]->Release();
			g_pTextureBlock[nCntTex] = NULL;
		}
	}
}

//========================
//�X�V����
//========================
void UpdateBlock(void)
{

}

//========================
//�`�揈��
//========================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX		  mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9	  matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL	  *pMat;					//�}�e���A���f�[�^�ւ̃|�C���^

												//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldBlock);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBlock, &g_mtxWorldBlock, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBlock);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatBlock->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBlock; nCntMat++)
			{

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_pMeshBlock->DrawSubset(nCntMat);
			}
		}
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�ݒ菈��
//========================
void SetBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == false)
		{
			g_Block[nCntBlock].buse = true;
		}
	}
}

//========================
//�A�C�e���ƃv���C���[�̓����蔻�菈��
//========================
void CollisionBlock(int nPlayerNum)
{
	Player_HDR *pPlayer = GetPlayer_HDR();//�v���C���[���擾

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == true)
		{
			if (pPlayer[nPlayerNum].pos.x >= g_Block[nCntBlock].pos.x - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.x <= g_Block[nCntBlock].pos.x + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z >= g_Block[nCntBlock].pos.z - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z <= g_Block[nCntBlock].pos.z + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.y >= g_Block[nCntBlock].pos.y - COLLISION_SIZE_Y
				&&pPlayer[nPlayerNum].pos.y <= g_Block[nCntBlock].pos.y + COLLISION_SIZE_Y)
			{//�v���C���[���͈͓̔��ɓ������Ƃ�

				if (pPlayer[nPlayerNum].bHipDrop == true)
				{
					pPlayer[nPlayerNum].HipDropPower -= g_Block[nCntBlock].nLife;

					if (pPlayer[nPlayerNum].HipDropPower >= 0)
					{
						g_Block[nCntBlock].buse = false;
					}
					else
					{
						pPlayer[nPlayerNum].HipDropPower = 0;
						pPlayer[nPlayerNum].move.y = 0;
						pPlayer[nPlayerNum].bHipDrop = false;
					}
				}
				else
				{
					pPlayer[nPlayerNum].pos.y = g_Block[nCntBlock].pos.y + COLLISION_SIZE_Y;
					pPlayer[nPlayerNum].move.y = 0;
					pPlayer[nPlayerNum].moveV0.y = 0;
					pPlayer[nPlayerNum].jumpTime = 0;
					pPlayer[nPlayerNum].bJump = false;
					pPlayer[nPlayerNum].bHipDrop = false;
				}
			}
		}
	}
}
//==========================================
//
//�A�C�e���v���O����[item.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "item.h"
#include "player.h"

//�}�N����`
#define MAX_ITEM			(5)			//�ő吔
#define INIT_POS_Y			(200.0f)	//������Y�ʒu
#define INIT_POS_XZ			(200.0f)	//�����̊O�ʒu
#define RATIO_MOVE			(100.0f)	//�ړ��ʂ̊���
#define COLLISION_SIZE_XZ	(30.0f)		//�c���̓����蔻��T�C�Y
#define COLLISION_SIZE_Y	(15.0f)		//�����̓����蔻��T�C�Y

//�O���[�o���ϐ��錾     
LPDIRECT3DTEXTURE9		g_pTextureItem[10] = {};	//�e�N�X�`���ւɃ|�C���^
LPD3DXMESH				g_pMeshItem = NULL;	//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatItem = NULL;	//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatItem = 0;	//�}�e���A���̐�
D3DXMATRIX				g_mtxWorldItem;			//���[���h�}�g���b�N�X
Item					g_Item[MAX_ITEM];		//�A�C�e���̏��

//========================
//����������
//========================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATERIAL	  *pMat;					//�}�e���A���ւ̃|�C���^

												//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX({ "data/MODEL/item/itembox.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatItem,
		NULL,
		&g_dwNumMatItem,
		&g_pMeshItem);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���@

		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureItem[nCntMat]);
		}
	}

	//�����ݒ�
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[MAX_ITEM].pos = ZERO_SET;
		g_Item[MAX_ITEM].rot = ZERO_SET;
		g_Item[MAX_ITEM].move = ZERO_SET;
		g_Item[MAX_ITEM].DespawnLimit = 0;
		g_Item[MAX_ITEM].fAlpha = 0.0f;					//�����x�̐ݒ�
		g_Item[MAX_ITEM].buse = false;
	}
	
}

//========================
//�I������
//========================
void UninitItem(void)
{
	//���b�V���̔j��
	if (g_pMeshItem != NULL)
	{
		g_pMeshItem->Release();
		g_pMeshItem = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatItem != NULL)
	{
		g_pBuffMatItem->Release();
		g_pBuffMatItem = NULL;
	}
}

//========================
//�X�V����
//========================
void UpdateItem(void) 
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == true)
		{
		
		}
	}
}

//========================
//�`�揈��
//========================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX		  mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9	  matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL	  *pMat;					//�}�e���A���f�[�^�ւ̃|�C���^

												//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldItem);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Item[MAX_ITEM].rot.y, g_Item[MAX_ITEM].rot.x, g_Item[MAX_ITEM].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_Item[MAX_ITEM].pos.x, g_Item[MAX_ITEM].pos.y, g_Item[MAX_ITEM].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
			{
				//�}�e���A���̐F�ݒ�
				pMat[nCntMat].MatD3D.Diffuse.a = g_Item[MAX_ITEM].fAlpha;

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureItem[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_pMeshItem->DrawSubset(nCntMat);
			}
		}
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�ݒ菈��
//========================
void SetItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == false)
		{
			g_Item[MAX_ITEM].RespawnDelay -= 1;//���X�|�[���f�B���C����

			if (g_Item[MAX_ITEM].RespawnDelay <= 0)
			{
				g_Item[MAX_ITEM].DespawnLimit = 0;
				g_Item[MAX_ITEM].buse = true;
			}
		}
	}
}

//========================
//�A�C�e���ƃv���C���[�̓����蔻�菈��
//========================
void CollisionIP(int nPlayerNum)
{
	Player *pPlayer = GetPlayer();//�v���C���[���擾

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[MAX_ITEM].buse == true)
		{
			if (pPlayer[nPlayerNum].pos.x >= g_Item[MAX_ITEM].pos.x - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.x <= g_Item[MAX_ITEM].pos.x + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z >= g_Item[MAX_ITEM].pos.z - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z <= g_Item[MAX_ITEM].pos.z + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.y >= g_Item[MAX_ITEM].pos.y - COLLISION_SIZE_Y
				&&pPlayer[nPlayerNum].pos.y <= g_Item[MAX_ITEM].pos.y + COLLISION_SIZE_Y)
			{//�v���C���[���͈͓̔��ɓ������Ƃ�

				pPlayer[nPlayerNum].nATKItemTime = 300;
				//�g���Ă��Ȃ���Ԃɂ���
				g_Item[MAX_ITEM].RespawnDelay = 3;
				g_Item[MAX_ITEM].fAlpha = 1.0f;
				g_Item[MAX_ITEM].buse = false;
			}
		}
	}
}

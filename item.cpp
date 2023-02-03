//==========================================
//
//�A�C�e���v���O����[item.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "item.h"
#include "player.h"


//�O���[�o���ϐ��錾     
LPDIRECT3DTEXTURE9		g_pTextureItem[10] = {};	//�e�N�X�`���ւɃ|�C���^
LPD3DXMESH				g_pMeshItem = NULL;	//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatItem = NULL;	//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatItem = 0;	//�}�e���A���̐�
D3DXMATRIX				g_mtxWorldItem;			//���[���h�}�g���b�N�X
Item					g_Item;					//�{�[�i�X�̏��

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
	g_Item.pos = ZERO_SET;
	g_Item.rot = ZERO_SET;
	g_Item.move = ZERO_SET;
	g_Item.DespawnLimit = 0;
	g_Item.fAlpha = 1.0f;					//�����x�̐ݒ�
	g_Item.buse = false;
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
	if (g_Item.buse == true)
	{
		//g_ParticleCounter--;

		/*if (g_ParticleCounter <= 0)
		{
			SetParticle(g_Item.pos, 10.0f, 30, PARTICLE_NORMAL);

			g_ParticleCounter = PARTICLE_LIMIT;
		};*/

		////�ʒu�̍X�V
		//g_Item.pos.x += g_Item.move.x;
		//g_Item.pos.z += g_Item.move.z;

		////�����̐ݒ�
		//g_Item.move.x += (NIL_F - g_Item.move.x) * 1.0f;
		//g_Item.move.z += (NIL_F - g_Item.move.z) * 1.0f;
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

	/*if (g_Item.buse == true)
	{*/
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldItem);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Item.rot.y, g_Item.rot.x, g_Item.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_Item.pos.x, g_Item.pos.y, g_Item.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
		{
			//�}�e���A���̐F�ݒ�
			pMat[nCntMat].MatD3D.Diffuse.a = g_Item.fAlpha;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItem[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshItem->DrawSubset(nCntMat);
		}
	//}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�A�C�e���ƃv���C���[�̓����蔻�菈��
//========================
void CollisionIP(int nPlayerNum)
{
	Player *pPlayer = GetPlayer();

	pPlayer[nPlayerNum].nATKItemTime = 300;

}

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
#define MAX_ITEM			(3)			//�ő吔
#define MAX_POS				(5)			//���X�|�[���ʒu�ő吔
#define INIT_POS_Y			(200.0f)	//������Y�ʒu
#define INIT_POS_XZ			(200.0f)	//�����̊O�ʒu
#define RATIO_MOVE			(100.0f)	//�ړ��ʂ̊���
#define COLLISION_SIZE_XZ	(30.0f)		//�c���̓����蔻��T�C�Y
#define COLLISION_SIZE_Y	(30.0f)		//�����̓����蔻��T�C�Y

const D3DXVECTOR3 g_ItemRespawnPos[MAX_POS] =
{
	D3DXVECTOR3(INIT_POS_XZ,NIL_F,NIL_F),
	D3DXVECTOR3(NIL_F,NIL_F,-INIT_POS_XZ),
	D3DXVECTOR3(-INIT_POS_XZ,NIL_F,NIL_F),
	D3DXVECTOR3(NIL_F,NIL_F,INIT_POS_XZ),
	D3DXVECTOR3(NIL_F,NIL_F,NIL_F),
};

//�O���[�o���ϐ��錾     
LPDIRECT3DTEXTURE9		g_pTextureItem[10] = {};	//�e�N�X�`���ւɃ|�C���^
LPD3DXMESH				g_pMeshItem = NULL;	//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatItem = NULL;	//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatItem = 0;	//�}�e���A���̐�
D3DXMATRIX				g_mtxWorldItem;			//���[���h�}�g���b�N�X
Item					g_Item[MAX_ITEM];		//�A�C�e���̏��
bool					bposuse[MAX_POS];		//���̍��W���g�p���Ă��邩�ǂ���

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
		g_Item[nCntItem].pos = ZERO_SET;
		g_Item[nCntItem].rot = ZERO_SET;
		g_Item[nCntItem].move = ZERO_SET;
		g_Item[nCntItem].DespawnLimit = 0;
		g_Item[nCntItem].fWidth = COLLISION_SIZE_XZ;
		g_Item[nCntItem].fAlpha = 1.0f;					//�����x�̐ݒ�
		g_Item[nCntItem].buse = false;
	}

	for (int nCntPos = 0; nCntPos < MAX_POS; nCntPos++)
	{
		bposuse[nCntPos] = false;
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
		if (g_Item[nCntItem].buse == true)
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
		if (g_Item[nCntItem].buse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldItem);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
			g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
			{
				//�}�e���A���̐F�ݒ�
				pMat[nCntMat].MatD3D.Diffuse.a = g_Item[nCntItem].fAlpha;

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
		if (g_Item[nCntItem].buse == false)
		{
			g_Item[nCntItem].RespawnDelay -= 1;//���X�|�[���f�B���C����

			if (g_Item[nCntItem].RespawnDelay <= 0)
			{//�p�����[�^�[�ݒ�

				g_Item[nCntItem].RespawnPos = (rand() % MAX_POS);

				if (bposuse[g_Item[nCntItem].RespawnPos] == false)
				{
					g_Item[nCntItem].pos = g_ItemRespawnPos[g_Item[nCntItem].RespawnPos]
						+ D3DXVECTOR3(NIL_F, (float)(rand() % 200), NIL_F);
					//�A�C�e���̎�ނ�ݒ�
					g_Item[nCntItem].type = (ITEMTYPE)(rand() % ITEMTYPE_MAX);
					g_Item[nCntItem].DespawnLimit = 0;
					bposuse[g_Item[nCntItem].RespawnPos] = true;
					g_Item[nCntItem].buse = true;
				}
				
			}
		}
	}
}

//========================
//�A�C�e���ƃv���C���[�̓����蔻�菈��
//========================
void CollisionIP(int nPlayerNum)
{
	D3DXVECTOR3 pos0, pos1;
	D3DXVECTOR3 vecMove;
	D3DXVECTOR3 vecLine;
	D3DXVECTOR3 vecToPos, vecToPosOld;
	float fAreaA, fAreaB;

	Player *pPlayer = GetPlayer();//�v���C���[���擾

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].buse == true)
		{
			{
				//�ړ��x�N�g��
				//vecMove = pPlayer[nPlayerNum].pos - pPlayer[nPlayerNum].posOld;

				////�ʒu�v�Z
				//pos0.x = g_Item[nCntItem].pos.x - (g_Item[nCntItem].fWidth / 2) * cosf(g_Item[nCntItem].rot.y);
				//pos0.y = 0.0f;
				//pos0.z = g_Item[nCntItem].pos.z + (g_Item[nCntItem].fWidth / 2) * sinf(g_Item[nCntItem].rot.y);

				//pos1.x = g_Item[nCntItem].pos.x + (g_Item[nCntItem].fWidth / 2) * cosf(g_Item[nCntItem].rot.y);
				//pos1.y = 0.0f;
				//pos1.z = g_Item[nCntItem].pos.z - (g_Item[nCntItem].fWidth / 2) * sinf(g_Item[nCntItem].rot.y);

				//vecLine = pos1 - pos0;	//���E���x�N�g��
				//vecToPos = pPlayer[nPlayerNum].pos - pos0;
				//vecToPosOld = pPlayer[nPlayerNum].posOld - pos0;

				////�ʐϋ��߂�
				//fAreaA = TASUKIGAKE(vecToPos.x, vecToPos.z, vecMove.x, vecMove.z);
				//fAreaB = TASUKIGAKE(vecLine.x, vecLine.z, vecMove.x, vecMove.z);

				//if (pPlayer[nPlayerNum].pos.y >= g_Item[nCntItem].pos.y - COLLISION_SIZE_Y
				//	&&pPlayer[nPlayerNum].pos.y <= g_Item[nCntItem].pos.y + COLLISION_SIZE_Y)
				//{
				//	//��ɐL�тĂ�����̍����ɍs���ƒl���}�C�i�X�ɂȂ�炵����
				//	if ((vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z) >= 0.0f && (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0.0f)
				//	{//���s�l�ӌ`�̑傫���̊�����0.0~1.0�Ȃ�Ԃɓ����Ă���
				//		if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
				//		{//������

				//			pPlayer[nPlayerNum].nATKItemTime = 300;
				//			//�g���Ă��Ȃ���Ԃɂ���
				//			g_Item[nCntItem].RespawnDelay = 3;
				//			g_Item[nCntItem].fAlpha = 1.0f;
				//			g_Item[nCntItem].buse = false;
				//		}

				//	}
				//}
			}
			if (pPlayer[nPlayerNum].pos.x >= g_Item[nCntItem].pos.x - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.x <= g_Item[nCntItem].pos.x + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z >= g_Item[nCntItem].pos.z - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z <= g_Item[nCntItem].pos.z + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.y >= g_Item[nCntItem].pos.y - COLLISION_SIZE_Y
				&&pPlayer[nPlayerNum].pos.y <= g_Item[nCntItem].pos.y + COLLISION_SIZE_Y)
			{//�v���C���[���͈͓̔��ɓ������Ƃ�
				for (int nCntPos = 0; nCntPos < MAX_POS; nCntPos++)
				{
					if (g_Item[nCntItem].pos.x == g_ItemRespawnPos[nCntPos].x)
					{
						if (g_Item[nCntItem].pos.z == g_ItemRespawnPos[nCntPos].z)
						{
							bposuse[nCntPos] = false;
						}
					}

				}

				switch (g_Item[nCntItem].type)
				{
				case ITEMTYPE_ATK:
					pPlayer[nPlayerNum].nATKItemTime = 300;
					break;

				case ITEMTYPE_DEF:
					pPlayer[nPlayerNum].nDEFItemTime = 300;
					break;

				case ITEMTYPE_GHOST:
					pPlayer[nPlayerNum].nGhostItemTime = 150;
					break;

				}
		
				//�g���Ă��Ȃ���Ԃɂ���
				g_Item[nCntItem].RespawnDelay = 3;
				g_Item[nCntItem].fAlpha = 1.0f;
				g_Item[nCntItem].buse = false;
			}
		}
	}
}

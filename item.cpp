//==========================================
//
//�A�C�e���v���O����[item.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "item.h"
#include "PvP_player.h"
#include "color.h"

//�}�N����`
#define MAX_ITEM			(3)			//�ő吔
#define MAX_ITEM_TEX		(10)		//�e�N�X�`���̍ő吔
#define MAX_POS				(5)			//���X�|�[���ʒu�ő吔
#define INIT_POS_Y			(200.0f)	//������Y�ʒu
#define INIT_POS_XZ			(200.0f)	//�����̊O�ʒu
#define RATIO_MOVE			(100.0f)	//�ړ��ʂ̊���
#define COLLISION_SIZE_XZ	(30.0f)		//�c���̓����蔻��T�C�Y
#define COLLISION_SIZE_Y	(30.0f)		//�����̓����蔻��T�C�Y

//�A�C�e�����ԁi���ׂĕb�P�ʁj
#define ITEMTIME_ATK		(7)			//ATK�̎���
#define ITEMTIME_DEF		(7)			//DEF�̎���
#define ITEMTIME_GHOST		(10)		//�S�[�X�g���̎���
#define ITEMTIME_INV		(7)			//���G�̎���

const D3DXVECTOR3 g_ItemRespawnPos[MAX_POS] =
{
	D3DXVECTOR3(INIT_POS_XZ,NIL_F,NIL_F),
	D3DXVECTOR3(NIL_F,NIL_F,-INIT_POS_XZ),
	D3DXVECTOR3(-INIT_POS_XZ,NIL_F,NIL_F),
	D3DXVECTOR3(NIL_F,NIL_F,INIT_POS_XZ),
	D3DXVECTOR3(NIL_F,NIL_F,NIL_F),
};

//�O���[�o���ϐ��錾     
LPDIRECT3DTEXTURE9		g_pTextureItem[MAX_ITEM_TEX] = {};	//�e�N�X�`���ւɃ|�C���^
LPD3DXMESH				g_pMeshItem = NULL;					//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatItem = NULL;				//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatItem = 0;					//�}�e���A���̐�
D3DXMATRIX				g_mtxWorldItem;						//���[���h�}�g���b�N�X
Item					g_Item[MAX_ITEM];					//�A�C�e���̏��
bool					bposuse[MAX_POS];					//���̍��W���g�p���Ă��邩�ǂ���
int						g_nInvincibleItem;					//���G�A�C�e���擾��

//========================
//����������
//========================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATERIAL	  *pMat;					//�}�e���A���ւ̃|�C���^

												//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX({ "data\\MODEL\\konpeito\\konpeito.x" },
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

	g_nInvincibleItem = 0;
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

	//�}�e���A���̔j��
	for (int nCntTex = 0; nCntTex < MAX_ITEM_TEX; nCntTex++)
	{
		if (g_pTextureItem[nCntTex] != NULL)
		{
			g_pTextureItem[nCntTex]->Release();
			g_pTextureItem[nCntTex] = NULL;
		}
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
			g_Item[nCntItem].rot.y += 0.01f;
			g_Item[nCntItem].rot.x += 0.001f;
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
				pMat[nCntMat].MatD3D.Emissive;

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

		/*------------------------------------------------------------------
		�e�̕`��
		--------------------------------------------------------------------*/
		D3DXMATRIX	mtxShadow;		//�V���h�E�}�g���b�N�X
		D3DLIGHT9	light;			//���C�g���
		D3DXVECTOR4	posLight;		//���C�g�̈ʒu
		D3DXVECTOR3	pos, normal;	//���ʏ�̔C�ӂ̓_�A�@���x�N�g��
		D3DXPLANE	plane;			//���ʏ��

									//���C�g�̈ʒu��ݒ�
		pDevice->GetLight(3, &light);
		posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

		//���ʏ��𐶐�
		pos = ZERO_SET;
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		//�V���h�E�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxShadow);

		//�V���h�E�}�g���b�N�X�̍쐬
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &g_mtxWorldItem, &mtxShadow);

		//�V���h�E�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
		{
			D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//�}�e���A���f�[�^����

															//���F�ɐݒ�						//���Ȕ����𖳂���
			MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

			//�}�e���A���ݒ�
			pDevice->SetMaterial(&MatCopy);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, NULL);

			//���f���`��
			g_pMeshItem->DrawSubset(nCntMat);
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
					g_Item[nCntItem].type = (ITEMTYPE)(rand() % (ITEMTYPE_MAX - g_nInvincibleItem));

					//���G�A�C�e���Ȃ�J�E���g���₷
					(g_Item[nCntItem].type == ITEMTYPE_INVINCIBLE) ? g_nInvincibleItem++ : g_nInvincibleItem;

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

	Player *pPlayer = GetPlayer();//�v���C���[���擾

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].buse == true)
		{
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

							//�A�C�e�����ʃ��Z�b�g
							pPlayer[nPlayerNum].nATKItemTime = 0;
							pPlayer[nPlayerNum].nDEFItemTime = 0;
							pPlayer[nPlayerNum].nGhostItemTime = 0;
						}
					}
				}

				if (pPlayer[nPlayerNum].nInvincibleTime <= 0)
				{
					switch (g_Item[nCntItem].type)
					{
					case ITEMTYPE_ATK:
						pPlayer[nPlayerNum].nATKItemTime = CONVERT_FPS(ITEMTIME_ATK);
						break;

					case ITEMTYPE_DEF:
						pPlayer[nPlayerNum].nDEFItemTime = CONVERT_FPS(ITEMTIME_DEF);
						break;

					case ITEMTYPE_GHOST:
						pPlayer[nPlayerNum].nGhostItemTime = CONVERT_FPS(ITEMTIME_GHOST);
						break;

					case ITEMTYPE_INVINCIBLE:
						pPlayer[nPlayerNum].nInvincibleTime = CONVERT_FPS(ITEMTIME_INV);
						break;
					}
				}
		
				//�g���Ă��Ȃ���Ԃɂ���
				g_Item[nCntItem].RespawnDelay = 3;
				g_Item[nCntItem].fAlpha = 1.0f;
				g_Item[nCntItem].buse = false;
			}
		}
	}
}

//==========================================
//
//�A�C�e���v���O����[Block.cpp]
//Author:�ѓc�F��
//
//==========================================
#include "main.h"
#include "HDR_player.h"
#include "Block.h"
#include "sound.h"

//�}�N����`
#define MAX_PLAYER			(4)			//�ő吔
#define MAX_BLOCK_TEX		(10)		//�e�N�X�`���̍ő吔
#define MAX_POS				(5)			//���X�|�[���ʒu�ő吔
#define INIT_POS_Y			(200.0f)	//������Y�ʒu
#define INIT_POS_XZ			(200.0f)	//�����̊O�ʒu
#define COLLISION_SIZE_XZ	(30.0f)		//�c���̓����蔻��T�C�Y

//�v���g
BLOCKTYPE SelectBlock(void);

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureBlock[MAX_BLOCK_TEX] = {};	//�e�N�X�`���ւɃ|�C���^
LPD3DXMESH				g_pMeshBlock = NULL;					//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatBlock = NULL;					//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatBlock = 0;					//�}�e���A���̐�
D3DXMATRIX				g_mtxWorldBlock;						//���[���h�}�g���b�N�X
Block					g_Block[MAX_BLOCK * MAX_PLAYER];		//�u���b�N�̏��
int						g_aUseBlockNum[BLOCKTYPE_MAX];			//�u���b�N��

//�u���b�N�̗�
const int				c_aBlockLife[BLOCKTYPE_MAX] = { 100,200,400 };

//�z�u��
const int				c_aBlockNum[BLOCKTYPE_MAX] = { 25,30,25 };

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
	//�u���b�N���ݒ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		int nSetCount = 0;			//�z�u������
		float PlayerPos_X = GetPlayer_HDR()[nCntPlayer].pos.x;	//�v���C���[�̇]���W���i�[
		//������
		for (int nCntBlockType = 0; nCntBlockType < BLOCKTYPE_MAX; nCntBlockType++)
		{
			g_aUseBlockNum[nCntBlockType] = c_aBlockNum[nCntBlockType];
		}

		for (int nCntBlock = MAX_BLOCK * nCntPlayer; nCntBlock < MAX_BLOCK * MAX_PLAYER; nCntBlock++)
		{
			g_Block[nCntBlock].pos = D3DXVECTOR3(PlayerPos_X, (float)nSetCount * COLLISION_SIZE_Y, 0.0f);	//�ʒu��ݒ�
			g_Block[nCntBlock].type = SelectBlock();							//�ł��ݒ�
			g_Block[nCntBlock].nLife = c_aBlockLife[g_Block[nCntBlock].type];	//�̗͐ݒ�
			g_Block[nCntBlock].buse = true;										//�g��
			nSetCount++;	//�ݒ肵�������Z

			//�ݒ肵��������l���̏���ɒB����
			if (nSetCount == MAX_BLOCK)
			{//�������~�߂�
				break;
			}
		}
	}
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
//�u���b�N�I������
//========================
BLOCKTYPE SelectBlock(void)
{
	int nBlockType = rand() % BLOCKTYPE_MAX;

	if (g_aUseBlockNum[nBlockType] == 0)
	{//�u���b�N���z�u����ɒB���Ă���
		//2�O�i���[�v����j�ɂ���
		nBlockType = (nBlockType + (BLOCKTYPE_MAX - 1)) % BLOCKTYPE_MAX;

		if (g_aUseBlockNum[nBlockType] == 0)
		{//������z�u����ɒB���Ă���
			//2�O�i���[�v����j�ɂ���
			nBlockType = (nBlockType + (BLOCKTYPE_MAX - 1)) % BLOCKTYPE_MAX;
			//��΂ɒu����̂ŏI���
		}
	}

	//�u����̂Őݒ�
	g_aUseBlockNum[nBlockType]--;
	return (BLOCKTYPE)nBlockType;
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
			g_Block[nCntBlock].nLife = c_aBlockLife[g_Block[nCntBlock].type];
			g_Block[nCntBlock].buse = true;
		}
	}
}

//========================
//�u���b�N�ƃv���C���[�̓����蔻�菈��
//========================
void CollisionBlock(Player_HDR *pPlayer)
{
	Block *pBlock = &g_Block[0];						//�u���b�N�̏��擾

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++, pBlock++)
	{
		float
		Block_L = pBlock->pos.x - COLLISION_SIZE_XZ,	//���[
		Block_R = pBlock->pos.x + COLLISION_SIZE_XZ,	//�E�[
		Block_F = pBlock->pos.z - COLLISION_SIZE_XZ,	//��O
		Block_D = pBlock->pos.z + COLLISION_SIZE_XZ,	// �� 
		Block_U = pBlock->pos.y - COLLISION_SIZE_Y,		// �� 
		Block_B = pBlock->pos.y + COLLISION_SIZE_Y;		//�V��

		//�u���b�N���g���Ă���
		if (pBlock->buse == true)
		{
			if (pPlayer->pos.x + pPlayer->fBodySize >= Block_L	&&	//���[��� �E �ɂ���
				pPlayer->pos.x - pPlayer->fBodySize <= Block_R	&&	//�E�[��� �� �ɂ���
				pPlayer->pos.z + pPlayer->fBodySize >= Block_F	&&	//��O��� �� �ɂ���
				pPlayer->pos.z - pPlayer->fBodySize <= Block_D	&&	// �� ����O�ɂ���
				pPlayer->pos.y >= Block_U	&&	// �� ��� �� �ɂ���
				pPlayer->pos.y <= Block_B)		//�V�ʂ�� �� �ɂ���
			{
				//�v���C���[���q�b�v�h���b�v��
				if (pPlayer->bHipDrop == true)
				{
					//�u���b�N�̗̑͂��p���[�i�_���[�W�ʁj���傫������΃u���b�N������
					if (pPlayer->HipDropPower >= pBlock->nLife)
					{
						pBlock->buse = false;					//�Ώۂ̃u���b�N���g�p���Ȃ�
						pPlayer->HipDropPower -= pBlock->nLife;	//�g�p�������̃_���[�W�����炷
						PlaySoundSE(SOUND_LABEL_SE_HIPDROP, nCntBlock);	//�q�b�v�h���b�v���Đ�
						DestroyBlockSound(pPlayer->nDestroyCounter++, nCntBlock);	//�j�󂵂����ʃT�E���h�Đ��i�����ē����ɉ��Z
					}
					else
					{
						g_Block[nCntBlock].nLife -= pPlayer->HipDropPower;
						pPlayer->HipDropPower = 0;
						pPlayer->nDestroyCounter = 0;
						pPlayer->move.y = 0;
						pPlayer->bHipDrop = false;
						pPlayer->nAICT = c_aAIParamHDR[pPlayer->aiDiff].nAICT;
					}
				}
				else
				{
					//�v���C���[���܂��S�[�����Ă��Ȃ�
					if (!pPlayer->bGoal)
					{
						pPlayer->pos.y = g_Block[nCntBlock].pos.y + COLLISION_SIZE_Y;
						pPlayer->move.y = 0;
						pPlayer->moveV0.y = 0;
						pPlayer->jumpTime = 0;
						pPlayer->bJump = false;
						pPlayer->bHipDrop = false;
					}
				}

				//====================================
				//			X�̓����蔻��
				//====================================
				//�����瓖�������ꍇ
				if (pPlayer->posOld.x + pPlayer->fBodySize <= Block_L &&	//�O��ł̓v���C���[�����ɋ��āA
					pPlayer->pos.x + pPlayer->fBodySize >= Block_L)			//���݂ł̓u���b�N�����ɂ���
				{
					pPlayer->pos.x = Block_L - pPlayer->fBodySize;	//�u���b�N�̍��[�ɖ߂�
					pPlayer->move.x = 0.0f;		//�]�̈ړ��ʂ�����
					pPlayer->moveV0.x = 0.0f;	//�]�̈ړ��ʂ�����
				}

				//�E���瓖�������ꍇ
				if (Block_R <= pPlayer->posOld.x - pPlayer->fBodySize &&	//�O��ł̓v���C���[���E�ɋ��āA
					Block_R >= pPlayer->pos.x - pPlayer->fBodySize)			//���݂ł̓u���b�N���E�ɂ���
				{
					pPlayer->pos.x = Block_R + pPlayer->fBodySize;	//�u���b�N�̉E�[�ɖ߂�
					pPlayer->move.x = 0.0f;		//�]�̈ړ��ʂ�����
					pPlayer->moveV0.x = 0.0f;	//�]�̈ړ��ʂ�����
				}

				//====================================
				//			Z�̓����蔻��
				//====================================
				//��O���瓖�������ꍇ
				if (pPlayer->posOld.z + pPlayer->fBodySize <= Block_F &&	//�O��ł̓v���C���[����O�ɋ��āA
					pPlayer->pos.z + pPlayer->fBodySize >= Block_F)			//���݂ł̓u���b�N����O�ɂ���
				{
					pPlayer->pos.z = Block_F - pPlayer->fBodySize;	//�u���b�N�̎�O�ɖ߂�
					pPlayer->move.z = 0.0f;		//�y�̈ړ��ʂ�����
					pPlayer->moveV0.z = 0.0f;	//�y�̈ړ��ʂ�����
				}

				//�����瓖�������ꍇ
				if (Block_D <= pPlayer->posOld.z - pPlayer->fBodySize &&	//�O��ł̓v���C���[�����ɋ��āA
					Block_D >= pPlayer->pos.z - pPlayer->fBodySize)			//���݂ł̓u���b�N�����ɂ���
				{
					pPlayer->pos.z = Block_D + pPlayer->fBodySize;	//�u���b�N�̉��ɖ߂�
					pPlayer->move.z = 0.0f;		//�y�̈ړ��ʂ�����
					pPlayer->moveV0.z = 0.0f;	//�y�̈ړ��ʂ�����
				}
			}
		}
	}
}

//========================
//�u���b�N�̔j����ɂ��T�E���h����
//========================
void DestroyBlockSound(int nDestroyCounter, int nCntBlock)
{
	//�j�󂵂����ɂ��T�E���h�Đ�����
	switch (nDestroyCounter)
	{
	case 0:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_00, nCntBlock); break;	//�j�󂵂����P��
	case 1:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_01, nCntBlock); break;	//�j�󂵂����Q��
	case 2:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_02, nCntBlock); break;	//�j�󂵂����R��
	case 3:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_03, nCntBlock); break;	//�j�󂵂����S��
	case 4:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_04, nCntBlock); break;	//�j�󂵂����T��
	case 5:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_05, nCntBlock); break;	//�j�󂵂����U��
	case 6:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_06, nCntBlock); break;	//�j�󂵂����V��
	default:PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_07, nCntBlock); break;	//�j�󂵂����W�ȏ�
	}
}
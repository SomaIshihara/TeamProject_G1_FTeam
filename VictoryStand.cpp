/*==========================================================================================================================================================

�\���䏈��[VictoryStand.h]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "rank.h"
#include "debugproc.h"
#include "select_game.h"
#include "conversioninput.h"
#include "VictoryStand.h"
#include "resultPlayer.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "sound.h"

#define VICTORYSTAND_TEX		(10)			// �\����̃e�N�X�`���̍ő吔
#define NUM_VICTORYSTAND		(4)				// �\����̑���
#define VICSTD_FILL_HEIGHT		(2.0f)			// �������ߗ��ėʕ␳�l
#define VICSTD_STAIRS_HEIGHT	(8.0f)			// �\������K�i�̂悤�ɖ��ߗ��Ă�

#define VICSTD_APPEAR_TIME	(CONVERT_FPS(5))	// �\����̂�����鎞��
#define VICSTD_APPEAR_SPEED		(0.8f)			// �\����̂������X�s�[�h

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9			g_pTextureVictoryStand[VICTORYSTAND_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH					g_pMeshVictoryStand[NUM_VICTORYSTAND];			// ���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX					g_mtxWorldVictoryStand;							// ���[���h�}�g���b�N�X
LPD3DXBUFFER				g_pBuffMatVictoryStand[NUM_VICTORYSTAND];		// �}�e���A���ւ̃|�C���^
DWORD						g_dwNumMatVictoryStand[NUM_VICTORYSTAND];		// �}�e���A���̐�
VictoryStand				g_VictoryStand[NUM_VICTORYSTAND];				// �\����̏��
float						g_fLandPoint[NUM_VICTORYSTAND];					// �\���䂲�Ƃ̒��n�_�����i�������l�����āj
int							g_RankStrage[RANKING_MAX] = {};

//�\����̇]�t�@�C����
const char *c_apVicStdFilePath[NUM_VICTORYSTAND] = {
	"data\\MODEL\\victorystand\\Stand_No1.x",
	"data\\MODEL\\victorystand\\Stand_No2.x",
	"data\\MODEL\\victorystand\\Stand_No3.x",
	"data\\MODEL\\victorystand\\Stand_No4.x",
};

//----------------------------------------------------
//				�\����̏���������
//----------------------------------------------------
void InitVictoryStand(void)
{	
	//���n�_�ݒ�
	SearchVictoryStand_Land_Pos();

	//�Ώۂ̕\����̃|�C���^�擾
	VictoryStand *pVicStand = &g_VictoryStand[0];

	//���U���g�p�v���C���[�̃|�C���^�擾
	Player_RESULT *pResPlayer = GetPlayer_RESULT();

	//�\����̏�񏉊���
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStand++, pResPlayer++)
	{
		pVicStand->pos.x = pResPlayer->pos.x;		// �w�ʒu������
		pVicStand->pos.z = pVicStand->pos.y = 0.0f;	// �y�ʒu�� �x���W���O�ɂ���

		pVicStand->rot = ZERO_SET;					// ����������
		pVicStand->bUse = true;						// �g���Ă���悤�ɂ���
		pVicStand->bAppear = false;					// ����Ȃ��悤�ɂ���
		pVicStand->nRank = g_RankStrage[nCntVicStd] = 
			pResPlayer->nRank;	// ���ʂ�������
	}

	//���ʂ��\�[�g����
	RankingSort();
}

//�������l�����ă����L���O���ă\�[�g
void RankingSort(void)
{
	//���ʔ���p
	int nSortRanking[RANKING_MAX] = { 0,0,0,0 };

	//�����L���O���\�[�g����
	//��r�������̔z��ԍ�����
	for (int nCntComp = 0; nCntComp < RANKING_MAX; nCntComp++)
	{
		//��r������̔ԍ�����
		for (int nCntJudge = 0; nCntJudge < nCntComp; nCntJudge++)
		{
			switch (GetSelGameMode())
			{
				//=========================================
				//�����Ƀ\�[�g����
				//=========================================
			case SelGameMode_PVP:
			{
				//��r�u�����v���̃|�C���g�̂ق����傫��
				if (g_VictoryStand[nCntJudge].nRank < g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntJudge]++;//��r����ق��̏��ʂ�������
				}

				//��r�u����v���̃|�C���g�̂ق����傫��
				else if (g_VictoryStand[nCntJudge].nRank > g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntComp]++;//��r�����ق��̏��ʂ�������
				}
			}
				break;

				//=========================================
				//�~���Ƀ\�[�g����
				//=========================================
			case SelGameMode_HDR:
			{
				//��r�u�����v���̃|�C���g�̂ق����傫��
				if (g_VictoryStand[nCntJudge].nRank < g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntComp]++;//��r�����ق��̏��ʂ�������
				}

				//��r�u����v���̃|�C���g�̂ق����傫��
				else if (g_VictoryStand[nCntJudge].nRank > g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntJudge]++;//��r����ق��̏��ʂ�������
				}
			}
				break;
			}

			//*****************************
			//���_�̎��͉������Ȃ�
			//*****************************
		}
	}

	//�\�[�g���������L���O��������
	for (int nCntPay = 0; nCntPay < RANKING_MAX; nCntPay++)
	{
		//���ʁ@���@���͍팸�p�ϐ��@�ɑ��
		int nRank = g_VictoryStand[nCntPay].nRank = nSortRanking[nCntPay];
		
		// ���n�_��������
		g_VictoryStand[nCntPay].fLandheight = g_fLandPoint[nRank];	

		//�ŏ��͒n���ɖ��߂�i�����珇�Ԃɖ��ߗ��Ă�[���𑝂₵�Ă���
		g_VictoryStand[nCntPay].pos.y -= g_VictoryStand[nCntPay].fLandheight + VICSTD_FILL_HEIGHT * (nCntPay * VICSTD_STAIRS_HEIGHT);
	}
}

//�����蔻��Ɏg�p����ŏ��E�ő�l�ݒ菈��
void SearchVictoryStand_Land_Pos(void)
{
	for (int nCntLandPos = 0; nCntLandPos < NUM_VICTORYSTAND; nCntLandPos++)
	{
		g_pMeshVictoryStand[nCntLandPos] = NULL;	// ���b�V��������
		g_pBuffMatVictoryStand[nCntLandPos] = NULL;	// �}�e���A���ւ̃|�C���^������
		g_dwNumMatVictoryStand[nCntLandPos] = NULL;	// �}�e���A���̐�������

		/***********************************************
			X�t�@�C���̓ǂݍ��� and �e�N�X�`���ݒ�
		*************************************************/
		InitVicStdModel_Tex(nCntLandPos);

		int		nNumVtx;		//���_��
		DWORD	dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
		BYTE	*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

		//���_�����擾
		nNumVtx = g_pMeshVictoryStand[nCntLandPos]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshVictoryStand[nCntLandPos]->GetFVF());

		//���_�o�b�t�@�����b�N
		g_pMeshVictoryStand[nCntLandPos]->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

		//�����蔻��Ɏg�����_�̍ŏ��E�ő�ʒu���擾
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

			//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;

			// ���ݕ\����̎擾�������_���W�̂x���W���A�ϐ��ɑ������Ă��钅�n�ʒu��荂����΁A�㏑������
			g_fLandPoint[nCntLandPos] = g_fLandPoint[nCntLandPos] < vtx.y ? vtx.y : g_fLandPoint[nCntLandPos];
		}

		//���_�o�b�t�@���A�����b�N
		g_pMeshVictoryStand[nCntLandPos]->UnlockVertexBuffer();
	}
}

//���b�V����}�e���A���̏������E�ݒ菈��
void InitVicStdModel_Tex(int nCntModex) 
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX(c_apVicStdFilePath[nCntModex], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatVictoryStand[nCntModex], NULL, &g_dwNumMatVictoryStand[nCntModex], &g_pMeshVictoryStand[nCntModex]);

	//�\����̃e�N�X�`���ݒ�
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nCntModex]; nCntMat++)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nCntModex]->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureVictoryStand[nCntModex]);
			break;
		}
	}
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
	//�\����̃|�C���^���擾
	VictoryStand *pVicStd = &g_VictoryStand[0];

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
	{
		//�n�ʂ��猻��Ă��悢
		if (pVicStd->bAppear)
		{
			//�n�ʂ��猻���
			pVicStd->pos.y += VICSTD_APPEAR_SPEED;

			//�\���䂪�n�ʂ��畂���Ă��܂�����
			if (0.0f < pVicStd->pos.y)
			{
				//�n�ʂɖ߂�
				pVicStd->pos.y = 0.0f;
			}
		}
	}
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
	VictoryStand *pVicStd = &g_VictoryStand[0];	//�\����̃|�C���^���擾

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
	{
		//�g���Ă���
		if (pVicStd->bUse)
		{
			//���ʊi�[
			int nRank = pVicStd->nRank;

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldVictoryStand);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pVicStd->rot.y, pVicStd->rot.x, pVicStd->rot.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pVicStd->pos.x, pVicStd->pos.y, pVicStd->pos.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldVictoryStand);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nRank]->GetBufferPointer();

			//�}�e���A���ݒ�
			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nRank]; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureVictoryStand[nRank]);

				//�^����ꂽ���ʕʂ̃��f��(�p�[�c)�̕`��
				g_pMeshVictoryStand[nRank]->DrawSubset(nCntMat);
			}

			//�}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//�\����̓����蔻��
bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold)
{
	bool bCollision = false;		//�����������ǂ����̔���

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//�������팸�̂��߂ɁA�Ώۂ̕\����̏����i�[
		VictoryStand VicStd = g_VictoryStand[nCntVicStd];

		//�ŏ��͒n���ɖ��܂��Ă���̂ŁA���n�_��s�x�A�v�Z����
		float LandPoint = VicStd.pos.y + VicStd.fLandheight;

		//�Ώۂ̃v���C���[�ƕ\����̇]�ʒu����v���Ă���i�\����̈ʒu�́A�v���C���[�̏��ʂ��Ƃɐ^���ɔz�u����̂ŁA�]���W�ȊO�̔���͖���
		if (pPos->x == VicStd.pos.x)
		{
			if (pPos->y <= LandPoint)		//���݂͒��n�_�ȉ��̍����ɂ���
			{
				//�v���C���[�̈ʒu�𒅒n�_�ɏC������
				pPos->y = LandPoint;
			}
		}
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
			g_VictoryStand[nCntVicStd].pos = Setpos;	//�ʒu�ݒ�
			g_VictoryStand[nCntVicStd].rot = ZERO_SET;	//�����ݒ�

			g_VictoryStand[nCntVicStd].bUse = true;		//�g��
			break;//�����𔲂���
		}
	}
}

//----------------------------------------------------
//					�\����̏o������
//----------------------------------------------------
void SetAppearVictoryStand(void)
{
	//�\����̃|�C���^���擾
	VictoryStand *pVicStd = &g_VictoryStand[0];

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
	{
		//�o��������
		pVicStd->bAppear = true;
	}
}

//****************************************************
//	�\����A�v���C���[�̃��U���g���o�X�L�b�v���̏���
//****************************************************
void SkipVicStd_Player(void)
{
	VictoryStand *pVicStd = &g_VictoryStand[0];		//�\����̃|�C���^���擾
	Player_RESULT *pPlayer = GetPlayer_RESULT();	//�v���C���[�̃|�C���^���擾

	for (int nCntResSkip = 0; nCntResSkip < NUM_VICTORYSTAND; nCntResSkip++, pVicStd++, pPlayer++)
	{
		//-------------------
		//�\����̏���
		//-------------------
		pVicStd->pos.y = 0.0f;	//�\�����n�\�ɏo��
		pVicStd->bAppear = true;//�o��������

		//-------------------
		//�v���C���[�̏���
		//-------------------
		pPlayer->pos.y = pVicStd->fLandheight;
	}
}
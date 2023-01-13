//==========================================
//
//���f���֌W�v���O����[model.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "model.h"

//�O���[�o���ϐ�
Model g_aModel[ANIMAL_MAX];	//���f���\����

//�t�@�C���p�X
const char *c_pFileNameModel[] =
{
	"data/MODEL/ino/ino_body.x",
	"data/MODEL/ino/ino_head.x",
	"data/MODEL/ino/ino_left_forefoot.x",
	"data/MODEL/ino/ino_left_foretoes.x",
	"data/MODEL/ino/ino_left_hindleg.x",
	"data/MODEL/ino/ino_left_hindtoes.x",
	"data/MODEL/ino/ino_right_forefoot.x",
	"data/MODEL/ino/ino_right_foretoes.x",
	"data/MODEL/ino/ino_right_hindleg.x",
	"data/MODEL/ino/ino_right_hindtoes.x"
};

//========================
//����������
//========================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	
	//�ϐ�������
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aModel[nCntModel].aParts[nCntParts] = {};
			g_aModel[nCntModel].aParts[nCntParts].bUse = false;
		}
	}

	//X�t�@�C���ǂݍ���
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (SUCCEEDED(D3DXLoadMeshFromX(
				c_pFileNameModel[nCntParts],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aModel[nCntModel].aParts[nCntParts].pBuffMat,
				NULL,
				&g_aModel[nCntModel].aParts[nCntParts].dwNumMatModel,
				&g_aModel[nCntModel].aParts[nCntParts].pMesh)))
			{//�ǂݍ��ݐ���������
				//�e�N�X�`���ǂݍ���
				D3DXMATERIAL *pMat;	//�}�e���A���|�C���^

				//�}�e���A�����ɑ΂���|�C���^�擾
				pMat = (D3DXMATERIAL *)g_aModel[nCntModel].aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntTex = 0; nCntTex < (int)g_aModel[nCntModel].aParts[nCntParts].dwNumMatModel; nCntTex++)
				{
					if (pMat[nCntTex].pTextureFilename != NULL)
					{
						//�e�N�X�`���ǂݍ���
						D3DXCreateTextureFromFile(pDevice,
							pMat[nCntTex].pTextureFilename,
							&g_aModel[nCntModel].aParts[nCntParts].apTexture[nCntTex]);
					}
				}

				//�g�p���Ă����Ԃɂ���
				g_aModel[nCntModel].aParts[nCntParts].bUse = true;
			}
			else
			{//���s������1���f�����I��
				break;
			}
		}
#if 0
		//�K�w�\���ݒ�
		//��
		g_aModel[nCntModel].aParts[0].mIdxModelParent = -1;
		g_aModel[nCntModel].aParts[0].posOffset = D3DXVECTOR3(0.0f, 35.0f, 0.0f);
		g_aModel[nCntModel].aParts[0].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//��
		g_aModel[nCntModel].aParts[1].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[1].posOffset = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aModel[nCntModel].aParts[1].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//���r
		g_aModel[nCntModel].aParts[2].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[2].posOffset = D3DXVECTOR3(-5.0f, 7.0f, 0.0f);
		g_aModel[nCntModel].aParts[2].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//����
		g_aModel[nCntModel].aParts[3].mIdxModelParent = 2;
		g_aModel[nCntModel].aParts[3].posOffset = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].aParts[3].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//�E�r
		g_aModel[nCntModel].aParts[4].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[4].posOffset = D3DXVECTOR3(5.0f, 7.0f, 0.0f);
		g_aModel[nCntModel].aParts[4].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//�E��
		g_aModel[nCntModel].aParts[5].mIdxModelParent = 4;
		g_aModel[nCntModel].aParts[5].posOffset = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].aParts[5].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//����
		g_aModel[nCntModel].aParts[6].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[6].posOffset = D3DXVECTOR3(-3.0f, -8.0f, 0.0f);
		g_aModel[nCntModel].aParts[6].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//����
		g_aModel[nCntModel].aParts[7].mIdxModelParent = 6;
		g_aModel[nCntModel].aParts[7].posOffset = D3DXVECTOR3(0.0f, -12.0f, 0.0f);
		g_aModel[nCntModel].aParts[7].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//�E��
		g_aModel[nCntModel].aParts[8].mIdxModelParent = 0;
		g_aModel[nCntModel].aParts[8].posOffset = D3DXVECTOR3(3.0f, -8.0f, 0.0f);
		g_aModel[nCntModel].aParts[8].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//�E��
		g_aModel[nCntModel].aParts[9].mIdxModelParent = 8;
		g_aModel[nCntModel].aParts[9].posOffset = D3DXVECTOR3(0.0f, -12.0f, 0.0f);
		g_aModel[nCntModel].aParts[9].rotOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
#endif
	}
}

//========================
//�I�������i�����̂ݔj���iRelease�j�����GetModel�Ŏ擾��������NULL����邾����OK�j
//========================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			//���b�V���̔j��
			if (g_aModel[nCntModel].aParts[nCntParts].pMesh != NULL)
			{
				g_aModel[nCntModel].aParts[nCntParts].pMesh->Release();
				g_aModel[nCntModel].aParts[nCntParts].pMesh = NULL;
			}

			//�}�e���A���̔j��
			if (g_aModel[nCntModel].aParts[nCntParts].pBuffMat != NULL)
			{
				g_aModel[nCntModel].aParts[nCntParts].pBuffMat->Release();
				g_aModel[nCntModel].aParts[nCntParts].pBuffMat = NULL;
			}
		}
	}
}

//========================
//���f���擾����
//========================
//�w�肵�������̃��f�����ׂĂ�Ԃ��܂�
//���̃v���O�����Ŏ擾�������̂��ׂĂ�Ԃ����߁A���b�V���E�}�e���A���|�C���^���������̂�����܂�
//���̂��ߏI������������Ƃ���UninitModel�Ń��b�V���E�}�e���A���|�C���^��j����NULL����A�擾��������NULL����̂ݍs������
//========================
Model GetModel(ANIMAL animal)
{
	return g_aModel[animal];
}
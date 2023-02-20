//==========================================
//
//���f���֌W�v���O����[model.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "model.h"
#include "file.h"

//�v���g
void InitAnimalModel(void);
void InitObjectModel(void);

//�O���[�o���ϐ�
Model g_aAnimalModel[ANIMAL_MAX];	//�������f���\����
Model g_aObjModel[OBJECTTYPE_MAX];	//�I�u�W�F�N�g���f���\����

//========================
//����������
//========================
void InitModel(void)
{
	//�������f���̏�����
	InitAnimalModel();

	//�I�u�W�F�N�g���f���̏�����
	InitObjectModel();
}

//========================
//�������f������������
//========================
void InitAnimalModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�ǂݍ���
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aAnimalModel[nCntModel].aParts[nCntParts] = {};
			g_aAnimalModel[nCntModel].aParts[nCntParts].bUse = false;
		}

		//�g�p���郂�[�V�����ޓǂݍ���
		LoadMotionViewerFile(GetMotionFilePath(nCntModel), &g_aAnimalModel[nCntModel]);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (SUCCEEDED(D3DXLoadMeshFromX(
				&g_aAnimalModel[nCntModel].aModelFileName[nCntParts][0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat,
				NULL,
				&g_aAnimalModel[nCntModel].aParts[nCntParts].dwNumMatModel,
				&g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh)))
			{//�ǂݍ��ݐ���������
				//�e�N�X�`���ǂݍ���
				D3DXMATERIAL *pMat;	//�}�e���A���|�C���^

				//�}�e���A�����ɑ΂���|�C���^�擾
				pMat = (D3DXMATERIAL *)g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntTex = 0; nCntTex < (int)g_aAnimalModel[nCntModel].aParts[nCntParts].dwNumMatModel; nCntTex++)
				{
					if (pMat[nCntTex].pTextureFilename != NULL)
					{
						//�e�N�X�`���ǂݍ���
						D3DXCreateTextureFromFile(pDevice,
							pMat[nCntTex].pTextureFilename,
							&g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex]);
					}
				}

				//�g�p���Ă����Ԃɂ���
				g_aAnimalModel[nCntModel].aParts[nCntParts].bUse = true;
			}
			else
			{//���s������1���f�����I��
				break;
			}
		}
	}
}

//========================
//�I�u�W�F�N�g����������
//========================
void InitObjectModel(void)
{//���l�FaParts�̓L�����N�^�[���f���悤�ɕ����p�ӂ��Ă��邾���Ȃ̂ŃI�u�W�F�N�g�Ɋւ��Ă�1�����g��Ȃ��̂�0�ł���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{
		g_aAnimalModel[nCntModel].aParts[0] = {};
		g_aAnimalModel[nCntModel].aParts[0].bUse = false;

		if (SUCCEEDED(D3DXLoadMeshFromX(
			GetModelFilePath(nCntModel),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aAnimalModel[nCntModel].aParts[0].pBuffMat,
			NULL,
			&g_aAnimalModel[nCntModel].aParts[0].dwNumMatModel,
			&g_aAnimalModel[nCntModel].aParts[0].pMesh)))
		{//�ǂݍ��ݐ���������
			//�e�N�X�`���ǂݍ���
			D3DXMATERIAL *pMat;	//�}�e���A���|�C���^

			//�}�e���A�����ɑ΂���|�C���^�擾
			pMat = (D3DXMATERIAL *)g_aAnimalModel[nCntModel].aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntTex = 0; nCntTex < (int)g_aAnimalModel[nCntModel].aParts[0].dwNumMatModel; nCntTex++)
			{
				if (pMat[nCntTex].pTextureFilename != NULL)
				{
					//�e�N�X�`���ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntTex].pTextureFilename,
						&g_aAnimalModel[nCntModel].aParts[0].apTexture[nCntTex]);
				}
			}

			//�g�p���Ă����Ԃɂ���
			g_aAnimalModel[nCntModel].aParts[0].bUse = true;
		}
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
			if (g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh != NULL)
			{
				g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh->Release();
				g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh = NULL;
			}

			//�}�e���A���̔j��
			if (g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat != NULL)
			{
				g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat->Release();
				g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat = NULL;
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
	return g_aAnimalModel[animal];
}
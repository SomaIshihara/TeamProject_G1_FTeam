//==========================================
//
//���f���֌W�v���O����[model.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "model.h"
#include "file.h"

//�}�N��
#define MAX_PLACE_OBJ		(256)
#define MAX_TEMPSTR_CHAR	(4096)

//�O���[�o���ϐ�
Model g_aAnimalModel[ANIMAL_MAX];		//�������f���\����
Model g_aObjModel[OBJECTTYPE_MAX];		//�I�u�W�F�N�g���f���݌v�}�\����

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
		g_aObjModel[nCntModel].aParts[0] = {};
		g_aObjModel[nCntModel].aParts[0].bUse = false;

		if (SUCCEEDED(D3DXLoadMeshFromX(
			GetModelFilePath(nCntModel),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aObjModel[nCntModel].aParts[0].pBuffMat,
			NULL,
			&g_aObjModel[nCntModel].aParts[0].dwNumMatModel,
			&g_aObjModel[nCntModel].aParts[0].pMesh)))
		{//�ǂݍ��ݐ���������
			//�e�N�X�`���ǂݍ���
			D3DXMATERIAL *pMat;	//�}�e���A���|�C���^

			//�}�e���A�����ɑ΂���|�C���^�擾
			pMat = (D3DXMATERIAL *)g_aObjModel[nCntModel].aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntTex = 0; nCntTex < (int)g_aObjModel[nCntModel].aParts[0].dwNumMatModel; nCntTex++)
			{
				if (pMat[nCntTex].pTextureFilename != NULL)
				{
					//�e�N�X�`���ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntTex].pTextureFilename,
						&g_aObjModel[nCntModel].aParts[0].apTexture[nCntTex]);
				}
			}

			//�g�p���Ă����Ԃɂ���
			g_aObjModel[nCntModel].aParts[0].bUse = true;
		}
	}
}

//========================
//�I�������i�����̂ݔj���iRelease�j�����GetModel�Ŏ擾��������NULL����邾����OK�j
//========================
void UninitModel(void)
{
	//����
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			//�e�N�X�`���j��
			for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
			{
				if (g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex] != NULL)
				{
					g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex]->Release();
					g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex] = NULL;
				}
			}

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

	//�I�u�W�F�N�g
	//�݌v�}
	for (int nCntObj = 0; nCntObj < OBJECTTYPE_MAX; nCntObj++)
	{
		//�e�N�X�`���j��
		for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
		{
			if (g_aObjModel[nCntObj].aParts[0].apTexture[nCntTex] != NULL)
			{
				g_aObjModel[nCntObj].aParts[0].apTexture[nCntTex]->Release();
				g_aObjModel[nCntObj].aParts[0].apTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_aObjModel[nCntObj].aParts[0].pMesh != NULL)
		{
			g_aObjModel[nCntObj].aParts[0].pMesh->Release();
			g_aObjModel[nCntObj].aParts[0].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aObjModel[nCntObj].aParts[0].pBuffMat != NULL)
		{
			g_aObjModel[nCntObj].aParts[0].pBuffMat->Release();
			g_aObjModel[nCntObj].aParts[0].pBuffMat = NULL;
		}
	}
}

//========================
//�����擾����
//========================
//�w�肵�������̃��f�����ׂĂ�Ԃ��܂�
//�\���̂Ƃ��ɕۑ��Ƃ����Ȃ��Ă��ꎞ�I�Ɏ�荞��ł����Ďg���I�������̂Ă邮�炢�ł�����
//========================
Model GetAnimal(ANIMAL animal)
{
	return g_aAnimalModel[animal];
}

//========================
//�I�u�W�F�N�g�擾����
//========================
//��Ɠ���
//========================
Model GetXObject(OBJECTTYPE objtype)
{
	return g_aObjModel[objtype];
}
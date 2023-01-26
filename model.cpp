//==========================================
//
//���f���֌W�v���O����[model.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "model.h"
#include "file.h"

//�O���[�o���ϐ�
Model g_aModel[ANIMAL_MAX];	//���f���\����

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
		
		//�g�p���郂�[�V�����ޓǂݍ���
		LoadMotionViewerFile(GetMotionFilePath(nCntModel), &g_aModel[nCntModel]);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (SUCCEEDED(D3DXLoadMeshFromX(
				&g_aModel[nCntModel].aModelFileName[nCntParts][0],
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
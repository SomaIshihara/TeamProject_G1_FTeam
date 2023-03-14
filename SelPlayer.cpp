//==========================================
//
//�v���C���[�I���v���O����[SelPlayer.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "SelPlayer.h"
#include "conversioninput.h"
#include "color.h"
#include "file.h"

#include "TypeFrame.h"
#include "SelPlayer_bg.h"
#include "SelPlayer_camera.h"
#include "model.h"

//�O���[�o��
D3DXMATRIX g_mtxWorldSelPlayer[MAX_USE_GAMEPAD];
D3DXMATRIX g_mtxWorldModelSelPlayer[MAX_USE_GAMEPAD][MAX_PARTS];

//�v���C���[�ʒu����
const D3DXVECTOR3 c_aPosRotSelPlayer[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-75.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(-25.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(25.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(75.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
};

//========================
//����������
//========================
void InitSelPlayer(void)
{
	InitFile();				//�t�@�C��������
	LoadModelViewerFile("data\\model.txt");	// ���f���r���[���[�t�@�C���ǂݍ��݁i�e�I�u�W�F�N�g�������O�ɍs�����ƁI�j
	InitAnimalModel();		//�������f��������

	InitTypeFrame();
	InitSelPlayer_Bg();
	InitSelPlayer_Camera();
	InitSelPlayer_SetCameraPos(D3DXVECTOR3(0.0f, 50.0f, 150.0f), D3DXVECTOR3(0.0f, 20.0f, 0.0f));
}

//========================
//�I������
//========================
void UninitSelPlayer(void)
{
	UninitSelPlayer_Camera();
	UninitSelPlayer_Bg();
	UninitTypeFrame();

	UninitModel();				//�������f���I������
}

//========================
//�X�V����
//========================
void UpdateSelPlayer(void)
{
	UpdateTypeFrame();
	UpdateSelPlayer_Bg();
	UpdateSelPlayer_Camera();
}

//========================
//�`�揈��
//========================
void DrawSelPlayer(void)
{
	//�w�i
	DrawSelPlayer_Bg();

	//�^�C�v�g
	DrawTypeFrame();

	//�J�����ݒ�
	SetSelPlayer_Camera();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���f���`��
	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//�v���C���[�̐������J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (GetUsePlayer(nCntPlayer) == true)
		{
			Model useAnimal = GetAnimal(ANIMAL_WILDBOAR);

			//"�v���C���[��"���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_mtxWorldSelPlayer[nCntPlayer]);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, c_aPosRotSelPlayer[nCntPlayer][1].y, c_aPosRotSelPlayer[nCntPlayer][1].x, c_aPosRotSelPlayer[nCntPlayer][1].z);
			D3DXMatrixMultiply(&g_mtxWorldSelPlayer[nCntPlayer], &g_mtxWorldSelPlayer[nCntPlayer], &mtxRot);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTrans, c_aPosRotSelPlayer[nCntPlayer][0].x, c_aPosRotSelPlayer[nCntPlayer][0].y, c_aPosRotSelPlayer[nCntPlayer][0].z);
			D3DXMatrixMultiply(&g_mtxWorldSelPlayer[nCntPlayer], &g_mtxWorldSelPlayer[nCntPlayer], &mtxTrans);

			//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSelPlayer[nCntPlayer]);

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				if (useAnimal.aParts[nCntParts].bUse == true)
				{
					D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p
					D3DXMATRIX mtxParent;					//�e�̃}�g��

					//"���f����"���[���h�}�g���b�N�X������
					D3DXMatrixIdentity(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts]);

					//�����𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRotModel, useAnimal.aParts[nCntParts].rotOffset.y, useAnimal.aParts[nCntParts].rotOffset.x, useAnimal.aParts[nCntParts].rotOffset.z);
					D3DXMatrixMultiply(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &mtxRotModel);

					//�ʒu���f
					D3DXMatrixTranslation(&mtxTransModel, useAnimal.aParts[nCntParts].posOffset.x, useAnimal.aParts[nCntParts].posOffset.y, useAnimal.aParts[nCntParts].posOffset.z);
					D3DXMatrixMultiply(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &mtxTransModel);

					//�p�[�c�̐e�}�g���ݒ�
					if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
					{
						mtxParent = g_mtxWorldModelSelPlayer[nCntPlayer][useAnimal.aParts[nCntParts].mIdxModelParent];
					}
					else
					{
						mtxParent = g_mtxWorldSelPlayer[nCntPlayer];
					}

					//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
					D3DXMatrixMultiply(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &mtxParent);

					//"���f����"���[���h�}�g���b�N�X�ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts]);

					//�}�e���A���f�[�^�ւ̃|�C���^�擾
					pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						//�S�[�X�g�p
						D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

						if (nCntParts == MAX_PARTS - 1)
						{//�[�b�P���̎��͐F�ύX
							matChange.Diffuse = c_aColPlayer[nCntPlayer];
						}

						//�}�e���A���ݒ�
						pDevice->SetMaterial(&matChange);

						//�e�N�X�`���ݒ�
						pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

						//���f���`��
						useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}
		}
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
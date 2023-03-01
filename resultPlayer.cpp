//==========================================
//
//���U���g�v���C���[�v���O�����̏���[hdr_player.cpp]
//Author:���V�����@�Ό��D�n
//
//==========================================
#include "resultPlayer.h"
#include "select_game.h"

//�}�N����`

//�O���[�o���ϐ�
Player_RESULT g_aPlayerResult[MAX_USE_GAMEPAD];

//========================
//����������
//========================
void InitPlayer_RESULT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

												//�ϐ�������
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//�ϐ�������
		g_aPlayerResult[nCntPlayer].pos = ZERO_SET;
		g_aPlayerResult[nCntPlayer].posOld = ZERO_SET;
		g_aPlayerResult[nCntPlayer].move = ZERO_SET;
		g_aPlayerResult[nCntPlayer].rot = ZERO_SET;
		g_aPlayerResult[nCntPlayer].bHipDrop = false;
		g_aPlayerResult[nCntPlayer].bHipDropSpin = false;
		g_aPlayerResult[nCntPlayer].nHipDropWait = 0;

		g_aPlayerResult[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayerResult[nCntPlayer].nRank = -1;

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aPlayerResult[nCntPlayer].animalInst[nCntParts] = {};
		}
	}
}

//========================
//�I������
//========================
void UninitPlayer_RESULT(void)
{

}

//========================
//�X�V����
//========================
void UpdatePlayer_RESULT(void)
{
	//�v���C���[�l�����J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{

	}
}

//========================
//�`�揈��
//========================
void DrawPlayer_RESULT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

									//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//�v���C���[�̐������J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		Model useAnimal;

		//�Q�[�����[�h�ɉ����Ď擾������̂�ύX
		if (GetSelectGameMode() == SelectGameMode_PVP)
		{
			useAnimal = GetAnimal(GetPlayer()[nCntPlayer].animal);
		}
		else if (GetSelectGameMode() == SelectGameMode_HDR)
		{
			useAnimal = GetAnimal(GetPlayer_HDR()[nCntPlayer].animal);
		}

		//"�v���C���[��"���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_aPlayerResult[nCntPlayer].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerResult[nCntPlayer].rot.y, g_aPlayerResult[nCntPlayer].rot.x, g_aPlayerResult[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].mtxWorld, &g_aPlayerResult[nCntPlayer].mtxWorld, &mtxRot);

		//�ʒu���f
		D3DXMatrixTranslation(&mtxTrans, g_aPlayerResult[nCntPlayer].pos.x, g_aPlayerResult[nCntPlayer].pos.y, g_aPlayerResult[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].mtxWorld, &g_aPlayerResult[nCntPlayer].mtxWorld, &mtxTrans);

		//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerResult[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p
			D3DXMATRIX mtxParent;					//�e�̃}�g��

			//�����V�d�l
			//���ŃI�t�Z�b�g�����̂܂܎g���i�A�j���[�V�����g���悤�ɂȂ���������āj
			g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos = useAnimal.aParts[nCntParts].posOffset;
			g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot = useAnimal.aParts[nCntParts].rotOffset;

			//"���f����"���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot.y, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot.x, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].rot.z);
			D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTransModel, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos.x, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos.y, g_aPlayerResult[nCntPlayer].animalInst[nCntParts].pos.z);
			D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

			//�p�[�c�̐e�}�g���ݒ�
			if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
			{
				mtxParent = g_aPlayerResult[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_aPlayerResult[nCntPlayer].mtxWorld;
			}

			//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
			D3DXMatrixMultiply(&g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

			//"���f����"���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerResult[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^�擾
			pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

				//���f���`��
				useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
			}
		}
		
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//���U���g�p�v���C���[�̏����擾
Player_RESULT *GetPlayer_RESULT(void)
{
	return &g_aPlayerResult[0];
}
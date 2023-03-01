//==========================================
//
//���U���g�v���C���[�v���O�����̏���[hdr_player.cpp]
//Author:���V�����@�Ό��D�n
//
//==========================================
#include "resultPlayer.h"

//�}�N����`

//�O���[�o���ϐ�
Player_RESULT g_aPlayerRusult[MAX_USE_GAMEPAD];

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
		g_aPlayerRusult[nCntPlayer].pos = ZERO_SET;
		g_aPlayerRusult[nCntPlayer].posOld = ZERO_SET;
		g_aPlayerRusult[nCntPlayer].move = ZERO_SET;
		g_aPlayerRusult[nCntPlayer].rot = ZERO_SET;
		g_aPlayerRusult[nCntPlayer].bHipDrop = false;
		g_aPlayerRusult[nCntPlayer].bHipDropSpin = false;
		g_aPlayerRusult[nCntPlayer].nHipDropWait = 0;

		g_aPlayerRusult[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayerRusult[nCntPlayer].nRank = -1;

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aPlayerRusult[nCntPlayer].animalInst[nCntParts] = {};
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
		/*
		Model useAnimal = GetAnimal(g_aPlayerRusult[nCntPlayer].animal);

		//"�v���C���[��"���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_aPlayerRusult[nCntPlayer].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerRusult[nCntPlayer].rot.y, g_aPlayerRusult[nCntPlayer].rot.x, g_aPlayerRusult[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_aPlayerRusult[nCntPlayer].mtxWorld, &g_aPlayerRusult[nCntPlayer].mtxWorld, &mtxRot);

		//�ʒu���f
		D3DXMatrixTranslation(&mtxTrans, g_aPlayerRusult[nCntPlayer].pos.x, g_aPlayerRusult[nCntPlayer].pos.y, g_aPlayerRusult[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_aPlayerRusult[nCntPlayer].mtxWorld, &g_aPlayerRusult[nCntPlayer].mtxWorld, &mtxTrans);

		//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerRusult[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p
			D3DXMATRIX mtxParent;					//�e�̃}�g��

			//�����V�d�l
			//���ŃI�t�Z�b�g�����̂܂܎g���i�A�j���[�V�����g���悤�ɂȂ���������āj
			g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].pos = useAnimal.aParts[nCntParts].posOffset;
			g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].rot = useAnimal.aParts[nCntParts].rotOffset;

			//"���f����"���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].rot.y, g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].rot.x, g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].rot.z);
			D3DXMatrixMultiply(&g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTransModel, g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].pos.x, g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].pos.y, g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].pos.z);
			D3DXMatrixMultiply(&g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

			//�p�[�c�̐e�}�g���ݒ�
			if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
			{
				mtxParent = g_aPlayerRusult[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_aPlayerRusult[nCntPlayer].mtxWorld;
			}

			//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
			D3DXMatrixMultiply(&g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

			//"���f����"���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerRusult[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^�擾
			pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&matChange);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

				//���f���`��
				useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
			}
		}
		*/
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//���U���g�p�v���C���[�̏����擾
Player_RESULT *GetPlayer_RESULT(void)
{
	return &g_aPlayerRusult[0];
}
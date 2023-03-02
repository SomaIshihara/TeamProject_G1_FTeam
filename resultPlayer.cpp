//==========================================
//
//���U���g�v���C���[�v���O�����̏���[hdr_player.cpp]
//Author:���V�����@�Ό��D�n
//
//==========================================
#include "resultPlayer.h"
#include "select_game.h"
#include "VictoryStand.h"
#include "color.h"

//�}�N����`
#define FALL_RES_PLAYER			(3.0f)		//���U���g�v���C���[�̗������x

//�O���[�o���ϐ�
Player_RESULT g_ResultPlayer[MAX_USE_GAMEPAD];				/*���U���g�p�v���C���[�̏��*/
const D3DXVECTOR3 c_ResultPlayerPos[MAX_USE_GAMEPAD] = {	/*�v���C���[�̏����ʒu�ݒ�*/
	D3DXVECTOR3(-75.0f, 300.0f, 0.0f),
	D3DXVECTOR3(-25.0f, 300.0f, 0.0f),
	D3DXVECTOR3(+25.0f, 300.0f, 0.0f),
	D3DXVECTOR3(+75.0f, 300.0f, 0.0f),
};

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
		g_ResultPlayer[nCntPlayer].pos = 
		g_ResultPlayer[nCntPlayer].posOld = c_ResultPlayerPos[nCntPlayer];
		g_ResultPlayer[nCntPlayer].move = ZERO_SET;
		g_ResultPlayer[nCntPlayer].rot = ZERO_SET;
		g_ResultPlayer[nCntPlayer].bHipDrop = false;
		g_ResultPlayer[nCntPlayer].bHipDropSpin = false;
		g_ResultPlayer[nCntPlayer].nHipDropWait = 0;

		//�Q�[�����[�h�ɉ����Ď擾������̂�ύX
		if (GetSelGameMode() == SelGameMode_PVP)
		{
			g_ResultPlayer[nCntPlayer].animal = GetPlayer()[nCntPlayer].animal;			//�v���C���[�̃��f�����擾
		}
		else if (GetSelGameMode() == SelGameMode_HDR)
		{
			g_ResultPlayer[nCntPlayer].animal = GetPlayer_HDR()[nCntPlayer].animal;
		}

		g_ResultPlayer[nCntPlayer].nRank = -1;

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts] = {};
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
		//�Ώۂ̃v���C���[�̃|�C���^���擾
		Player_RESULT *pPlayer = &g_ResultPlayer[nCntPlayer];

		//�O��̈ʒu���L��
		pPlayer->posOld = pPlayer->pos;

		//�v���C���[�𗎉�������
		pPlayer->pos.y -= FALL_RES_PLAYER;

		//�\����ւ̓����蔻��
		CollisionVictoryStand(&pPlayer->pos, &pPlayer->posOld, &pPlayer->move);
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
		//�v���C���[�̃��f�����擾
		Model useAnimal = GetAnimal(g_ResultPlayer[nCntPlayer].animal);

		//"�v���C���[��"���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_ResultPlayer[nCntPlayer].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ResultPlayer[nCntPlayer].rot.y, g_ResultPlayer[nCntPlayer].rot.x, g_ResultPlayer[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].mtxWorld, &g_ResultPlayer[nCntPlayer].mtxWorld, &mtxRot);

		//�ʒu���f
		D3DXMatrixTranslation(&mtxTrans, g_ResultPlayer[nCntPlayer].pos.x, g_ResultPlayer[nCntPlayer].pos.y, g_ResultPlayer[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].mtxWorld, &g_ResultPlayer[nCntPlayer].mtxWorld, &mtxTrans);

		//"�v���C���[��"���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_ResultPlayer[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//�v�Z�p
			D3DXMATRIX mtxParent;					//�e�̃}�g��

			//�����V�d�l
			//���ŃI�t�Z�b�g�����̂܂܎g���i�A�j���[�V�����g���悤�ɂȂ���������āj
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos = useAnimal.aParts[nCntParts].posOffset;
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot = useAnimal.aParts[nCntParts].rotOffset;

			//"���f����"���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.y, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.x, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.z);
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTransModel, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.x, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.y, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.z);
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

			//�p�[�c�̐e�}�g���ݒ�
			if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
			{
				mtxParent = g_ResultPlayer[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_ResultPlayer[nCntPlayer].mtxWorld;
			}

			//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

			//"���f����"���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld);

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

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&matChange);

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
	return &g_ResultPlayer[0];
}
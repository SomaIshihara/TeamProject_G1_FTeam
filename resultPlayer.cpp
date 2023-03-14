//==========================================
//
//���U���g�v���C���[�v���O�����̏���[resultPlayer.cpp]
//Author:���V�����@�Ό��D�n
//
//==========================================
#include "resultPlayer.h"
#include "select_game.h"
#include "VictoryStand.h"
#include "debugproc.h"
#include "color.h"
#include "sound.h"

//�}�N����`
#define JUMP_RES_PLAYER			(6.0f)		//���U���g�v���C���[�̃W�����v��
#define ADVANCE_RES_PLAYER		(-2.4f)		//���U���g�v���C���[�̑O�i���x
#define FALL_RES_PLAYER			(-4.0f)		//���U���g�v���C���[�̗������x
#define GRAVITY_MAG				(0.03f)		//�d�͌v�Z�ɗp����{��
#define PLAYER_HIPDROPWAIT		(10)		//�q�b�v�h���b�v�J�n�̍d���t���[��
#define PLAYER_HIPDROP_SPEED	(-8.0f)		//�q�b�v�h���b�v���̗������x
#define PLAYER_HIPSPIN_SPEED	(-0.5f)		//�q�b�v�h���b�v�X�s���̉�]�l
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//�q�b�v�h���b�v�X�s�������Ƃ��̂P��������Ƃ�l

//�O���[�o���ϐ�
Player_RESULT g_ResultPlayer[MAX_USE_GAMEPAD];			/*���U���g�p�v���C���[�̏��*/
const D3DXVECTOR3 c_ResultPlayerPos[MAX_USE_GAMEPAD] = {	/*�v���C���[�̏����ʒu�ݒ�*/
	D3DXVECTOR3(-75.0f, 700.0f, 80.0f),
	D3DXVECTOR3(-25.0f, 700.0f, 80.0f),
	D3DXVECTOR3(+25.0f, 700.0f, 80.0f),
	D3DXVECTOR3(+75.0f, 700.0f, 80.0f),
};

//========================
//����������
//========================
void InitPlayer_RESULT(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���U���g�p�v���C���[�̃|�C���^���擾
	Player_RESULT *pResPlayer = &g_ResultPlayer[0];

	//�ϐ�������
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++, pResPlayer++)
	{
		//�ϐ�������
		*pResPlayer = ResetResultPlayerInfo(nCntPlayer);
	}

	//�|�C���^���Ď擾
	pResPlayer = &g_ResultPlayer[0];

	//�Q�[�����[�h�ɉ����Ď擾������̂�ύX
	switch (GetSelGameMode())
	{
	case SelGameMode_PVP:	//�Q�[�����[�h�uPVP�v�̏ꍇ�̏���������
		InitResultPlayerType_GAMEMODE_PVP(pResPlayer);
		break;

	case SelGameMode_HDR:	//�Q�[�����[�h�uHDR�v�̏ꍇ�̏���������
		InitResultPlayerType_GAMEMODE_HDR(pResPlayer);
		break;
	}
}

//========================
//�Q�[�����[�h�� PVP �̂Ƃ��̏���������
//========================
void InitResultPlayerType_GAMEMODE_PVP(Player_RESULT *pResPlayer)
{
	//PVP �v���C���[�̃|�C���^�擾
	Player *pPVPPlayer = GetPlayer();

	for (int nCntPVP = 0; nCntPVP < MAX_USE_GAMEPAD; nCntPVP++, pPVPPlayer++, pResPlayer++)
	{
		//�v���C���[�̃X�R�A���擾
		pResPlayer->nRank = pPVPPlayer->nScore;
	}
}

//========================
//�Q�[�����[�h�� HDR �̂Ƃ��̏���������
//========================
void InitResultPlayerType_GAMEMODE_HDR(Player_RESULT *pResPlayer)
{
	//HDR �̃v���C���[���擾
	Player_HDR *pHDRPlayer = GetPlayer_HDR();

	for (int nCntHDR = 0; nCntHDR < MAX_USE_GAMEPAD; nCntHDR++, pResPlayer++, pHDRPlayer++)
	{
		//�v���C���[�̏��ʂ��擾
		pResPlayer->nRank = pHDRPlayer->nRank;
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
	//�Ώۂ̃v���C���[�̃|�C���^���擾
	Player_RESULT *pPlayer = &g_ResultPlayer[0];

	//�v���C���[�l�����J��Ԃ�
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++, pPlayer++)
	{
		//�O��̈ʒu���L��
		pPlayer->posOld = pPlayer->pos;

		//�v���C���[�̏d�͏���
		UpdatePosPlayer_RESULT(pPlayer);

		//��~����
		StopPlayer_RESULT(pPlayer, nCntPlayer);

		//�q�b�v�h���b�v�X�s������
		HipSpinResPlayer(pPlayer);

		//�\����ւ̓����蔻��
		CollisionVictoryStand(&pPlayer->pos, &pPlayer->posOld);
	}
}

//�d�͏���
void UpdatePosPlayer_RESULT(Player_RESULT *pPlayer)
{
	//�v���C���[���q�b�v�h���b�v�X�s�������Ă��Ȃ����
	if (!pPlayer->bHipDropSpin)
	{
		//�v���C���[����э���ł�����
		if (pPlayer->bDive)
		{
			//�������x�㏸
			pPlayer->fFallSpeed += (FALL_RES_PLAYER - pPlayer->fFallSpeed) * GRAVITY_MAG;
		}

		//�q�b�v�h���b�v�d��������
		if (0 < pPlayer->nHipDropWait)
		{
			//�d���t���[�����Z
			pPlayer->nHipDropWait--;
		}

		//�q�b�v�h���b�v�d�����Ȃ�
		else
		{
			//�x���W�X�V								//�y���W�X�V
			pPlayer->pos.y += pPlayer->fFallSpeed;		pPlayer->pos.z += pPlayer->fAdvance;
		}
	}
}

//��~����
void StopPlayer_RESULT(Player_RESULT *pPlayer, int nCntPlayer)
{
	//�v���C���[���y�̌��_�ʒu�𒴂��Ď�O�ɗ��� and �q�b�v�h���b�v���Ă��Ȃ�
	if (pPlayer->pos.z <= 0.0f && !pPlayer->bHipDrop)
	{
		pPlayer->pos.z =				//���_�ʒu�ɖ߂�
		pPlayer->fAdvance = 0.0f;		//�O�i���x���O�ɂ���
		pPlayer->bHipDrop =				//�q�b�v�h���b�v��
		pPlayer->bHipDropSpin = true;	//�q�b�v�h���b�v�X�s�����J�n����
		pPlayer->nHipDropWait = PLAYER_HIPDROPWAIT; //�q�b�v�h���b�v�̊J�n�d����ݒ�
		pPlayer->fFallSpeed = PLAYER_HIPDROP_SPEED;	//�q�b�v�h���b�v�̏�������

		PlaySoundSE(SOUND_LABEL_SE_HIPSPIN, nCntPlayer);//�q�b�v�h���b�v���Đ�
	}
}

//�q�b�v�h���b�v�X�s������
void HipSpinResPlayer(Player_RESULT *pPlayer)
{
	//�������v���C���[���q�b�v�h���b�v�X�s������������
	if (pPlayer->bHipDropSpin)
	{
		//��]�l�����Z
		pPlayer->rot.x += PLAYER_HIPSPIN_SPEED;

		//�P������
		if (pPlayer->rot.x <= PLAYER_HIPSPIN_LAP)
		{
			pPlayer->rot.x = 0.0f;			//��]�l�����ɖ߂�
			pPlayer->bHipDropSpin = false;	//�X�s�����I�����
		}
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

//�v���C���[�̔�э��݊J�n
void SetDivePlayer(void)
{
	//�v���C���[�̃|�C���^�擾
	Player_RESULT *pPlayer = &g_ResultPlayer[0];

	for (int nCntDive = 0; nCntDive < MAX_USE_GAMEPAD; nCntDive++, pPlayer++)
	{
		//�v���C���[���_�C�u���Ă��Ȃ�
		if (!pPlayer->bDive)
		{
			pPlayer->fFallSpeed = JUMP_RES_PLAYER;	// �����������x�i��э��݂Ȃ̂ŁA����l�̓W�����v�ʂƓ����j����
			pPlayer->fAdvance = ADVANCE_RES_PLAYER;	// �O�i���x���
			pPlayer->bDive = true;					// ��э���
			pPlayer->bHipDropSpin = false;			// �q�b�v�h���b�v�X�s�������Ă��Ȃ�

			break;	// �������I����
		}
	}
}

//���U���g�p�v���C���[�̏����擾
Player_RESULT *GetPlayer_RESULT(void)
{
	return &g_ResultPlayer[0];
}

//���U���g�p�v���C���[�̍\���̏�����
Player_RESULT ResetResultPlayerInfo(int nCntResetPlayer)
{
	return {
		c_ResultPlayerPos[nCntResetPlayer],	// �ʒu������
		c_ResultPlayerPos[nCntResetPlayer],	// �O��ʒu������
		ZERO_SET,	// ����������
		0.0f,		// �������x������
		0.0f,		// �O�i���x������
		false,		// ��э��ݏ�����
		false,		// �q�b�v�h���b�v������
		false,		// �q�b�v�h���b�v�X�s��������
		0,			// �q�b�v�h���b�v�J�n�d��������
		-1,			// ���ʏ�����

		ANIMAL_WILDBOAR,	// �g�p���Ă��铮����񏉊���

		{},		// ���f����񏉊���
		{},		// �̃p�[�c�̏�񏉊���
		{},		// ���[���h�}�g���b�N�X������
		{},		// �v���C���[���f���̍\���̏�����

		false,	// �v���C���[�g�p�E�s�g�p������
	};
}
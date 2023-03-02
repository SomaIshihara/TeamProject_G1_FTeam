//============================================================================================
//
//�{�[�i�X�̏���[bonus.cpp]
//Author:�������P
//
//============================================================================================
#include"main.h"
#include"color.h"
#include"input.h"
#include"score.h"
#include"bonus.h"
#include"time.h"
#include"particle.h"

//****************************//
//		�@ �}�N����`		  //
//****************************//
#define INIT_POS_Y			(200.0f)	//������Y�ʒu
#define INIT_POS_XZ			(200.0f)	//�����̊O�ʒu
#define RATIO_MOVE			(100.0f)	//�ړ��ʂ̊���
#define COLLISION_SIZE_XZ	(60.0f)		//�c���̓����蔻��T�C�Y
#define COLLISION_SIZE_Y	(15.0f)		//�����̓����蔻��T�C�Y

#define DESPAWN_LIMIT_BONUS (1200)		//�{�[�i�X��������܂ł̃��~�b�g
#define PARTICLE_LIMIT_BONUS (6)		//�{�[�i�X�p�[�e�B�N���̃��~�b�g
#define LIFE_CT				(60)		//�{�[�i�X�����蔻��̃N�[���^�C��

//****************************//
//		�@�@�o�����		  //
//****************************//
//�ʒu
const D3DXVECTOR3 g_RespawnPos[] =
{
	D3DXVECTOR3(INIT_POS_XZ,INIT_POS_Y,NIL_F),
	D3DXVECTOR3(NIL_F,INIT_POS_Y,-INIT_POS_XZ),
	D3DXVECTOR3(-INIT_POS_XZ,INIT_POS_Y,NIL_F),
	D3DXVECTOR3(NIL_F,INIT_POS_Y,INIT_POS_XZ),
};

//�p�x
const D3DXVECTOR3 g_RespawnRot[] =
{
	ZERO_SET,
	D3DXVECTOR3(NIL_F, 1.57f,NIL_F),
	D3DXVECTOR3(NIL_F, 3.14f,NIL_F),
	D3DXVECTOR3(NIL_F,-1.57f,NIL_F),
};

//****************************//
//	   �O���[�o���ϐ��錾     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTextureBonus[10] = {};	//�e�N�X�`���ւɃ|�C���^
LPD3DXMESH				g_pMeshBonus	    = NULL;	//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatBonus		= NULL;	//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatBonus		= 0;	//�}�e���A���̐�
D3DXMATRIX				g_mtxWorldBonus;			//���[���h�}�g���b�N�X
Bonus					g_Bonus;					//�{�[�i�X�̏��
int						g_ParticleCounter;
int						g_LifeCT;					//�̗͂̃N�[���^�C��
int						g_PlayerNumTemp;				//�v���C���[�ԍ��̕ۑ��p
//===================================================
//�{�[�i�X�̏���������
//===================================================
void InitBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATERIAL	  *pMat;					//�}�e���A���ւ̃|�C���^

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX({ "data/MODEL/bird/bonus_bird.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBonus,
		NULL,
		&g_dwNumMatBonus,
		&g_pMeshBonus);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatBonus->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���@
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureBonus[nCntMat]);
		}
	}

	//�����ݒ�
	g_Bonus.Respawn = DIRECTION_ZERO;			//���X�|�[���̈ʒu�ԍ�
	g_Bonus.pos = ZERO_SET;						//�ʒu
	g_Bonus.rot = ZERO_SET;						//�p�x
	g_Bonus.move = ZERO_SET;					//�ړ���
	g_Bonus.DespawnLimit = 0;					//�����鎞��
	g_Bonus.a = 0.0f;							//�����x�̐ݒ�
	g_Bonus.buse = false;						//�g�p���Ă��邩�ǂ���
	g_ParticleCounter = PARTICLE_LIMIT_BONUS;	//�p�[�e�B�N���̂ł�Ԋu
	g_Bonus.nLife = 3;							//�̗�		
	g_LifeCT = 0;
	g_PlayerNumTemp = -1;
}
//===================================================
//�{�[�i�X�̏I������
//===================================================
void UninitBonus(void)
{   
	//���b�V���̔j��
	if (g_pMeshBonus != NULL)
	{
		g_pMeshBonus->Release();
		g_pMeshBonus = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatBonus != NULL)
	{
		g_pBuffMatBonus->Release();
		g_pBuffMatBonus = NULL;
	}
}
//===================================================
//�{�[�i�X�̍X�V����
//===================================================
void UpdateBonus(void)
{
	if (g_Bonus.buse == true)
	{//�g�p����Ă���Ƃ�

		//�p�[�e�B�N���Z�b�g�܂ł̐�������
		g_ParticleCounter--;

		if (g_ParticleCounter <= 0)
		{//0�ɂȂ����Ƃ�
		
			//�p�[�e�B�N���̃Z�b�g
			SetParticle(g_Bonus.pos, 12.0f, 15, PARTICLE_NORMAL,OBJECT_BONUS);
			SetParticle(g_Bonus.pos, 7.0f, 15, PARTICLE_ACSORPTION, OBJECT_BONUS);

			//���~�b�g�̍Đݒ�
			g_ParticleCounter = PARTICLE_LIMIT_BONUS;
		}

		//�ړ�����
		MoveBonus();

		//���o������
		AppearandDisAppearBonus();

		//�ʒu�̍X�V
		g_Bonus.pos.x += g_Bonus.move.x;
		g_Bonus.pos.z += g_Bonus.move.z;

		//�����̐ݒ�
		g_Bonus.move.x += (NIL_F - g_Bonus.move.x) * 1.0f;
		g_Bonus.move.z += (NIL_F - g_Bonus.move.z) * 1.0f;
	}
}
//===================================================
//�{�[�i�X�̕`�揈��
//===================================================
void DrawBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX		  mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9	  matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL	  *pMat;					//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (g_Bonus.buse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldBonus);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Bonus.rot.y, g_Bonus.rot.x, g_Bonus.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldBonus, &g_mtxWorldBonus, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_Bonus.pos.x, g_Bonus.pos.y, g_Bonus.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldBonus, &g_mtxWorldBonus, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBonus);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatBonus->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
		{
			//�}�e���A���̐F�ݒ�
			pMat[nCntMat].MatD3D.Diffuse.a = g_Bonus.a;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBonus[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshBonus->DrawSubset(nCntMat);
		}

		//************************//
		//************************//
		//		  �e�̕`��		  //
		//************************//
		//************************//

		D3DXMATRIX	mtxShadow;		//�V���h�E�}�g���b�N�X
		D3DLIGHT9	light;			//���C�g���
		D3DXVECTOR4	posLight;		//���C�g�̈ʒu
		D3DXVECTOR3	pos, normal;	//���ʏ�̔C�ӂ̓_�A�@���x�N�g��
		D3DXPLANE	plane;			//���ʏ��

									//���C�g�̈ʒu��ݒ�
		pDevice->GetLight(3, &light);
		posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

		//���ʏ��𐶐�
		pos = ZERO_SET;
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		//�V���h�E�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxShadow);

		//�V���h�E�}�g���b�N�X�̍쐬
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &g_mtxWorldBonus, &mtxShadow);

		//�V���h�E�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatBonus->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
		{
			D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//�}�e���A���f�[�^����

			//���F�ɐݒ�						//���Ȕ����𖳂���
			MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

			//�}�e���A���ݒ�
			pDevice->SetMaterial(&MatCopy);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, NULL);

			//���f���`��
			g_pMeshBonus->DrawSubset(nCntMat);
		}
	}
	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}
//===================================================
//�{�[�i�X�̐ݒ菈��
//===================================================
void SetBonus(void)
{
	if (g_Bonus.buse == false)
	{
		//���ݎ��Ԃ̎擾
		srand((unsigned int)time(0));

		//�����ݒ�
		g_Bonus.Respawn = (BONUS)(rand() % 4);
		g_Bonus.pos = g_RespawnPos[g_Bonus.Respawn];
		g_Bonus.rot = g_RespawnRot[g_Bonus.Respawn];
		g_Bonus.DespawnLimit = DESPAWN_LIMIT_BONUS;
		g_Bonus.buse = true;
	}
}
//===================================================
//�{�[�i�X�̈ړ�����
//===================================================
void MoveBonus(void)
{
	//�p�x���Z
	g_Bonus.rot.y += 0.01f;

	//�ړ������ɉ���������
	switch (g_Bonus.Respawn)
	{
	case DIRECTION_ZERO:

		//�ړ��ʐݒ�
		g_Bonus.move.x += sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ZERO].x / RATIO_MOVE;
		g_Bonus.move.z += cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ZERO].x / RATIO_MOVE;

		break;

	case DIRECTION_ONE:

		//�ړ��ʐݒ�
		g_Bonus.move.x -= sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ONE].z / RATIO_MOVE;
		g_Bonus.move.z -= cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ONE].z / RATIO_MOVE;

		break;

	case DIRECTION_TWO:

		//�ړ��ʐݒ�
		g_Bonus.move.x -= sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_TWO].x / RATIO_MOVE;
		g_Bonus.move.z -= cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_TWO].x / RATIO_MOVE;

		break;

	case DIRECTION_THREE:

		//�ړ��ʐݒ�
		g_Bonus.move.x += sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_THREE].z / RATIO_MOVE;
		g_Bonus.move.z += cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_THREE].z / RATIO_MOVE;

		break;
	}
}
//===================================================
//�{�[�i�X�̏��o������
//===================================================
void AppearandDisAppearBonus(void)
{
	//������܂ł̃J�E���g�_�E��
	g_Bonus.DespawnLimit--;

	if (g_Bonus.DespawnLimit <= 0)
	{
		if (g_Bonus.a > 0.0f)
		{
			//�����ɂ��Ă���
			g_Bonus.a -= 0.01f;
		}
		else
		{
			//�g���Ă��Ȃ���Ԃɂ���
			g_Bonus.buse = false;
		}
	}
	else if (g_Bonus.a < 1.0f)
	{
		//�s�����ɂ��Ă���
		g_Bonus.a += 0.01f;
	}
}
//===================================================
//�{�[�i�X�̓����蔻�菈��
//===================================================
void CollisionBonus(D3DXVECTOR3 nPlayer , int NumPlayer)
{
	if (g_LifeCT > 0)
	{
		g_LifeCT--;
	}
	else
	{
		g_PlayerNumTemp = -1;
	}

	if (g_Bonus.buse == true && g_PlayerNumTemp != NumPlayer)
	{//�g�p����Ă���Ƃ�
		if (nPlayer.x >= g_Bonus.pos.x - COLLISION_SIZE_XZ
			&&nPlayer.x <= g_Bonus.pos.x + COLLISION_SIZE_XZ
			&&nPlayer.z >= g_Bonus.pos.z - COLLISION_SIZE_XZ
			&&nPlayer.z <= g_Bonus.pos.z + COLLISION_SIZE_XZ
			&&nPlayer.y >= g_Bonus.pos.y - COLLISION_SIZE_Y
			&&nPlayer.y <= g_Bonus.pos.y + COLLISION_SIZE_Y)
		{//�v���C���[���{�[�i�X�͈͓̔��ɓ������Ƃ�

			if (g_Bonus.nLife > 0)
			{
				//�̗͂̌���

				g_Bonus.nLife--;

				//�N�[���^�C���̐ݒ�
				g_LifeCT = LIFE_CT;

				//�G�ꂽ�v���C���[�̕ۑ�
				g_PlayerNumTemp = NumPlayer;
			}

			if (g_Bonus.nLife <= 0)
			{
				//�g���Ă��Ȃ���Ԃɂ���
				g_Bonus.buse = false;

				//�X�R�A�����Z
				AddScore(3, NumPlayer);
			}
		}
	}
}
//====================================================
//�{�[�i�X�̎擾����
//====================================================
Bonus GetBonus(void)
{
	return g_Bonus;
}
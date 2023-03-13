/*==========================================================================================================================================================

�J�����̏���[HDR_camera.cpp]
Author:�ѓc�F�� (��{����  ���V����  �Ό��D�n)

============================================================================================================================================================*/
#include "HDR_camera.h"
#include "camera.h"
#include "input.h"
#include "HDR_player.h"
#include "camera_frame.h"
#include "input.h"

//���_���
#define POSV_ROTSPEED	(0.05f)		//���_�̉�]���x
#define POSV_SPEED		(10.0f)		//���_�̈ړ����x
#define POSR_ADD		(8.0f)		//�����_�̏㉺�ړ�
#define CAMERA_LENGTH	(375.0f)	//�J�����Ԃ̋���

//������x�N�g�����
#define VECU_OVER		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	//������x�N�g��

//�J�������
#define ANGLE_OF_VIEW	(45.0f)		//��p

//�`��͈�
#define MIN_DRAW		(10.0f)		//�ŏ��`��͈�
#define MAX_DRAW		(40000.0f)	//�ő�`��͈�

//�ʒu�̍�����2��
#define DIFF_TIMES		(2.0f)		//2��

//�J�����P�̂̏ꍇ�̎��_���W
#define ALONE_CAMERA_POS	(D3DXVECTOR3(0.0f, 400.0f, 500.0f))

//�Q�l�̎��_���̃}�N��
#define SPS_posR_HEIGHT	(100.0f)		//�����_�̍���
#define SPS_posV_HEIGHT	(140.0f)		//���_�̍���

//�J���������ɂ�����}�N����`
#define SETUP_FLOAT_SPEED		(30.0f)		//�J�����̏㏸��	(float �́u�����v�Ƃ����Ӗ�

//�O���[�o���ϐ�
HDRCamera	g_HDRCamera[NUM_HDRCAMERA];	//�J�����̏��
NumCamera	g_NumHDRCameraType;			//�J���������̏��
HDR_Ready	g_HDR_Ready;				//�J�����������

//=========================================
//�J�����̈ʒu�ݒ菈��
//Author:�Ό��D�n
//=========================================
void InitSetHDRCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumHDRCamera)
{
	//�J�����̃|�C���^�擾
	HDRCamera *pCamera = &g_HDRCamera[nNumHDRCamera];

	pCamera->posV = posV;	//���_
	pCamera->posR = posR;	//�����_

	//������x�N�g�������Œ�
	pCamera->vecU = VECU_OVER;

	//���ꂼ��̈ʒu�̍������Z�o
	float PosDiffX = powf(pCamera->posR.x - pCamera->posV.x, DIFF_TIMES);
	float PosDiffZ = powf(pCamera->posR.z - pCamera->posV.z, DIFF_TIMES);

	//�����̎Z�o
	pCamera->fLength = sqrtf(PosDiffX + PosDiffZ);

	//�p�x���Z�o
	pCamera->rot.y = 0.0f;
}

//=========================================
//�J�����̏���������
//=========================================
void InitHDRCamera(NumCamera type)
{
	Set_NumHDRCamera(type);	//�J���������̏�����
	g_HDR_Ready = HDR_Ready_SETUP;//�J����������
	HDRCamera *pCamera = &g_HDRCamera[0];

	//�J�����̏�񏉊�������
	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pCamera++)
	{	
		pCamera->ready = HDR_Ready_SETUP;	//�J��������������
		pCamera->posV.x = 0.0f;				//���_�͌��_�ɂƂǂ܂�
		pCamera->posR.x = GetPlayer_HDR()[nCntHDRCamera].pos.x;	//�����_���e�v���C���[��

																//���ꂼ��̈ʒu�̍������Z�o
		float PosDiffX = powf(pCamera->posR.x - pCamera->posV.x, DIFF_TIMES);
		float PosDiffZ = powf(pCamera->posR.z - pCamera->posV.z, DIFF_TIMES);

		//�����̎Z�o
		pCamera->fLength = sqrtf(PosDiffX + PosDiffZ);

		//�p�x���Z�o
		pCamera->rot.y = -atan2f(PosDiffX, PosDiffZ);
	}
}

//=========================================
//�J�����̏I������
//=========================================
void UninitHDRCamera(void)
{

}

//=========================================
//�J�����̍X�V����
//=========================================
void UpdateHDRCamera(void)
{
	Player_HDR *pPlayer = GetPlayer_HDR();	//�v���C���[�̃|�C���^�[���擾
	HDRCamera *pCamera = &g_HDRCamera[0];	//�J�����̃|�C���^�[���擾

	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pCamera++, pPlayer++)
	{
		pCamera->posR.x = pPlayer->pos.x;	//�����_
		pCamera->posR.z = pPlayer->pos.z;	//�����_
		pCamera->posV.x = 0.0f;
		
		//�J������������
		if (g_HDR_Ready == HDR_Ready_SETUP)
		{
			//�J��������
			SetUpHDRCamera(pCamera, pPlayer->pos.y);
		}

		//��������
		else if (g_HDR_Ready == HDR_Ready_OK)
		{
			//�v���C���[���S�[��������
			if (pPlayer->bGoal)
			{
				//�v���C���[�̐��ʂɌ����悤�ɏ��X�Ɋp�x��␳
				pCamera->rot.y += (0.0f - pCamera->rot.y) * 0.3f;
				pCamera->fLength = CAMERA_LENGTH;

				//���_�̈ʒu�ݒ�
				UpdatePosVHDRCamera(pCamera);
			}

			//�����_�̈ʒu�ݒ�
			SetPosRHDRCamera(pCamera, pPlayer->pos.y);
		}
	}

	//�Z�b�g�A�b�v�������ǂ�������
	SetUpCheckHDRCamera();
}

//�J������������
void SetUpHDRCamera(HDRCamera *pCamera, float PlayerPosY) 
{
	//���_�E�����_���㏸
	pCamera->posR.y += SETUP_FLOAT_SPEED;
	pCamera->posV.y += SETUP_FLOAT_SPEED;

	//�v���C���[�̈ʒu���A�����_����ɍs����
	if (PlayerPosY <= pCamera->posR.y)
	{
		pCamera->posR.y = PlayerPosY + SPS_posR_HEIGHT;	//�����_�̈ʒu�C��
		pCamera->posV.y = PlayerPosY + SPS_posV_HEIGHT;	//���_�̈ʒu�C��
		pCamera->ready = HDR_Ready_OK;					//����OK
	}
}

//�Z�b�g�A�b�v������Ԍ�������
void SetUpCheckHDRCamera(void)
{
	//-------------------------------------
	//���łɏ����������Ă���Ȃ珈�����Ȃ�
	//-------------------------------------
	if (g_HDR_Ready == HDR_Ready_OK)
	{
		return;
	}

	int nSetUpCounter = 0;	//�����������Ă��鐔���i�[

	for (int nCntSetUp = 0; nCntSetUp < NUM_HDRCAMERA; nCntSetUp++)
	{
		//�J����������OK���ǂ���
		if (g_HDRCamera[nCntSetUp].ready == HDR_Ready_OK)
		{
			//�����������Ă��鐔�����Z
			nSetUpCounter++;
		}
	}

	//�����������Ă��鐔���A�J�����̑䐔�Ɠ���
	if (nSetUpCounter == NUM_HDRCAMERA)
	{
		//�S�J������������
		g_HDR_Ready = HDR_Ready_OK;
	}
}

//�J�����̈ړ�����
void MoveHDRCamera(HDRCamera *pCamera)
{
#ifdef _DEBUG
	//----------------------
	//		���_�̈ړ�	
	//----------------------
	//���_�̏㉺
	if (GetKeyboardPress(DIK_T) == true)
	{
		pCamera->posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		pCamera->posV.y -= POSV_SPEED;
	}

	//���_�̑O��
	if (GetKeyboardPress(DIK_N) == true)
	{
		pCamera->fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		pCamera->fLength -= POSV_SPEED;
	}

	//���_�̍��E
	if (GetKeyboardPress(DIK_Z) == true)
	{
		pCamera->rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		pCamera->rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		�����_�̈ړ�	
	//------------------------
	//�����_�̏㉺
	if (GetKeyboardPress(DIK_I) == true)
	{
		pCamera->posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		pCamera->posR.y -= POSR_ADD;
	}
#endif // _DEBUG
}

//�����_�̈ʒu�ݒ�
void SetPosRHDRCamera(HDRCamera *pCamera, float PlayerPosY)
{
	//�v���C���[�̈ʒu���A�����_�����Ⴉ�����ꍇ
	if (PlayerPosY < pCamera->posR.y - SPS_posR_HEIGHT && g_HDR_Ready == HDR_Ready_OK)
	{
		pCamera->posR.y = PlayerPosY + SPS_posR_HEIGHT;	//�Ώۂ̃v���C���[�ɒ����_�����킹��
		pCamera->posV.y = PlayerPosY + SPS_posV_HEIGHT;	//�Ώۂ̃v���C���[�Ɏ��_�����킹��
	}
}

//�Q�l�̎��_
void SPS_ChaseHDRCamera(HDRCamera *pCamera)
{
	//�v���C���[�̐��ʂɌ����悤�ɏ��X�Ɋp�x��␳
	pCamera->rot.y -= (0.0f - pCamera->rot.y) * 0.3f;

	//���_�̈ʒu�X�V
	pCamera->posV.x = pCamera->posR.x + sinf(D3DX_PI - pCamera->rot.y) * pCamera->fLength;
	pCamera->posV.z = pCamera->posR.z + cosf(D3DX_PI - pCamera->rot.y) * pCamera->fLength;
}

//���_�̈ʒu�X�V
void UpdatePosVHDRCamera(HDRCamera *pCamera)
{
	//���_�̈ʒu�X�V
	pCamera->posV.x = pCamera->posR.x + sinf(pCamera->rot.y - D3DX_PI) * pCamera->fLength;
	pCamera->posV.z = pCamera->posR.z + cosf(pCamera->rot.y - D3DX_PI) * pCamera->fLength;
}

//=========================================
//�J�����̐ݒ菈��
//=========================================
void SetHDRCamera(int nIdx)
{
	//�J�������g���Ă���
	if (g_HDRCamera[nIdx].bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//�r���[�|�[�g�̐ݒ�
		pDevice->SetViewport(&g_HDRCamera[nIdx].viewport);

		//�v���W�F�N�V�����}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_HDRCamera[nIdx].mtxProjection);

		//�v���W�F�N�V�����}�g���b�N�X�̍쐬
		D3DXMatrixPerspectiveFovLH(&g_HDRCamera[nIdx].mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_HDRCamera[nIdx].viewport.Width / (float)g_HDRCamera[nIdx].viewport.Height, MIN_DRAW, g_HDRCamera[nIdx].fMaxLength);

		//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_PROJECTION, &g_HDRCamera[nIdx].mtxProjection);

		//�r���[�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_HDRCamera[nIdx].mtxView);

		//�r���[�}�g���b�N�X�̍쐬
		D3DXMatrixLookAtLH(&g_HDRCamera[nIdx].mtxView, &g_HDRCamera[nIdx].posV, &g_HDRCamera[nIdx].posR, &g_HDRCamera[nIdx].vecU);

		//�r���[�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_VIEW, &g_HDRCamera[nIdx].mtxView);
	}
}

//=========================================
//�J�����̑䐔�ʁ@�ݒ菈��
//=========================================
void Set_NumHDRCamera(NumCamera type)
{
	int nCntHDRCamera = 0;				//�J�E���^�[������
	g_NumHDRCameraType = type;			//�J�����̕������i�[
	Player_HDR *pPlayer_HDR = GetPlayer_HDR();	//�v���C���[�̏��擾
	HDRCamera *pCamera = &g_HDRCamera[0];		//�J�����̃|�C���^�擾

	switch (type)
	{
		/*----------------------------------------------------------------
		���ՂőS�̂������鎋�_�̏���
		----------------------------------------------------------------*/
	case NumHDRCamera_ONLY:
	{
		pCamera->posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
		pCamera->posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

		pCamera->fMaxLength = MAX_DRAW;			//�ő�`�拗���ݒ�

		pCamera->viewport.X = (DWORD)0.0f;		//���_�]�ʒu���
		pCamera->viewport.Y = (DWORD)0.0f;		//���_�x�ʒu���
		pCamera->viewport.Width = SCREEN_WIDTH;	//��ʕ�������
		pCamera->viewport.Height = SCREEN_HEIGHT;	//��ʍ���������
		pCamera->viewport.MinZ = 0.0f;
		pCamera->viewport.MaxZ = 1.0f;
		pCamera->bUse = true;
		nCntHDRCamera++;	//���̃J������
	}
	break;

	/*----------------------------------------------------------------
	���Q�����̎��_�̏���
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pPlayer_HDR++, pCamera++)
		{
			pCamera->posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
			pCamera->posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

			pCamera->fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			pCamera->viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntHDRCamera;	//���_�]�ʒu���
			pCamera->viewport.Y = (DWORD)NIL_F;									//���_�x�ʒu���
			pCamera->viewport.Width = SCREEN_WIDTH / 4;							//��ʕ�������
			pCamera->viewport.Height = SCREEN_HEIGHT;							//��ʍ���������
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(pCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�c�Q�����̎��_�̏���
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_HIGH_row:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA_HALF; nCntHDRCamera++, pPlayer_HDR++, pCamera++)
		{
			pCamera->posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
			pCamera->posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

			pCamera->fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			pCamera->viewport.X = (DWORD)NIL_F;										//���_�]�ʒu���
			pCamera->viewport.Y = (DWORD)(SCREEN_HEIGHT / 2) * (nCntHDRCamera % 2);	//���_�x�ʒu���
			pCamera->viewport.Width = SCREEN_WIDTH;									//��ʕ�������
			pCamera->viewport.Height = SCREEN_HEIGHT / 2;								//��ʍ���������
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(pCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�e�v���C���[��p�̃J�����̏���
	----------------------------------------------------------------*/
	case NumHDRCamera_FOUR_Separate:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pPlayer_HDR++, pCamera++)
		{
			pCamera->posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
			pCamera->posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

			pCamera->fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			pCamera->viewport.X = (SCREEN_WIDTH / 2) * (nCntHDRCamera % 2);	//���_�]�ʒu���
			pCamera->viewport.Y = (SCREEN_HEIGHT / 2) * (nCntHDRCamera / 2);	//���_�x�ʒu���
			pCamera->viewport.Width = SCREEN_WIDTH / 2;						//��ʕ�������
			pCamera->viewport.Height = SCREEN_HEIGHT / 2;						//��ʍ���������
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(pCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�e�v���C���[��p�̃J�����̏���
	----------------------------------------------------------------*/
	case NumCamera_FOUR_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_CAMERA; nCntHDRCamera++, pCamera++)
		{
			pCamera->fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			pCamera->viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntHDRCamera;	//���_�]�ʒu���
			pCamera->viewport.Y = (DWORD)NIL_F;								//���_�x�ʒu���
			pCamera->viewport.Width = SCREEN_WIDTH / 4;						//��ʕ�������
			pCamera->viewport.Height = SCREEN_HEIGHT;						//��ʍ���������
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(pCamera);
		}
	}
	}

	//��ʕ����̘g��ݒ�
	SetUseFrame(type);

	//�ݒ肳��Ȃ������J�������g�p���Ȃ��悤�ɂ���
	for (int nCntUse = nCntHDRCamera; nCntUse < NUM_HDRCAMERA; nCntUse++, pCamera++)
	{
		pCamera->bUse = false;
	}
}

//�J�����̎擾
HDRCamera *GetHDRCamera(void)
{
	return &g_HDRCamera[0];
}

//�J����������Ԃ̎擾
HDR_Ready *GetHDR_Ready(void)
{
	//������Ԃ�Ԃ�
	return &g_HDR_Ready;
}
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

	//���_�̈ʒu�X�V
	UpdatePosVHDRCamera(nNumHDRCamera);
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

		//���_�̈ʒu�X�V
		UpdatePosVHDRCamera(nCntHDRCamera);
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

	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
	{
		pCamera->posR.x = GetPlayer_HDR()[nCntHDRCamera].pos.x;	//�����_
		pCamera->posR.z = GetPlayer_HDR()[nCntHDRCamera].pos.z;	//�����_
		
		//�J������������
		if (g_HDR_Ready == HDR_Ready_SETUP)
		{
			//�J��������
			SetUpHDRCamera(pCamera, pPlayer->pos.y);
		}

		//�J�������g���Ă���
		else if (pCamera->bUse == true)
		{
			//�J�����̈ړ�����
			MoveHDRCamera(nCntHDRCamera);
		}

		UpdatePosVHDRCamera(nCntHDRCamera);
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
void MoveHDRCamera(int nCntHDRCamera)
{
#ifdef _DEBUG
	//----------------------
	//		���_�̈ړ�	
	//----------------------
	//���_�̏㉺
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_HDRCamera[nCntHDRCamera].posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		g_HDRCamera[nCntHDRCamera].posV.y -= POSV_SPEED;
	}

	//���_�̑O��
	if (GetKeyboardPress(DIK_N) == true)
	{
		g_HDRCamera[nCntHDRCamera].fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_HDRCamera[nCntHDRCamera].fLength -= POSV_SPEED;
	}

	//���_�̍��E
	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_HDRCamera[nCntHDRCamera].rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		g_HDRCamera[nCntHDRCamera].rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		�����_�̈ړ�	
	//------------------------
	//�����_�̏㉺
	if (GetKeyboardPress(DIK_I) == true)
	{
		g_HDRCamera[nCntHDRCamera].posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_HDRCamera[nCntHDRCamera].posR.y -= POSR_ADD;
	}
#endif // _DEBUG

	//�����_�̈ʒu�ݒ�
	SetPosRHDRCamera(nCntHDRCamera);
}

//�����_�̈ʒu�ݒ�
void SetPosRHDRCamera(int nCntHDRCamera)
{
	//�v���C���[�̂x���W���i�[
	float PlayerPos_Y = GetPlayer_HDR()[nCntHDRCamera].pos.y;

	//�v���C���[�̈ʒu���A�����_�������������ꍇ
	if (PlayerPos_Y < g_HDRCamera[nCntHDRCamera].posR.y - SPS_posR_HEIGHT && g_HDR_Ready == HDR_Ready_OK)
	{
		g_HDRCamera[nCntHDRCamera].posR.y = PlayerPos_Y + SPS_posR_HEIGHT;	//�Ώۂ̃v���C���[�ɒ����_�����킹��
		g_HDRCamera[nCntHDRCamera].posV.y = PlayerPos_Y + SPS_posV_HEIGHT;	//�Ώۂ̃v���C���[�Ɏ��_�����킹��
	}
}

//�Q�l�̎��_
void SPS_ChaseHDRCamera(int nCntHDRCamera)
{
	//�v���C���[�̐��ʂɌ����悤�ɏ��X�Ɋp�x��␳
	g_HDRCamera[nCntHDRCamera].rot.y -= (0.0f - g_HDRCamera[nCntHDRCamera].rot.y) * 0.3f;
}

//���_�̈ʒu�X�V
void UpdatePosVHDRCamera(int nCntHDRCamera)
{
	//���ꂼ��̈ʒu�̍������Z�o
	float PosDiffX = powf(g_HDRCamera[nCntHDRCamera].posR.x - g_HDRCamera[nCntHDRCamera].posV.x, DIFF_TIMES);
	float PosDiffZ = powf(g_HDRCamera[nCntHDRCamera].posR.z - g_HDRCamera[nCntHDRCamera].posV.z, DIFF_TIMES);

	//�����̎Z�o
	g_HDRCamera[nCntHDRCamera].fLength = sqrtf(PosDiffX + PosDiffZ);

	//�p�x���Z�o
	g_HDRCamera[nCntHDRCamera].rot.y = atan2f(PosDiffX, PosDiffZ);

	//���_�̈ʒu�X�V
	g_HDRCamera[nCntHDRCamera].posV.x = g_HDRCamera[nCntHDRCamera].posR.x + sinf(D3DX_PI - g_HDRCamera[nCntHDRCamera].rot.y) * g_HDRCamera[nCntHDRCamera].fLength;
	g_HDRCamera[nCntHDRCamera].posV.z = g_HDRCamera[nCntHDRCamera].posR.z + cosf(D3DX_PI - g_HDRCamera[nCntHDRCamera].rot.y) * g_HDRCamera[nCntHDRCamera].fLength;
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
	Player_HDR *pPlayer_HDR = GetPlayer_HDR();		//�v���C���[�̏��擾
	g_NumHDRCameraType = type;			//�J�����̕������i�[

	switch (type)
	{
		/*----------------------------------------------------------------
		���ՂőS�̂������鎋�_�̏���
		----------------------------------------------------------------*/
	case NumHDRCamera_ONLY:
	{
		g_HDRCamera[nCntHDRCamera].posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
		g_HDRCamera[nCntHDRCamera].posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

		g_HDRCamera[nCntHDRCamera].rot.y = D3DX_PI;					//�p�x��������

		g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;			//�ő�`�拗���ݒ�

		g_HDRCamera[nCntHDRCamera].viewport.X = (DWORD)0.0f;		//���_�]�ʒu���
		g_HDRCamera[nCntHDRCamera].viewport.Y = (DWORD)0.0f;		//���_�x�ʒu���
		g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH;	//��ʕ�������
		g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT;	//��ʍ���������
		g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
		g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
		g_HDRCamera[nCntHDRCamera].bUse = true;
		nCntHDRCamera++;
	}
	break;

	/*----------------------------------------------------------------
	���Q�����̎��_�̏���
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pPlayer_HDR++)
		{
			g_HDRCamera[nCntHDRCamera].posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
			g_HDRCamera[nCntHDRCamera].posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

			g_HDRCamera[nCntHDRCamera].rot.y = D3DX_PI;			//�p�x��������

			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_HDRCamera[nCntHDRCamera].viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntHDRCamera;	//���_�]�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Y = (DWORD)NIL_F;								//���_�x�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH / 4;						//��ʕ�������
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT;							//��ʍ���������
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�c�Q�����̎��_�̏���
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_HIGH_row:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA_HALF; nCntHDRCamera++, pPlayer_HDR++)
		{
			g_HDRCamera[nCntHDRCamera].posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
			g_HDRCamera[nCntHDRCamera].posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

			g_HDRCamera[nCntHDRCamera].rot.y = D3DX_PI;			//�p�x��������

			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_HDRCamera[nCntHDRCamera].viewport.X = (DWORD)NIL_F;										//���_�]�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Y = (DWORD)(SCREEN_HEIGHT / 2) * (nCntHDRCamera % 2);	//���_�x�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH;									//��ʕ�������
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT / 2;								//��ʍ���������
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�e�v���C���[��p�̃J�����̏���
	----------------------------------------------------------------*/
	case NumHDRCamera_FOUR_Separate:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pPlayer_HDR++)
		{
			g_HDRCamera[nCntHDRCamera].posR.x = pPlayer_HDR->pos.x;		//�����_�̇]���W�ݒ�
			g_HDRCamera[nCntHDRCamera].posR.z = pPlayer_HDR->pos.z;		//�����_�̂y���W�ݒ�

			g_HDRCamera[nCntHDRCamera].rot.y = D3DX_PI;			//�p�x��������

			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_HDRCamera[nCntHDRCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntHDRCamera % 2);	//���_�]�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntHDRCamera / 2);	//���_�x�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH / 2;						//��ʕ�������
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT / 2;						//��ʍ���������
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�e�v���C���[��p�̃J�����̏���
	----------------------------------------------------------------*/
	case NumCamera_FOUR_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_CAMERA; nCntHDRCamera++)
		{
			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_HDRCamera[nCntHDRCamera].viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntHDRCamera;	//���_�]�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Y = (DWORD)NIL_F;								//���_�x�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH / 4;						//��ʕ�������
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT;						//��ʍ���������
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}

	}

	//��ʕ����̘g��ݒ�
	SetUseFrame(type);

	//�e�J�����̐ݒ�
	for (int nCntUse = 0; nCntUse < NUM_HDRCAMERA; nCntUse++)
	{
		//��̃X�C�b�`���Ŏg���邱�ƂɂȂ����J����
		if (nCntUse < nCntHDRCamera)
		{
			//�����_�̈ʒu�ݒ�
			SetPosRHDRCamera(nCntHDRCamera);

			//���_�̈ʒu�ݒ�
			UpdatePosVHDRCamera(nCntUse);
		}

		//�ݒ肳�ꂽ�J�����ȊO�̃J�������g�p���Ă��Ȃ��悤�ɂ���
		else
		{
			g_HDRCamera[nCntUse].bUse = false;
		}
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
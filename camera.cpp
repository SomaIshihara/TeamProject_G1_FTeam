/*==========================================================================================================================================================

�J�����̏���[camera.cpp]
Author:��{����  ���V����  �Ό��D�n

============================================================================================================================================================*/
#include "camera.h"
#include "input.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "camera_frame.h"
#include "input.h"

//�����_���
#define POSR_SPEED		(10.0f)		//�����_�̈ړ����x
#define POSR_WIDTH		(0.0f)		//�����_�̕�
#define POSR_HEIGHT		(0.0f)		//�����_�̍���
#define POSR_DEPTH		(0.0f)		//�����_�̉��s��

//���_���
#define POSV_ROTSPEED	(0.05f)		//���_�̉�]���x
#define POSV_SPEED		(10.0f)		//���_�̈ړ����x
#define POSR_ADD		(8.0f)		//�����_�̏㉺�ړ�
#define POSV_DISTANCE	(100.0f)	//���_�̋���
#define POSV_WIDTH		(0.0f)		//���_�̕�
#define POSV_HEIGHT		(1050.0f)	//���_�̍���
#define POSV_DEPTH		(1000.0f)	//���_�̉��s��

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

//�R�l�̎��_���̃}�N��
#define TPS_LENGTH		(150.0f)	//�����_�`���_�Ԃ̋���
#define TPS_posV_HEIGHT	(100.0f)	//���_�̍���
#define TPS_posR_HEIGHT	(30.0f)		//�����_�̍���

//�O���[�o���ϐ�
Camera		g_Camera[NUM_CAMERA];	//�J�����̏��
NumCamera	g_NumCameraType;		//�J���������̏��

//=========================================
//�J�����̈ʒu�ݒ菈��
//Author:�Ό��D�n
//=========================================
void InitSetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera)
{
	//�ݒ�
	g_Camera[nNumCamera].posV = posV;	//���_
	g_Camera[nNumCamera].posR = posR;	//�����_

										//������x�N�g�������Œ�
	g_Camera[nNumCamera].vecU = VECU_OVER;	//������x�N�g��

											//���ꂼ��̈ʒu�̍������i�[����ϐ�
	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_Camera[nNumCamera].posR.x - g_Camera[nNumCamera].posV.x, DIFF_TIMES);	//�Q��
	PosDiffZ = powf(g_Camera[nNumCamera].posR.z - g_Camera[nNumCamera].posV.z, DIFF_TIMES);	//�Q��

																							//�����̎Z�o
	g_Camera[nNumCamera].fLength = sqrtf(PosDiffX + PosDiffZ);

	//���_�̈ʒu�X�V
	UpdatePosVCamera(nNumCamera);
}

//=========================================
//�J�����̏���������
//=========================================
void InitCamera(NumCamera type)
{
	//��{���̏���������
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		g_Camera[nCntCamera].rot = ZERO_SET;
	}

	Set_NumCamera(type);	//�J�����̐��ɂ��J�������̏�����
}

//=========================================
//�J�����̏I������
//=========================================
void UninitCamera(void)
{

}

//=========================================
//�J�����̍X�V����
//=========================================
void UpdateCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		//�J�������g���Ă���
		if (g_Camera[nCntCamera].bUse == true)
		{
			//�J�����̈ړ�����
			MoveCamera(nCntCamera);
		}
	}
}

//=========================================
//�J�����̐ݒ菈��
//=========================================
void SetCamera(int nIdx)
{
	//�J�������g���Ă���
	if (g_Camera[nIdx].bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//�r���[�|�[�g�̐ݒ�
		pDevice->SetViewport(&g_Camera[nIdx].viewport);

		//�v���W�F�N�V�����}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Camera[nIdx].mtxProjection);

		//�v���W�F�N�V�����}�g���b�N�X�̍쐬
		D3DXMatrixPerspectiveFovLH(&g_Camera[nIdx].mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_Camera[nIdx].viewport.Width / (float)g_Camera[nIdx].viewport.Height, MIN_DRAW, g_Camera[nIdx].fMaxLength);

		//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera[nIdx].mtxProjection);

		//�r���[�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Camera[nIdx].mtxView);

		//�r���[�}�g���b�N�X�̍쐬
		D3DXMatrixLookAtLH(&g_Camera[nIdx].mtxView, &g_Camera[nIdx].posV, &g_Camera[nIdx].posR, &g_Camera[nIdx].vecU);

		//�r���[�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_VIEW, &g_Camera[nIdx].mtxView);
	}
}

//=========================================
//�J�����̑䐔�ʁ@�ݒ菈��
//=========================================
void Set_NumCamera(NumCamera type)
{
	int nCntCamera = 0;				//�J�E���^�[������
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾
	g_NumCameraType = type;			//�J�����̕������i�[

	switch (type)
	{
		/*----------------------------------------------------------------
		���ՂőS�̂������鎋�_�̏���
		----------------------------------------------------------------*/
	case NumCamera_ONLY:
	{
		g_Camera[nCntCamera].posV = ALONE_CAMERA_POS;			//���_���W������
		g_Camera[nCntCamera].posR = ZERO_SET;					//�����_���W������

		g_Camera[nCntCamera].fMaxLength = MAX_DRAW;				//�ő�`�拗���ݒ�

		g_Camera[nCntCamera].viewport.X = (DWORD)0.0f;					//���_�]�ʒu���
		g_Camera[nCntCamera].viewport.Y = (DWORD)0.0f;					//���_�x�ʒu���
		g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH;		//��ʕ�������
		g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//��ʍ���������
		g_Camera[nCntCamera].viewport.MinZ = 0.0f;
		g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
		g_Camera[nCntCamera].bUse = true;
	}
	break;

	/*----------------------------------------------------------------
	���Q�����̎��_�̏���
	----------------------------------------------------------------*/
	case NumCamera_HALF_SIDE:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA_HALF; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_Camera[nCntCamera].viewport.X = (DWORD)(SCREEN_WIDTH / 2) * (nCntCamera % 2);	//���_�]�ʒu���
			g_Camera[nCntCamera].viewport.Y = (DWORD)NIL_F;									//���_�x�ʒu���
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 2;						//��ʕ�������
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT;						//��ʍ���������
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVCamera(nCntCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�c�Q�����̎��_�̏���
	----------------------------------------------------------------*/
	case NumCamera_HALF_HIGH_row:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA_HALF; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_Camera[nCntCamera].viewport.X = (DWORD)NIL_F;									//���_�]�ʒu���
			g_Camera[nCntCamera].viewport.Y = (DWORD)(SCREEN_HEIGHT / 2) * (nCntCamera % 2);	//���_�x�ʒu���
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH;							//��ʕ�������
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT / 2;					//��ʍ���������
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVCamera(nCntCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�e�v���C���[��p�̃J�����̏���
	----------------------------------------------------------------*/
	case NumCamera_FOUR_Separate:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_Camera[nCntCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntCamera % 2);	//���_�]�ʒu���
			g_Camera[nCntCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntCamera / 2);	//���_�x�ʒu���
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 2;						//��ʕ�������
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT / 2;					//��ʍ���������
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVCamera(nCntCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	�e�v���C���[��p�̃J�����̏���
	----------------------------------------------------------------*/
	case NumCamera_FOUR_SIDE:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_Camera[nCntCamera].viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntCamera;	//���_�]�ʒu���
			g_Camera[nCntCamera].viewport.Y = (DWORD)NIL_F;								//���_�x�ʒu���
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 4;						//��ʕ�������
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT;						//��ʍ���������
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVCamera(nCntCamera);
		}
	}
	}

	//��ʕ����̘g��ݒ�
	SetUseFrame(type);

	//�e�J�����̐ݒ�
	for (int nCntUse = 0; nCntUse < NUM_CAMERA; nCntUse++)
	{
		//��̃X�C�b�`���Ŏg���邱�ƂɂȂ����J����
		if (nCntUse <= nCntCamera)
		{
			//�����_�̈ʒu�ݒ�
			SetPosRCamera(nCntCamera);

			//���_�̈ʒu�ݒ�
			UpdatePosVCamera(nCntUse);
		}

		//�ݒ肳�ꂽ�J�����ȊO�̃J�������g�p���Ă��Ȃ��悤�ɂ���
		else
		{
			g_Camera[nCntUse].bUse = false;
		}
	}
}

//�J�����̈ړ�����
void MoveCamera(int nCntCamera)
{
#ifdef _DEBUG
	//----------------------
	//		���_�̈ړ�	
	//----------------------
	//���_�̏㉺
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_Camera[nCntCamera].posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		g_Camera[nCntCamera].posV.y -= POSV_SPEED;
	}

	//���_�̑O��
	if (GetKeyboardPress(DIK_N) == true)
	{
		g_Camera[nCntCamera].fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_Camera[nCntCamera].fLength -= POSV_SPEED;
	}

	//���_�̍��E
	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_Camera[nCntCamera].rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		g_Camera[nCntCamera].rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		�����_�̈ړ�	
	//------------------------
	//�����_�̏㉺
	if (GetKeyboardPress(DIK_I) == true)
	{
		g_Camera[nCntCamera].posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_Camera[nCntCamera].posR.y -= POSR_ADD;
	}
#endif // _DEBUG

	//�����_�̈ʒu�ݒ�
	SetPosRCamera(nCntCamera);

	//���_�J�����X�V
	UpdatePosVCamera(nCntCamera);
}

//�R�l�̎��_
void TPS_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot)
{
	//�v���C���[�̊p�x�̋t�ɐݒ�
	g_Camera[nCntCamera].rot.y = (D3DX_PI - rot.y);

	//�p�x�C��
	FIX_ROT(g_Camera[nCntCamera].rot.y);

	//���_�̍����ݒ�
	g_Camera[nCntCamera].posV.y = g_Camera[nCntCamera].posR.y + TPS_posV_HEIGHT;
	g_Camera[nCntCamera].posR.y += TPS_posR_HEIGHT;
	
	//�����_�`���_�@�Ԃ̋����ݒ�
	g_Camera[nCntCamera].fLength = TPS_LENGTH;

	//���_�ʒu�X�V
	UpdatePosVCamera(nCntCamera);
}

//���_�̈ʒu�X�V
void UpdatePosVCamera(int nCntCamera)
{
	//���_�̈ʒu�X�V
	g_Camera[nCntCamera].posV.x = g_Camera[nCntCamera].posR.x + sinf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
	g_Camera[nCntCamera].posV.z = g_Camera[nCntCamera].posR.z + cosf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
}

//�����_�̈ʒu�ݒ�
void SetPosRCamera(int nCntCamera)
{
	//�v���C���[���擾
	Player *pPlayer = GetPlayer();

	//�Ώۂ̃v���C���[�ɒ����_�����킹��
	g_Camera[nCntCamera].posR = pPlayer[nCntCamera].pos;

	//3�l�̎��_�ݒ�
	TPS_ChaseCamera(nCntCamera, pPlayer[nCntCamera].rot);
}

//�J�����̎擾
Camera *GetCamera(void)
{
	return &g_Camera[0];
}

//*********************************************
//�^�C�g���p�J��������
//MEMO : �J�����͂P�Ԗڂ̃J�����݂̂��g�p����
//*********************************************
void CameraForTitle(void)
{
	//�J�����̌���Y�@�̃|�C���^���擾
	float *pRot_Y = &g_Camera[0].rot.y;

	//��]
	*pRot_Y += 0.04f;

	//������3.14�𒴂���
	if (D3DX_PI <= *pRot_Y)
	{
		FIX_ROT(*pRot_Y);
	}

	//���_�̈ʒu�X�V
	UpdatePosVCamera(0);
}
/*==========================================================================================================================================================

�J�����̏���[HDR_camera.cpp]
Author:�ѓc�F�� (��{����  ���V����  �Ό��D�n)

============================================================================================================================================================*/
#include "HDR_camera.h"
#include "input.h"
#include "PvP_player.h"
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
HDRCamera		g_HDRCamera[NUM_HDRCAMERA];	//�J�����̏��
NumHDRCamera	g_NumHDRCameraType;		//�J���������̏��
bool		g_bChase_HDR = true;		//�v���C���[�ɒ����_��u�����ǂ���				TRUE�F�ǂ�������@�@false�F���_������
bool		g_bTPS_HDR = false;			//�R�l�̂��A��_�J������ �������A�S�������̂�	TRUE�F�R�l��		false�F��_�J����

//=========================================
//�J�����̈ʒu�ݒ菈��
//Author:�Ό��D�n
//=========================================
void InitSetHDRCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumHDRCamera)
{
	//�ݒ�
	g_HDRCamera[nNumHDRCamera].posV = posV;	//���_
	g_HDRCamera[nNumHDRCamera].posR = posR;	//�����_

										//������x�N�g�������Œ�
	g_HDRCamera[nNumHDRCamera].vecU = VECU_OVER;	//������x�N�g��

											//���ꂼ��̈ʒu�̍������i�[����ϐ�
	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_HDRCamera[nNumHDRCamera].posR.x - g_HDRCamera[nNumHDRCamera].posV.x, DIFF_TIMES);	//�Q��
	PosDiffZ = powf(g_HDRCamera[nNumHDRCamera].posR.z - g_HDRCamera[nNumHDRCamera].posV.z, DIFF_TIMES);	//�Q��

																							//�����̎Z�o
	g_HDRCamera[nNumHDRCamera].fLength = sqrtf(PosDiffX + PosDiffZ);

	//���_�̈ʒu�X�V
	UpdatePosVHDRCamera(nNumHDRCamera);
}

//=========================================
//�J�����̏���������
//=========================================
void InitHDRCamera(NumHDRCamera type)
{
	//��{���̏���������
	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
	{
		g_HDRCamera[nCntHDRCamera].rot = ZERO_SET;
	}

	g_bChase_HDR = true;		//�v���C���[��ǂ�������
	g_bTPS_HDR = true;			//�R�l�̃J�����ɐݒ�
	Set_NumHDRCamera(type);	//�J�����̐��ɂ��J�������̏�����
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
	//�Ǐ]  ON / OFF �؂�ւ�
	if (GetKeyboardTrigger(DIK_F4))
	{
		g_bChase_HDR = g_bChase_HDR ? false : true;

		//�Ǐ]��OFF
		if (g_bChase_HDR == false)
		{
			//�����_���O�ɂ���
			for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
			{
				g_HDRCamera[nCntHDRCamera].posR = ZERO_SET;
			}
		}
	}

	//�R�l�́@ON / OFF �؂�ւ�
	if (GetKeyboardTrigger(DIK_F6))
	{
		g_bTPS_HDR = g_bTPS_HDR ? false : true;
	}
	
	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
	{
		//�J�������g���Ă���
		if (g_HDRCamera[nCntHDRCamera].bUse == true)
		{
			//�J�����̈ړ�����
			MoveHDRCamera(nCntHDRCamera);
		}
	}
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
void Set_NumHDRCamera(NumHDRCamera type)
{
	int nCntHDRCamera = 0;				//�J�E���^�[������
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾
	g_NumHDRCameraType = type;			//�J�����̕������i�[

	switch (type)
	{
		/*----------------------------------------------------------------
		���ՂőS�̂������鎋�_�̏���
		----------------------------------------------------------------*/
	case NumHDRCamera_ONLY:
	{
		g_HDRCamera[nCntHDRCamera].posV = ALONE_CAMERA_POS;			//���_���W������
		g_HDRCamera[nCntHDRCamera].posR = ZERO_SET;					//�����_���W������

		g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;				//�ő�`�拗���ݒ�

		g_HDRCamera[nCntHDRCamera].viewport.X = 0.0f;					//���_�]�ʒu���
		g_HDRCamera[nCntHDRCamera].viewport.Y = 0.0f;					//���_�x�ʒu���
		g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH;		//��ʕ�������
		g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT;	//��ʍ���������
		g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
		g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
		g_HDRCamera[nCntHDRCamera].bUse = true;
	}
	break;

	/*----------------------------------------------------------------
	���Q�����̎��_�̏���
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA_HALF; nCntHDRCamera++)
		{
			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_HDRCamera[nCntHDRCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntHDRCamera % 2);	//���_�]�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Y = NIL_F;									//���_�x�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH / 2;						//��ʕ�������
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT;						//��ʍ���������
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
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA_HALF; nCntHDRCamera++)
		{
			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_HDRCamera[nCntHDRCamera].viewport.X = NIL_F;									//���_�]�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntHDRCamera % 2);	//���_�x�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH;							//��ʕ�������
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT / 2;					//��ʍ���������
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
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++)
		{
			g_HDRCamera[nCntHDRCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

			g_HDRCamera[nCntHDRCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntHDRCamera % 2);	//���_�]�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntHDRCamera / 2);	//���_�x�ʒu���
			g_HDRCamera[nCntHDRCamera].viewport.Width = SCREEN_WIDTH / 2;						//��ʕ�������
			g_HDRCamera[nCntHDRCamera].viewport.Height = SCREEN_HEIGHT / 2;					//��ʍ���������
			g_HDRCamera[nCntHDRCamera].viewport.MinZ = 0.0f;
			g_HDRCamera[nCntHDRCamera].viewport.MaxZ = 1.0f;
			g_HDRCamera[nCntHDRCamera].bUse = true;

			//�����_�̈ʒu�X�V
			UpdatePosVHDRCamera(nCntHDRCamera);
		}
	}
	break;
	}

	//��ʕ����̘g��ݒ�
	//SetUseFrame(type);

	//�e�J�����̐ݒ�
	for (int nCntUse = 0; nCntUse < NUM_HDRCAMERA; nCntUse++)
	{
		//��̃X�C�b�`���Ŏg���邱�ƂɂȂ����J����
		if (nCntUse <= nCntHDRCamera)
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

	//���_�J�����X�V
	UpdatePosVHDRCamera(nCntHDRCamera);
}

//�R�l�̎��_
void TPS_ChaseHDRCamera(int nCntHDRCamera, D3DXVECTOR3 rot)
{
	//�v���C���[�̊p�x�̋t�ɐݒ�
	g_HDRCamera[nCntHDRCamera].rot.y = (D3DX_PI - rot.y);

	//�p�x�C��
	FIX_ROT(g_HDRCamera[nCntHDRCamera].rot.y);

	//���_�̍����ݒ�
	g_HDRCamera[nCntHDRCamera].posV.y = g_HDRCamera[nCntHDRCamera].posR.y + TPS_posV_HEIGHT;
	g_HDRCamera[nCntHDRCamera].posR.y += TPS_posR_HEIGHT;
	
	//�����_�`���_�@�Ԃ̋����ݒ�
	g_HDRCamera[nCntHDRCamera].fLength = TPS_LENGTH;

	//���_�ʒu�X�V
	UpdatePosVHDRCamera(nCntHDRCamera);
}

//���_�̈ʒu�X�V
void UpdatePosVHDRCamera(int nCntHDRCamera)
{
	//���_�̈ʒu�X�V
	g_HDRCamera[nCntHDRCamera].posV.x = g_HDRCamera[nCntHDRCamera].posR.x + sinf(D3DX_PI - g_HDRCamera[nCntHDRCamera].rot.y) * g_HDRCamera[nCntHDRCamera].fLength;
	g_HDRCamera[nCntHDRCamera].posV.z = g_HDRCamera[nCntHDRCamera].posR.z + cosf(D3DX_PI - g_HDRCamera[nCntHDRCamera].rot.y) * g_HDRCamera[nCntHDRCamera].fLength;
}

//�����_�̈ʒu�ݒ�
void SetPosRHDRCamera(int nCntHDRCamera)
{
	//�Ǐ] �܂��� �R�l�̂�  ON
	if (g_bChase_HDR == true || g_bTPS_HDR == true)
	{
		//�v���C���[���擾
		Player *pPlayer = GetPlayer();

		//�Ώۂ̃v���C���[�ɒ����_�����킹��
		g_HDRCamera[nCntHDRCamera].posR = pPlayer[nCntHDRCamera].pos;

		//�R�l�̎��_
		if (g_bTPS_HDR)
		{
			//3�l�̎��_�ݒ�
			TPS_ChaseHDRCamera(nCntHDRCamera, pPlayer[nCntHDRCamera].rot);
		}
	}
}

//�J�����̎擾
HDRCamera *GetHDRCamera(void)
{
	return &g_HDRCamera[0];
}
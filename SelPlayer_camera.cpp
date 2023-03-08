/*==========================================================================================================================================================

�J�����̏���[camera.cpp]
Author:��{����  ���V����  �Ό��D�n

============================================================================================================================================================*/
#include "SelPlayer_camera.h"
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

//�R�l�̎��_���̃}�N��
#define TPS_LENGTH		(150.0f)	//�����_�`���_�Ԃ̋���
#define TPS_posV_HEIGHT	(100.0f)	//���_�̍���
#define TPS_posR_HEIGHT	(30.0f)		//�����_�̍���

//�O���[�o���ϐ�
Camera		g_SelPlayer_Camera;	//�J�����̏��

//=========================================
//�J�����̈ʒu�ݒ菈��
//Author:�Ό��D�n
//=========================================
void InitSelPlayer_SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	//�ݒ�
	g_SelPlayer_Camera.posV = posV;	//���_
	g_SelPlayer_Camera.posR = posR;	//�����_

										//������x�N�g�������Œ�
	g_SelPlayer_Camera.vecU = VECU_OVER;	//������x�N�g��

											//���ꂼ��̈ʒu�̍������i�[����ϐ�
	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_SelPlayer_Camera.posR.x - g_SelPlayer_Camera.posV.x, DIFF_TIMES);	//�Q��
	PosDiffZ = powf(g_SelPlayer_Camera.posR.z - g_SelPlayer_Camera.posV.z, DIFF_TIMES);	//�Q��

																							//�����̎Z�o
	g_SelPlayer_Camera.fLength = sqrtf(PosDiffX + PosDiffZ);

	//���_�̈ʒu�X�V
	UpdateSelPlayer_PosVCamera();
}

//=========================================
//�J�����̏���������
//=========================================
void InitSelPlayer_Camera(void)
{
	//��{���̏���������
	g_SelPlayer_Camera.rot = ZERO_SET;
}

//=========================================
//�J�����̏I������
//=========================================
void UninitSelPlayer_Camera(void)
{

}

//=========================================
//�J�����̍X�V����
//=========================================
void UpdateSelPlayer_Camera(void)
{
	//�J�������g���Ă���
	if (g_SelPlayer_Camera.bUse == true)
	{
		//�J�����̈ړ�����
		MoveSelPlayer_Camera();
	}
}

//=========================================
//�J�����̐ݒ菈��
//=========================================
void SetSelPlayer_Camera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�|�[�g�̐ݒ�
	g_SelPlayer_Camera.fMaxLength = MAX_DRAW;				//�ő�`�拗���ݒ�

	g_SelPlayer_Camera.viewport.X = (DWORD)0.0f;			//���_�]�ʒu���
	g_SelPlayer_Camera.viewport.Y = (DWORD)0.0f;			//���_�x�ʒu���
	g_SelPlayer_Camera.viewport.Width = SCREEN_WIDTH;		//��ʕ�������
	g_SelPlayer_Camera.viewport.Height = SCREEN_HEIGHT;		//��ʍ���������
	g_SelPlayer_Camera.viewport.MinZ = 0.0f;
	g_SelPlayer_Camera.viewport.MaxZ = 1.0f;
	g_SelPlayer_Camera.bUse = true;

	pDevice->SetViewport(&g_SelPlayer_Camera.viewport);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_SelPlayer_Camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_SelPlayer_Camera.mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_SelPlayer_Camera.viewport.Width / (float)g_SelPlayer_Camera.viewport.Height, MIN_DRAW, g_SelPlayer_Camera.fMaxLength);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_SelPlayer_Camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_SelPlayer_Camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_SelPlayer_Camera.mtxView, &g_SelPlayer_Camera.posV, &g_SelPlayer_Camera.posR, &g_SelPlayer_Camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_SelPlayer_Camera.mtxView);
}

//�J�����̈ړ�����
void MoveSelPlayer_Camera(void)
{
#ifdef _DEBUG
	//----------------------
	//		���_�̈ړ�	
	//----------------------
	//���_�̏㉺
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_SelPlayer_Camera.posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		g_SelPlayer_Camera.posV.y -= POSV_SPEED;
	}

	//���_�̑O��
	if (GetKeyboardPress(DIK_N) == true)
	{
		g_SelPlayer_Camera.fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_SelPlayer_Camera.fLength -= POSV_SPEED;
	}

	//���_�̍��E
	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_SelPlayer_Camera.rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		g_SelPlayer_Camera.rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		�����_�̈ړ�	
	//------------------------
	//�����_�̏㉺
	if (GetKeyboardPress(DIK_I) == true)
	{
		g_SelPlayer_Camera.posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_SelPlayer_Camera.posR.y -= POSR_ADD;
	}
#endif // _DEBUG

	//���_�J�����X�V
	UpdateSelPlayer_PosVCamera();
}

//�R�l�̎��_
void TPS_SelPlayer_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot)
{
	//�v���C���[�̊p�x�̋t�ɐݒ�
	g_SelPlayer_Camera.rot.y = (D3DX_PI - rot.y);

	//�p�x�C��
	FIX_ROT(g_SelPlayer_Camera.rot.y);

	//���_�̍����ݒ�
	g_SelPlayer_Camera.posV.y = g_SelPlayer_Camera.posR.y + TPS_posV_HEIGHT;
	g_SelPlayer_Camera.posR.y += TPS_posR_HEIGHT;
	
	//�����_�`���_�@�Ԃ̋����ݒ�
	g_SelPlayer_Camera.fLength = TPS_LENGTH;

	//���_�ʒu�X�V
	UpdateSelPlayer_PosVCamera();
}

//���_�̈ʒu�X�V
void UpdateSelPlayer_PosVCamera(void)
{
	//���_�̈ʒu�X�V
	g_SelPlayer_Camera.posV.x = g_SelPlayer_Camera.posR.x + sinf(D3DX_PI - g_SelPlayer_Camera.rot.y) * g_SelPlayer_Camera.fLength;
	g_SelPlayer_Camera.posV.z = g_SelPlayer_Camera.posR.z + cosf(D3DX_PI - g_SelPlayer_Camera.rot.y) * g_SelPlayer_Camera.fLength;
}

//�J�����̎擾
Camera *GetSelPlayer_Camera(void)
{
	return &g_SelPlayer_Camera;
}

//*********************************************
//�^�C�g���p�J��������
//MEMO : �J�����͂P�Ԗڂ̃J�����݂̂��g�p����
//*********************************************
void SelPlayer_CameraForTitle(void)
{
	//�J�����̌���Y�@�̃|�C���^���擾
	float *pRot_Y = &g_SelPlayer_Camera.rot.y;

	//��]
	*pRot_Y += 0.04f;

	//������3.14�𒴂���
	if (D3DX_PI <= *pRot_Y)
	{
		FIX_ROT(*pRot_Y);
	}

	//���_�̈ʒu�X�V
	UpdateSelPlayer_PosVCamera();
}
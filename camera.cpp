/*==========================================================================================================================================================

�J�����̏���[camera.cpp]
Author:��{����

============================================================================================================================================================*/
#include "camera.h"
#include "input.h"
#include "player.h"

//�����_���
#define POSR_SPEED		(10.0f)		//�����_�̈ړ����x
#define POSR_WIDTH		(0.0f)		//�����_�̕�
#define POSR_HEIGHT		(0.0f)		//�����_�̍���
#define POSR_DEPTH		(0.0f)		//�����_�̉��s��

//���_���
#define POSV_ROTSPEED	(0.05f)		//���_�̉�]���x
#define POSV_SPEED		(10.0f)		//���_�̈ړ����x
#define POSV_ADD		(100)		//���_�̏㉺�ړ�
#define POSV_DISTANCE	(100)		//���_�̋���
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

//�O���[�o���ϐ�
Camera g_Camera[NUM_CAMERA];		//�J�����̏��

//�J�����̏�����
void InitCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//�ő�`�拗���ݒ�

		g_Camera[nCntCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntCamera % 2);	//���_�]�ʒu���
		g_Camera[nCntCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntCamera / 2);	//���_�x�ʒu���
		g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 2;						//��ʕ�������
		g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT / 2;					//��ʍ���������
		g_Camera[nCntCamera].viewport.MinZ = 0.0f;
		g_Camera[nCntCamera].viewport.MaxZ = 1.0f;

		//�����_�̈ʒu�X�V
		UpdatePosVCamera(nCntCamera);
	}
}

//�J�����̏I������
void UninitCamera(void)
{

}

//�J�����̍X�V����
void UpdateCamera(void)
{
	//�J�����̈ړ�����
	MoveCamera();
}

//�J�����̐ݒ菈��
void SetCamera(int nIdx)
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

//�J�����̈ړ�����
void MoveCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
#ifdef _DEBUG
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
		if (GetKeyboardPress(DIK_Y) == true)
		{
			g_Camera[nCntCamera].fLength += POSV_SPEED;
		}
		if (GetKeyboardPress(DIK_N) == true)
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
#endif // _DEBUG

		//�����_�̈ʒu�X�V
		UpdatePosVCamera(nCntCamera);
	}
}

//���_�̈ʒu�X�V
void UpdatePosVCamera(int nCntCamera)
{
	//�v���C���[���擾
	Player *pPlayer = GetPlayer();

	//�Ώۂ̃v���C���[�ɒ����_�����킹��
	pPlayer += nCntCamera;

	g_Camera[nCntCamera].posR = pPlayer->pos;

	//���_�̈ʒu�X�V
	g_Camera[nCntCamera].posV.x = g_Camera[nCntCamera].posR.x + sinf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
	g_Camera[nCntCamera].posV.z = g_Camera[nCntCamera].posR.z + cosf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
	g_Camera[nCntCamera].posV.y += pPlayer->move.y;
}

//�J�����̎擾
Camera *GetCamera(void)
{
	return &g_Camera[0];
}

//=========================================
//�J�����̈ʒu�ݒ菈��
//Author:�Ό��D�n
//=========================================
void SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera)
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
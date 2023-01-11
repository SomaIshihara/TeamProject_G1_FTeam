/*==========================================================================================================================================================

�J�����̏���[camera.cpp]
Author:��{����

============================================================================================================================================================*/

#include "camera.h"
#include "input.h"
#include "player.h"

//�����_�ړ����x(�㉺)
#define POSR_SPEED	(10.0f)

//���_�ړ����x(�㉺�E���E)
#define POSV_ROTSPEED	(0.05f)
#define POSV_SPEED	(10.0f)

//���_�̋���
#define POSV_DISTANCE	(1000)
#define POSV_UP			(400)

//�O���[�o���ϐ�
Camera g_Camera;					//�J�����̏��
float  g_MaxDrawCamera = 50000.0f;	//�ő�`��͈�

									//�J�����̏�����
void InitCamera(void) //����������
{
	g_Camera.posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	Player * pPlayer = GetPlayer();

	//���_
	g_Camera.posV.x = pPlayer->pos.x;
	g_Camera.posV.y = pPlayer->pos.y + POSV_UP;
	g_Camera.posV.z = pPlayer->pos.z - POSV_DISTANCE;

	//�����_
	g_Camera.posR.x = pPlayer->pos.x;
	g_Camera.posR.y = pPlayer->pos.y;
	g_Camera.posR.z = pPlayer->pos.z;

	//���ꂼ��̈ʒu�̍������i�[����ϐ�
	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = pow(g_Camera.posR.x - g_Camera.posV.x, 2.0f);	//�Q��
	PosDiffY = pow(g_Camera.posR.y - g_Camera.posV.y, 2.0f);	//�Q��
	PosDiffZ = pow(g_Camera.posR.z - g_Camera.posV.z, 2.0f);	//�Q��

																//�����̎Z�o
	g_Camera.fLength = sqrtf(PosDiffX + PosDiffY + PosDiffZ);
}

//�J�����̏I������
void UninitCamera(void)
{

}

//�J�����̍X�V����
void UpdateCamera(void)
{
	
}


//�J�����̐ݒ菈��
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, g_MaxDrawCamera);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.posV, &g_Camera.posR, &g_Camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//�J�����̎擾
Camera *GetCamera(void)
{
	return &g_Camera;
}
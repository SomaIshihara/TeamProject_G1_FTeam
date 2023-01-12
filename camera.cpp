/*==========================================================================================================================================================

�J�����̏���[camera.cpp]
Author:��{����

============================================================================================================================================================*/

#include "camera.h"
#include "input.h"
#include "player.h"

//�����_���
#define POSR_SPEED	(10.0f)		//�����_�̈ړ����x
#define POSR_WIDTH	(0.0f)		//�����_�̕�
#define POSR_HEIGHT	(0.0f)		//�����_�̍���
#define POSR_DEPTH	(0.0f)		//�����_�̉��s��

//���_���
#define POSV_ROTSPEED	(0.05f)		//���_�̉�]���x
#define POSV_SPEED		(10.0f)		//���_�̈ړ����x
#define POSV_DISTANCE	(1000)		//���_�̋���
#define POSV_ADD		(400)		//���_�̏㉺�ړ�
#define POSV_WIDTH		(0.0f)		//���_�̕�
#define POSV_HEIGHT		(500.0f)	//���_�̍���
#define POSV_DEPTH		(0.0f)		//���_�̉��s��

//������x�N�g�����
#define VECU_WIDTH      (0.0f)		//������x�N�g���̕�
#define VECU_HEIGHT		(1.0f)		//������x�N�g���̍���
#define VECV_DEPTH		(0.0f)		//������x�N�g���̉��s��

//�`��͈�
#define DRAWING_AREA	(50000.0f)

//�O���[�o���ϐ�
Camera g_Camera;						//�J�����̏��
float  g_MaxDrawCamera = DRAWING_AREA;	//�ő�`��͈�

//�J�����̏�����
void InitCamera(void)
{
	g_Camera.posV = D3DXVECTOR3(POSV_WIDTH, POSV_HEIGHT, POSV_DEPTH);	//���_
	g_Camera.posR = D3DXVECTOR3(POSR_WIDTH, POSR_HEIGHT, POSR_DEPTH);
	g_Camera.vecU = D3DXVECTOR3(VECU_WIDTH, VECU_HEIGHT, VECV_DEPTH);

	Player * pPlayer = GetPlayer();

	//���_
	g_Camera.posV.x = pPlayer->pos.x;
	g_Camera.posV.y = pPlayer->pos.y + POSV_ADD;
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
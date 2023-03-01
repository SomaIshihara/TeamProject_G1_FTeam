/*==========================================================================================================================================================

�^�C�g���p�J��������[titleCamera.cpp]
Author:���V����

============================================================================================================================================================*/
#include "titleCamera.h"
#include "conversioninput.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 200.0f, -800.0f)	//���_�̏����ʒu
#define POS_POSR			D3DXVECTOR3(0.0f, 100.0f, 0.0f)		//�����_�̏����ʒu
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//������x�N�g���̏����l
#define VIEW_ANGLE			(45.0f)								//����p
#define MIN_DRAW_Z			(10.0f)								//�`�揈���̍Œቜ�s��
#define MAX_DRAW_Z			(20000.0f)							//�`�揈���̍ō����s��
#define SPIN_ROT			(0.01f)								//��]��

//�O���[�o���ϐ�
TitleCamera				g_TitleCamera;							//�J�����̏��

//------------------------------------------------
//			�^�C�g���p�J�����̏���������
//------------------------------------------------
void InitTitleCamera(void)
{
	g_TitleCamera.posR = POS_POSR;
	g_TitleCamera.posV = POS_POSV;
	g_TitleCamera.vecU = POS_VECU;

	D3DXVECTOR3 fDiffPos = g_TitleCamera.posR - g_TitleCamera.posV;	//���_�ƒ����_�̈ʒu�̍�
	fDiffPos.x = powf(fDiffPos.x, 2.0f);					//���_���m�̈ʒu�̍����Q��
	fDiffPos.z = powf(fDiffPos.z, 2.0f);					//���_���m�̈ʒu�̍����Q��

	g_TitleCamera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y�����̊p�x���v�Z

	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_TitleCamera.posR.x - g_TitleCamera.posV.x, 2.0f);	//2��
	PosDiffZ = powf(g_TitleCamera.posR.z - g_TitleCamera.posV.z, 2.0f);	//2��

	g_TitleCamera.fLength = sqrtf(PosDiffX + PosDiffZ);	//�������Z�o

	//�ʒu�ݒ�
	MoveTitleCamera(g_TitleCamera.posR, &g_TitleCamera.posV, D3DX_PI);
}

//------------------------------------------------
//			�^�C�g���p�J�����̏I������
//------------------------------------------------
void UninitTitleCamera(void)
{

}

//------------------------------------------------
//			�^�C�g���p�J�����̍X�V����
//------------------------------------------------
void UpdateTitleCamera(void)
{
	//���_�ړ�
	SpinTitlePosV();

	//�ʒu�ݒ�
	MoveTitleCamera(g_TitleCamera.posR, &g_TitleCamera.posV, D3DX_PI);
}

//���_��]
void SpinTitlePosV(void)
{
	//��]����
	g_TitleCamera.rot.y += SPIN_ROT;

	//�p�x���~�����𒴂�����
	if (D3DX_PI < g_TitleCamera.rot.y)
	{
		//�p�x�𒼂�
		FIX_ROT(g_TitleCamera.rot.y);
	}
}

//�J�����̈ړ�����
void MoveTitleCamera(D3DXVECTOR3 ParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = ParentPos.x + sinf((CorRot - g_TitleCamera.rot.y)) * g_TitleCamera.fLength;
	pChildPos->z = ParentPos.z + cosf((CorRot - g_TitleCamera.rot.y)) * g_TitleCamera.fLength;
}

//------------------------------------------------
//			�^�C�g���p�J�����̐ݒ菈��
//------------------------------------------------
void SetTitleCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

												//�v���W�F�N�V�����}�g���b�N�X������
	D3DXMatrixIdentity(&g_TitleCamera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_TitleCamera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_TitleCamera.mtxProjection);

	//�r���[�}�g���b�N�X
	D3DXMatrixIdentity(&g_TitleCamera.mtxview);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_TitleCamera.mtxview,
		&g_TitleCamera.posV,		//���_
		&g_TitleCamera.posR,		//�����_
		&g_TitleCamera.vecU);	//������x�N�g��

								//�r���[�}�g���b�N�X
	pDevice->SetTransform(D3DTS_VIEW, &g_TitleCamera.mtxview);
}

//�^�C�g���p�J�����̏��擾
TitleCamera *GetTitleCamera(void)
{
	return &g_TitleCamera;
}
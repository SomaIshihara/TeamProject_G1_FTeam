/*==========================================================================================================================================================

���U���g�p�J��������[resultCamera.cpp]
Author:���V����

============================================================================================================================================================*/
#include "resultCamera.h"
#include "input.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 100.0f, -100.0f)	//���_�̏����ʒu
#define POS_POSR			D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//�����_�̏����ʒu
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//������x�N�g���̏����l
#define VIEW_ANGLE			(45.0f)								//����p
#define MOVE_CAMERA_SPEED	(2.0f)								//�J�����E�����_���W�̈ړ���
#define MIN_DRAW_Z			(10.0f)								//�`�揈���̍Œቜ�s��
#define MAX_DRAW_Z			(2000.0f)							//�`�揈���̍ō����s��
#define MIN_LENGTH			(100.0f)							//���_�ƒ����_�̍Œ዗��
#define MAX_LENGTH			(300.0f)							//���_�ƒ����_�̍ő勗��
#define CHANGE_LENGHT		(0.5f)								//�����̈ړ���
#define SPIN_ROT			(0.05f)								//��]��
#define COR_ROT_POSV		(3.14f)								//�p�x�v�Z�ɂ�鎋�_����̒����l
#define COR_ROT_POSR		(0.0f)								//�p�x�v�Z�ɂ�钍���_����̒����l
#define DRAW_LENGTH			(500.0f)							//�I�u�W�F�N�g��`�悳���Ă�����͈�

//�O���[�o���ϐ�
ResultCamera				g_ResultCamera;							//�J�����̏��

//------------------------------------------------
//			���U���g�p�J�����̏���������
//------------------------------------------------
void InitResultCamera(void)
{
	ResetResultCameraPos();

	g_ResultCamera.vecU = POS_VECU;

	D3DXVECTOR3 fDiffPos = g_ResultCamera.posR - g_ResultCamera.posV;	//���_�ƒ����_�̈ʒu�̍�
	fDiffPos.x = powf(fDiffPos.x, 2.0f);					//���_���m�̈ʒu�̍����Q��
	fDiffPos.z = powf(fDiffPos.z, 2.0f);					//���_���m�̈ʒu�̍����Q��

	g_ResultCamera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y�����̊p�x���v�Z

	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = powf(g_ResultCamera.posR.x - g_ResultCamera.posV.x, 2.0f);	//2��
	PosDiffY = powf(g_ResultCamera.posR.y - g_ResultCamera.posV.y, 2.0f);	//2��
	PosDiffZ = powf(g_ResultCamera.posR.z - g_ResultCamera.posV.z, 2.0f);	//2��

	g_ResultCamera.fLength = sqrtf(PosDiffX + PosDiffY + PosDiffZ);	//�������Z�o

}

//------------------------------------------------
//			���U���g�p�J�����̏I������
//------------------------------------------------
void UninitResultCamera(void)
{

}

//------------------------------------------------
//			���U���g�p�J�����̍X�V����
//------------------------------------------------
void UpdateResultCamera(void)
{

}

//���_�ړ�
void MovePosV(void)
{

}

//���_��]
void SpinPosV(void)
{

}

//�p�x�C��
void FixResultCameraRot(void)
{

}

//�J�����̈ړ�����
void MoveResultCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = pParentPos->x + sinf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
	pChildPos->z = pParentPos->z + cosf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
}

//�J�����̈ʒu���Z�b�g
void ResetResultCameraPos(void)
{
	g_ResultCamera.posR = ZERO_SET;
	g_ResultCamera.posV = POS_POSV;
}

//���_�Ԃ̒�������
void LengthResultCamera(void)
{

}

//------------------------------------------------
//			���U���g�p�J�����̐ݒ菈��
//------------------------------------------------
void SetResultCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

												//�v���W�F�N�V�����}�g���b�N�X������
	D3DXMatrixIdentity(&g_ResultCamera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_ResultCamera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_ResultCamera.mtxProjection);

	//�r���[�}�g���b�N�X
	D3DXMatrixIdentity(&g_ResultCamera.mtxview);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_ResultCamera.mtxview,
		&g_ResultCamera.posV,		//���_
		&g_ResultCamera.posR,		//�����_
		&g_ResultCamera.vecU);	//������x�N�g��

							//�r���[�}�g���b�N�X
	pDevice->SetTransform(D3DTS_VIEW, &g_ResultCamera.mtxview);
}

//���U���g�p�J�����̏��擾
ResultCamera *GetResultCamera(void)
{
	return &g_ResultCamera;
}
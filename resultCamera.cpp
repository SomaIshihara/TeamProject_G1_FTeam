/*==========================================================================================================================================================

���U���g�p�J��������[resultCamera.cpp]
Author:���V����

============================================================================================================================================================*/
#include "debugproc.h"
#include "resultCamera.h"
#include "resultPlayer.h"
#include "conversioninput.h"
#include "VictoryStand.h"

//�}�N����`
#define POS_POSV			D3DXVECTOR3(0.0f, 400.0f, -200.0f)	//���_�̏����ʒu
#define POS_POSR			D3DXVECTOR3(0.0f, 300.0f, 0.0f)		//�����_�̏����ʒu
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//������x�N�g���̏����l
#define VIEW_ANGLE			(45.0f)								//����p
#define MOVE_CAMERA_SPEED	(3.0f)								//�J�����E�����_���W�̈ړ���
#define SPIN_CAMERA_SPEED	(0.03f)								//�J�����̉�]�X�s�[�h
#define MIN_DRAW_Z			(10.0f)								//�`�揈���̍Œቜ�s��
#define MAX_DRAW_Z			(2000.0f)							//�`�揈���̍ō����s��
#define MIN_LENGTH			(100.0f)							//���_�ƒ����_�̍Œ዗��
#define MAX_LENGTH			(300.0f)							//���_�ƒ����_�̍ő勗��
#define CHANGE_LENGHT		(0.5f)								//�����̈ړ���
#define SPIN_ROT			(0.05f)								//��]��
#define COR_ROT_POSV		(3.14f)								//�p�x�v�Z�ɂ�鎋�_����̒����l
#define COR_ROT_POSR		(0.0f)								//�p�x�v�Z�ɂ�钍���_����̒����l
#define DRAW_LENGTH			(500.0f)							//�I�u�W�F�N�g��`�悳���Ă�����͈�
#define WAVE_CAMERA_INFO	"data\\CSV\\WaveCamera.csv"			//�A�j���[�V�����J�����̏��t�@�C��

//���ꉉ�o�̃}�N��
#define SP_PLAYER_DIVE_TIME		(80)		//�v���C���[����э��ގ��ԁi�E�F�[�u�Q�̍d���t���[����
#define SP_WAVE_03_START_Y		(100.0f)	//�E�F�[�u�R�J�n���̃v���C���[�x�ʒu����
#define SP_WAVE_LAST_START_Y	(100.0f)	//�E�F�[�u�Ō�J�n���̃v���C���[�x�ʒu����

//�O���[�o���ϐ�
ResultCamera		g_ResultCamera;						//�J�����̏��
WAVECamera			g_WaveCamera;						//�J�����̃E�F�[�u���
AnimResCamera		g_AnimResCamera[WAVECamera_MAX];	//�A�j���[�V�����J�����̏��
int					g_nAnimTime = 0;					//��f���Ԃ��v�Z

//------------------------------------------------
//			���U���g�p�J�����̏���������
//------------------------------------------------
void InitResultCamera(void)
{
	//�A�j���[�V�����p�J�������̏�����
	LoadWaveCamera();

	g_ResultCamera.vecU = POS_VECU;									//������x�N�g��
	g_WaveCamera = WAVECamera_01_SideWays;							//�E�F�[�u��񏉊���
	SetNextWave(&g_ResultCamera, g_AnimResCamera[0], g_WaveCamera);	//�����ʒu�ݒ�
	g_nAnimTime = 0;					//��f���Ԃ�������

	//---------------------------------
	//�J�����̊p�x�⋗�����Z�o
	//---------------------------------
	{
		D3DXVECTOR3 fDiffPos = g_ResultCamera.posR - g_ResultCamera.posV;	//���_�ƒ����_�̈ʒu�̍�
		fDiffPos.x = powf(fDiffPos.x, 2.0f);					//���_���m�̈ʒu�̍����Q��
		fDiffPos.z = powf(fDiffPos.z, 2.0f);					//���_���m�̈ʒu�̍����Q��

		g_ResultCamera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y�����̊p�x���v�Z

		float PosDiffX, PosDiffZ;

		PosDiffX = powf(g_ResultCamera.posR.x - g_ResultCamera.posV.x, 2.0f);	//2��
		PosDiffZ = powf(g_ResultCamera.posR.z - g_ResultCamera.posV.z, 2.0f);	//2��

		g_ResultCamera.fLength = sqrtf(PosDiffX + PosDiffZ);	//�������Z�o

		//�ʒu�ݒ�
		MoveResultCamera(g_ResultCamera.posR, &g_ResultCamera.posV, D3DX_PI);
	}
}

//���U���g�p�J�����̃A�j���[�V������񏉊���
void LoadWaveCamera(void)
{
	FILE *pFile;
	char aData[256];
	AnimResCamera *pAnimCamera = &g_AnimResCamera[0];

	//�t�@�C���ǂݍ���
	pFile = fopen(WAVE_CAMERA_INFO, "r");

	//�t�@�C�����ǂݍ��߂Ă���
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);	//�ŏ��̃Z���̍��ږ��ǂݍ���

		for (int nCntWave = 0; nCntWave < WAVECamera_MAX; nCntWave++, pAnimCamera++)
		{
			//��s�ǂݍ���
			fscanf(pFile, "%s", &aData[0]);

			//�o�߃t���[��������
			pAnimCamera->nFrameCounter = 0;

			//�S�̃t���[��
			pAnimCamera->nWholeFrame = atoi(strtok(&aData[0], ","));

			//�t���[�����B���菉����
			pAnimCamera->bEnd = false;

			//�d���t���[��������
			pAnimCamera->nWaitCount = 0;

			//�S�̍d���t���[��
			pAnimCamera->WaitTime = atoi(strtok(NULL, ","));

			//**************************************************
			//					���_	�J�n�ʒu
			//**************************************************
			pAnimCamera->StartPosV.x = atof(strtok(NULL, ","));
			pAnimCamera->StartPosV.y = atof(strtok(NULL, ","));
			pAnimCamera->StartPosV.z = atof(strtok(NULL, ","));

			//**************************************************
			//				   �����_	�J�n�ʒu
			//**************************************************
			pAnimCamera->StartPosR.x = atof(strtok(NULL, ","));
			pAnimCamera->StartPosR.y = atof(strtok(NULL, ","));
			pAnimCamera->StartPosR.z = atof(strtok(NULL, ","));

			//**************************************************
			//					���_	�ڕW�ʒu
			//**************************************************
			pAnimCamera->OffsetPosV.x = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosV.y = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosV.z = atof(strtok(NULL, ","));

			//**************************************************
			//					�����_	�ڕW�ʒu
			//**************************************************
			pAnimCamera->OffsetPosR.x = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosR.y = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosR.z = atof(strtok(NULL, ","));

			//**************************************************
			//					���_	�ړ���
			//**************************************************
			pAnimCamera->movePosV.x = (pAnimCamera->OffsetPosV.x - pAnimCamera->StartPosV.x) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosV.y = (pAnimCamera->OffsetPosV.y - pAnimCamera->StartPosV.y) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosV.z = (pAnimCamera->OffsetPosV.z - pAnimCamera->StartPosV.z) / pAnimCamera->nWholeFrame;

			//**************************************************
			//					�����_	�ړ���
			//**************************************************
			pAnimCamera->movePosR.x = (pAnimCamera->OffsetPosR.x - pAnimCamera->StartPosR.x) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosR.y = (pAnimCamera->OffsetPosR.y - pAnimCamera->StartPosR.y) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosR.z = (pAnimCamera->OffsetPosR.z - pAnimCamera->StartPosR.z) / pAnimCamera->nWholeFrame;
		}
	}
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
	//�J�����̈ړ������i���͎g�p���Ȃ�
#if 0
	//���_�ړ�		//�����_�ړ�	//���_��]
	MovePosV();		MovePosR();		SpinPosV();

	//�����؂�ւ�
	LengthCamera();

	//�ʒu�ݒ�
	MoveResultCamera(g_ResultCamera.posR, &g_ResultCamera.posV, D3DX_PI);
#endif

	//�J�����̃E�F�[�u����
	WaveResultCamera();

	//�E�F�[�u�ʂ̓��ʏ���
	SpecialWave();
	
	int nWave = g_WaveCamera;
	ResultCamera Camera = g_ResultCamera;
	AnimResCamera AnimCam = g_AnimResCamera[nWave];

	if (GetKeyboardTrigger(DIK_BACKSPACE))
	{
		InitResultCamera();
	}
}

//�J�����̃A�j���[�V��������
void WaveResultCamera(void)
{
	//�E�F�[�u���I�����Ă��Ȃ�
	if (g_WaveCamera != WAVECamera_MAX)
	{
		int nWave = g_WaveCamera;								//���݂̃E�F�[�u���擾
		ResultCamera *pCamera = &g_ResultCamera;				//�J�����̃|�C���^�擾
		AnimResCamera *pAnimCamera = &g_AnimResCamera[nWave];	//�A�j���[�V�����p�J�����̃|�C���^

		//�����S�̃t���[���ɓ��B���Ă��Ȃ�
		if (!pAnimCamera->bEnd)
		{
			//���_�E�����_�ړ�
			pCamera->posV += pAnimCamera->movePosV;
			pCamera->posR += pAnimCamera->movePosR;

			//�o�߃t���[�����Z
			pAnimCamera->nFrameCounter++;

			//�o�߃t���[�����S�̃t���[���ɒB����
			if (pAnimCamera->nWholeFrame <= pAnimCamera->nFrameCounter)
			{
				//�S�̃t���[���ɓ��B����
				pAnimCamera->bEnd = true;
			}
		}
		//�S�̃t���[���ɓ��B���Ă���
		else
		{
			//�d���t���[�����Z
			pAnimCamera->nWaitCount++;

			//�d���t���[�����I�����
			if (pAnimCamera->WaitTime <= pAnimCamera->nWaitCount)
			{
				//���̃E�F�[�u��
				g_WaveCamera = (WAVECamera)(g_WaveCamera + 1);

				//�E�F�[�u���I�����Ă��Ȃ�
				if (g_WaveCamera != WAVECamera_MAX)
				{
					//���̃A�j���[�V�����J������
					pAnimCamera++;

					//���̃E�F�[�u��
					SetNextWave(pCamera, *pAnimCamera, g_WaveCamera);
				}
			}
		}

		PrintDebugProc("\n�E�F�[�u�ԍ��F%d    ��f����:%d\n", nWave, g_nAnimTime++);
		PrintDebugProc("�o�߃t���[���F%d        �d���t���[���F%d\n", pAnimCamera->nFrameCounter, pAnimCamera->nWaitCount);
		PrintDebugProc("�S�̃t���[���F%d    �S�̍d���t���[���F%d\n\n", pAnimCamera->nWholeFrame, pAnimCamera->WaitTime);
	}

	else
	{
		PrintDebugProc("�E�F�[�u�I��    ��f����:%d�i��%d�b�j\n", g_nAnimTime, g_nAnimTime / MAX_FPS);
	}
}

//�E�F�[�u�ʂ̓��ʏ���
void SpecialWave(void)
{
	int nWave = g_WaveCamera;							//���݂̃E�F�[�u���擾
	AnimResCamera AnimCamera = g_AnimResCamera[nWave];	//�A�j���[�V�����p�J�����̃|�C���^

	//----------------------------------------------
	//			�E�F�[�u��  ���� �ݒ�
	//----------------------------------------------
	switch (g_WaveCamera)
	{
	//*******************************
	//	�E�F�[�u�Q�̏ꍇ�̓��ꏈ��
	//*******************************
	case WAVECamera_02_BirdEyes:
	{
		//--------------------------------------------
		//			�E�F�[�u�Q�̏����ݒ�
		// MEMO:�d���t���[�����@���Ԋu�Ńv���C���[���э��܂���
		//--------------------------------------------
		//�d���t���[������э��ݎ��ԂɂȂ���
		if (AnimCamera.nWaitCount % SP_PLAYER_DIVE_TIME == 0 && AnimCamera.nWaitCount > 0)
		{
			//�v���C���[���P�̂���э��܂���
			SetDivePlayer();
		}
	}
	break;

	//*******************************
	//	�E�F�[�u�R�̏ꍇ�̓��ꏈ��
	//*******************************
	case WAVECamera_03_SkyDiving:
	{
		//--------------------------------------------
		//			�E�F�[�u�R�̏����ݒ�
		// MEMO:�v���C���[�̊J�n�ʒu��ݒ肷��
		//--------------------------------------------
		if (AnimCamera.nFrameCounter == 0)
		{
			//�v���C���[�̏����ʒu��ς���
			for (int nCntSpWave = 0; nCntSpWave < MAX_USE_GAMEPAD; nCntSpWave++)
			{
				//�����_�̊J�n�ʒu�@+�@�����ʁ@�̏ꏊ�Ƀv���C���[���Z�b�g
				GetPlayer_RESULT()[nCntSpWave].pos.y = AnimCamera.StartPosR.y + SP_WAVE_03_START_Y;
			}
		}
	}
	break;

	//*******************************
	//	�E�F�[�u�S�̏ꍇ�̓��ꏈ��
	//*******************************
	case WAVECamera_LAST_VictoryStand:
	{
		//--------------------------------------------
		//			�E�F�[�u�S�̏����ݒ�
		// MEMO:�v���C���[�̊J�n�ʒu��ݒ肷��
		//--------------------------------------------
		if (AnimCamera.nFrameCounter == 0)
		{
			//�v���C���[�̏����ʒu��ς���
			for (int nCntSpWave = 0; nCntSpWave < MAX_USE_GAMEPAD; nCntSpWave++)
			{
				//�����_�̊J�n�ʒu�@+�@�����ʁ@�̏ꏊ�Ƀv���C���[���Z�b�g
				GetPlayer_RESULT()[nCntSpWave].pos.y = AnimCamera.StartPosR.y + SP_WAVE_LAST_START_Y;
			}
		}

		//--------------------------------------------
		//			�E�F�[�u�Ō�̕\����ݒ�
		//			MEMO:�\�����o�ꂳ����
		//--------------------------------------------
		if (AnimCamera.nFrameCounter == AnimCamera.nWholeFrame)
		{//�d���t���[�����I�����
			SetAppearVictoryStand();
		}
	}
	break;
	}
}

//���̃E�F�[�u��
void SetNextWave(ResultCamera *pCamera, AnimResCamera AnimCamera, int nWave)
{
	//���_�E�����_�������ʒu��
	pCamera->posV = AnimCamera.StartPosV;
	pCamera->posR = AnimCamera.StartPosR;
}

//���_�ړ�
void MovePosV(void)
{
	//���_�A�����_�Ƃ��ɍ��Ɉړ�
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_ResultCamera.posV.x -= g_ResultCamera.posR.x -= cosf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
		g_ResultCamera.posV.z -= g_ResultCamera.posR.z -= sinf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//���_�A�����_�Ƃ��ɉE�Ɉړ�
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_ResultCamera.posV.x += g_ResultCamera.posR.x += cosf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
		g_ResultCamera.posV.z += g_ResultCamera.posR.z += sinf(g_ResultCamera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//���_�㏸
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_ResultCamera.posV.y += MOVE_CAMERA_SPEED;
	}

	//���_���~
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_ResultCamera.posV.y -= MOVE_CAMERA_SPEED;
	}
}

//�����_�ړ�
void MovePosR(void)
{
	if (GetKeyboardTrigger(DIK_LSHIFT) == true)
	{
		//�����_�㏸
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_ResultCamera.posR.y += MOVE_CAMERA_SPEED;
		}

		//�����_���~
		if (GetKeyboardPress(DIK_S) == true)
		{
			g_ResultCamera.posR.y -= MOVE_CAMERA_SPEED;
		}
	}
	else
	{
		//�����_�㏸
		if (GetKeyboardPress(DIK_T) == true)
		{
			g_ResultCamera.posR.y += MOVE_CAMERA_SPEED;
		}

		//�����_���~
		if (GetKeyboardPress(DIK_H) == true)
		{
			g_ResultCamera.posR.y -= MOVE_CAMERA_SPEED;
		}
	}
}

//���_��]
void SpinPosV(void)
{
	if (GetKeyboardPress(DIK_Z))
	{
		g_ResultCamera.rot.y -= SPIN_CAMERA_SPEED;
	}

	if (GetKeyboardPress(DIK_C))
	{
		g_ResultCamera.rot.y += SPIN_CAMERA_SPEED;
	}

	//�p�x�C��
	FIX_ROT(g_ResultCamera.rot.y);
}

//���_�Ԃ̒�������
void LengthCamera(void)
{
	//�������߂Â���
	if (GetKeyboardPress(DIK_UP))
	{
		g_ResultCamera.fLength -= MOVE_CAMERA_SPEED;
	}

	//�������߂Â���
	if (GetKeyboardPress(DIK_DOWN))
	{
		g_ResultCamera.fLength += MOVE_CAMERA_SPEED;
	}

	//�Œ዗�����������
	if (g_ResultCamera.fLength < MIN_LENGTH)
	{
		//�Œ዗���ɏC��
		g_ResultCamera.fLength = MIN_LENGTH;
	}
}

//���_��]
void SpinResultPosV(void)
{

}

//�J�����̈ړ�����
void MoveResultCamera(D3DXVECTOR3 ParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = ParentPos.x + sinf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
	pChildPos->z = ParentPos.z + cosf((CorRot - g_ResultCamera.rot.y)) * g_ResultCamera.fLength;
}

//�J�����̈ʒu���Z�b�g
void ResetResultCameraPos(void)
{
	g_ResultCamera.posR = POS_POSR;
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
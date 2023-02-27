/*==========================================================================================================================================================

�Q�[����ʏ���[HDR_game.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "HDRgame.h"
#include "input.h"
#include "file.h"
#include "color.h"
#include "fade.h"
#include "HDR_camera.h"
#include "camera_frame.h"
#include "pause.h"
#include "hdr_player.h"
#include "model.h"
#include "light.h"
#include "timer.h"
#include "sound.h"
#include "bg.h"
#include "block.h"
#include "meshfield.h"
#include "meshdome.h"
#include "fence.h"

//�O���[�o���ϐ��錾
bool g_bPause_HDR = false;				// �|�[�Y
int  g_nUseContNum_HDR;					// �g�p���Ă���R���g���[���[�̐�
bool g_abUsePlayer_HDR[MAX_USE_GAMEPAD];// �g�p���Ă���v���C���[�i�ڑ��`�F�b�N�Ɏg�p�j
bool g_bDisconnectPlayer_HDR;			// �ڑ����؂ꂽ���Ƃ��m�F����
int  g_numGamePad_HDR;
CHECKMODE	g_CheckMode_HDR;
NumCamera	g_NumCamera_HDR;
bool		g_bPhotoMode_HDR;			// �t�H�g���[�h�ؑ�		true:�|�[�Y��ʔ�\��	false:�{�[�Y��ʕ\��

//------------------------------------------------
//				�Q�[���̏���������
//------------------------------------------------
void InitHDRGame(void)
{
	g_nUseContNum_HDR = SetUseController_HDR();		// �R���g���[���[�̎g�p�ݒ�
	InitFile();								// �t�@�C���̏����������i���f���r���[���[�t�@�C���ǂݍ��ݑO�ɍs�����ƁI�j
	LoadModelViewerFile("data\\model.txt");	// ���f���r���[���[�t�@�C���ǂݍ��݁i�e�I�u�W�F�N�g�������O�ɍs�����ƁI�j
	LoadModelOriginalFile("data\\originalmodel.txt");	// ���f���I���W�i���t�@�C���ǂݍ���
	g_NumCamera_HDR = NumCamera_ONLY;			// �����J�����̐ݒ�i���݂�Player0�𒍎��_�Ƃ����J�����@�@��ʕ����i�V�j

	
	InitLight();				// ���C�g����������
	InitModel();				// ���f���̏����������i�v���C���[�̑O�ɍs�����ƁI�j
	InitPlayer_HDR();
	InitCameraFrame();			// ��ʕ����̘g����������
	InitCamera(g_NumCamera_HDR);	// �J�����̏���������
	InitPause();				// �|�[�Y��ʂ̏���������
	InitBlock();				// �u���b�N�̏���������
	InitMeshfield();
	InitMeshDome();
	InitFence();
	InitTime();					// �^�C�}�[�̏���������

	SetTime(LIMIT_TIMER);		// �^�C�}�[�̐ݒ菈��

	g_bPause_HDR = false;				// �|�[�Y�̏�����
	g_bDisconnectPlayer_HDR = false;	//����ɃR���g���[���[���ڑ�����Ă����ԂƂ���
	g_bPhotoMode_HDR = false;			// �t�H�g���[�h������

	//�Q�[��BGM�J�n
	PlaySound(SOUND_LABEL_BGM_GAME);
}

//------------------------------------------------
//				�Q�[���̏I������
//------------------------------------------------
void UninitHDRGame(void)
{
	/*======================================================================
	�e��I�u�W�F�N�g�̏I������
	========================================================================*/

	UninitLight();			// ���C�g�I������
	UninitCamera();			// �J�����̏I������
	UninitPause();			// �|�[�Y��ʂ̏I������
	UninitPlayer_HDR();
	UninitModel();			// ���f���̏I������
	UninitBlock();			// �u���b�N�̏I������
	UninitMeshfield();
	UninitMeshDome();
	UninitFence();

	UninitCameraFrame();	// ��ʕ����̘g�I������
	UninitTime();			// �^�C�}�[�̏I�������i�����͏��Ԃ͖��Ȃ��j

	//�Q�[��BGM��~
	StopSound(SOUND_LABEL_BGM_GAME);
}

//------------------------------------------------
//				�Q�[���̍X�V����
//------------------------------------------------
void UpdateHDRGame(void)
{
	FADE fadeState = GetFade();

	//�|�[�Y��OFF
	if (g_bPause_HDR == false)
	{
		UpdateLight();			// ���C�g�̍X�V����
		UpdateCamera();			// �J�����̍X�V����
		UpdateTime();			//�^�C�}�[�̍X�V����
		UpdateBlock();			// �u���b�N�̍X�V����
		UpdatePlayer_HDR();


		ChangeNumCamera_HDR();	//�J�����̐��ύX����

								//�|�[�Y�擾
		for (int nCntPause = 0; nCntPause < 4; nCntPause++)
		{
			//�|�[�Y��Ԑ؂�ւ�
			if (GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(nCntPause, XINPUT_GAMEPAD_START) == true)
			{
				//���Ԗڂ̃Q�[���p�b�h���ۑ�����
				g_numGamePad_HDR = nCntPause;

				PlaySound(SOUND_LABEL_SE_PAUSE_DECISION);

				//�|�[�Y��Ԃɂ���
				g_bPause_HDR = true;

				//�|�[�Y�����R���g���[���[��pause.cpp�ɓn��
				SetPadPause(false, nCntPause);
			}
		}
	}

	else
	{
		//�|�[�Y��ʂ̍X�V����
		UpdatePause(MODE_RaceGAME);

		//�t�H�g���[�hON
		if (g_bPhotoMode_HDR)
		{
			UpdateCamera();

			//1�t���[�������X�V����
			if (GetKeyboardTrigger(DIK_RIGHT))
			{
				UpdateLight();			// ���C�g�̍X�V����
				UpdateTime();			//�^�C�}�[�̍X�V����
			}
		}
	}

	//�R���g���[���[�̎g�p�`�F�b�N�̓|�[�Y��Ԋ֌W�Ȃ��Ăяo��
	CheckUseController_HDR(CHECKMODE_DISCONPAUSE);

	//�t�H�g���[�h�ؑ�
	if (GetKeyboardTrigger(DIK_F9))
	{
		g_bPhotoMode_HDR = g_bPhotoMode_HDR ? false : true;
	}
}

//------------------------------------------------
//				�Q�[���̕`�揈��
//------------------------------------------------
void DrawHDRGame(void)
{
	/*======================================================================
	�e��I�u�W�F�N�g�̕`�揈��
	========================================================================*/
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		//�Q�[�����I�u�W�F�N�g�̕`�揈��
		SetCamera(nCntCamera);		// �J�����̐ݒ菈��
		DrawCameraFrame();			// ��ʕ����̘g�`�揈��
		DrawTime();					//�^�C�}�[�̕`�揈��
		DrawBlock();				// �u���b�N�̕`�揈��
		DrawPlayer_HDR();
		DrawMeshfield();
		DrawMeshDome();
		DrawFence();

									//�|�[�Y��ON
		if (g_bPause_HDR == true && g_bPhotoMode_HDR == false)
		{
			DrawPause();		//�|�[�Y��ʕ`�揈��
		}
	}
}

//------------------------------------------------
//		�g�p����R���g���[���[�̐ݒ�
//		Author:�Ό��D�n
//		�Ԃ�l:�g�p����Ă���i�ڑ�����Ă���j�R���g���[���[�̐�
//		Memo:InitGame�ŌĂяo���iGetUseController���Ăяo�����O�ɌĂяo�����邱�Ɓj
//------------------------------------------------
int SetUseController_HDR(void)
{
	int nUseController = 0;
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		if (GetUseGamepad(nCntController) == true)
		{//�g�p����Ă���
			g_abUsePlayer_HDR[nCntController] = true;
			nUseController++;	//�R���g���[���[�̐��ǉ�
		}
		else
		{//�g�p����Ă��Ȃ�
			g_abUsePlayer_HDR[nCntController] = false;
		}
	}
	return nUseController;	//�g�p����Ă���R���g���[���[�̐���Ԃ�
}

//------------------------------------------------
//		�g�p���Ă���R���g���[���[�̐ڑ��`�F�b�N
//		Author:�Ό��D�n
//		����1:�`�F�b�N���[�h�w��
//		CHECKMODE_DISCONNECT:�ؒf����Ă���ꍇ�����|�[�Y��Ԃɂ��郂�[�h
//		CHECKMODE_REMOVE	:�ؒf����Ă���ꍇ���̃R���g���[���[���g�p���Ă��Ȃ���Ԃɂ���
//		Memo:UpdateGame���Ń|�[�Y���ł��Ăяo�����邱��
//------------------------------------------------
bool CheckUseController_HDR(CHECKMODE mode)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//�Ή��R���g���[���[���J��Ԃ�
		if (g_abUsePlayer_HDR[nCntPlayer] == true && g_abUsePlayer_HDR[nCntPlayer] != GetUseGamepad(nCntPlayer))
		{//�ؒf�����m
			switch (mode)
			{
			case CHECKMODE_DISCONPAUSE:
				g_bPause_HDR = true;				//�����I�Ƀ|�[�Y��Ԃɂ���
				g_bDisconnectPlayer_HDR = true;		//�ؒf���ꂽ���Ƃɂ���
				SetPadPause(true);
				return true;	//�֐��I��
				break;
			case CHECKMODE_DISCONNOPAUSE:
				g_bDisconnectPlayer_HDR = true;		//�ؒf���ꂽ���Ƃɂ���
				return true;	//�֐��I��
				break;
			case CHECKMODE_REMOVE:
				g_abUsePlayer_HDR[nCntPlayer] = false;
				g_nUseContNum_HDR--;	//�R���g���[���[�g�p�����炷
				break;
			default:
				break;
			}
		}
	}
	g_bDisconnectPlayer_HDR = false;	//for��������ɏI����������Ȃ�
	return false;
}

//------------------------------------------------
//		�R���g���[���[�̎g�p�L���擾����
//		Author:�Ό��D�n
//		����1:���ׂ�p�b�h�̔ԍ�
//------------------------------------------------
bool GetUseController_HDR(int nPadNum)
{
	return g_abUsePlayer_HDR[nPadNum];
}

//------------------------------------------------
//		�R���g���[���[�̎g�p���擾����
//		Author:�Ό��D�n
//------------------------------------------------
int GetUseControllerNum_HDR(void)
{
	return g_nUseContNum_HDR;
}

//------------------------------------------------
//		�|�[�Y��Ԃ̐ݒ菈��
//		Author:�Ό��D�n
//------------------------------------------------
void SetEnablePause_HDR(bool pause)
{
	g_bPause_HDR = pause;
}

//------------------------------------------------
//		�J�����̐��ύX����
//		Author:���V����
//------------------------------------------------
void ChangeNumCamera_HDR(void)
{
	if (GetKeyboardTrigger(DIK_F7))
	{
		//���݂̃J�����̎�ނ�ۑ�
		int nType = g_NumCamera_HDR;

		//���̎�ނɕύX
		nType++;

		//�ύX�ɔ����A�S��ނ̐��𒴂��Ȃ��悤�ɂ���
		nType %= NumCamera_MAX;

		//�ύX�������̂�����
		g_NumCamera_HDR = (NumCamera)nType;

		//�J�����̎�ނ�ݒ�
		Set_NumCamera(g_NumCamera_HDR);
	}
}
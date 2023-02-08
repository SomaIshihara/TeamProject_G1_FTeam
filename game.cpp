/*==========================================================================================================================================================

�Q�[����ʏ���[game.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "file.h"
#include "camera.h"
#include "color.h"
#include "player.h"
#include "model.h"
#include "wall.h"
#include "game.h"
#include "fade.h"
#include "bg.h"
#include "meshfield.h"
#include "light.h"
#include "meshcylinder.h"
#include "pause.h"
#include "meshdome.h"
#include "Gauge.h"
#include "charge_effect.h"
#include "score.h"
#include "bonus.h"
#include "item.h"
#include "camera_frame.h"
#include "timer.h"
#include "particle.h"
#include "attack_effect.h"
#include "tremor_effect.h"
#include "meshfault.h"
#include "charge_cylinder.h"
//#include "sound.h"

//�O���[�o���ϐ��錾
bool g_bPause = false;				// �|�[�Y
int  g_nUseContNum;					// �g�p���Ă���R���g���[���[�̐�
bool g_abUsePlayer[MAX_USE_GAMEPAD];// �g�p���Ă���v���C���[�i�ڑ��`�F�b�N�Ɏg�p�j
bool g_bDisconnectPlayer;			// �ڑ����؂ꂽ���Ƃ��m�F����
int  g_numGamePad;
CHECKMODE	g_CheckMode;
NumCamera	g_NumCamera;
bool		g_bPhotoMode;			// �t�H�g���[�h�ؑ�		true:�|�[�Y��ʔ�\��	false:�{�[�Y��ʕ\��

//------------------------------------------------
//				�Q�[���̏���������
//------------------------------------------------
void InitGame(void)
{
	g_nUseContNum = SetUseController();		// �R���g���[���[�̎g�p�ݒ�
	InitFile();								// �t�@�C���̏����������i���f���r���[���[�t�@�C���ǂݍ��ݑO�ɍs�����ƁI�j
	LoadModelViewerFile("data\\model.txt");	// ���f���r���[���[�t�@�C���ǂݍ��݁i�e�I�u�W�F�N�g�������O�ɍs�����ƁI�j
	g_NumCamera = NumCamera_ONLY;			// �����J�����̐ݒ�i���݂�Player0�𒍎��_�Ƃ����J�����@�@��ʕ����i�V�j

	InitBg();					// �w�i�̏���������
	InitLight();				// ���C�g����������
	InitMeshfield();			// �X�e�[�W����������
	InitMeshCylinder();			// ���b�V���V�����_�[����������
	InitMeshDome();				// ���b�V���h�[������������
	InitMeshFault();			// ���b�V���̒f�ʏ���������
	InitModel();				// ���f���̏����������i�v���C���[�̑O�ɍs�����ƁI�j
	InitPlayer();				// �v���C���[�̏���������
	InitBonus();				// �{�[�i�X�̏���������
	InitItem();					// �A�C�e���̏���������
	InitCameraFrame();			// ��ʕ����̘g����������
	InitCamera(g_NumCamera);	// �J�����̏���������
	InitGauge();				// �Q�[�W�̏���������
	InitWall();					// �ǂ̏���������
	InitPause();				// �|�[�Y��ʂ̏���������
	InitParticle();				// �p�[�e�B�N���̏���������
	{// �G�t�F�N�g�̏���������
		InitChargeEffect();//�`���[�W�G�t�F�N�g
		InitChargeCylinder();//�`���[�W�G�t�F�N�g(����񂾁[)
		InitAttackEffect();//�U���G�t�F�N�g
		InitTremorEffect();//�q�b�v�h���b�v�G�t�F�N�g
	}	
	InitScore();				// �X�R�A�̏�����
	InitTime();					// �^�C�}�[�̏���������
	SetTime(LIMIT_TIMER);		// �^�C�}�[�̐ݒ菈��

	SetScore(0,4);				// �X�R�A�̐ݒ菈��

	g_bPause = false;			// �|�[�Y�̏�����
	g_bDisconnectPlayer = false;	//����ɃR���g���[���[���ڑ�����Ă����ԂƂ���
	g_bPhotoMode = false;		// �t�H�g���[�h������
									//�Q�[��BGM�J�n
									//PlaySound(SOUND_LABEL_GAMEBGM);
}

//------------------------------------------------
//				�Q�[���̏I������
//------------------------------------------------
void UninitGame(void)
{
	/*======================================================================
	�e��I�u�W�F�N�g�̏I������
	========================================================================*/

	/*�G�t�F�N�g�Ȃǂ̔|���S���̏I������*/
	UninitBg();				// �w�i�̏I������
	UninitLight();			// ���C�g�I������
	UninitMeshfield();		// �X�e�[�W�I������
	UninitMeshCylinder();	// ���b�V���V�����_�[�I������
	UninitMeshDome();		// ���b�V���h�[���I������
	UninitMeshFault();		// ���b�V���̒f�ʏI������
	UninitWall();			// �ǂ̏I������
	UninitCamera();			// �J�����̏I������
	UninitPlayer();			// �v���C���[�̏I������
	UninitBonus();			// �{�[�i�X�̏I������
	UninitItem();			// �A�C�e���̏I������
	UninitPause();			// �|�[�Y��ʂ̏I������
	UninitGauge();			// �Q�[�W�̏I������
	UninitModel();			// ���f���̏I������
	UninitParticle();		// �p�[�e�B�N���̏I������
	{	// �G�t�F�N�g�̏I������
		UninitChargeEffect();	//�`���[�W�G�t�F�N�g
		UninitChargeCylinder();	//�`���[�W�G�t�F�N�g(����񂾁[)
		UninitAttackEffect();	//�U���G�t�F�N�g
		UninitTremorEffect();	//�q�b�v�h���b�v�G�t�F�N�g
	}
	UninitScore();		// �X�R�A�̏I������
	UninitCameraFrame();// ��ʕ����̘g�I������
	UninitTime();		//�^�C�}�[�̏I�������i�����͏��Ԃ͖��Ȃ��j

						//�Q�[��BGM��~
						//StopSound(SOUND_LABEL_GAMEBGM);
}

//------------------------------------------------
//				�Q�[���̍X�V����
//------------------------------------------------
void UpdateGame(void)
{
	FADE fadeState = GetFade();
	Player *pPlayer = GetPlayer();	

	//�|�[�Y��OFF
	if (g_bPause == false)
	{
		UpdateBg();				// �w�i�̍X�V����
		UpdateLight();			// ���C�g�̍X�V����
		UpdateMeshfield();		// �X�e�[�W�X�V����
		UpdateMeshCylinder();	// ���b�V���V�����_�[�X�V����
		UpdateMeshDome();		// ���b�V���h�[���X�V����
		UpdateMeshFault();		// ���b�V���̒f�ʍX�V����
		UpdatePlayer();			// �v���C���[�̍X�V����
		UpdateBonus();			// �{�[�i�X�̍X�V����
		UpdateItem();			// �A�C�e���̍X�V����
		UpdateCamera();			// �J�����̍X�V����
		UpdateWall();			// �ǂ̍X�V����
		UpdateGauge();			// �Q�[�W�̍X�V����
		{// �G�t�F�N�g�̍X�V����
			UpdateChargeEffect();	//�`���[�W�G�t�F�N�g
			UpdateChargeCylinder();	//�`���[�W�G�t�F�N�g(����񂾁[)
			UpdateAttackEffect();	//�U���G�t�F�N�g
			UpdateTremorEffect();	//�q�b�v�h���b�v�G�t�F�N�g
		}
		UpdateParticle();	// �p�[�e�B�N���̍X�V����
		UpdateScore();		//�X�R�A�̍X�V����
		UpdateTime();		//�^�C�}�[�̍X�V����

		ChangeNumCamera();	//�J�����̐��ύX����

		//�|�[�Y�擾
		for (int nCntPause = 0; nCntPause < 4; nCntPause++)
		{
			//�|�[�Y��Ԑ؂�ւ�
			if (GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(nCntPause, XINPUT_GAMEPAD_START) == true)
			{
				//���Ԗڂ̃Q�[���p�b�h���ۑ�����
				g_numGamePad = nCntPause;

				//�|�[�Y��Ԃɂ���
				g_bPause = true;

				//�|�[�Y�����R���g���[���[��pause.cpp�ɓn��
				SetPadPause(false, nCntPause);
			}
		}
	}

	else
	{
		//�t�H�g���[�hOFF
		if (g_bPhotoMode == false)
		{
			//�|�[�Y��ʂ̍X�V����
			UpdatePause();
		}

		//�J���������͓�������
		else
		{
			UpdateCamera();
		}
	}

	//�R���g���[���[�̎g�p�`�F�b�N�̓|�[�Y��Ԋ֌W�Ȃ��Ăяo��
	CheckUseController(CHECKMODE_DISCONPAUSE);

	//�t�H�g���[�h�ؑ�
	if (GetKeyboardTrigger(DIK_F9) && g_bPause)
	{
		g_bPhotoMode = g_bPhotoMode ? false : true;
	}
}

//------------------------------------------------
//				�Q�[���̕`�揈��
//------------------------------------------------
void DrawGame(void)
{
	/*======================================================================
	�e��I�u�W�F�N�g�̕`�揈��
	========================================================================*/
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		//�Q�[�����I�u�W�F�N�g�̕`�揈��
		SetCamera(nCntCamera);		// �J�����̐ݒ菈��

		DrawMeshfield();			// �X�e�[�W�̕`�揈��
									//DrawMeshCylinder();	// ���b�V���V�����_�[�̕`�揈��
		DrawMeshDome();				// ���b�V���h�[���̕`�揈��	
		DrawMeshFault();			// ���b�V���̒f�ʕ`�揈��
		DrawWall();					// �ǂ̕`�揈��
		{// �G�t�F�N�g�̕`�揈��
			DrawChargeEffect();		//�`���[�W�G�t�F�N�g
			DrawChargeCylinder();	//�`���[�W�G�t�F�N�g(����񂾁[)
			DrawAttackEffect();		//�U���G�t�F�N�g
			DrawTremorEffect();		//�q�b�v�h���b�v�G�t�F�N�g
		}	
		DrawParticle();				// �p�[�e�B�N���̕`�揈��
		DrawPlayer();				// �v���C���[�̕`�揈��
		DrawBonus();				// �{�[�i�X�̕`�揈��
		DrawItem();					// �A�C�e���̕`�揈��
		DrawGauge();				// �Q�[�W�̕`�揈��
		DrawScore();				// �X�R�A�̕`�揈��
		DrawCameraFrame();			// ��ʕ����̘g�`�揈��
		DrawTime();					//�^�C�}�[�̕`�揈��

									//�|�[�Y��ON
		if (g_bPause == true && g_bPhotoMode == false)
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
int SetUseController(void)
{
	int nUseController = 0;
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		if (GetUseGamepad(nCntController) == true)
		{//�g�p����Ă���
			g_abUsePlayer[nCntController] = true;
			nUseController++;	//�R���g���[���[�̐��ǉ�
		}
		else
		{//�g�p����Ă��Ȃ�
			g_abUsePlayer[nCntController] = false;
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
bool CheckUseController(CHECKMODE mode)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//�Ή��R���g���[���[���J��Ԃ�
		if (g_abUsePlayer[nCntPlayer] == true && g_abUsePlayer[nCntPlayer] != GetUseGamepad(nCntPlayer))
		{//�ؒf�����m
			switch (mode)
			{
			case CHECKMODE_DISCONPAUSE:
				g_bPause = true;				//�����I�Ƀ|�[�Y��Ԃɂ���
				g_bDisconnectPlayer = true;		//�ؒf���ꂽ���Ƃɂ���
				SetPadPause(true);
				return true;	//�֐��I��
				break;
			case CHECKMODE_DISCONNOPAUSE:
				g_bDisconnectPlayer = true;		//�ؒf���ꂽ���Ƃɂ���
				return true;	//�֐��I��
				break;
			case CHECKMODE_REMOVE:
				g_abUsePlayer[nCntPlayer] = false;
				g_nUseContNum--;	//�R���g���[���[�g�p�����炷
				break;
			default:
				break;
			}
		}
	}
	g_bDisconnectPlayer = false;	//for��������ɏI����������Ȃ�
	return false;
}

//------------------------------------------------
//		�R���g���[���[�̎g�p�L���擾����
//		Author:�Ό��D�n
//		����1:���ׂ�p�b�h�̔ԍ�
//------------------------------------------------
bool GetUseController(int nPadNum)
{
	return g_abUsePlayer[nPadNum];
}

//------------------------------------------------
//		�R���g���[���[�̎g�p���擾����
//		Author:�Ό��D�n
//------------------------------------------------
int GetUseControllerNum(void)
{
	return g_nUseContNum;
}

//------------------------------------------------
//		�|�[�Y��Ԃ̐ݒ菈��
//		Author:�Ό��D�n
//------------------------------------------------
void SetEnablePause(bool pause)
{
	g_bPause = pause;
}

//------------------------------------------------
//		�J�����̐��ύX����
//		Author:���V����
//------------------------------------------------
void ChangeNumCamera(void)
{
	if (GetKeyboardTrigger(DIK_F7))
	{
		//���݂̃J�����̎�ނ�ۑ�
		int nType = g_NumCamera;

		//���̎�ނɕύX
		nType++;

		//�ύX�ɔ����A�S��ނ̐��𒴂��Ȃ��悤�ɂ���
		nType %= NumCamera_MAX;

		//�ύX�������̂�����
		g_NumCamera = (NumCamera)nType;

		//�J�����̎�ނ�ݒ�
		Set_NumCamera(g_NumCamera);
	}
}
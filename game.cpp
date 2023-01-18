/*==========================================================================================================================================================

�Q�[����ʏ���[game.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
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
//#include "sound.h"

//�v���g

int SetUsePlayer(void);
void CheckUsePlayer(void);

//�O���[�o���ϐ��錾
bool g_bPause = false;				// �|�[�Y
bool g_abUsePlayer[MAX_USE_GAMEPAD];//�g�p���Ă���v���C���[�i�ڑ��`�F�b�N�Ɏg�p�j
bool g_bDisconnectPlayer;			//�ڑ����؂ꂽ���Ƃ��m�F����

//------------------------------------------------
//				�Q�[���̏���������
//------------------------------------------------
void InitGame(void)
{
	InitBg();			// �w�i�̏���������
	InitLight();		// ���C�g����������
	InitMeshfield();	// �X�e�[�W����������
	InitMeshCylinder();	// ���b�V���V�����_�[����������
	InitModel();		// ���f���̏����������i�v���C���[�̑O�ɍs�����ƁI�j
	InitPlayer();		// �v���C���[�̏���������
	InitCamera();		// �J�����̏���������
	InitWall();			// �ǂ̏���������
	InitPause();		// �|�[�Y��ʂ̏���������

	g_bPause = false;	// �|�[�Y�̏�����
	g_bDisconnectPlayer = false;	//����ɃR���g���[���[���ڑ�����Ă����ԂƂ���

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
	UninitBg();			// �w�i�̏I������
	UninitLight();		// ���C�g�I������
	UninitMeshfield();	// �X�e�[�W�I������
	UninitMeshCylinder();	// ���b�V���V�����_�[�I������
	UninitWall();		// �ǂ̏I������
	UninitCamera();		// �J�����̏I������
	UninitPlayer();		// �v���C���[�̏I������
	UninitPause();		// �|�[�Y��ʂ̏I������
	UninitModel();		// ���f���̏I�������i�����͏��Ԃ͖��Ȃ��j

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
			UpdateBg();			// �w�i�̍X�V����
			UpdateLight();		// ���C�g�̍X�V����
			UpdateMeshfield();	// �X�e�[�W�X�V����
			UpdateMeshCylinder();	// ���b�V���V�����_�[�X�V����
			UpdatePlayer();		// �v���C���[�̍X�V����
			UpdateCamera();		// �J�����̍X�V����
			UpdateWall();		// �ǂ̍X�V����
	}

	else
	{
		//�|�[�Y��ʂ̍X�V����
		UpdatePause();

		//�|�[�Y����������Ƃ��Ƀ��j���[���R���e�B�j���[�ɐݒ�
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			SetPause(PAUSE_CONTINUE);
		}

		if (*GetPause() == PAUSE_CONTINUE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				g_bPause = false;
			}
		}
	}

	//�|�[�Y��Ԑ؂�ւ�
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
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
	//�Q�[�����I�u�W�F�N�g�̕`�揈��
	SetCamera();		// �J�����̐ݒ菈��

	DrawMeshfield();	// �X�e�[�W�̕`�揈��
	DrawMeshCylinder();	// ���b�V���V�����_�[�̕`�揈��
	DrawWall();			// �ǂ̕`�揈��
	DrawPlayer();		// �v���C���[�̕`�揈��

	//�|�[�Y��OFF
	if (g_bPause == true)
	{
		DrawPause();		//�|�[�Y��ʕ`�揈��
	}

}

//------------------------------------------------
//		�g�p����v���C���[�̐ݒ�
//		Author:�Ό��D�n
//		�Ԃ�l:�g�p����Ă���i�ڑ�����Ă���j�R���g���[���[�̐�
//		Memo:InitGame�ŌĂяo��
//------------------------------------------------
int SetUsePlayer(void)
{
	int nUsePlayer = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (GetUseGamepad(nCntPlayer) == true)
		{//�g�p����Ă���
			g_abUsePlayer[nCntPlayer] = true;
			nUsePlayer++;	//�R���g���[���[�̐��ǉ�
		}
		else
		{//�g�p����Ă��Ȃ�
			g_abUsePlayer[nCntPlayer] = false;
		}
	}
	return nUsePlayer;	//�g�p����Ă���R���g���[���[�̐���Ԃ�
}

//------------------------------------------------
//		�g�p���Ă���v���C���[�̐ڑ��`�F�b�N
//		Author:�Ό��D�n
//		Memo:UpdateGame���Ń|�[�Y���ł��Ăяo�����邱��
//------------------------------------------------
void CheckUsePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//�Ή��R���g���[���[���J��Ԃ�
		if (g_abUsePlayer[nCntPlayer] == true && g_abUsePlayer[nCntPlayer] != GetUseGamepad(nCntPlayer))
		{//�ؒf�����m
			g_bPause = true;				//�����I�Ƀ|�[�Y��Ԃɂ���
			g_bDisconnectPlayer = true;		//�ؒf���ꂽ���Ƃɂ���
			return;	//�֐��I��
		}
	}
	g_bDisconnectPlayer = false;	//for��������ɏI����������Ȃ�
}

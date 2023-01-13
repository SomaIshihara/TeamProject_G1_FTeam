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
//#include "pause.h"
//#include "sound.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
bool					g_bPause = false;				// �|�[�Y

//------------------------------------------------
//				�Q�[���̏���������
//------------------------------------------------
void InitGame(void)
{
	InitBg();			// �w�i�̏���������					
	InitLight();		//���C�g����������
	InitMeshfield();	// �X�e�[�W����������
	InitModel();		// ���f���̏����������i�v���C���[�̑O�ɍs�����ƁI�j
	InitPlayer();		// �v���C���[�̏���������
	InitCamera();		// �J�����̏���������
	InitWall();			// �ǂ̏���������

	g_bPause = false;	// �|�[�Y�̏�����

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
	UninitLight();		//���C�g�I������
	UninitMeshfield();	// �X�e�[�W�I������
	UninitWall();		// �ǂ̏I������
	UninitCamera();		// �J�����̏I������
	UninitPlayer();		// �v���C���[�̏I������
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
			UpdateLight();		//���C�g�̍X�V����
			UpdateMeshfield();	// �X�e�[�W�X�V����
			UpdatePlayer();		// �v���C���[�̍X�V����
			UpdateCamera();		// �J�����̍X�V����
			UpdateWall();		// �ǂ̍X�V����
	}

	else
	{
		//�|�[�Y��ʂ̍X�V����
		//UpdatePause();

		//�|�[�Y����������Ƃ��Ƀ��j���[���R���e�B�j���[�ɐݒ�
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			//SetPause(PAUSE_MENU_CONTINUE);
		}

		//if (*GetPause() == PAUSE_MENU_CONTINUE)
		//{
		//	if (GetKeyboardTrigger(DIK_RETURN) == true)
		//	{
		//		g_bPause = false;
		//	}
		//}
	}

	//�|�[�Y��Ԑ؂�ւ�
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = g_bPause ? false : true;
	}

	//Enter�������ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//�^�C�g����ʂɑJ��
		SetFade(MODE_TITLE);
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

	//DrawBg();			// �w�i�̕`�揈��
	DrawMeshfield();	// �X�e�[�W�̕`�揈��
	DrawWall();			// �ǂ̕`�揈��
	DrawPlayer();		// �v���C���[�̕`�揈��
}
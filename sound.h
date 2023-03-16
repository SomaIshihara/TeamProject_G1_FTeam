//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �Ό��D�n  ���V����
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//�}�N����`
#define NUM_DUPLICATION	(4)		//�d����

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g����
	SOUND_LABEL_BGM_TUTORIAL,		// �`���[�g���A����
	SOUND_LABEL_BGM_GAME_PVP,		// �Q�[����(PVP)
	SOUND_LABEL_BGM_GAME_HDR,		// �Q�[����(HDR)
	SOUND_LABEL_BGM_RESULT_01,		// ���U���g��1�i�J�n
	SOUND_LABEL_BGM_RESULT_02,		// ���U���g��2�i�]�C
	SOUND_LABEL_BGM_MAX,
} SOUND_LABEL_BGM;

typedef enum
{
	SOUND_LABEL_SE_WARNING,				// �t�F���X�������ӌ��ʉ�
	SOUND_LABEL_SE_COUNTDOWN,			// �Q�[���I���J�E���g�_�E��
	SOUND_LABEL_SE_CONNECT,				// �R���g���[���[�ڑ����̌��ʉ�
	SOUND_LABEL_SE_TITLE_DECIDE,		// �^�C�g����ʂ̌��艹
	SOUND_LABEL_SE_COLLISION,			// �v���C���[���m�̏Փˉ�
	SOUND_LABEL_SE_ENERGY_00,			// �v���C���[1�̃Q�[�W�[�U��
	SOUND_LABEL_SE_DROP,				// �v���C���[�̗�����
	SOUND_LABEL_SE_GRASSDASH,			// �v���C���[�̃_�b�V����
	SOUND_LABEL_SE_HIPSPIN,				// �v���C���[�̃q�b�v�h���b�v�X�s����
	SOUND_LABEL_SE_HIPDROP,				// �v���C���[�̃q�b�v�h���b�v��
	SOUND_LABEL_SE_JUMP,				// �v���C���[�̃W�����v��
	SOUND_LABEL_SE_HIPRANK_00_BAD,		// �q�b�v�h���b�v�����N �uBad...�v�̌��ʉ�
	SOUND_LABEL_SE_HIPRANK_01_NICE,		//			�V			�uNICE�v
	SOUND_LABEL_SE_HIPRANK_02_GOOD,		//			�V			�uGOOD!�v
	SOUND_LABEL_SE_HIPRANK_03_GREAT,	//			�V			�u��GREAT���v
	SOUND_LABEL_SE_HIPRANK_04_PERFECT,	//			�V			�u��PERFECT���v
	SOUND_LABEL_SE_DESTROY_BLOCK_00,	// �u���b�N�j��	�P�ڂ̌��ʉ�
	SOUND_LABEL_SE_DESTROY_BLOCK_01,	//		�V		�Q��
	SOUND_LABEL_SE_DESTROY_BLOCK_02,	//		�V		�R��
	SOUND_LABEL_SE_DESTROY_BLOCK_03,	//		�V		�S��
	SOUND_LABEL_SE_DESTROY_BLOCK_04,	//		�V		�T��
	SOUND_LABEL_SE_DESTROY_BLOCK_05,	//		�V		�U��
	SOUND_LABEL_SE_DESTROY_BLOCK_06,	//		�V		�V��
	SOUND_LABEL_SE_DESTROY_BLOCK_07,	//		�V		�W��
	SOUND_LABEL_SE_PAUSE_DECISION,		// �|�[�Y�̌��艹
	SOUND_LABEL_SE_PAUSE_SELECTION,		// �|�[�Y�̑I����
	SOUND_LABEL_SE_PAUSE_TRANSITION,	// �|�[�Y�̑J�ډ�
	SOUND_LABEL_SE_ROAR,				// �|�[�Y�̙��K��
	SOUND_LABEL_SE_CHEERS,				// ���U���g�̊���
	SOUND_LABEL_SE_MAX,
} SOUND_LABEL_SE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
void StopAllSound(void);

HRESULT PlaySoundBGM(SOUND_LABEL_BGM label);
void StopSoundBGM(SOUND_LABEL_BGM label);
void StopSoundBGM(void);
void PauseSound(SOUND_LABEL_BGM label);
void RestartSound(SOUND_LABEL_BGM label);

HRESULT PlaySoundSE(SOUND_LABEL_SE label, int nNumSound);
void StopSoundSE(SOUND_LABEL_SE label);
void StopSoundSE(void);

#endif

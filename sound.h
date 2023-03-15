//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �Ό��D�n
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
	SOUND_LABEL_BGM_RESULT,			// ���U���g��
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
	SOUND_LABEL_SE_HIPRANK_00_BAD,		// �q�b�v�h���b�v�����N�uBad...�v�̌��ʉ�
	SOUND_LABEL_SE_HIPRANK_01_NICE,		// �q�b�v�h���b�v�����N�uNICE�v�̌��ʉ�
	SOUND_LABEL_SE_HIPRANK_02_GOOD,		// �q�b�v�h���b�v�����N�uGOOD!�v�̌��ʉ�
	SOUND_LABEL_SE_HIPRANK_03_GREAT,	// �q�b�v�h���b�v�����N�u��GREAT���v�̌��ʉ�
	SOUND_LABEL_SE_HIPRANK_04_PERFECT,	// �q�b�v�h���b�v�����N�u��PERFECT���v�̌��ʉ�
	SOUND_LABEL_SE_DESTROY_BLOCK_00,	// �u���b�N�j����P��
	SOUND_LABEL_SE_DESTROY_BLOCK_01,	// �u���b�N�j����Q��
	SOUND_LABEL_SE_DESTROY_BLOCK_02,	// �u���b�N�j����R��
	SOUND_LABEL_SE_DESTROY_BLOCK_03,	// �u���b�N�j����S��
	SOUND_LABEL_SE_DESTROY_BLOCK_04,	// �u���b�N�j����T��
	SOUND_LABEL_SE_DESTROY_BLOCK_05,	// �u���b�N�j����U��
	SOUND_LABEL_SE_DESTROY_BLOCK_06,	// �u���b�N�j����V��
	SOUND_LABEL_SE_DESTROY_BLOCK_07,	// �u���b�N�j����W��
	SOUND_LABEL_SE_PAUSE_DECISION,		// �|�[�Y�̌��艹
	SOUND_LABEL_SE_PAUSE_SELECTION,		// �|�[�Y�̑I����
	SOUND_LABEL_SE_PAUSE_TRANSITION,	// �|�[�Y�̑J�ډ�
	SOUND_LABEL_SE_ROAR,				// �|�[�Y�̙��K��
	SOUND_LABEL_SE_MAX,
} SOUND_LABEL_SE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySoundBGM(SOUND_LABEL_BGM label);
void StopSoundBGM(SOUND_LABEL_BGM label);
void StopSoundBGM(void);
void PauseSound(SOUND_LABEL_BGM label);
void RestartSound(SOUND_LABEL_BGM label);

HRESULT PlaySoundSE(SOUND_LABEL_SE label, int nNumSound);
void StopSoundSE(SOUND_LABEL_SE label);
void StopSoundSE(void);

#endif

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
#define NUM_DUPLICATION	(1)		//�d����

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g����
	SOUND_LABEL_BGM_TUTORIAL,		// �`���[�g���A����
	SOUND_LABEL_BGM_GAME,			// �Q�[����
	SOUND_LABEL_BGM_RESULT,			// ���U���g��
	SOUND_LABEL_SE_CONNECT,			// �R���g���[���[�ڑ����̌��ʉ�
	SOUND_LABEL_SE_TITLE_DECIDE,	// �^�C�g����ʂ̌��艹
	SOUND_LABEL_SE_COLLISION,		// �v���C���[���m�̏Փˉ�
	SOUND_LABEL_SE_ENERGY_00,		// �v���C���[1�̃Q�[�W�[�U��
	SOUND_LABEL_SE_ENERGY_01,		// �v���C���[2�̃Q�[�W�[�U��
	SOUND_LABEL_SE_ENERGY_02,		// �v���C���[3�̃Q�[�W�[�U��
	SOUND_LABEL_SE_ENERGY_03,		// �v���C���[4�̃Q�[�W�[�U��
	SOUND_LABEL_SE_DROP,			// �v���C���[�̗�����
	SOUND_LABEL_SE_GRASSDASH,		// �v���C���[�̃_�b�V����
	SOUND_LABEL_SE_HIPDROP,			// �v���C���[�̃q�b�v�h���b�v��
	SOUND_LABEL_SE_JUMP,			// �v���C���[�̃W�����v��
	SOUND_LABEL_SE_PAUSE_DECISION,	// �|�[�Y�̌��艹
	SOUND_LABEL_SE_PAUSE_SELECTION,	// �|�[�Y�̑I����
	SOUND_LABEL_SE_PAUSE_TRANSITION,// �|�[�Y�̑J�ډ�
	SOUND_LABEL_SE_ROAR,			// �|�[�Y�̙��K��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label/*, int nNumSound*/);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
void RestartSound(SOUND_LABEL label);

#endif

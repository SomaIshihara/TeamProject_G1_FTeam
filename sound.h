//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �Ό��D�n
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_CONNECT = 0,		// �R���g���[���[�ڑ����̌��ʉ�
	SOUND_LABEL_SE_TITLE_DECIDE,	// �^�C�g����ʂ̌��艹
	SOUND_LABEL_SE_COLLISION,		// �v���C���[���m�̏Փˉ�
	SOUND_LABEL_SE_ENERGY_00,		// �v���C���[1�̃Q�[�W�[�U��
	SOUND_LABEL_SE_ENERGY_01,		// �v���C���[2�̃Q�[�W�[�U��
	SOUND_LABEL_SE_ENERGY_02,		// �v���C���[3�̃Q�[�W�[�U��
	SOUND_LABEL_SE_ENERGY_03,		// �v���C���[4�̃Q�[�W�[�U��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
void RestartSound(SOUND_LABEL label);

#endif

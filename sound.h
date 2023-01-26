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

//==========================================================================================
//
//���̓v���O�����̃w�b�_[input.h]
//Author:�Ό��D�n
//
//==========================================================================================
//Ver.2.2.1�i�v���W�F�N�g���쒆�̂��́j
//�g�p�\�ȓ���:�L�[�{�[�h�E�}�E�X�i�J�[�\���ʒu�E�ړ��ʁj�E�Q�[���p�b�h�i1~4��j
//==========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//==========================================================================================
//�}�N����`
//==========================================================================================
#define STICK_MAX			(32767)	//�X�e�B�b�N�ő�l
#define MOUSE_CLICK_LEFT	(0)		//�}�E�X���N���b�N
#define MOUSE_CLICK_RIGHT	(1)		//�}�E�X�E�N���b�N
#define MAX_USE_GAMEPAD		(4)		//�Q�[���p�b�h�g�p�\�䐔

//==========================================================================================
//�v���g�^�C�v�錾
//==========================================================================================
//=========================================================
//�����ɕK�v�Ȃ���
//=========================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

//=========================================================
//�L�[�{�[�h
//=========================================================
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeate(int nKey);

//=========================================================
//�Q�[���p�b�h
//=========================================================
//�{�^��
bool GetGamepadPress(int nPadNum, int nButton);
bool GetGamepadTrigger(int nPadNum, int nButton);
bool GetGamepadRelease(int nPadNum, int nButton);
bool GetGamepadRepeate(int nPadNum, int nButton);

//�X�e�B�b�N
SHORT GetLStickX(int nPadNum);
SHORT GetLStickY(int nPadNum);
SHORT GetRStickX(int nPadNum);
SHORT GetRStickY(int nPadNum);

//�g�p�̗L��
bool GetUseGamepad(int nPadNum);

//=========================================================
//�}�E�X
//=========================================================
//�N���b�N
bool GetMouseClickPress(int nButton);
bool GetMouseClickTrigger(int nButton);
bool GetMouseClickRepeate(int nButton);

//�ʒu
D3DXVECTOR3 GetMousePos(void);

//�ړ���
D3DXVECTOR3 GetMouseMove(void);

#endif // !_INPUT_H_

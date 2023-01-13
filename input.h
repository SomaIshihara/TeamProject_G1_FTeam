/*==========================================================================================================================================================

�L�[�{�[�h�E�R���g���[���[����[input.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//�v���g�^�C�v�錾
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
/*--------------------------------------------
�����ɃR���g���[���[�̓��͏����擾����֐��������܂�
---------------------------------------------*/
XINPUT_STATE *GetXInputState(void);
bool GetGamePadPress(int nKey);
bool GetGamePadTrigger(int nKey);
bool GetGamePadRelease(int nKey);
 
#endif
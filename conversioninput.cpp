//==========================================
//
//�R���g���[���[�̓��͕ϊ��v���O����[conversioninput.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "conversioninput.h"

//�ϊ�����͍\���̒�`
typedef struct
{
	bool press[BUTTON_MAX];
	bool trigger[BUTTON_MAX];
	bool release[BUTTON_MAX];
	bool repeate[BUTTON_MAX];
} ConvButton;

//�O���[�o��
ConvButton g_aConvButton[MAX_USE_GAMEPAD] = {};
CONVSTICK g_aConvStick[MAX_USE_GAMEPAD] = {};

//�L�[�{�[�h�Ή�����
const int c_aKeyboardInput[BUTTON_MAX] = { DIK_SPACE,DIK_RETURN,DIK_P };
const int c_aGamePadInput[BUTTON_MAX] = { XINPUT_GAMEPAD_X,XINPUT_GAMEPAD_A,XINPUT_GAMEPAD_START };

//========================
//����������
//========================
void InitConvertionInput(void)
{
	//���ʂɏ�����
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		g_aConvButton[nCntController] = {};
		g_aConvStick[nCntController] = {};
	}
}

//========================
//�X�V����
//========================
void UpdateConvertionInput(void)
{
	
	
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		if (GetUseGamepad(nCntController) == true)
		{//�Q�[���p�b�h�̓��͂����i���ׂāj
			//�{�^��
			for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
			{
				g_aConvButton[nCntController].press[nCntButton] = GetGamepadPress(nCntController, c_aGamePadInput[nCntButton]);
				g_aConvButton[nCntController].trigger[nCntButton] = GetGamepadTrigger(nCntController, c_aGamePadInput[nCntButton]);
				g_aConvButton[nCntController].release[nCntButton] = GetGamepadRelease(nCntController, c_aGamePadInput[nCntButton]);
				g_aConvButton[nCntController].repeate[nCntButton] = GetGamepadRepeate(nCntController, c_aGamePadInput[nCntButton]);
			}

			//�X�e�B�b�N
			if (GetLStickX(nCntController) < 0)
			{//���ɌX���Ă���
				g_aConvStick[nCntController] = CONVSTICK_LEFT;
			}
			else if (GetLStickX(nCntController) > 0)
			{//�E�ɌX���Ă���
				g_aConvStick[nCntController] = CONVSTICK_RIGHT;
			}
			else
			{//�X���Ă��Ȃ�
				g_aConvStick[nCntController] = CONVSTICK_NEUTRAL;
			}
		}
		else if (nCntController == 0)
		{//�L�[�{�[�h�̓��͂����i1P�̂݁j
			for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
			{
				//�{�^��
				g_aConvButton[0].press[nCntButton] = GetKeyboardPress(c_aKeyboardInput[nCntButton]);
				g_aConvButton[0].trigger[nCntButton] = GetKeyboardTrigger(c_aKeyboardInput[nCntButton]);
				g_aConvButton[0].release[nCntButton] = GetKeyboardRelease(c_aKeyboardInput[nCntButton]);
				g_aConvButton[0].repeate[nCntButton] = GetKeyboardRepeate(c_aKeyboardInput[nCntButton]);
			}

			//�i�Q�[���p�b�h�Ō����j�X�e�B�b�N
			if (GetKeyboardPress(DIK_A) == true)
			{//���ɌX���Ă���
				g_aConvStick[0] = CONVSTICK_LEFT;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//�E�ɌX���Ă���
				g_aConvStick[0] = CONVSTICK_RIGHT;
			}
			else
			{//�X���Ă��Ȃ�
				g_aConvStick[0] = CONVSTICK_NEUTRAL;
			}
		}
	}
}

//========================
//�{�^�����͐ݒ菈��
//========================
void SetButton(int nPadNum, INPUTTYPE type, BUTTON button, bool stat)
{
	switch (type)
	{
	case INPUTTYPE_PRESS:
		g_aConvButton[nPadNum].press[button] = stat;
		break;
	case INPUTTYPE_TRIGGER:
		g_aConvButton[nPadNum].trigger[button] = stat;
		break;
	case INPUTTYPE_RELEASE:
		g_aConvButton[nPadNum].release[button] = stat;
		break;
	case INPUTTYPE_REPEATE:
		g_aConvButton[nPadNum].repeate[button] = stat;
		break;
	}
}

//========================
//�{�^�����͎擾����
//========================
bool GetButton(int nPadNum, INPUTTYPE type, BUTTON button)
{
	switch (type)
	{
	case INPUTTYPE_PRESS:
		return g_aConvButton[nPadNum].press[button];
		break;
	case INPUTTYPE_TRIGGER:
		return g_aConvButton[nPadNum].trigger[button];
		break;
	case INPUTTYPE_RELEASE:
		return g_aConvButton[nPadNum].release[button];
		break;
	case INPUTTYPE_REPEATE:
		return g_aConvButton[nPadNum].repeate[button];
		break;
	}
	return false;
}

//========================
//�X�e�B�b�N���͐ݒ菈��
//========================
void SetStick(int nPadNum, CONVSTICK stick)
{
	g_aConvStick[nPadNum] = stick;
}

//========================
//�X�e�B�b�N���͎擾����
//========================
CONVSTICK GetStick(int nPadNum)
{
	return g_aConvStick[nPadNum];
}
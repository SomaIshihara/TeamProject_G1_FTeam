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
	//�Q�[���p�b�h�̓��͂����i���ׂāj[�D��x�F��]
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
		{
			g_aConvButton[nCntController].press[nCntButton] = GetGamepadPress(nCntButton, c_aGamePadInput[nCntButton]);
			g_aConvButton[nCntController].trigger[nCntButton] = GetGamepadTrigger(nCntButton, c_aGamePadInput[nCntButton]);
			g_aConvButton[nCntController].release[nCntButton] = GetGamepadRelease(nCntButton, c_aGamePadInput[nCntButton]);
			g_aConvButton[nCntController].repeate[nCntButton] = GetGamepadRepeate(nCntButton, c_aGamePadInput[nCntButton]);
		}
	}

	//�L�[�{�[�h�̓��͂����i1P�̂݁j[�D��x�F��]
	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		g_aConvButton[0].press[nCntButton] = GetKeyboardPress(c_aKeyboardInput[nCntButton]);
		g_aConvButton[0].trigger[nCntButton] = GetKeyboardTrigger(c_aKeyboardInput[nCntButton]);
		g_aConvButton[0].release[nCntButton] = GetKeyboardRelease(c_aKeyboardInput[nCntButton]);
		g_aConvButton[0].repeate[nCntButton] = GetKeyboardRepeate(c_aKeyboardInput[nCntButton]);
	}
}

//========================
//���͐ݒ菈��
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
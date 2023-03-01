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

//�X�e�B�b�N�p���͍\����
typedef struct
{
	StickXY press;
	StickXY trigger;
	StickXY release;
} ConvStickState;

//�O���[�o��
ConvButton g_aConvButton[MAX_USE_GAMEPAD] = {};
ConvStickState g_aConvStick[MAX_USE_GAMEPAD] = {};

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
		//�{�^�����Z�b�g
		for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
		{
			g_aConvButton[nCntController].press[nCntButton] = false;
			g_aConvButton[nCntController].trigger[nCntButton] = false;
			g_aConvButton[nCntController].release[nCntButton] = false;
			g_aConvButton[nCntController].repeate[nCntButton] = false;
		}

		if (GetUseGamepad(nCntController) == true)
		{//�Q�[���p�b�h�̓��͂����i���ׂāj
			//�{�^��
			for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
			{
				g_aConvButton[nCntController].press[nCntButton] = GetGamepadPress(nCntController, c_aGamePadInput[nCntButton]);
				g_aConvButton[nCntController].trigger[nCntButton] = GetGamepadTrigger(nCntController, c_aGamePadInput[nCntButton]);
				g_aConvButton[nCntController].release[nCntButton] = GetGamepadRelease(nCntController, c_aGamePadInput[nCntButton]);
			}

			//�X�e�B�b�N
			//X
			if (GetLStickX(nCntController) < 0)
			{//���ɌX���Ă���
				g_aConvStick[nCntController].trigger.x = g_aConvStick[nCntController].press.x != CONVSTICK_LEFT ? CONVSTICK_LEFT : CONVSTICK_NEUTRAL;
				g_aConvStick[nCntController].release.x = g_aConvStick[nCntController].press.x != CONVSTICK_NEUTRAL ? CONVSTICK_NEUTRAL : CONVSTICK_LEFT;
				g_aConvStick[nCntController].press.x = CONVSTICK_LEFT;
			}
			else if (GetLStickX(nCntController) > 0)
			{//�E�ɌX���Ă���
				g_aConvStick[nCntController].trigger.x = g_aConvStick[nCntController].press.x != CONVSTICK_RIGHT ? CONVSTICK_RIGHT : CONVSTICK_NEUTRAL;
				g_aConvStick[nCntController].release.x = g_aConvStick[nCntController].press.x != CONVSTICK_NEUTRAL ? CONVSTICK_NEUTRAL : CONVSTICK_RIGHT;
				g_aConvStick[nCntController].press.x = CONVSTICK_RIGHT;
			}
			else
			{//�X���Ă��Ȃ�
				g_aConvStick[nCntController].trigger.x = 
				g_aConvStick[nCntController].release.x = 
				g_aConvStick[nCntController].press.x = CONVSTICK_NEUTRAL;
			}

			//Y
			if (GetLStickY(nCntController) < 0)
			{//���ɌX���Ă���
				g_aConvStick[nCntController].trigger.y = g_aConvStick[nCntController].press.y != CONVSTICK_DOWN ? CONVSTICK_DOWN : CONVSTICK_NEUTRAL;
				g_aConvStick[nCntController].release.y = g_aConvStick[nCntController].press.y != CONVSTICK_NEUTRAL ? CONVSTICK_NEUTRAL : CONVSTICK_DOWN;
				g_aConvStick[nCntController].press.y = CONVSTICK_DOWN;
			}
			else if (GetLStickY(nCntController) > 0)
			{//��ɌX���Ă���
				g_aConvStick[nCntController].trigger.y = g_aConvStick[nCntController].press.y != CONVSTICK_UP ? CONVSTICK_UP : CONVSTICK_NEUTRAL;
				g_aConvStick[nCntController].release.y = g_aConvStick[nCntController].press.y != CONVSTICK_NEUTRAL ? CONVSTICK_NEUTRAL : CONVSTICK_UP;
				g_aConvStick[nCntController].press.y = CONVSTICK_UP;
			}
			else
			{//�X���Ă��Ȃ�
				g_aConvStick[nCntController].trigger.y =
				g_aConvStick[nCntController].release.y =
				g_aConvStick[nCntController].press.y = CONVSTICK_NEUTRAL;
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
			//X
			if (GetKeyboardPress(DIK_A) == true)
			{//���ɌX���Ă���
				g_aConvStick[0].press.x = CONVSTICK_LEFT;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//�E�ɌX���Ă���
				g_aConvStick[0].press.x = CONVSTICK_RIGHT;
			}
			else
			{//�X���Ă��Ȃ�
				g_aConvStick[0].press.x = CONVSTICK_NEUTRAL;
			}

			//Y
			if (GetKeyboardPress(DIK_W) == true)
			{//���ɌX���Ă���
				g_aConvStick[0].press.y = CONVSTICK_UP;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//�E�ɌX���Ă���
				g_aConvStick[0].press.y = CONVSTICK_DOWN;
			}
			else
			{//�X���Ă��Ȃ�
				g_aConvStick[0].press.y = CONVSTICK_NEUTRAL;
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
//�X�e�B�b�N���͐ݒ菈���iX�̂݁j
//========================
void SetStick(int nPadNum, CONVSTICK stick)
{
	g_aConvStick[nPadNum].press.x = stick;
}

//========================
//�X�e�B�b�N���͎擾����
//========================
StickXY GetStick(int nPadNum, INPUTTYPE type)
{
	switch (type)
	{
	case INPUTTYPE_PRESS:
		return g_aConvStick[nPadNum].press;
		break;
	case INPUTTYPE_TRIGGER:
		return g_aConvStick[nPadNum].trigger;
		break;
	case INPUTTYPE_RELEASE:
		return g_aConvStick[nPadNum].release;
		break;
	}

	return{ CONVSTICK_NEUTRAL,CONVSTICK_NEUTRAL };
}

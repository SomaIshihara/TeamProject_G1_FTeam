//==========================================================================================
//
//���̓v���O����[input.cpp]
//Author:�Ό��D�n
//
//==========================================================================================
//Ver.2.2.1�i�v���W�F�N�g���쒆�̂��́j
//�g�p�\�ȓ���:�L�[�{�[�h�E�}�E�X�i�J�[�\���ʒu�E�ړ��ʁj�E�Q�[���p�b�h�i1~4��j
//==========================================================================================
#include "input.h"

//�}�N����`
#define NUM_KEY_MAX			(256)	//�L�[�̍ő吔
#define MOUSE_BUTTON_MAX	(3)		//�}�E�X�{�^���̍ő吔
#define REPEATE_TIME		(150)	//���s�[�g�̊Ԋu
#define GAMEPAD_BUTTON_NUM	(14)	//�Q�[���p�b�h�̃{�^����
#define STICK_DEADZONE		(655)	//�V��

//�O���[�o���ϐ�
//�L�[�{�[�h��
LPDIRECTINPUT8 g_pInputKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];				//�v���X���
BYTE g_akeyStateTrigger[NUM_KEY_MAX];		//�g���K�[���
BYTE g_akeyStateRelease[NUM_KEY_MAX];		//�����[�X���
BYTE g_akeyStateRepeate[NUM_KEY_MAX];		//���s�[�g���
DWORD g_aKeyboardCurrentTime[NUM_KEY_MAX];			//���݂̎��ԁi���s�[�g�g�p�j
DWORD g_aKeyboardExecLastTime[NUM_KEY_MAX];			//�Ō��true��Ԃ������ԁi���s�[�g�g�p)

//�Q�[���p�b�h�iXInput�g�p�j��
XINPUT_STATE g_XinputState[MAX_USE_GAMEPAD];					//XInput�̏��
WORD g_wButtonTrigger[MAX_USE_GAMEPAD];
WORD g_wButtonRelease[MAX_USE_GAMEPAD];
WORD g_wButtonRepeate[MAX_USE_GAMEPAD];
DWORD g_GamepadCurrentTime[MAX_USE_GAMEPAD];
DWORD g_GamepadExecLastTime[MAX_USE_GAMEPAD];
bool g_bUseGamepad[MAX_USE_GAMEPAD];

//�}�E�X��
LPDIRECTINPUT8 g_pInputMouse = NULL;
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;
DIMOUSESTATE g_MouseState;						//�}�E�X�̏��i�v���X�j
BYTE g_aMouseStateTrigger[MOUSE_BUTTON_MAX];	//�{�^���̃g���K�[���
BYTE g_aMouseStateRelease[MOUSE_BUTTON_MAX];	//�{�^���̃����[�X���
BYTE g_aMouseStateRepeate[MOUSE_BUTTON_MAX];	//���s�[�g���
DWORD g_aMouseCurrentTime[NUM_KEY_MAX];			//���݂̎��ԁi���s�[�g�g�p�j
DWORD g_aMouseExecLastTime[NUM_KEY_MAX];		//�Ō��true��Ԃ������ԁi���s�[�g�g�p)
D3DXVECTOR3 g_posPoint;							//�}�E�X���W
D3DXVECTOR3 g_moveMouse;						//�}�E�X�ړ���

//========================
//���͏���������
//========================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	int nCntInit;		//�J�E���^

	//�L�[�{�[�h��
	//DireceInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pInputKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�擾
	if (FAILED(g_pInputKeyboard->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h�ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();


	//�}�E�X��
	//DireceInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pInputMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�擾
	if (FAILED(g_pInputMouse->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//�������[�h�ݒ�
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�}�E�X�ւ̃A�N�Z�X�����l��
	g_pDevMouse->Acquire();


	//���ԏ�����
	for (nCntInit = 0; nCntInit < NUM_KEY_MAX; nCntInit++)
	{
		g_aKeyboardCurrentTime[nCntInit] = 0;
		g_aKeyboardExecLastTime[nCntInit] = timeGetTime() - REPEATE_TIME;
	}
	for (nCntInit = 0; nCntInit < MOUSE_BUTTON_MAX; nCntInit++)
	{
		g_aMouseCurrentTime[nCntInit] = 0;
		g_aMouseExecLastTime[nCntInit] = timeGetTime() - REPEATE_TIME;
	}

	//�Q�[���p�b�h��
	for (int nCntGPad = 0; nCntGPad < MAX_USE_GAMEPAD; nCntGPad++)
	{
		g_GamepadCurrentTime[nCntGPad] = 0;
		g_GamepadExecLastTime[nCntGPad] = timeGetTime();
		g_bUseGamepad[nCntGPad] = false;
	}

	return S_OK;
}

//========================
//���͏I������
//========================
void UninitKeyboard(void)
{
	//���̓f�o�C�X�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInputKeyboard != NULL)
	{
		g_pInputKeyboard->Release();
		g_pInputKeyboard = NULL;
	}
	if (g_pInputMouse != NULL)
	{
		g_pInputMouse->Release();
		g_pInputMouse = NULL;
	}

	//XInput�I��
	XInputEnable(false);
}

//========================
//���͍X�V����
//========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	DIMOUSESTATE MouseState;
	XINPUT_STATE xInputState;
	POINT point;
	int nCntKey;

	//���̓f�o�C�X����f�o�C�X���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++) 
		{
			//�g���K�[�ƃ����[�X
			g_akeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
 			g_akeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];

			//���s�[�g
			g_aKeyboardCurrentTime[nCntKey] = timeGetTime();
			if (aKeyState[nCntKey] && (g_aKeyboardCurrentTime[nCntKey] - g_aKeyboardExecLastTime[nCntKey]) > REPEATE_TIME)
			{
				g_aKeyboardExecLastTime[nCntKey] = g_aKeyboardCurrentTime[nCntKey];
				g_akeyStateRepeate[nCntKey] = aKeyState[nCntKey];
			}
			else
			{
				g_akeyStateRepeate[nCntKey] = 0;
			}
			//�v���X
			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}


	//�}�E�X�擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)))
	{
		for (nCntKey = 0; nCntKey < MOUSE_BUTTON_MAX; nCntKey++)
		{
			//�g���K�[�ƃ����[�X
			g_aMouseStateTrigger[nCntKey] = (g_MouseState.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & MouseState.rgbButtons[nCntKey];
			g_aMouseStateRelease[nCntKey] = (g_MouseState.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & ~MouseState.rgbButtons[nCntKey];

			//���s�[�g
			g_aMouseCurrentTime[nCntKey] = timeGetTime();
			if (MouseState.rgbButtons[nCntKey] && (g_aMouseCurrentTime[nCntKey] - g_aMouseExecLastTime[nCntKey]) > REPEATE_TIME)
			{
				g_aMouseExecLastTime[nCntKey] = g_aMouseCurrentTime[nCntKey];
				g_aMouseStateRepeate[nCntKey] = MouseState.rgbButtons[nCntKey];
			}
			else
			{
				g_aMouseStateRepeate[nCntKey] = 0;
			}
		}

		//�v���X
		g_MouseState = MouseState;	//�L�[�{�[�h�̃v���X����ۑ�

		//�ړ���
		g_moveMouse = D3DXVECTOR3((float)g_MouseState.lX, (float)g_MouseState.lY, 0.0f);
	}
	else
	{
		g_pDevMouse->Acquire();
	}

	//�}�E�X���W�擾
	GetCursorPos(&point);
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &point);
	g_posPoint.x = (float)point.x;
	g_posPoint.y = (float)point.y;


	//�Q�[���p�b�h������擾
	for (int nCntGPad = 0; nCntGPad < MAX_USE_GAMEPAD; nCntGPad++)
	{
		if (XInputGetState(nCntGPad, &xInputState) == ERROR_SUCCESS)
		{
			//�g���Ă��
			g_bUseGamepad[nCntGPad] = true;

			//�{�^���g���K�[���擾
			g_wButtonTrigger[nCntGPad] = (g_XinputState[nCntGPad].Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & xInputState.Gamepad.wButtons;

			//�{�^�������[�X���擾
			g_wButtonRelease[nCntGPad] = (g_XinputState[nCntGPad].Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & ~xInputState.Gamepad.wButtons;

			//���s�[�g��񐶐�
			g_GamepadCurrentTime[nCntGPad] = timeGetTime();
			for (nCntKey = 0; nCntKey < GAMEPAD_BUTTON_NUM; nCntKey++)
			{
				if (g_XinputState[nCntGPad].Gamepad.wButtons & 1 << nCntKey && (g_GamepadCurrentTime[nCntGPad] - g_GamepadExecLastTime[nCntGPad]) > REPEATE_TIME)
				{
					g_GamepadExecLastTime[nCntGPad] = g_GamepadCurrentTime[nCntGPad];
					g_wButtonRepeate[nCntGPad] += 1 << nCntKey;
				}
			}

			//�v���X��񂻂̑��������ݒ�
			g_XinputState[nCntGPad] = xInputState;
		}
		else
		{
			//�g���ĂȂ���
			g_bUseGamepad[nCntGPad] = false;
		}

		
	}
}

//========================
//�v���X����Ԃ�����
//========================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;	//?����������邱�ƂɂȂ��Ă�݂�����
}

//========================
//�g���K�[����Ԃ�����
//========================
bool GetKeyboardTrigger(int nKey)
{
	return (g_akeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================
//�����[�X����Ԃ�����
//========================
bool GetKeyboardRelease(int nKey)
{
	return (g_akeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================
//���s�[�g����Ԃ�����
//========================
bool GetKeyboardRepeate(int nKey)
{
 	return (g_akeyStateRepeate[nKey] & 0x80) ? true : false;
}



//========================
//�Q�[���p�b�h�̃v���X����Ԃ�����
//=======================
bool GetGamepadPress(int nPadNum, int nButton)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		return g_XinputState[nPadNum].Gamepad.wButtons & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//�Q�[���p�b�h�̃g���K�[����Ԃ�����
//=======================
bool GetGamepadTrigger(int nPadNum, int nButton)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		return g_wButtonTrigger[nPadNum] & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//�Q�[���p�b�h�̃����[�X����Ԃ�����
//=======================
bool GetGamepadRelease(int nPadNum, int nButton)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		return g_wButtonRelease[nPadNum] & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//�Q�[���p�b�h�̃��s�[�g����Ԃ�����
//=======================
bool GetGamepadRepeate(int nPadNum, int nButton)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		return g_wButtonRepeate[nPadNum] & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//���X�e�B�b�N�̉�����Ԃ�����
//=======================
SHORT GetLStickX(int nPadNum)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_XinputState[nPadNum].Gamepad.sThumbLX < -STICK_MAX)
		{
			g_XinputState[nPadNum].Gamepad.sThumbLX = -STICK_MAX;
		}

		if (abs(g_XinputState[nPadNum].Gamepad.sThumbLX) > STICK_DEADZONE)
		{
			return g_XinputState[nPadNum].Gamepad.sThumbLX;
		}
		else
		{
			return 0;
		}
	}
	else
	{//�g���Ă��Ȃ�������0�Ԃ�
		return 0;
	}
}

//========================
//���X�e�B�b�N�̉�����Ԃ�����
//=======================
SHORT GetLStickY(int nPadNum)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_XinputState[nPadNum].Gamepad.sThumbLY < -STICK_MAX)
		{
			g_XinputState[nPadNum].Gamepad.sThumbLY = -STICK_MAX;
		}

		if (abs(g_XinputState[nPadNum].Gamepad.sThumbLY) > STICK_DEADZONE)
		{
			return g_XinputState[nPadNum].Gamepad.sThumbLY;
		}
		else
		{
			return 0;
		}
	}
	else
	{//�g���Ă��Ȃ�������0�Ԃ�
		return 0;
	}
}

//========================
//�E�X�e�B�b�N�̉�����Ԃ�����
//=======================
SHORT GetRStickX(int nPadNum)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_XinputState[nPadNum].Gamepad.sThumbRX < -STICK_MAX)
		{
			g_XinputState[nPadNum].Gamepad.sThumbRX = -STICK_MAX;
		}

		if (abs(g_XinputState[nPadNum].Gamepad.sThumbRX) > STICK_DEADZONE)
		{
			return g_XinputState[nPadNum].Gamepad.sThumbRX;
		}
		else
		{
			return 0;
		}
	}
	else
	{//�g���Ă��Ȃ�������0�Ԃ�
		return 0;
	}
}
//========================
//�E�X�e�B�b�N�̉�����Ԃ�����
//=======================
SHORT GetRStickY(int nPadNum)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_XinputState[nPadNum].Gamepad.sThumbRY < -STICK_MAX)
		{
			g_XinputState[nPadNum].Gamepad.sThumbRY = -STICK_MAX;
		}

		if (abs(g_XinputState[nPadNum].Gamepad.sThumbRY) > STICK_DEADZONE)
		{
			return g_XinputState[nPadNum].Gamepad.sThumbRY;
		}
		else
		{
			return 0;
		}
	}
	else
	{//�g���Ă��Ȃ�������0�Ԃ�
		return 0;
	}
}

//========================
//�Q�[���p�b�h�g�p���Ă��邩�Ԃ�����
//=======================
bool GetUseGamepad(int nPadNum)
{
	if (g_bUseGamepad[nPadNum] == true)
	{
		return g_bUseGamepad[nPadNum];
	}
	else
	{
		return false;
	}
}

//========================
//�}�E�X�̃v���X����Ԃ�����
//=======================
bool GetMouseClickPress(int nButton)
{
	return g_MouseState.rgbButtons[nButton] & 0x80 ? true : false;
}

//========================
//�}�E�X�̃g���K�[����Ԃ�����
//=======================
bool GetMouseClickTrigger(int nButton)
{
	return g_aMouseStateTrigger[nButton] & 0x80 ? true : false;
}

//========================
//�}�E�X�̃��s�[�g����Ԃ�����
//=======================
bool GetMouseClickRepeate(int nButton)
{
	return g_aMouseStateRepeate[nButton] & 0x80 ? true : false;
}

//========================
//�}�E�X�̃J�[�\���ʒu��Ԃ�����
//=======================
D3DXVECTOR3 GetMousePos(void)
{
	return g_posPoint;
}

//========================
//�}�E�X�̃J�[�\���ړ��ʂ�Ԃ�����
//=======================
D3DXVECTOR3 GetMouseMove(void)
{
	return g_moveMouse;
}
//==========================================================================================
//
//���̓v���O����[input.cpp]
//Author:�Ό��D�n
//
//==========================================================================================
//Ver.2.2.2�i�v���W�F�N�g���쒆�̂��́j
//�g�p�\�ȓ���:�L�[�{�[�h�E�}�E�X�i�J�[�\���ʒu�E�ړ��ʁj�E�Q�[���p�b�h�i1~4��j
//==========================================================================================
#include "input.h"
#include "sound.h"
#include "debugproc.h"

//�}�N����`
#define NUM_KEY_MAX			(256)	//�L�[�̍ő吔
#define REPEATE_TIME		(150)	//���s�[�g�̊Ԋu
#define GAMEPAD_BUTTON_NUM	(14)	//�Q�[���p�b�h�̃{�^����
#define STICK_DEADZONE		(655)	//�V��
#define PAD_VIBE_FADE		(200)	//�U���̌�����

//�O���[�o���ϐ�
//�L�[�{�[�h��
LPDIRECTINPUT8 g_pInputKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
Keyboard g_keyboard[NUM_KEY_MAX];	//�L�[�{�[�h�\����

//�Q�[���p�b�h�iXInput�g�p�j��
GamePad g_gamePad[MAX_USE_GAMEPAD];	//�Q�[���p�b�h���


//�}�E�X��
LPDIRECTINPUT8 g_pInputMouse = NULL;
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;
Mouse g_mouse;	//�}�E�X���

//==================================================
//						�L�[�{�[�h
//==================================================
//========================
//�L�[�{�[�h����������
//========================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
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

	//���ԏ�����
	for (int nCntInit = 0; nCntInit < NUM_KEY_MAX; nCntInit++)
	{
		g_keyboard[nCntInit].currentTime = 0;
		g_keyboard[nCntInit].execLastTime = timeGetTime() - REPEATE_TIME;
	}

	return S_OK;
}

//========================
//�L�[�{�[�h�I������
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

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInputKeyboard != NULL)
	{
		g_pInputKeyboard->Release();
		g_pInputKeyboard = NULL;
	}
}

//========================
//�L�[�{�[�h�X�V����
//========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//���̓f�o�C�X����f�o�C�X���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_keyboard[nCntKey].trigger = (g_keyboard[nCntKey].state ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_keyboard[nCntKey].release = (g_keyboard[nCntKey].state ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];

			//���s�[�g
			g_keyboard[nCntKey].currentTime = timeGetTime();
			if (aKeyState[nCntKey] && (g_keyboard[nCntKey].currentTime - g_keyboard[nCntKey].execLastTime) > REPEATE_TIME)
			{
				g_keyboard[nCntKey].execLastTime = g_keyboard[nCntKey].currentTime;
				g_keyboard[nCntKey].repeate = aKeyState[nCntKey];
			}
			else
			{
				g_keyboard[nCntKey].repeate = 0;
			}
			//�v���X
			g_keyboard[nCntKey].state = aKeyState[nCntKey];	//�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//========================
//�L�[�{�[�h�̃v���X����Ԃ�����
//========================
bool GetKeyboardPress(int nKey)
{
	return (g_keyboard[nKey].state & 0x80) ? true : false;	//?����������邱�ƂɂȂ��Ă�݂�����
}

//========================
//�L�[�{�[�h�̃g���K�[����Ԃ�����
//========================
bool GetKeyboardTrigger(int nKey)
{
	return (g_keyboard[nKey].trigger & 0x80) ? true : false;
}

//========================
//�L�[�{�[�h�̃����[�X����Ԃ�����
//========================
bool GetKeyboardRelease(int nKey)
{
	return (g_keyboard[nKey].release & 0x80) ? true : false;
}

//========================
//�L�[�{�[�h�̃��s�[�g����Ԃ�����
//========================
bool GetKeyboardRepeate(int nKey)
{
	return (g_keyboard[nKey].repeate & 0x80) ? true : false;
}


//==================================================
//						�}�E�X
//==================================================
//========================
//�}�E�X����������
//========================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
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
	for (int nCntInit = 0; nCntInit < MOUSE_BUTTON_MAX; nCntInit++)
	{
		g_mouse.mb[nCntInit].currentTime = 0;
		g_mouse.mb[nCntInit].execLastTime = timeGetTime() - REPEATE_TIME;
	}

	return S_OK;
}

//========================
//�}�E�X�I������
//========================
void UninitMouse(void)
{
	//���̓f�o�C�X�̔j��
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInputMouse != NULL)
	{
		g_pInputMouse->Release();
		g_pInputMouse = NULL;
	}
}

//========================
//�}�E�X�X�V����
//========================
void UpdateMouse(void)
{
	DIMOUSESTATE MouseState;
	POINT point;

	//�}�E�X�擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)))
	{
		for (int nCntKey = 0; nCntKey < MOUSE_BUTTON_MAX; nCntKey++)
		{
			//�g���K�[�ƃ����[�X
			g_mouse.mb[nCntKey].trigger = (g_mouse.state.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & MouseState.rgbButtons[nCntKey];
			g_mouse.mb[nCntKey].release = (g_mouse.state.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & ~MouseState.rgbButtons[nCntKey];

			//���s�[�g
			g_mouse.mb[nCntKey].currentTime = timeGetTime();
			if (MouseState.rgbButtons[nCntKey] && (g_mouse.mb[nCntKey].currentTime - g_mouse.mb[nCntKey].execLastTime) > REPEATE_TIME)
			{
				g_mouse.mb[nCntKey].execLastTime = g_mouse.mb[nCntKey].currentTime;
				g_mouse.mb[nCntKey].repeate = MouseState.rgbButtons[nCntKey];
			}
			else
			{
				g_mouse.mb[nCntKey].repeate = 0;
			}
		}

		//�v���X
		g_mouse.state = MouseState;	//�L�[�{�[�h�̃v���X����ۑ�

		//�ړ���
		g_mouse.move = D3DXVECTOR3((float)g_mouse.state.lX, (float)g_mouse.state.lY, 0.0f);
	}
	else
	{
		g_pDevMouse->Acquire();
	}

	//�}�E�X���W�擾
	GetCursorPos(&point);
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &point);
	g_mouse.pos.x = (float)point.x;
	g_mouse.pos.y = (float)point.y;
}

//========================
//�}�E�X�̃v���X����Ԃ�����
//=======================
bool GetMouseClickPress(int nButton)
{
	return g_mouse.state.rgbButtons[nButton] & 0x80 ? true : false;
}

//========================
//�}�E�X�̃g���K�[����Ԃ�����
//=======================
bool GetMouseClickTrigger(int nButton)
{
	return g_mouse.mb[nButton].trigger & 0x80 ? true : false;
}

//========================
//�}�E�X�̃��s�[�g����Ԃ�����
//=======================
bool GetMouseClickRepeate(int nButton)
{
	return g_mouse.mb[nButton].repeate & 0x80 ? true : false;
}

//========================
//�}�E�X�̃J�[�\���ʒu��Ԃ�����
//=======================
D3DXVECTOR3 GetMousePos(void)
{
	return g_mouse.pos;
}

//========================
//�}�E�X�̃J�[�\���ړ��ʂ�Ԃ�����
//=======================
D3DXVECTOR3 GetMouseMove(void)
{
	return g_mouse.move;
}


//==================================================
//						�Q�[���p�b�h
//==================================================
//========================
//�Q�[���p�b�h����������
//========================
void InitGamePad(void)
{
	//�Q�[���p�b�h��
	for (int nCntGPad = 0; nCntGPad < MAX_USE_GAMEPAD; nCntGPad++)
	{
		g_gamePad[nCntGPad].currentTime = 0;
		g_gamePad[nCntGPad].execLastTime = timeGetTime();
		g_gamePad[nCntGPad].bUse = false;
		g_gamePad[nCntGPad].Vibe_State = VIBE_STATE_00_STOP;
		g_gamePad[nCntGPad].wVibePower = 0;
		g_gamePad[nCntGPad].nVibeTime = 0;
	}
}

//========================
//�Q�[���p�b�h�I������
//========================
void UninitGamePad(void)
{
	//�S�R���g���[���[�̐U����~
	StopVibration();

	//XInput�I��
	XInputEnable(false);
}

//========================
//�Q�[���p�b�h�X�V����
//========================
void UpdateGamePad(void)
{
	XINPUT_STATE xInputState;

	//�Q�[���p�b�h������擾
	for (int nCntGPad = 0; nCntGPad < MAX_USE_GAMEPAD; nCntGPad++)
	{
		bool bOldUseGPad = g_gamePad[nCntGPad].bUse;
		if (XInputGetState(nCntGPad, &xInputState) == ERROR_SUCCESS)
		{
			//�g���Ă��
			g_gamePad[nCntGPad].bUse = true;

			//�{�^���g���K�[���擾
			g_gamePad[nCntGPad].trigger = (g_gamePad[nCntGPad].state.Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & xInputState.Gamepad.wButtons;

			//�{�^�������[�X���擾
			g_gamePad[nCntGPad].release = (g_gamePad[nCntGPad].state.Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & ~xInputState.Gamepad.wButtons;

			//���s�[�g��񐶐�
			g_gamePad[nCntGPad].currentTime = timeGetTime();
			for (int nCntKey = 0; nCntKey < GAMEPAD_BUTTON_NUM; nCntKey++)
			{
				if (g_gamePad[nCntGPad].state.Gamepad.wButtons & 1 << nCntKey && (g_gamePad[nCntGPad].currentTime - g_gamePad[nCntGPad].execLastTime) > REPEATE_TIME)
				{
					g_gamePad[nCntGPad].execLastTime = g_gamePad[nCntGPad].currentTime;
					g_gamePad[nCntGPad].repeate += 1 << nCntKey;
				}
			}

			//�v���X��񂻂̑��������ݒ�
			g_gamePad[nCntGPad].state = xInputState;

			//�R���g���[���[�̐U����ԍX�V
			UpdateVibeGamePad(nCntGPad);
		}
		else
		{
			//�g���ĂȂ���
			g_gamePad[nCntGPad].bUse = false;
		}

		//�R���g���[���[�ڑ����ω������特�炷
		if (g_gamePad[nCntGPad].bUse != bOldUseGPad)
		{
			PlaySoundSE(SOUND_LABEL_SE_CONNECT, nCntGPad);
		}
	}
}

//========================
//�Q�[���p�b�h�U����Ԃ̍X�V
//========================
void UpdateVibeGamePad(int nPadNum)
{
	//�U����~��
	if (g_gamePad[nPadNum].nVibeTime <= 0)
	{
		g_gamePad[nPadNum].wVibePower = 0;					// �U����    �O   �ɐݒ�
		g_gamePad[nPadNum].nVibeTime = 0;					//�p������   �O   �ɐݒ�
		g_gamePad[nPadNum].Vibe_State = VIBE_STATE_00_STOP;	//  ���  �u��~�v�ɐݒ�

		StopVibration(nPadNum);	//�Ώۂ̃R���g���[���[�̐U����~
	}

	//�U����
	else
	{
		//�p�����Ԍ���
		g_gamePad[nPadNum].nVibeTime--;

		//�o�C�u�̏�Ԃ��A���X�Ɏ�܂��Ă������̂ł���
		if (g_gamePad[nPadNum].Vibe_State == VIBE_STATE_02_FADE)
		{
			//�U���ʂ��Œ�l�����������
			if (g_gamePad[nPadNum].wVibePower <= VIBE_POWER_MIN)
			{
				//�U�����~�߂�
				g_gamePad[nPadNum].wVibePower = 0;
				g_gamePad[nPadNum].nVibeTime = 0;
				g_gamePad[nPadNum].Vibe_State = VIBE_STATE_00_STOP;

				StopVibration(nPadNum);	//�Ώۂ̃R���g���[���[�̐U����~
			}
			else
			{
				//���X�Ɍ���������
				g_gamePad[nPadNum].wVibePower -= PAD_VIBE_FADE;
			}
		}

		XINPUT_VIBRATION Vibration;

		Vibration.wLeftMotorSpeed = g_gamePad[nPadNum].wVibePower;	//���̃��[�^�[�̉�]���x��ݒ�
		Vibration.wRightMotorSpeed = g_gamePad[nPadNum].wVibePower;	//�E�̃��[�^�[�̉�]���x��ݒ�

		XInputSetState(nPadNum, &Vibration);	//�Ώۂ̃R���g���[���[�ɐU����ݒ�

		PrintDebugProc("�U���ʁF%d    �c�莞�ԁF%d\n", g_gamePad[nPadNum].wVibePower, g_gamePad[nPadNum].nVibeTime / MAX_FPS);
	}
}

//========================
//�S�Q�[���p�b�h�̐U����~
//========================
void StopVibration(void)
{
	XINPUT_VIBRATION Vibration;

	//���E�̃��[�^�[�̉�]���x���O�ɐݒ�
	Vibration.wLeftMotorSpeed = Vibration.wRightMotorSpeed = 0;

	//�S�R���g���[���[�̐U����OFF��
	for (int nCntPad = 0; nCntPad < MAX_USE_GAMEPAD; nCntPad++)
	{
		//�U����OFF
		XInputSetState(nCntPad, &Vibration);
	}
}

//========================
//�Ώۂ̃R���g���[���[�̐U����~
//========================
void StopVibration(int nPadNum)
{
	XINPUT_VIBRATION Vibration = {0,0};

	//�Ώۂ̃R���g���[���[�̐U����OFF
	XInputSetState(nPadNum, &Vibration);
}

//========================
//�Q�[���p�b�h�̃v���X����Ԃ�����
//=======================
bool GetGamepadPress(int nPadNum, int nButton)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].state.Gamepad.wButtons & nButton ? true : false;
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
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].trigger & nButton ? true : false;
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
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].release & nButton ? true : false;
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
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].repeate & nButton ? true : false;
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
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_gamePad[nPadNum].state.Gamepad.sThumbLX < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbLX = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbLX) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbLX;
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
//���X�e�B�b�N�̏c����Ԃ�����
//=======================
SHORT GetLStickY(int nPadNum)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_gamePad[nPadNum].state.Gamepad.sThumbLY < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbLY = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbLY) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbLY;
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
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_gamePad[nPadNum].state.Gamepad.sThumbRX < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbRX = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbRX) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbRX;
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
//�E�X�e�B�b�N�̏c����Ԃ�����
//=======================
SHORT GetRStickY(int nPadNum)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
		if (g_gamePad[nPadNum].state.Gamepad.sThumbRY < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbRY = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbRY) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbRY;
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
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].bUse;
	}
	else
	{
		return false;
	}
}

//========================
//�Q�[���p�b�h�U���ݒ�
//========================
void SetPadVibration(int nPadNum, WORD wPower, int nVibeTime, VIBE_STATE State)
{
	g_gamePad[nPadNum].wVibePower = wPower;		//�����ݒ�
	g_gamePad[nPadNum].nVibeTime = nVibeTime;	//���Ԑݒ�
	g_gamePad[nPadNum].Vibe_State = State;		//��Ԑݒ�

	XINPUT_VIBRATION Vibration;

	Vibration.wLeftMotorSpeed = wPower;		//���̃��[�^�[�̉�]���x��ݒ�
	Vibration.wRightMotorSpeed = wPower;	//�E�̃��[�^�[�̉�]���x��ݒ�

	XInputSetState(nPadNum, &Vibration);	//�Ώۂ̃R���g���[���[�ɐU����ݒ�
}
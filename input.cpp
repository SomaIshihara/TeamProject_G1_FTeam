/*==========================================================================================================================================================

�L�[�{�[�h�E�R���g���[���[����[main.cpp]
Author:���V����

============================================================================================================================================================*/
#include "input.h"

//�}�N����`
#define NUM_KEY_MAX		(256)		//�L�[�̍ő吔

//�Q�[���p�b�h�̍ő�{�^�������}�N����`���܂��傤

//�O���[�o���ϐ��錾
LPDIRECTINPUT8		 g_pInput = NULL;			//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

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
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void* *)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̍쐬
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//--------------------------------------------------------
//				�L�[�{�[�h�̏I������
//--------------------------------------------------------
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();		//�L�[�{�[�h�ւ̃A�N�Z�X����j��
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//--------------------------------------------------------
//				�L�[�{�[�h�̍X�V����
//--------------------------------------------------------
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏��
	/*�@���������ɃR���g���[���[�̓��͏����ꎞ�i�[����ϐ������܂��傤*/

									//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRelease[nCntKey] = (aKeyState[nCntKey] ^ g_aKeyState[nCntKey]) & g_aKeyState[nCntKey];//�L�[�{�[�h�̃����[�X����ۑ�����

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//�L�[�{�[�h�̃g���K�[����ۑ�����

			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//�L�[�{�[�h�̃v���X����ۑ�����
		}
	}

	else
	{
		//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}

	//�Q�[���p�b�h������擾
	/*----------------------------------------------------------------------------------------------------------------

	�����ŉ��Ԗڂ̃R���g���[���[������͂��ꂽ���𔻒肵�܂��@�����炭�����ŕ����̃R���g���[���[�̐ݒ肪�ł��܂�
	
	------------------------------------------------------------------------------------------------------------------*/

	/*----------------------------------------------------------------------------------------------------------------

	�����Ă����ŉ����ꂽ����ۑ����Ă����܂��傤�@�o����Ί֐��������ł����Good Job!

	------------------------------------------------------------------------------------------------------------------*/
	//�Q�[���p�b�h�̃����[�X����ۑ�����
	//�Q�[���p�b�h�̃g���K�[����ۑ�����
	//�Q�[���p�b�h�̃v���X����ۑ�����
}

//--------------------------------------------------------
//			�L�[�{�[�h�̓��͏��ԓ�����
//--------------------------------------------------------
//�L�[�{�[�h�̃v���X�����擾
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;		//�R�����Z�q
}

//�L�[�{�[�h�̃g���K�[�����擾
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�̃����[�X�����擾
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------------
//			�R���g���[���[�̓��͏��ԓ�����
//--------------------------------------------------------
/*----------------------------------------------------------------------

�����ɃR���g���[���[�́u�v���X�v�u�g���K�[�v�u�����[�X�v�̊֐��������܂�

------------------------------------------------------------------------*/
XINPUT_STATE *GetXInputState(void)
{
	return 0;
}

//�Q�[���p�b�h�̃v���X���
bool GetGamePadPress(int nPad)
{
	return 0;
}

//�Q�[���p�b�h�̃g���K�[���
bool GetGamePadTrigger(int nPad)
{
	return 0;
}

//�Q�[���p�b�h�̃����[�X���
bool GetGamePadRelease(int nPad)
{
	return 0;
}
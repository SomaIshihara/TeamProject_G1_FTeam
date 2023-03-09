//==========================================================================================
//
//���̓v���O�����̃w�b�_[input.h]
//Author:�Ό��D�n
//
//==========================================================================================
//Ver.2.2.2�i�v���W�F�N�g���쒆�̂��́j
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
#define MOUSE_BUTTON_MAX	(3)		//�}�E�X�{�^���̍ő吔

#define VIBE_POWER_MIN			(7000)	//�Œ�U����
#define VIBE_POWER_LEVEL_01		(10000)	//��߂̐U��
#define VIBE_POWER_LEVEL_02		(23000)	//�����炢�̐U��
#define VIBE_POWER_LEVEL_03		(37000)	//���߂̐U��
#define VIBE_POWER_LEVEL_04		(50000)	//���Ȃ苭�߂̐U��
#define VIBE_POWER_LEVEL_05		(65535)	//�ő�U��

//==========================================================================================
//�\���̒�`
//==========================================================================================
//�L�[�{�[�h
typedef struct
{
	BYTE state;		//��ԁi�ۑ��p����уv���X���j
	BYTE trigger;	//�g���K�[���
	BYTE release;	//�����[�X���
	BYTE repeate;	//���s�[�g���
	DWORD currentTime;	//���݂̎���
	DWORD execLastTime;	//�Ō��true�ɂ�������
} Keyboard;

//�Q�[���p�b�h�̐U�����
typedef enum
{
	VIBE_STATE_00_STOP,	//��~
	VIBE_STATE_01_LOOP,	//�����Ɠ����U��
	VIBE_STATE_02_FADE,	//���X�ɗ��������Ă���
	VIBE_STATE_MAX
}VIBE_STATE;

//�Q�[���p�b�h
typedef struct
{
	XINPUT_STATE state;		//��ԑS���i����уv���X���j
	WORD trigger;			//�g���K�[���
	WORD release;			//�����[�X���
	WORD repeate;			//���s�[�g���
	DWORD currentTime;		//���݂̎���
	DWORD execLastTime;		//�Ō��true�ɂ�������
	VIBE_STATE Vibe_State;	//�U���̏��
	WORD wVibePower;		//�U���̋���
	int nVibeTime;			//�U�������鎞��
	bool bUse;				//�g�p�̗L��
} GamePad;

//�}�E�X�̃{�^����
typedef struct
{
	BYTE trigger;		//�{�^���̃g���K�[���
	BYTE release;		//�{�^���̃����[�X���
	BYTE repeate;		//���s�[�g���
	DWORD currentTime;	//���݂̎��ԁi���s�[�g�g�p�j
	DWORD execLastTime;	//�Ō��true��Ԃ������ԁi���s�[�g�g�p)
} MouseButton;

//�}�E�X���ׂ�
typedef struct
{
	MouseButton mb[MOUSE_BUTTON_MAX];	//�}�E�X�{�^��
	DIMOUSESTATE state;					//��Ԃ���уv���X���
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 move;					//�ړ���
} Mouse;

//==========================================================================================
//�v���g�^�C�v�錾
//==========================================================================================
//=========================================================
//�����ɕK�v�Ȃ���
//=========================================================
//=============================
//�L�[�{�[�h
//=============================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

//=============================
//�}�E�X
//=============================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);

//=============================
//�Q�[���p�b�h
//=============================
void InitGamePad(void);
void UninitGamePad(void);
void UpdateGamePad(void);

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
//=============================
//�{�^��
//=============================
bool GetGamepadPress(int nPadNum, int nButton);
bool GetGamepadTrigger(int nPadNum, int nButton);
bool GetGamepadRelease(int nPadNum, int nButton);
bool GetGamepadRepeate(int nPadNum, int nButton);

//=============================
//�X�e�B�b�N
//=============================
SHORT GetLStickX(int nPadNum);
SHORT GetLStickY(int nPadNum);
SHORT GetRStickX(int nPadNum);
SHORT GetRStickY(int nPadNum);

//=============================
//�U��
//=============================
void UpdateVibeGamePad(int nPadNum);
void StopVibration(void);
void StopVibration(int nPadNum);
void SetPadVibration(int nPadNum, WORD wPower, int nVibeTime, VIBE_STATE State);

//=============================
//�g�p�̗L��
//=============================
bool GetUseGamepad(int nPadNum);

//=========================================================
//�}�E�X
//=========================================================
//=============================
//�N���b�N
//=============================
bool GetMouseClickPress(int nButton);
bool GetMouseClickTrigger(int nButton);
bool GetMouseClickRepeate(int nButton);

//=============================
//�ʒu
//=============================
D3DXVECTOR3 GetMousePos(void);

//=============================
//�ړ���
//=============================
D3DXVECTOR3 GetMouseMove(void);

#endif // !_INPUT_H_

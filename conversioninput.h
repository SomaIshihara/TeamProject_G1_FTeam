//==========================================
//
//�R���g���[���[�̓��͕ϊ��v���O�����̃w�b�_[conversioninput.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _CONVERSION_INPUT_H_
#define _CONVERSION_INPUT_H_

#include "main.h"
#include "input.h"

//�}�N��
typedef enum
{
	INPUTTYPE_PRESS = 0,
	INPUTTYPE_TRIGGER,
	INPUTTYPE_RELEASE,
	INPUTTYPE_REPEATE,
	INPUTTYPE_MAX
} INPUTTYPE;

//�ϊ�����͗�
typedef enum
{
	BUTTON_X = 0,
	BUTTON_A,
	BUTTON_PAUSE,
	BUTTON_MAX
} BUTTON;

//�X�e�B�b�N����
typedef enum
{
	CONVSTICK_NEUTRAL = 0,
	CONVSTICK_LEFT,
	CONVSTICK_RIGHT,
	CONVSTICK_UP,
	CONVSTICK_DOWN,
	CONVSTICK_MAX
} CONVSTICK;

//�X�e�B�b�NXY�̍\����
typedef struct
{
	CONVSTICK x;
	CONVSTICK y;
} StickXY;

//�v���g�^�C�v�錾
void InitConvertionInput(void);
void UpdateConvertionInput(void);
void SetButton(int nPadNum, INPUTTYPE type, BUTTON button, bool stat);
bool GetButton(int nPadNum, INPUTTYPE type, BUTTON button);
void SetStick(int nPadNum, CONVSTICK stick);
StickXY GetStick(int nPadNum, INPUTTYPE type);

#endif // !_CONVERSION_INPUT_H_

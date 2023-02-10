/*==========================================================================================================================================================

�F�̃}�N����`[color.h]
Author:���V���� �Ό��D�n

============================================================================================================================================================*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

#define COLOR_MIN	(0)		// RGBACOLOR�̍ŏ���
#define COLOR_HALF	(127)	// RGBACOLOR�̒��Ԑ�
#define COLOR_MAX	(255)	// RGBACOLOR�̍ő吔

//D3DXCOLOR_RGBA �̃}�N��
#define RGBA_WHITE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX)	// ��
#define RGBA_BLACK		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// ��
#define RGBA_RED		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// ��
#define RGBA_ORANGE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_HALF,COLOR_MIN, COLOR_MAX)	// �I�����W
#define RGBA_GREEN		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MAX, COLOR_MIN, COLOR_MAX)	// ��
#define RGBA_BLUE		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MAX, COLOR_MAX)	// ��
#define RGBA_RIGHTBLUE	D3DCOLOR_RGBA(COLOR_HALF,COLOR_HALF,COLOR_MAX, COLOR_MAX)	// ���C�g�u���[

//D3DXCOLOR �̃}�N��
#define XCOL_NONE			D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)	// ���F����
#define XCOL_WHITE			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// ���F
#define XCOL_BLACK			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// ��
#define XCOL_BLACKSHADOW	D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.85f)	// ���e
#define XCOL_YELLOW			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)	// ���F
#define XCOL_RED			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)	// �ԐF

//HSB�p�F�\����
typedef struct
{
	int h;	//0�`360
	int s;	//0�`255
	int b;	//0�`255
} HSBCOLOR;

//�v���g
D3DXCOLOR HSBToRGB(HSBCOLOR hsb);

#endif
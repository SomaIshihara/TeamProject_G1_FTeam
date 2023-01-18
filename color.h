/*==========================================================================================================================================================

FΜ}Nθ`[color.h]
Author:½ΰV

============================================================================================================================================================*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

#define COLOR_MIN	(0)		// RGBACOLORΜΕ¬
#define COLOR_MAX	(255)	// RGBACOLORΜΕε

//D3DXCOLOR_RGBA Μ}N
#define RGBA_WHITE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX)	// 
#define RGBA_BLACK		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// 
#define RGBA_RED		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// Τ
#define RGBA_GREEN		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MAX, COLOR_MIN, COLOR_MAX)	// Ξ
#define RGBA_BLUE		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MAX, COLOR_MAX)	// Β

//D3DXCOLOR Μ}N
#define XCOL_NONE			D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)	// ³F§Ύ
#define XCOL_WHITE			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// F
#define XCOL_BLACK			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// 
#define XCOL_BLACKSHADOW	D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.85f)	// e
#define XCOL_YELLOW			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)	// ©F
#define XCOL_RED			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)	// ΤF

#endif
/*==========================================================================================================================================================

JŠÉæéŦÚĖg\Ķ[camera_frame.h]
Author:―āV

============================================================================================================================================================*/
#ifndef _CAMERAFRAME_H_
#define _CAMERAFRAME_H_

#include "camera.h"

//gp·égüĖņ^
typedef enum
{
	UseFrame_WIDTH = 0,	//Ąü
	UseFrame_HEIGHT,	//cü
	UseFrame_MAX,
}UseFrame;

//vg^Cvéū
void InitCameraFrame(void);
void UninitCameraFrame(void);
void DrawCameraFrame(void);
void SetUseFrame(NumCamera type);

#endif
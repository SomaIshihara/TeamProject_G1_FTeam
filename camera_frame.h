/*==========================================================================================================================================================

�J���������ɂ�鋫�ڂ̘g�\������[camera_frame.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _CAMERAFRAME_H_
#define _CAMERAFRAME_H_

#include "camera.h"

//�g�p����g���̗񋓌^
typedef enum
{
	UseFrame_WIDTH = 0,	//����
	UseFrame_HEIGHT,	//�c��
	UseFrame_MAX,
}UseFrame;

//�v���g�^�C�v�錾
void InitCameraFrame(void);
void UninitCameraFrame(void);
void DrawCameraFrame(void);
void SetUseFrame(NumCamera type);

#endif
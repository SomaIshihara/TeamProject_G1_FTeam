/*==========================================================================================================================================================

�J���������ɂ�鋫�ڂ̘g�\������[camera_frame.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _CAMERAFRAME_H_
#define _CAMERAFRAME_H_

#include "camera.h"
#include "HDR_camera.h"

//�g�p����g���̗񋓌^
typedef enum
{
	UseFrame_WIDTH = 0,	//����
	UseFrame_HEIGHT,	//�c��
	UseFrame_HEIGHT4_1,	//�c��4����1�Ԗ�
	UseFrame_HEIGHT4_3,	//�c��4����3�Ԗ�
	UseFrame_MAX,
}UseFrame;

//�v���g�^�C�v�錾
void InitCameraFrame(void);
void UninitCameraFrame(void);
void DrawCameraFrame(void);
void SetUseFrame(NumCamera type);

#endif
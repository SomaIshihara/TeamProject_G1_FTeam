/*==========================================================================================================================================================

�J���������ɂ�鋫�ڂ̘g�\������[camera_frame.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _CAMERAFRAME_H_
#define _CAMERAFRAME_H_

//�g�p����g���̗񋓌^
typedef enum
{
	UseFrame_WIDTH = 0,	//���������g��
	UseFrame_HEIGHT,	//�c�������g��
	UseFrame_MAX,		//�����g��
}UseFrame;

//�v���g�^�C�v�錾
void InitCameraFrame(void);
void UninitCameraFrame(void);
void DrawCameraFrame(void);

#endif
/*==========================================================================================================================================================

カメラ分割による境目の枠表示処理[camera_frame.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _CAMERAFRAME_H_
#define _CAMERAFRAME_H_

//使用する枠線の列挙型
typedef enum
{
	UseFrame_WIDTH = 0,	//横線だけ使う
	UseFrame_HEIGHT,	//縦線だけ使う
	UseFrame_MAX,		//両方使う
}UseFrame;

//プロトタイプ宣言
void InitCameraFrame(void);
void UninitCameraFrame(void);
void DrawCameraFrame(void);

#endif
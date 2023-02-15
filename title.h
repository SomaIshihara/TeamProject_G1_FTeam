/*==========================================================================================================================================================

�^�C�g����ʏ���[title.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _TITLE_H_
#define _TITLE_H_

//�t�F�[�h�̏��
typedef enum
{
	TITLE_NONE = 0,
	TITLE_START,	
	TITLE_EXIT,		
	TITLE_MAX
}TITLE;

void InitTitle(void);
void LoadTitle(void);

void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void DirecUpTitle(int NumPad);
void DirecDownTitle(int NumPad);

#endif
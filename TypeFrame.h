//==========================================
//
//�v���C���[�I���̃^�C�v�g�v���O�����̃w�b�_[TypeFrame.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _TYPEFRAME_H_
#define _TYPEFRAME_H_

#include "main.h"

//�v���C���[��ޗ�
typedef enum
{
	PLAYERTYPE_PLAYER = 0,
	PLAYERTYPE_COM,
	PLAYERTYPE_NONE,
	PLAYERTYPE_MAX
} PLAYERTYPE;

//�v���g�^�C�v�錾
void InitTypeFrame(void);
void UninitTypeFrame(void);
void UpdateTypeFrame(void);
void DrawTypeFrame(void);
bool GetUsePlayer(int nPlayerNum);

#endif // !_TYPEFRAME_H_
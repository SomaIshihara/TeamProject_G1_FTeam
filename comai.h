//==========================================
//
//�R���s���[�^�[��AI�v���O�����̃w�b�_[comai.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _COMAI_H_
#define _COMAI_H_

#include "main.h"
#include "player.h"
#include "ai.h"

//�v���g�^�C�v�錾
void InitComAI(void);
void UninitComAI(void);
void UpdateComAI(void);
void SelectAIMove(Player *pCom);
ComAIBrain *GetAI(void);

#endif // !_COMAI_H_

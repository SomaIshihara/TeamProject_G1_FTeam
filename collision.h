//==========================================
//
//�����蔻��v���O�����̃w�b�_[collision.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"
#include "PvP_player.h"

//�v���g�^�C�v�錾
bool CollisionPP(Player *pATKPlayer, float fWidth, float fHeight, float fDepth);										//�v���C���[���m�̏Փ˔���
bool CollisionHipDropPP(Player *pATKPlayer, float fWidth, float fHeight, float fDepth, float fRadius, float fPower);	//�q�b�v�h���b�v���̏Փ˔���
bool CollisionFence(Player *pPlayer, float fFenceWidth, float fPlayerHeight, float fFenceDepth, float fenceSize);		//�t�F���X�̏Փˏ���

#endif // !_COLLISION_H_

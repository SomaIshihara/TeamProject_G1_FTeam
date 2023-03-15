//==========================================
//
//�������ʂ������Ȃ��I�u�W�F�N�g�v���O�����̃w�b�_[fence.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "model.h"

//�}�N��
#define MAX_OBJECT		(128)	//�I�u�W�F�N�g�z�u�ő吔

//�t�F���X�p�\����
typedef struct
{
	//�ʒu�֌W
	D3DXVECTOR3 pos, posOld;	//�ʒu�E�O��ʒu
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 rot;			//����
	int nFallTime;				//��������

	//�`��֌W
	OBJECTTYPE type;			//�g�p�I�u�W�F�N�g
	D3DXMATRIX mtxWorld;		//���[���h�}�g��

	//�g�p�L��
	bool bUse;					//�g�p�L��

} Object;

//�v���g�^�C�v�錾
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);

void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type);
Object *GetObjectInfo(void);

#endif // !_FENCE_H_
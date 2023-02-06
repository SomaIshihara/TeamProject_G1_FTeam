//==========================================
//
//�A�C�e���v���O�����̃w�b�_[item.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _ITEM_H_
#define _ITEM_H_

//�A�C�e���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 move;			//�ʒu
	D3DXVECTOR3 rot;			//�p�x
	int			RespawnDelay;	//���X�|�[������܂ł̎���
	int			DespawnLimit;	//�����郊�~�b�g
	float		fAlpha;			//�A���t�@�l
	bool        buse;			//�g�p���Ă��邩�ǂ���
}Item;

//�v���g�^�C�v�錾
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(void);
void CollisionIP(int nPlayerNum);//�A�C�e���ƃv���C���[�̓����蔻��

#endif
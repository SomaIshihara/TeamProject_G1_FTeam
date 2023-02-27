//==========================================
//
//�u���b�N�v���O�����̃w�b�_[Block.h]
//Author:�������P
//
//==========================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//�A�C�e����ޗ񋓌^
typedef enum
{
	BLOCKTYPE_ATK = 0,		//�͂�������
	BLOCKTYPE_DEF,			//������������
	BLOCKTYPE_GHOST,		//�S�[�X�g
	BLOCKTYPE_INVINCIBLE,	//���G
	BLOCKTYPE_MAX
}BLOCKTYPE;

//�A�C�e���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	BLOCKTYPE	type;			//���
	float		fWidth;			//
	int			nLife;			//
	bool        buse;			//�g�p���Ă��邩�ǂ���
}Block;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(void);
void CollisionBlock(int nPlayerNum);//�u���b�N�̓����蔻��

#endif
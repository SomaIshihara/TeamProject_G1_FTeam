//==========================================
//
//�u���b�N�v���O�����̃w�b�_[Block.h]
//Author:�������P
//
//==========================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//�v���C���[�̈ʒu�ݒ�̂��߁A�w�b�_�Ɉړ����܂���
#define MAX_BLOCK			(80)		//�ő吔
#define COLLISION_SIZE_Y	(80.0f)		//�����̓����蔻��T�C�Y

//�u���b�N��ޗ񋓌^
typedef enum
{
	BLOCKTYPE_DIRT = 0,
	BLOCKTYPE_HARDDIRT,
	BLOCKTYPE_ROCK,
	BLOCKTYPE_MAX
}BLOCKTYPE;

//�A�C�e���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	BLOCKTYPE	type;			//���
	int			nLife;			//�̗�
	bool		buse;			//�g�p���Ă��邩�ǂ���
}Block;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(void);
void CollisionBlock(Player_HDR *pPlayer);//�u���b�N�̓����蔻��
void DestroyBlockSound(int nDestroyCounter, int nCntBlock);

#endif
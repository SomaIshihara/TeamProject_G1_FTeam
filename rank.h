//==========================================
//
//�u���b�N�v���O�����̃w�b�_[Block.h]
//Author:�������P
//
//==========================================
#ifndef _RANK_H_
#define _RANK_H_

#include "main.h"

//�v���C���[�̈ʒu�ݒ�̂��߁A�w�b�_�Ɉړ����܂���
#define MAX_RANK			(4)		//�ő吔

//�A�C�e����ޗ񋓌^
typedef enum
{
	RANK_1ST = 0,
	RANK_2ND,
	RANK_3RD,
	RANK_4TH,
	RANK_MAX
}RANK;

//�A�C�e���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	RANK        rank;			//���
	bool        buse;			//�g�p���Ă��邩�ǂ���
}Rank;

//�v���g�^�C�v�錾
void InitRank(void);
void UninitRank(void);
void UpdateRank(int nPlayer);
void DrawRank(void);

#endif

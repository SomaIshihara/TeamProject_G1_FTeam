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
	RANKING_1ST = 0,
	RANKING_2ND,
	RANKING_3RD,
	RANKING_4TH,
	RANKING_MAX
}RANKING;

//�A�C�e���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	bool		bDisp;	//�\�����邩�ǂ���
}Rank;

//�v���g�^�C�v�錾
void InitRank(void);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);

void UpperRank(void);
int JoinPlayer(void);
int SetRank(int nCntPlayer);

#endif

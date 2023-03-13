/*==========================================================================================================================================================

�q�b�v�h���b�v�̃����NUI����[HipDropRankUI.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _HIP_DROP_RANK_UI_H_
#define _HIP_DROP_RANK_UI_H_

//�q�b�v�h���b�v���x���ɕK�v�ȍ����i���̒l�ȏ㎟�̃��x���̒l�ȉ��j
#define HIPDROP_HEIGHT_NICE		(5.0f)			//���x��1
#define HIPDROP_HEIGHT_GOOD		(3.7f)			//���x��2
#define HIPDROP_HEIGHT_GREAT	(2.0f)			//���x��3
#define HIPDROP_HEIGHT_PERFECT	(0.5f)			//���x��MAX

//�q�b�v�h���b�v���x���ɉ���������
#define HIPDROP_POWER_NICE		(100)			//���x��1�i100�_���[�W�j
#define HIPDROP_POWER_GOOD		(200)			//���x��2�i200�_���[�W�j
#define HIPDROP_POWER_GREAT		(300)			//���x��3�i300�_���[�W�j
#define HIPDROP_POWER_PERFECT	(500)			//���x��MAX�i500�_���[�W�j

//�q�b�v�h���b�v���x���񋓌^
typedef enum
{
	HIPDROP_RANK_BAD = 0,
	HIPDROP_RANK_NICE,
	HIPDROP_RANK_GOOD,
	HIPDROP_RANK_GREAT,
	HIPDROP_RANK_PERFECT,
	HIPDROP_RANK_MAX
};

//�����NUI�̍\����
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	float		fWidth;	// ��
	float		fHeight;// ����
	int			nLife;	// ����
	bool		bDisp;	// �\�����邩�ۂ�
}HDRankUI;

//�v���g�^�C�v�錾
void InitHipDropRankUI(void);
void InitHipDropRankUIVertex(void);
void UninitHipDropRankUI(void);
void UpdateHipDropRankUI(void);
void DrawHipDropRankUI(void);
void SetRankUI(int nNumUI, int nTexHeight);

#endif
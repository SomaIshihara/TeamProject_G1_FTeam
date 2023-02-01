//==========================================
//
//�G�t�F�N�g�v���O����[effect.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//�G�t�F�N�g��ޗ�
typedef enum
{
	EFFECTTYPE_CHARGE = 0,	//�`���[�W�G�t�F�N�g
	EFFECTTYPE_ATTACK,		//�A�^�b�N�G�t�F�N�g
	EFFECTTYPE_MAX
} EFFECTTYPE;

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int			nType;		//�G�t�F�N�g�̃^�C�v
	int			nCntLoop;	//���[�v�����񐔁i�������j
	float		fSize;		//���a
	float		fAlpha;		//���l
	bool		bUse;		//�g���Ă��邩�ǂ���
	bool		bUseCharge;	//�`���[�W���g���Ă��邩�ǂ���
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);								//����������
void UninitEffect(void);							//�I������
void UpdateEffect(void);							//�X�V����
void UpdateEffectSize(int nEffect);					//�G�t�F�N�g�̃T�C�Y�X�V

void DrawEffect(void);								//�`�揈��

void SetEffectPos(void);							//�v���C���[�̈ʒu�ɐݒ肷��

void SetEffect(D3DXVECTOR3 pos, int nCntEffect, EFFECTTYPE type);	//�G�t�F�N�g��ݒ�

#endif

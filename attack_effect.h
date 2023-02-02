//==========================================
//
//�G�t�F�N�g�v���O����[attack_effect.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _ATTACKEFFECT_H_
#define _ATTACKEFFECT_H_

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int			nType;		//�G�t�F�N�g�̃^�C�v
	int			nCntLoop;	//���[�v�����񐔁i�������j
	float		fSize;		//���a
	float		fAlpha;		//���l
	float		fResize;	//�T�C�Y�ύX�p
	bool		bUse;		//�g���Ă��邩�ǂ���
}AttackEffect;

//�v���g�^�C�v�錾
void InitAttackEffect(void);								//����������
void UninitAttackEffect(void);							//�I������
void UpdateAttackEffect(void);							//�X�V����
void UpdateAttackEffectSize(int nEffect);					//�G�t�F�N�g�̃T�C�Y�X�V

void DrawAttackEffect(void);								//�`�揈��

void SetAttackEffectPos(void);							//�v���C���[�̈ʒu�ɐݒ肷��

void SetAttackEffect(D3DXVECTOR3 pos, int nCntEffect);	//�G�t�F�N�g��ݒ�

#endif


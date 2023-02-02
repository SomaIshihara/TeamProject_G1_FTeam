//==========================================
//
//�G�t�F�N�g�v���O����[charge_effect.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

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
}ChargeEffect;

//�v���g�^�C�v�錾
void InitChargeEffect(void);								//����������
void UninitChargeEffect(void);							//�I������
void UpdateChargeEffect(void);							//�X�V����
void UpdateChargeEffectSize(int nEffect);					//�G�t�F�N�g�̃T�C�Y�X�V

void DrawChargeEffect(void);								//�`�揈��

void SetChargeEffectPos(void);							//�v���C���[�̈ʒu�ɐݒ肷��

void SetChargeEffect(D3DXVECTOR3 pos, int nCntEffect);	//�G�t�F�N�g��ݒ�

#endif

//==========================================
//
//�|�[�Y�v���O����[effect.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//�G�t�F�N�g��ޗ�
typedef enum
{
	EFECTTYPE_CHARGE = 0,	//�`���[�W�G�t�F�N�g
	EFECTTYPE_ATTACK,		//�A�^�b�N�G�t�F�N�g
	EFECTTYPE_MAX
} EFECTTYPE;

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);		//����������
void UninitEffect(void);	//�I������
void UpdateEffect(void);	//�X�V����
void DrawEffect(void);		//�`�揈��

#endif

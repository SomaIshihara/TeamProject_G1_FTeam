//==========================================
//
//�|�[�Y�v���O����[effect.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

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

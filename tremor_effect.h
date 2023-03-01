//==========================================
//
//�G�t�F�N�g�v���O����[tremor_effect.h]
//Author:�Ό��D�n�i�x�[�X�F�ѓc�F��j
//
//==========================================
#ifndef _TREMOR_EFFECT_H_
#define _TREMOR_EFFECT_H_

#include "main.h"

//�}�N��
#define TREMOR_EFFECT_TIME		(30)		//�G�t�F�N�g�������ԁi�t���[���P�ʁj

//�r���{�[�h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int			nType;		//�G�t�F�N�g�̃^�C�v
	int			nCounter;	//�G�t�F�N�g�����J�n����o�߂�������
	float		fSize;		//���a
	float		fAlpha;		//���l
	bool		bUse;		//�g���Ă��邩�ǂ���
}TremorEffect;

//�v���g�^�C�v�錾
void InitTremorEffect(void);							//����������
void UninitTremorEffect(void);							//�I������
void UpdateTremorEffect(void);							//�X�V����
void UpdateTremorEffectSize(int nEffect);				//�G�t�F�N�g�̃T�C�Y�X�V
void DrawTremorEffect(void);							//�`�揈��
void SetTremorEffect(D3DXVECTOR3 pos);	//�G�t�F�N�g��ݒ�

#endif

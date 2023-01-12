//==========================================
//
//���b�V���t�B�[���h�̃w�b�_[meshfield.h]
//Author:�ѓc�F��
//
//==========================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_


//���b�V���t�B�[���h�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;			//�t�B�[���h�̈ʒu
	D3DXVECTOR3 rot;			//�t�B�[���h�̌���
	float fLength_X;			//X���̕��i�����j
	float fLength_Z;			//Y���̕��i�����j
	int nBlock_X;				//X���̕�����
	int nBlock_Z;				//Y���̕�����
}MESHFIELD;

//�v���g�^�C�v�錾
void InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);

#endif 
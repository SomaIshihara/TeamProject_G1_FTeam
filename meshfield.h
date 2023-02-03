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
	float		fRadius;		//���a
}MESHFIELD;

//�v���g�^�C�v�錾
void InitMeshfield(void);
void SetFieldVertex(void);
void SetFieldIndex(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);
void SetMeshField(int nTexNum, MESHFIELD mf);
MESHFIELD *GetMeshField(void);
bool LandMeshfield(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif 
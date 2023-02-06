#ifndef _PARTICLE_H_

#define _PARTICLE_H_

#include"main.h"

#define MAX_PARTICLE			(8192)	//�p�[�e�B�N���̍ő吔
#define NORMAL_PARTICLE			(16)	//�ʏ�p�[�e�B�N���̔�����
#define ACSORPTION_PARTICLE		(16)	//�z���p�[�e�B�N���̔�����

//�p�[�e�B�N���̎��
typedef enum
{
	PARTICLE_NORMAL = 0,
	PARTICLE_ACSORPTION,
	PARTICLE_MAX

}PARTICLE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;	//�ړ���
	int nLife;
	int nLifeTemp;
	float fRadius;
	int nType;
	bool bUse;

}Particle;

void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, float fRadius, int nLife, int nType);
void StateParticle(int nCount,VERTEX_3D *pVtx);
void DirectionParticle(int nCount);
#endif
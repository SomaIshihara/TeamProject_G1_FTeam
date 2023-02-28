#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"

#define MAX_PARTICLE			(512)	//�p�[�e�B�N���̍ő吔
#define NORMAL_PARTICLE			(16)	//�ʏ�p�[�e�B�N���̔�����
#define ACSORPTION_PARTICLE		(4)	//�z���p�[�e�B�N���̔�����

//�p�[�e�B�N���̎��
typedef enum
{
	PARTICLE_NORMAL = 0,
	PARTICLE_ACSORPTION,
	PARTICLE_ACSORPTION_GHOST,
	PARTICLE_MAX

}PARTICLE;

//�p�[�e�B�N���̎��
typedef enum
{
	OBJECT_PLAYER_ATK = 0,
	OBJECT_PLAYER_DEF,
	OBJECT_PLAYER_GHOST,
	OBJECT_PLAYER_INVINCIBLE,
	OBJECT_BONUS,
	OBJECT_MAX,

}PARTICLE_OBJECT;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;	//�ړ���
	int nLife;
	int nLifeTemp;
	float fRadius;
	PARTICLE nType;
	PARTICLE_OBJECT nObject;

	bool bUse;

}Particle;

void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, float fRadius, int nLife, PARTICLE nType, PARTICLE_OBJECT nObject);
void StateParticle(int nCount,VERTEX_3D *pVtx);
void DirectionParticle(int nCount);

#endif
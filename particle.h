#ifndef _PARTICLE_H_

#define _PARTICLE_H_

#include"main.h"

#define MAX_PARTICLE			(8192)	//パーティクルの最大数
#define NORMAL_PARTICLE			(16)	//通常パーティクルの発生数
#define ACSORPTION_PARTICLE		(16)	//吸収パーティクルの発生数

//パーティクルの種類
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
	D3DXVECTOR3 move;	//移動量
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
void StateParticle(int nCount);
void DirectionParticle(int nCount);
#endif
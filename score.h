#ifndef _SCORE_H_

#define _SCORE_H_

#include"main.h"

void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void SetScore(int nScore, int nPlayer);
void AddScore(int nValue, int nPlayer);
#endif

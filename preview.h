/*==========================================================================================================================================================

�v���C���[�̈ʒu�v���r���[����[preview.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _PREVIEW_H_
#define _PREVIEW_H_

//�����NUI�̍\����
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	float		fPlayerUIHeight;	// �v���C���[UI��\�����鍂��
	float		fHeightPer;			// �v���C���[UI��\�����鍂���̃p�[�Z���e�[�W�iSTART���F100���@�@GOAL���F0���j
}Preview;

//�v���g�^�C�v�錾
void InitPreview(void);
void UninitPreview(void);
void UpdatePreview(void);
void DrawPreview(void);

void FixPlayerUIHeight(int nNumPreview);

#endif
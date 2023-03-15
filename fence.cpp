//==========================================
//
//�����h�~�t�F���X�v���O����[fence.cpp]
//Author:�Ό��D�n  ���V����
//
//==========================================
#include "main.h"
#include "fence.h"
#include "model.h"
#include "camera.h"
#include "meshfield.h"

//�}�N��
#define FENCE_MARGIN_SPACE	(10.0f)		//�t�F���X���|���ۂ̌��̗]�T�X�y�[�X
#define FENCE_DESTROY_POS	(-800.0f)	//�t�F���X�������ʒu
#define FENCE_DROP_SPEED	(1.3f)		//�t�F���X�̗��������x
#define FENCE_FALL_SPEED	(0.005f)	//�t�F���X�̓|�������x
#define FENCE_LIMIT_FALL	(3.1f)		//�t�F���X�̓|�����E��]�l
#define FENCE_SHIFT_LENGTH	(1.0f)		//�t�F���X���|��鎞�̉��ɂ��炷�ړ���

//�O���[�o��
Fence g_aFence[MAX_USE_FENCE];

//========================
//����������
//========================
void InitFence(void)
{
	//��{�̔��a�̑傫�����i�[
	float fRadius = FENCE_RADIUS;

	//�ϐ�������
	for (int nCntfence = 0; nCntfence < MAX_USE_FENCE; nCntfence++)
	{
		//��񏉊���
		g_aFence[nCntfence] = {};
		g_aFence[nCntfence].fRadius = fRadius;	//���a�ݒ�

		//�t�F���X��ݒu
		SetFence(fRadius, FIX_ROT(((float)nCntfence / MAX_USE_FENCE) * 2 * D3DX_PI));
	}
}

//========================
//�I������
//========================
void UninitFence(void)
{
	
}

//========================
//�X�V����
//========================
void UpdateFence(void)
{
	//�t�F���X�̃|�C���^���擾
	Fence *pFence = &g_aFence[0];

	//�t�B�[���h�̔��a�擾
	float FieldRadius = GetMeshField()->fRadius;

	for (int nCntFence = 0; nCntFence < MAX_USE_FENCE; nCntFence++, pFence++)
	{
		//�Ώۂ̃t�F���X���g���Ă��Ȃ�������A���̃t�F���X�܂ŏ������΂�
		if (pFence->bUse == false)
		{
			continue;
		}

		//�t�F���X�̓|��鏈��
		FallFence(pFence, FieldRadius);

		//�t�F���X�̗�������
		DropFence(pFence, FieldRadius);
	}
}

//�t�F���X�̗�������
void DropFence(Fence *pFence, float FieldRadius)
{
	//�t�F���X�̈ʒu���A�t�B�[���h�̔��a�ȉ�
	if (FieldRadius <= pFence->fRadius)
	{
		pFence->bCollision = true;				//��̓����蔻����s��Ȃ�
		pFence->fDropSpeed += FENCE_DROP_SPEED;	//�������x���ǂ�ǂ�グ��
		pFence->pos.y -= pFence->fDropSpeed;	//����������

		//���Ń|�C���g�܂ŗ�����
		if (pFence->pos.y <= FENCE_DESTROY_POS)
		{
			//�g�p���Ȃ�
			pFence->bUse = false;
		}
	}
}

//�t�F���X�̓|��鏈��
void FallFence(Fence *pFence, float FieldRadius)
{
	//�t�F���X�̓����蔻�肪�c���Ă�����A�������΂�
	if (pFence->bCollision)
	{
		return;
	}

	pFence->fFallSpeed += FENCE_FALL_SPEED;		//�|��鑬�x���ǂ�ǂ�グ��
	pFence->rot.x += pFence->fFallSpeed;		//��]�l�����Z
	
	//�t�F���X�̒u����Ă���ʒu+�]�T�����������X�y�[�X�@���@�t�B�[���h�̔��a���ł���
	if (pFence->fRadius + FENCE_MARGIN_SPACE <= FieldRadius)
	{
		//�^���ȏ�ɂ܂œ|�ꂽ
		if (D3DX_PI * RIGHT < pFence->rot.x)
		{
			pFence->rot.x = D3DX_PI * RIGHT;	//�^���ɖ߂�
			pFence->fFallSpeed = 0.0f;			//�|���X�s�[�h���O�ɂ���
		}
	}

	//�]�T�����������X�y�[�X���t�B�[���h�̔��a�������������ꍇ�A�ő���܂œ|�ꂳ����
	//�t�F���X���t���܂ɂȂ肩���邭�炢�A�|�ꂽ
	else
	{
		//���������ɂ��炷
		pFence->fRadius += FENCE_SHIFT_LENGTH;
		
		//�ő���܂œ|�ꂽ
		if (FENCE_LIMIT_FALL < pFence->rot.x)
		{
			pFence->bUse = false;	//�t�F���X���g��Ȃ�
		}

		//�ʒu�ݒ�
		SetFencePos(&pFence->pos, pFence->fRadius, pFence->rot.y);
	}
}

//========================
//�`�揈��
//========================
void DrawFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxScall, mtxRot, mtxTrans, mtxTexture;	//�v�Z�p
	D3DXMATRIX mtxInvNor;			//�@���p
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//�t�F���X���f���擾
	Model fenceModel = GetXObject(OBJECTTYPE_FENCE);

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	for (int nCount = 0; nCount < MAX_USE_FENCE; nCount++)
	{
		if (g_aFence[nCount].bUse == true)
		{
			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aFence[nCount].mtxWorld);

			//�g�k�𔽉f
			D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFence[nCount].rot.y, g_aFence[nCount].rot.x, g_aFence[nCount].rot.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxRot);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTrans, g_aFence[nCount].pos.x, g_aFence[nCount].pos.y, g_aFence[nCount].pos.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aFence[nCount].mtxWorld);

			//�e�N�X�`���g�k
			D3DXMatrixScaling(&mtxTexture, FENCE_SCALE, FENCE_SCALE, 1.0f);
			pDevice->SetTransform(D3DTS_TEXTURE0, &mtxTexture);

			//�}�e���A���f�[�^�ւ̃|�C���^�擾
			pMat = (D3DXMATERIAL*)fenceModel.aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)fenceModel.aParts[0].dwNumMatModel; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, fenceModel.aParts[0].apTexture[nCntMat]);

				//���f���`��
				fenceModel.aParts[0].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	
	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�ݒ菈��
//========================
void SetFence(float fRadius, float rotY)
{
	//�Ώۂ̃t�F���X�̃|�C���^���擾
	Fence *pFence = &g_aFence[0];

	for (int nCntObj = 0; nCntObj < MAX_USE_FENCE; nCntObj++, pFence++)
	{
		//�Ώۂ̃t�F���X���g���Ă��Ȃ�
		if (pFence->bUse == false)
		{
			//�����̏���ǉ�			
			SetFencePos(&pFence->pos, fRadius, rotY);		//�ʒu�ݒ�
			pFence->pos.y = 0.0f;							//�����ʒu�ݒ�
			pFence->posOld = pFence->pos;					//�����ʒu�ݒ�
			pFence->fRadius = fRadius;						//���a�ݒ�
			pFence->rot = D3DXVECTOR3(0.0f, rotY, 0.0f);	//�p�x��ݒ�
			pFence->fDropSpeed = pFence->fFallSpeed = 0.0f;	//�����E�|��鑬�x��������

			//�g�p���Ă��邱�Ƃɂ���
			pFence->bUse = true;
			pFence->bCollision = true;
			break;			  //������
		}
	}
}

//�ʒu�ݒ�
void SetFencePos(D3DXVECTOR3 *pPos, float fRadius, float rotY) 
{
	pPos->x = fRadius * sinf(rotY);	//�]�ʒu�ݒ�
	pPos->z = fRadius * cosf(rotY);	//�y�ʒu�ݒ�
}

//========================
//�擾����
//========================
Fence *GetFence(void)
{
	return &g_aFence[0];
}
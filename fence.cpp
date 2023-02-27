//==========================================
//
//�����h�~�t�F���X�v���O����[fence.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "fence.h"
#include "model.h"
#include "camera.h"
#include "meshfield.h"

//�O���[�o��
Fence g_aFence[MAX_USE_FENCE];

//========================
//����������
//========================
void InitFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�ϐ�������
	for (int nCntfence = 0; nCntfence < MAX_USE_FENCE; nCntfence++)
	{
		g_aFence[nCntfence] = {};
		SetFence(GetMeshField()->fRadius, D3DXVECTOR3(0.0f, FIX_ROT(((float)nCntfence / MAX_USE_FENCE) * 2 * D3DX_PI), 0.0f));
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
//�\������
//========================
void SetFence(float fLength, D3DXVECTOR3 rot)
{
	fLength -= 10.0f;
	for (int nCntObj = 0; nCntObj < MAX_USE_FENCE; nCntObj++)
	{
		if (g_aFence[nCntObj].bUse == false)
		{
			//�����̏���ǉ�
			g_aFence[nCntObj].pos.x = fLength * sinf(rot.y);
			g_aFence[nCntObj].pos.y = 0.0f;
			g_aFence[nCntObj].pos.z = fLength * cosf(rot.y);
			g_aFence[nCntObj].posOld = g_aFence[nCntObj].pos;
			g_aFence[nCntObj].rot = rot;
			g_aFence[nCntObj].nFallTime = 0;

			//�g�p���Ă��邱�Ƃɂ���
			g_aFence[nCntObj].bUse = true;

			//������
			break;
		}
	}
}

//========================
//�擾����
//========================
Fence *GetFence(void)
{
	return &g_aFence[0];
}
//==========================================
//
//�����h�~��v���O����[fence.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "fence.h"
#include "model.h"
#include "camera.h"

//�}�N��

//�O���[�o��
Model g_aFenceModel[MAX_USE_FENCE] = {};

//========================
//����������
//========================
void InitFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;	//�}�e���A���|�C���^
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�|�C���^

	//�ϐ�������
	for (int nCntInitBP = 0; nCntInitBP < MAX_USE_FENCE; nCntInitBP++)
	{
		g_aFenceModel[nCntInitBP].aParts[0] = {};
	}
}

//========================
//�I������
//========================
void UninitFence(void)
{
	for (int nCount = 0; nCount < MAX_USE_FENCE; nCount++)
	{//NULL�����̂�
		//���b�V���̔j��
		if (g_aFenceModel[nCount].aParts[0].pMesh != NULL)
		{
			g_aFenceModel[nCount].aParts[0].pMesh->Release();
			g_aFenceModel[nCount].aParts[0].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aFenceModel[nCount].aParts[0].pBuffMat != NULL)
		{
			g_aFenceModel[nCount].aParts[0].pBuffMat->Release();
			g_aFenceModel[nCount].aParts[0].pBuffMat = NULL;
		}
	}
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
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (g_aFence[nCount].bUse == true)
		{
			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aFence[nCount].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFence[nCount].rot.y, g_aFence[nCount].rot.x, g_aFence[nCount].rot.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxRot);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTrans, g_aFence[nCount].pos.x, g_aFence[nCount].pos.y, g_aFence[nCount].pos.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aFence[nCount].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^�擾
			pMat = (D3DXMATERIAL*)g_aBPrint[g_aFence[nCount].bpidx].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aBPrint[g_aFence[nCount].bpidx].dwNumMat; nCntMat++)
			{
				//�}�e���A���ݒ�
				D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;
				//�_���[�W��ԂȂ�Ԓǉ�
				if (g_aFence[nCount].state == OBJSTATE_DAMAGE)
				{
					changeMat.Diffuse.r = 1.0f * OBJ_RED_ALPHA + pMat[nCntMat].MatD3D.Diffuse.r * (1.0f - OBJ_RED_ALPHA);
					changeMat.Diffuse.g = 0.0f * OBJ_RED_ALPHA + pMat[nCntMat].MatD3D.Diffuse.g * (1.0f - OBJ_RED_ALPHA);
					changeMat.Diffuse.b = 0.0f * OBJ_RED_ALPHA + pMat[nCntMat].MatD3D.Diffuse.b * (1.0f - OBJ_RED_ALPHA);
				}
				pDevice->SetMaterial(&changeMat);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, g_aBPrint[g_aFence[nCount].bpidx].apTexture[nCntMat]);

				//���f���`��
				g_aBPrint[g_aFence[nCount].bpidx].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	
	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�\������
//========================
void SetFence(BLUEPRINTIDX bpidx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bLifeUse, int nLife)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (g_aFence[nCntObj].bUse == false)
		{
			//�����̏���ǉ�
			g_aFence[nCntObj].bpidx = bpidx;
			g_aFence[nCntObj].pos = pos;
			g_aFence[nCntObj].rot = rot;
			g_aFence[nCntObj].bLifeUse = bLifeUse;
			g_aFence[nCntObj].nLife = nLife;

			//�̗͂��g�p����ꍇ�̂݉e�ݒ�
			if (g_aFence[nCntObj].bLifeUse == true)
			{
				//�e�ݒ�
				g_aFence[nCntObj].nIdxShadow = SetShadow();
			}

			//�g�p���Ă��邱�Ƃɂ���
			g_aFence[nCntObj].bUse = true;

			//�I�u�W�F�N�g�����₷
			g_nNumObj++;

			//������
			break;
		}
	}
}
//==========================================
//
//�������ʂ������Ȃ��I�u�W�F�N�g�̃v���O����[object.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "object.h"
#include "model.h"
#include "camera.h"
#include "meshfield.h"

//�O���[�o��
Object g_aObject[MAX_OBJECT];

//========================
//����������
//========================
void InitObject(void)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		g_aObject[nCntObj] = {};
	}
}

//========================
//�I������
//========================
void UninitObject(void)
{
	
}

//========================
//�X�V����
//========================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_aObject[nCntObject].bUse == true)
		{
			g_aObject[nCntObject].nFallTime++;
			//���Ƃ�
			g_aObject[nCntObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObject[nCntObject].move.y = -(ACCELERATION_GRAVITY * g_aObject[nCntObject].nFallTime / MAX_FPS);

			//�ړ��オy<0�ł���A���݂̈ʒu���A�t�B�[���h�ȏ�̍����ɂ���Ȃ�ړ��ʏ���
			if (g_aObject[nCntObject].pos.y + g_aObject[nCntObject].move.y < 0.0f && g_aObject[nCntObject].pos.y >= 0.0f)
			{
				//���_�ʒu����̃v���C���[�̋������v�Z
				float fLength = sqrtf(powf((g_aObject[nCntObject].pos.x + g_aObject[nCntObject].move.x), 2) +
					powf((g_aObject[nCntObject].pos.z + g_aObject[nCntObject].move.z), 2));

				//���_�ʒu����̋������A�t�B�[���h�̔��a�ȉ��@�@�Ȃ�A�t�B�[���h�ɏ���Ă���
				if (fLength <= GetMeshField()->fRadius)
				{
					g_aObject[nCntObject].move.y = 0.0f;
					g_aObject[nCntObject].nFallTime = 0;
					g_aObject[nCntObject].pos.y = 0.0f;
				}
			}

			//���
			g_aObject[nCntObject].pos.y += g_aObject[nCntObject].move.y;
		}
	}
}

//========================
//�`�揈��
//========================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;	//�v�Z�p
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (g_aObject[nCount].bUse == true)
		{
			//���f���擾
			Model objectModel = GetXObject(g_aObject[nCount].type);

			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_aObject[nCount].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCount].rot.y, g_aObject[nCount].rot.x, g_aObject[nCount].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCount].mtxWorld, &g_aObject[nCount].mtxWorld, &mtxRot);

			//�ʒu���f
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCount].pos.x, g_aObject[nCount].pos.y, g_aObject[nCount].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCount].mtxWorld, &g_aObject[nCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCount].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^�擾
			pMat = (D3DXMATERIAL*)objectModel.aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)objectModel.aParts[0].dwNumMatModel; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, objectModel.aParts[0].apTexture[nCntMat]);

				//���f���`��
				objectModel.aParts[0].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	
	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//�ݒ菈��
//========================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//�Ώۂ̃t�F���X���g���Ă��Ȃ�
		if (g_aObject[nCntObj].bUse == false)
		{
			//�����̏���ǉ�			
			g_aObject[nCntObj].posOld = g_aObject[nCntObj].pos = pos;			//�����ʒu�ݒ�
			g_aObject[nCntObj].rot = rot;										//�p�x��ݒ�
			g_aObject[nCntObj].type = type;										//�I�u�W�F�N�g��ގw��

			//�g�p���Ă��邱�Ƃɂ���
			g_aObject[nCntObj].bUse = true;

			//������
			break;
		}
	}
}

//========================
//�I�u�W�F�N�g����Ԃ�����
//GetObject�Ƃ����}�N���֐����f�t�H���g�Ŏ��݂������߁A����Info�����܂����B
//========================
Object *GetObjectInfo(void)
{
	return &g_aObject[0];
}
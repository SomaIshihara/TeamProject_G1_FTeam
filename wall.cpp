/*==========================================================================================================================================================

�ǂ̏���[wall.cpp]
Author:��{����

============================================================================================================================================================*/

#include "main.h"
#include "wall.h"
//#include "player.h"
//#include "input.h"

#define MAX_WALL	(4)

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;		//�e�N�X�`���ւ̃|�C���^
D3DXMATRIX				g_mtxWorldWall;				//���[���h�}�g���b�N�X
Wall					g_aWall[MAX_WALL];			//�ǂ̏��

													//�ǂ̏�����
void InitWall(void)
{
	//�f�o�C�X
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall003.jpg",
		&g_pTextureWall);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall, NULL);

	//�ǂ�ǂݍ���
	LoadWall();

	//�|�C���^��ݒ�
	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N���A���_���W�ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, +g_aWall[nCntWall].fDepth);	//����[
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, +g_aWall[nCntWall].fHeight, -g_aWall[nCntWall].fDepth);	//�E��[
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, 0.0f, +g_aWall[nCntWall].fDepth);							//�����[
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, 0.0f, -g_aWall[nCntWall].fDepth);							//�E���[

																														//�@���x�N�g���̐ݒ�
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//����[
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//�E��[
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//�����[
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//�E���[

															//���_�J���[�ݒ�
		pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//����[
		pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//�E��[
		pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//�����[
		pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(255, 255, 255, 255);	//�E���[

															//�e�N�X�`�����W�̐ݒ�
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);				//����[
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);				//�E��[
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);				//�����[
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);				//�E���[
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//�ǂ̏I������
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//�ǂ̍X�V����
void UpdateWall(void)
{

}

//�ǂ̕`�揈��
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

								 //���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldWall);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * VTX_MAX, 2);
		}
	}
}

//�ǂ�ǂݍ���
void LoadWall(void)
{
	int nLetter = 0;	//����sw
	int nColumn = 0;	//��
	int nLine = 0;		//�s
	char aData[100];	//������

	//�t�@�C���|�C���^
	FILE *pFile;

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	//�o�b�t�@�̏�����
	memset(aData, 0, sizeof(aData));

	//�t�@�C�����J��
	pFile = fopen("data/CSV/Wall.csv", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

	 //�t�@�C������ꕶ�����ǂݍ���
		while (fgetc(pFile) != '\n');

		while (nLetter != EOF)
		{//EOF�ȊO�̎�
			while (1)
			{//���̕���������
				nLetter = fgetc(pFile);

				if (nLetter == EOF)
				{//EOF�̎�
					break;		//�����𔲂���
				}

				if (nLetter != '\n'&& nLetter != ',')
				{
					//�J���}or���s�o�Ȃ���Ε������Ȃ�
					strcat(aData, (const char*)&nLetter);
				}
				else
				{//����ȊO�̎�
					break;		//�����𔲂���
				}
			}

			switch (nColumn)
			{
				//���
			case 0:
				g_aWall[nLine].nType = atoi(aData);
				break;

				//�ǂ̌���
			case 1:
				g_aWall[nLine].wallAngle = (WALLANGLE)atoi(aData);

				switch (g_aWall[nLine].wallAngle)
				{
					//������
				case WALLANGLE_LEFT:
					g_aWall[nLine].rot.y = D3DX_PI * 0.5f;
					break;

					//�E����
				case WALLANGLE_RIGHT:
					g_aWall[nLine].rot.y = D3DX_PI * -0.5f;
					break;

					//�����
				case WALLANGLE_BACK:
					g_aWall[nLine].rot.y = D3DX_PI;
					break;

					//�O����
				case WALLANGLE_FRONT:
					g_aWall[nLine].rot.y = 0.0f;
					break;
				}

				//�����𔲂���
				break;

				//��
			case 2:
				g_aWall[nLine].fWidth = atof(aData);
				break;

				//����
			case 3:
				g_aWall[nLine].fHeight = atof(aData);
				break;

				//X���W
			case 4:
				g_aWall[nLine].pos.x = atof(aData);
				break;

				//Y���W
			case 5:
				g_aWall[nLine].pos.y = atof(aData);
				break;

				//Z���W
			case 6:
				g_aWall[nLine].pos.z = atof(aData);
				break;
			}

			//�o�b�t�@�̏�����
			memset(aData, 0, sizeof(aData));

			//�񐔂𑫂�
			nColumn++;

			if (nLetter == '\n')
			{//�ǂݍ��񂾕��������s���������A
			 // �g�p���Ă����Ԃɂ���
				g_aWall[nLine].bUse = true;

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, +g_aWall[nLine].fDepth);	//����[
				pVtx[1].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, -g_aWall[nLine].fDepth);	//�E��[
				pVtx[2].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, 0.0f, +g_aWall[nLine].fDepth);						//�����[
				pVtx[3].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, 0.0f, -g_aWall[nLine].fDepth);						//�E���[

				pVtx += 4;

				//�񐔂����������čs���𑝂₷
				nColumn = 0;
				nLine++;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//�ǂƂ̓����蔻��
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bCollision = false;		//�����������ǂ����̔���
	D3DXVECTOR3 vecLine;			//�I�u�W�F�N�g�̒��_ 0�Ԗڂ���[�܂ł̃x�N�g��(���E���x�N�g��)
	D3DXVECTOR3 vecToPos;			//�I�u�W�F�N�g�̒��_ 0�Ԗڂ��猩���Ώۂ̈ʒu�ւ̊p�x


									//�|�C���^��ݒ�
	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N���A���_���W�ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{//�ǂ��g�p����Ă���
			switch (g_aWall[nCntWall].wallAngle)
			{
				//�������̕ǂɓ���������
			case WALLANGLE_LEFT:
				if (pPosOld->x <= g_aWall[nCntWall].pos.x && pPos->x >= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;
				}
				break;

				//�E�����̕ǂɓ���������
			case WALLANGLE_RIGHT:
				if (pPosOld->x >= g_aWall[nCntWall].pos.x && pPos->x <= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;
				}
				break;

				//�O�����̕ǂɓ���������
			case WALLANGLE_FRONT:
				if (pPosOld->z <= g_aWall[nCntWall].pos.z && pPos->z >= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;
				}
				break;

				//�������̕ǂɓ���������
			case WALLANGLE_BACK:
				if (pPosOld->z >= g_aWall[nCntWall].pos.z && pPos->z <= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;
				}
				break;
			}

		}
	}

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += 4)
	{
#if 0
		vecToPos = *pPos - pVtx[0].pos;										//�Ώۂ̈ʒu����ǂ̌��_�܂ł̃x�N�g���v�Z
		vecLine = pVtx[1].pos - pVtx[0].pos;								//���̒��_���璸�_0�Ԗڂ̃x�N�g���v�Z

																			//�O�όv�Z
		fVecAnswer = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);	//�Ώۂ̈ʒu�ւ̃x�N�g���v�Z

																			//���݂̌�����[��](��) �ł���
		if (fVecAnswer <= 0.0f)
		{
			bCollision = true;		//��������
			pPos = pPosOld;
			pMove->x = 0.0f;		//�ړ��ʂ��[���ɂ���
			pMove->z = 0.0f;		//�ړ��ʂ��[���ɂ���
			break;					//for���𔲂���
		}
#endif
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();

	return bCollision;	//�����Ԃ�
}
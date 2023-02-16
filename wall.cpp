/*==========================================================================================================================================================

�ǂ̏���[wall.cpp]
Author:��{����

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "wall.h"
#include "player.h"
#include "input.h"

//�}�N����`
#define MAX_WALL		(4)		//�ǂ̍ő吔

//�@�����
#define NOR_WIDTH		(0.0f)	//�@���̕�
#define NOR_HEGHT		(1.0f)	//�@���̍���
#define	NOR_DEPTH		(0.0f)	//�@���̉��s��

//�e�N�X�`�����
#define MAX_TEX			(1.0f)	//�e�N�X�`���̍ő�l
#define MIN_TEX			(0.0f)	//�e�N�X�`���̍ŏ��l

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
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, 0.0f, +g_aWall[nCntWall].fDepth);						//�����[
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, 0.0f, -g_aWall[nCntWall].fDepth);						//�E���[

		//�@���x�N�g���̐ݒ�
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//����[
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//�E��[
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//�����[
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//�E���[

		//���_�J���[�ݒ�
		pVtx[VTX_LE_UP].col = RGBA_WHITE;	//����[
		pVtx[VTX_RI_UP].col = RGBA_WHITE;	//�E��[
		pVtx[VTX_LE_DO].col = RGBA_WHITE;	//�����[
		pVtx[VTX_RI_DO].col = RGBA_WHITE;	//�E���[

		//�e�N�X�`�����W�̐ݒ�
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(MIN_TEX, MIN_TEX);				//����[
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_TEX, MIN_TEX);				//�E��[
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(MIN_TEX, MAX_TEX);				//�����[
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_TEX, MAX_TEX);				//�E���[
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

	//���ʃJ�����O��ON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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

	//�ʏ�J�����O�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//�ǂ�ǂݍ���
void LoadWall(void)
{
	int nLetter = 0;		//����
	int nColumn = 0;		//��
	int nLine = 0;			//�s
	char aData[MAX_MOJI];	//������

	//�t�@�C���|�C���^
	FILE *pFile;

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

			//�ǂݍ��񂾕������ϐ��ɑ��
			switch (nColumn)
			{
				//���
			case WALLCASE_TYPE:
				g_aWall[nLine].nType = atoi(aData);
				break;

				//�ǂ̌���
			case WALLCASE_WALLANGLE:
				g_aWall[nLine].wallAngle = (WALLANGLE)atoi(aData);
				//�����𔲂���
				break;

				//��
			case WALLCASE_WIDTH:
				g_aWall[nLine].fWidth = atof(aData);
				break;

				//����
			case WALLCASE_HEIGHT:
				g_aWall[nLine].fHeight = atof(aData);
				break;

				//���s��
			case WALLCASE_DEPTH:
				g_aWall[nLine].fDepth = atof(aData);
				break;

				//X���W
			case WALLCASE_POSX:
				g_aWall[nLine].pos.x = atof(aData);
				break;

				//Y���W
			case WALLCASE_POSY:
				g_aWall[nLine].pos.y = atof(aData);
				break;

				//Z���W
			case WALLCASE_POSZ:
				g_aWall[nLine].pos.z = atof(aData);
				break;
			}

			//�o�b�t�@�̏�����
			memset(aData, 0, sizeof(aData));

			//�񐔂𑫂�
			nColumn++;

			if (nLetter == '\n')
			{//�ǂݍ��񂾕��������s���������A
			 
				g_aWall[nLine].rot = ZERO_SET;	// ����������
				g_aWall[nLine].bUse = true;		// �g�p���Ă����Ԃɂ���

				nColumn = 0;					// �񐔂�������
				nLine++;						// �s���𑝂₷
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();

	return bCollision;	//�����Ԃ�
}

//================================================
//�ǂ̐ݒ菈���i���f���r���[���[�t�@�C������ݒ�j
//Author:�Ό��D�n
//Memo:���d�l���c���Ă��邪�f�[�^�̍\�����Ⴄ���ߕʊ֐��p�Ӂj
//================================================

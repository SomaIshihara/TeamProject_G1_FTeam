/*==========================================================================================================================================================

�ǂ̏���[wall.cpp]
Author:��{����

============================================================================================================================================================*/

#include "main.h"
#include "wall.h"
#include "player.h"
#include "input.h"

//�}�N����`
#define MAX_WALL		(4)		//�ǂ̍ő吔
#define MAX_COLOR		(255)	//�F�̍ő�l
#define MAX_STRING		(100)	//������̍ő吔
#define DISPLAY_ADD		(4)		//�\�����镪�������Z

//�ʒu���
#define MIN_POS			(0.0f)	//�ʒu�̍ŏ��l

//�@�����
#define NOR_WIDTH		(0.0f)	//�@���̕�
#define NOR_HEGHT		(1.0f)	//�@���̍���
#define	NOR_DEPTH		(0.0f)	//�@���̉��s��

//�e�N�X�`�����
#define MAX_TEX			(1.0f)	//�e�N�X�`���̍ő�l
#define MIN_TEX			(0.0f)	//�e�N�X�`���̍ŏ��l

//CSV�֘A
#define START_LE_NUM	(0)		//�����̊J�n�ԍ�
#define START_CO_NUM	(0)		//��̊J�n�ԍ�
#define START_LI_NUM	(0)		//�s�̊J�n�ԍ�

//�ǂ̌���
#define ANGLE_R			(-0.5f)	//�E����
#define ANGLE_L			(0.5f)	//������
#define ANGLE_F			(0.0f)	//���ʌ���

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
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, MIN_POS, +g_aWall[nCntWall].fDepth);						//�����[
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aWall[nCntWall].fWidth, MIN_POS, -g_aWall[nCntWall].fDepth);						//�E���[

		//�@���x�N�g���̐ݒ�
		pVtx[VTX_LE_UP].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//����[
		pVtx[VTX_RI_UP].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//�E��[
		pVtx[VTX_LE_DO].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//�����[
		pVtx[VTX_RI_DO].nor = D3DXVECTOR3(NOR_WIDTH, NOR_HEGHT, NOR_DEPTH);		//�E���[

		//���_�J���[�ݒ�
		pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//����[
		pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//�E��[
		pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//�����[
		pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(MAX_COLOR, MAX_COLOR, MAX_COLOR, MAX_COLOR);	//�E���[

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
	int nLetter = START_LE_NUM;	//����
	int nColumn = START_CO_NUM;	//��
	int nLine = START_LI_NUM;	//�s
	char aData[MAX_STRING];		//������

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
			case WALLCASE_TYPE:
				g_aWall[nLine].nType = atoi(aData);
				break;

				//�ǂ̌���
			case WALLCASE_WALLANGLE:
				g_aWall[nLine].wallAngle = (WALLANGLE)atoi(aData);

				switch (g_aWall[nLine].wallAngle)
				{
					//������
				case WALLANGLE_LEFT:
					g_aWall[nLine].rot.y = D3DX_PI * ANGLE_L;
					break;

					//�E����
				case WALLANGLE_RIGHT:
					g_aWall[nLine].rot.y = D3DX_PI * ANGLE_R;
					break;

					//�����
				case WALLANGLE_BACK:
					g_aWall[nLine].rot.y = D3DX_PI;
					break;

					//�O����
				case WALLANGLE_FRONT:
					g_aWall[nLine].rot.y = ANGLE_F;
					break;
				}

				//�����𔲂���
				break;

				//��
			case WALLCASE_WIDTH:
				g_aWall[nLine].fWidth = (float)atof(aData);
				break;

				//����
			case WALLCASE_HEIGHT:
				g_aWall[nLine].fHeight = (float)atof(aData);
				break;

				//X���W
			case WALLCASE_POSX:
				g_aWall[nLine].pos.x = (float)atof(aData);
				break;

				//Y���W
			case WALLCASE_POSY:
				g_aWall[nLine].pos.y = (float)atof(aData);
				break;

				//Z���W
			case WALLCASE_POSZ:
				g_aWall[nLine].pos.z = (float)atof(aData);
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
				pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, +g_aWall[nLine].fDepth);	//����[
				pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, +g_aWall[nLine].fHeight, -g_aWall[nLine].fDepth);	//�E��[
				pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aWall[nLine].fWidth, MIN_POS, +g_aWall[nLine].fDepth);					//�����[
				pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aWall[nLine].fWidth, MIN_POS, -g_aWall[nLine].fDepth);					//�E���[

				pVtx += DISPLAY_ADD;

				//�񐔂����������čs���𑝂₷
				nColumn = START_CO_NUM;
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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();

	return bCollision;	//�����Ԃ�
}

//================================================
//�ǂ̐ݒ菈���i���f���r���[���[�t�@�C������ݒ�j
//Author:�Ό��D�n
//Memo:���d�l���c���Ă��邪�f�[�^�̍\�����Ⴄ���ߕʊ֐��p�Ӂj
//================================================

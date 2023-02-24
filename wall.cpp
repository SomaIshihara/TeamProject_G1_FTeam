/*==========================================================================================================================================================

�ǂ̏���[wall.cpp]
Author:��{����  ���V����

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "wall.h"
#include "PvP_player.h"
#include "input.h"

//�}�N����`
#define MAX_WALL		(4)		//�ǂ̍ő吔
#define WALL_NOR		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	//�ǂ̖@���x�N�g��

//���ꋎ��Ƃ��̃}�N����`
#define COLLAPSE_FALL			(0.1f)	//���ꗎ����X�s�[�h
#define COLLAPSE_CLEAR_SPEED	(0.02f)	//�����l�̌��Z�x
#define COLLAPSE_VIBE_WIDTH		(5.0f)	//�h�ꕝ

//�e�N�X�`�����
#define MAX_TEX			(1.0f)	//�e�N�X�`���̍ő�l
#define MIN_TEX			(0.0f)	//�e�N�X�`���̍ŏ��l

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;		//�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;		//�e�N�X�`���ւ̃|�C���^
D3DXMATRIX				g_mtxWorldWall;				//���[���h�}�g���b�N�X
Wall					g_aWall[MAX_WALL];			//�ǂ̏��

//==============================================================
//�ǂ̏���������
//===============================================================
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

	//���_����ݒ肷��
	SetWallVertex();
}

//�ǂ�ǂݍ���
void LoadWall(void)
{
	int nLetter = 0;		//����
	int nColumn = 0;		//��
	int nLine = 0;			//�s
	char aData[MAX_MOJI];	//������
	FILE *pFile;			//�t�@�C���|�C���^

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
				g_aWall[nLine].fWidth = fatof(aData);
				break;

				//����
			case WALLCASE_HEIGHT:
				g_aWall[nLine].fHeight = fatof(aData);
				break;

				//���s��
			case WALLCASE_DEPTH:
				g_aWall[nLine].fDepth = fatof(aData);
				break;

				//X���W
			case WALLCASE_POSX:
				g_aWall[nLine].pos.x = fatof(aData);
				break;

				//Y���W
			case WALLCASE_POSY:
				g_aWall[nLine].pos.y = fatof(aData);
				break;

				//Z���W
			case WALLCASE_POSZ:
				g_aWall[nLine].pos.z = fatof(aData);
				break;
			}

			//�o�b�t�@�̏�����
			memset(aData, 0, sizeof(aData));

			//�񐔂𑫂�
			nColumn++;

			if (nLetter == '\n')
			{//�ǂݍ��񂾕��������s���������A

				g_aWall[nLine].rot = ZERO_SET;		// ����������
				g_aWall[nLine].fAlpha = 1.0f;		// �����x������
				g_aWall[nLine].bSideVibe = false;	// �k��������
				g_aWall[nLine].fSideVibe = 0.0f;	// �k���̕�������
				g_aWall[nLine].bCollision = false;	// �����蔻�菉����
				g_aWall[nLine].bDisp = true;		// �`�悷��

				nColumn = 0;					// �񐔂�������
				nLine++;						// �s���𑝂₷
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//���_���̐ݒ菈��
void SetWallVertex(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N���A���_���W�ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		//���݂̕ǂ̃|�C���^�[��ݒ�
		Wall *pWall = &g_aWall[nCntWall];

		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-pWall->fWidth + pWall->fSideVibe, +pWall->fHeight, +pWall->fDepth + pWall->fSideVibe);	//����[
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+pWall->fWidth + pWall->fSideVibe, +pWall->fHeight, -pWall->fDepth + pWall->fSideVibe);	//�E��[
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-pWall->fWidth + pWall->fSideVibe, 0.0f, +pWall->fDepth + pWall->fSideVibe);				//�����[
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+pWall->fWidth + pWall->fSideVibe, 0.0f, -pWall->fDepth + pWall->fSideVibe);				//�E���[

		//�@���x�N�g���̐ݒ�
		pVtx[VTX_LE_UP].nor = WALL_NOR;		//����[
		pVtx[VTX_RI_UP].nor = WALL_NOR;		//�E��[
		pVtx[VTX_LE_DO].nor = WALL_NOR;		//�����[
		pVtx[VTX_RI_DO].nor = WALL_NOR;		//�E���[

											//���_�J���[�ݒ�
		pVtx[VTX_LE_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//����[
		pVtx[VTX_RI_UP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//�E��[
		pVtx[VTX_LE_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//�����[
		pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pWall->fAlpha);	//�E���[

											//�e�N�X�`�����W�̐ݒ�
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(MIN_TEX, MIN_TEX);	//����[
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_TEX, MIN_TEX);	//�E��[
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(MIN_TEX, MAX_TEX);	//�����[
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_TEX, MAX_TEX);	//�E���[
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//==============================================================
//�ǂ̏I������
//===============================================================
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

//==============================================================
//�ǂ̍X�V����
//===============================================================
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//���ꋎ�鏈��
		CollapseWall(nCntWall);

		if (GetKeyboardTrigger(DIK_TAB))
		{
			g_aWall[nCntWall].bCollision = true;
		}
	}

	//���_����ݒ肷��
	SetWallVertex();
}

//��x���������ǂ����ꋎ�鏈��
void CollapseWall(int nCntWall)
{
	//�ǂ̃|�C���^�\��ݒ�
	Wall *pWall = &g_aWall[nCntWall];

	//��x���Ă��Ă��āA�܂������ɂȂ��Ă��Ȃ�
	if (pWall->bCollision == true && pWall->bDisp == true && pWall->fAlpha > 0.0f)
	{
		//�E�ɐk���Ă���
		if (pWall->bSideVibe)
		{
			pWall->bSideVibe = false;					//���ɐk��������
			pWall->fSideVibe = -COLLAPSE_VIBE_WIDTH;	//���ɐk��������
		}

		//���ɐk���Ă���
		else
		{
			pWall->bSideVibe = true;					//�E�ɐk��������
			pWall->fSideVibe = +COLLAPSE_VIBE_WIDTH;	//�E�ɐk��������
		}

		//�ʒu��������
		pWall->pos.y -= COLLAPSE_FALL;

		//�����x�����Z
		pWall->fAlpha -= COLLAPSE_CLEAR_SPEED;

		//�����ȉ��ɂȂ���
		if (pWall->fAlpha <= 0.0f)
		{
			pWall->fAlpha = 0.0f;	//�����ɂ���
			pWall->bDisp = false;	//�`�悵�Ȃ�
			pWall->fSideVibe = 0.0f;//�k�����~�߂�
		}
	}
}

//==============================================================
//�ǂ̕`�揈��
//===============================================================
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
		if (g_aWall[nCntWall].bDisp == true)
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

//�ǂƂ̓����蔻��
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	bool bCollision = false;		//�����������ǂ����̔���
	D3DXVECTOR3 vecLine;			//�I�u�W�F�N�g�̒��_ 0�Ԗڂ���[�܂ł̃x�N�g��(���E���x�N�g��)
	D3DXVECTOR3 vecToPos;			//�I�u�W�F�N�g�̒��_ 0�Ԗڂ��猩���Ώۂ̈ʒu�ւ̊p�x

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���W�ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//�܂��@���Ă��Ă��Ȃ��āA�`�������Ă���
		if (g_aWall[nCntWall].bCollision == false && g_aWall[nCntWall].bDisp == true)
		{//�ǂ��g�p����Ă���
			switch (g_aWall[nCntWall].wallAngle)
			{
				//�������̕ǂɓ���������
			case WALLANGLE_LEFT:
				if (pPosOld->x <= g_aWall[nCntWall].pos.x && pPos->x >= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;		//X�̈ʒu�ɖ߂�
					g_aWall[nCntWall].
						bCollision =		//�ǂ�
						bCollision = true;	//��������
				}
				break;

				//�E�����̕ǂɓ���������
			case WALLANGLE_RIGHT:
				if (pPosOld->x >= g_aWall[nCntWall].pos.x && pPos->x <= g_aWall[nCntWall].pos.x)
				{
					pPos->x = g_aWall[nCntWall].pos.x;		//X�̈ʒu�ɖ߂�
					g_aWall[nCntWall].
						bCollision =		//�ǂ�
						bCollision = true;	//��������
				}
				break;

				//�O�����̕ǂɓ���������
			case WALLANGLE_FRONT:
				if (pPosOld->z <= g_aWall[nCntWall].pos.z && pPos->z >= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;		//Z�̈ʒu�ɖ߂�
					g_aWall[nCntWall].
						bCollision =		//�ǂ�
						bCollision = true;	//��������
				}
				break;

				//�������̕ǂɓ���������
			case WALLANGLE_BACK:
				if (pPosOld->z >= g_aWall[nCntWall].pos.z && pPos->z <= g_aWall[nCntWall].pos.z)
				{
					pPos->z = g_aWall[nCntWall].pos.z;		//Z�̈ʒu�ɖ߂�
					g_aWall[nCntWall].
						bCollision =		//�ǂ�
						bCollision = true;	//��������
				}
				break;
			}
			
			//���������珈���𔲂���
			if (bCollision == true)
			{
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
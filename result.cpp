/*----------------------------------------------------------------------------------------------------------------------------------------------------------

���U���g��ʏ���[result.cpp]
Author:���V����

------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "fade.h"
#include "file.h"
#include "model.h"
#include "resultPlayer.h"
#include "resultCamera.h"
#include "VictoryStand.h"
#include "resultCylinder.h"
#include "meshfield.h"
#include "conversioninput.h"

#define NUM_RESULT			(1)		//���U���g�摜�̎��

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;			//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9			g_pTextureResult[NUM_RESULT] = {};	//�e�N�X�`���̃|�C���^
bool						g_bStop = false;					//���U���g�ꎞ��~

//************************************************
//				���U���g�̏���������
//************************************************
void InitResult(void)
{
	//----------------------------------------------=
	//���U���g�p�I�u�W�F�N�g�̏����������֐��Ăяo��
	//----------------------------------------------=
	InitResultObject();

	g_bStop = false;
}

//------------------------------------------------
//		���U���g�p�I�u�W�F�N�g�̏���������
//------------------------------------------------
void InitResultObject(void)
{
	InitFile();				//�t�@�C��������
	LoadModelViewerFile("data\\model.txt");	// ���f���r���[���[�t�@�C���ǂݍ��݁i�e�I�u�W�F�N�g�������O�ɍs�����ƁI�j
	InitAnimalModel();		//�������f��������
	InitPlayer_RESULT();	//�v���C���[�̏���������
	InitMeshfield();		//�n�ʂ̏���������
	InitResultCylinder();	//�w�i�̏���������
	InitVictoryStand();		//�\����̏���������
	InitResultCamera();		//�J�����̏���������
}

//************************************************
//				���U���g�̏I������
//************************************************
void UninitResult(void)
{
	//�e�N�X�`��(�Q��)�̔j��
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//----------------------------------------------=
	//���U���g�p�I�u�W�F�N�g�̏I�������֐��Ăяo��
	//----------------------------------------------=
	UninitResultObject();
}

//------------------------------------------------
//		���U���g�p�I�u�W�F�N�g�̏I������
//------------------------------------------------
void UninitResultObject(void)
{
	UninitMeshfield();			//�n�ʂ̏I������
	UninitResultCylinder();		//�w�i�̏I������
	UninitModel();				//�������f���I������
	UninitPlayer_RESULT();		//�v���C���[�̏I������
	UninitResultCamera();		//�J�����̏I������
	UninitVictoryStand();		//�\����̏I������
}

//************************************************
//				���U���g�̍X�V����
//************************************************
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_P))
	{
		g_bStop ^= 1;
	}

	//----------------------------------------------=
	//���U���g�p�I�u�W�F�N�g�̍X�V�����֐��Ăяo��
	//----------------------------------------------=
	UpdateResultObject();

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TITLE);
	}
}

//------------------------------------------------
//		���U���g�p�I�u�W�F�N�g�̍X�V����
//------------------------------------------------
void UpdateResultObject(void)
{
	//�ꎞ��~�ł͂Ȃ��@�������́@�ꎞ��~���ł��E��󂪉����ꂽ
	if (!g_bStop || (g_bStop && GetKeyboardTrigger(DIK_RIGHT)))
	{
		UpdatePlayer_RESULT();		//�v���C���[�̍X�V����
		UpdateResultCamera();		//�J�����̍X�V����
		UpdateVictoryStand();		//�\����̍X�V����
		UpdateResultCylinder();		//�w�i�̍X�V����
	}
}

//************************************************
//				���U���g�̕`�揈��
//MEMO:2DUI��`�悷��ꍇ�̓��e�X�g��Y�ꂸ�ɁI
//************************************************
void DrawResult(void)
{
	//----------------------------------------------=
	//���U���g�p�I�u�W�F�N�g�̕`�揈���֐��Ăяo��
	//----------------------------------------------=
	DrawResultObject();
}

//------------------------------------------------
//		���U���g�p�I�u�W�F�N�g�̕`�揈��
//------------------------------------------------
void DrawResultObject(void)
{
	SetResultCamera();			//�J�����̐ݒ菈��
	DrawResultCylinder();		//�w�i�̕`�揈��
	DrawMeshfield();			//�n�ʂ̕`�揈��
	DrawPlayer_RESULT();		//�v���C���[�̕`�揈��
	DrawVictoryStand();			//�\����̕`�揈��
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------

���U���g��ʏ���[result.cpp]
Author:���V����

------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "fade.h"
#include "sound.h"
#include "file.h"
#include "model.h"
#include "resultPlayer.h"
#include "resultCamera.h"
#include "VictoryStand.h"
#include "resultCylinder.h"
#include "meshfield.h"
#include "conversioninput.h"

#define NUM_RESULT_UI		(3)			//�X�L�b�v���ɏ㉺���狲�ޗp�̖��i�Q�j�ƃX�L�b�v�\��UI�́@�v�R��
#define SKIP_UI_TEX			"data\\TEXTURE\\ResultSKipUI.png"		//�X�L�b�v�\��UI�̃e�N�X�`���p�X
#define SKIP_UI_POSX		(1050.0f)	//�X�L�b�v�\��UI��X�ʒu
#define SKIP_UI_POSY		(650.0f)	//�X�L�b�v�\��UI��Y�ʒu
#define SKIP_UI_WIDTH		(200.0f)	//�X�L�b�v�\��UI�̕�
#define SKIP_UI_HEIGHT		(100.0f)	//�X�L�b�v�\��UI�̍���

#define HALF_SCREEN_HEIGHT	(SCREEN_HEIGHT / 2)	//��ʔ����̍���
#define SKIP_SHUTTER_HEIGHT	(SCREEN_HEIGHT / 4)	//�V���b�^�[�̍����i��ʂ̔����̔���
#define SKIP_SHUTTER_POS_X	(SCREEN_WIDTH / 2)	//�V���b�^�[��X�ʒu(��ʂ̔���
#define SHUTTER_SPEED		(100.0f)			//�V���b�^�[�̊J���x

//���U���g�X�L�b�v�̍\����
typedef struct
{
	D3DXVECTOR3 pos[NUM_RESULT_UI];	//�eUI�̌��_�ʒu
	bool bShutter;					//�V���b�^�[�̊J��ON/OFF
	int nUI_DispCounter;			//UI�̕\������
}SkipUI;

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffResult = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTextureResult = NULL;	//�e�N�X�`���̃|�C���^
SkipUI					g_SkipUI;					//���U���g�X�L�b�v���̏��

//************************************************
//				���U���g�̏���������
//************************************************
void InitResult(void)
{
	//�f�o�C�X�ւ̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, SKIP_UI_TEX, &g_pTextureResult);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_RESULT_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < NUM_RESULT_UI; nCntResult++, pVtx += VTX_MAX)
	{
		//UI�̐ݒ�
		if (nCntResult == 0)
		{
			//UI�̈ʒu�ݒ�
			g_SkipUI.pos[nCntResult] = D3DXVECTOR3(SKIP_UI_POSX, SKIP_UI_POSY, 0.0f);

			//���_���W�̐ݒ�i���_����㉺���E�ɒ��_��ݒ肷��
			pVtx[0].pos = D3DXVECTOR3(SKIP_UI_POSX - SKIP_UI_WIDTH, SKIP_UI_POSY - SKIP_UI_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SKIP_UI_POSX + SKIP_UI_WIDTH, SKIP_UI_POSY - SKIP_UI_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SKIP_UI_POSX - SKIP_UI_WIDTH, SKIP_UI_POSY + SKIP_UI_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SKIP_UI_POSX + SKIP_UI_WIDTH, SKIP_UI_POSY + SKIP_UI_HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = XCOL_WHITE;
		}

		//�V���b�^�[�t���[���̐ݒ�
		// MEMO :�J�E���^�[���u�P�v�Ȃ��̃V���b�^�[�A�J�E���^�[���u�Q�v�Ȃ牺�̃V���b�^�[�Ƃ��Ďg�p����
		else
		{
			//��̃V���b�^�t���[���̏ꍇ
			if (nCntResult == 1)
			{
				//��̃V���b�^�[�t���[���̈ʒu�ݒ�
				g_SkipUI.pos[nCntResult] = D3DXVECTOR3(SKIP_SHUTTER_POS_X, -SKIP_SHUTTER_HEIGHT, 0.0f);
			}
			else
			{
				//��̃V���b�^�[�t���[���̈ʒu�ݒ�
				g_SkipUI.pos[nCntResult] = D3DXVECTOR3(SKIP_SHUTTER_POS_X, SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT, 0.0f);
			}

			//���_���W�̐ݒ�i���_����㉺���E�ɒ��_��ݒ肷��
			pVtx[0].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X - SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y - SKIP_SHUTTER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X + SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y - SKIP_SHUTTER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X - SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y + SKIP_SHUTTER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X + SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y + SKIP_SHUTTER_HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = XCOL_BLACK;
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = RHW;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffResult->Unlock();

	//----------------------------------------------=
	//���U���g�p�I�u�W�F�N�g�̏����������֐��Ăяo��
	//----------------------------------------------=
	InitResultObject();

	g_SkipUI.bShutter = true;	//�V���b�^�[���J����
	PlaySoundBGM(SOUND_LABEL_BGM_RESULT_01);	//�ŏ��̃��U���g���y�𗬂�
}

//------------------------------------------------
//		���U���g�p�I�u�W�F�N�g�̏���������
//------------------------------------------------
void InitResultObject(void)
{
	InitFile();				// �t�@�C��������
	LoadModelViewerFile("data\\model.txt");	// ���f���r���[���[�t�@�C���ǂݍ��݁i�e�I�u�W�F�N�g�������O�ɍs�����ƁI�j
	InitAnimalModel();		// �������f��������
	InitPlayer_RESULT();	// �v���C���[�̏���������
	InitMeshfield();		// �n�ʂ̏���������
	InitResultCylinder();	// �w�i�̏���������
	InitVictoryStand();		// �\����̏���������
	InitResultCamera();		// �J�����̏���������
}

//************************************************
//				���U���g�̏I������
//************************************************
void UninitResult(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
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
	//���U���g���΂�
	for (int nCntPad = 0; nCntPad < MAX_USE_GAMEPAD; nCntPad++)
	{
		//�L�[�{�[�h�́@ENTER�L�[�@�܂��́A�ڑ�����Ă���R���g���[���[��A�{�^���������ꂽ
		if (GetKeyboardTrigger(DIK_RETURN) || GetButton(nCntPad, INPUTTYPE_TRIGGER, BUTTON_A))
		{
			//���U���g���΂�������
			SkipResult();
			break;
		}
	}

	//�V���b�^�[����邩�J���邩
	MoveSkipShutter();

	//�X�L�b�vUI�̒��_���̍X�V
	UpdateSkipUIVtx();

	//----------------------------------------------=
	//���U���g�p�I�u�W�F�N�g�̍X�V�����֐��Ăяo��
	//----------------------------------------------=
	UpdateResultObject();
}

//���U���g���΂�
void SkipResult(void)
{
	//���݂̃E�F�[�u���擾
	WAVECamera *pWave = GetWaveCamera();

	switch (*pWave)
	{
		//------------------
		//	�S�E�F�[�u�I��
		//------------------
	case WAVECamera_FINISH:
		SetFade(MODE_TITLE);	//�^�C�g����ʂ�
		break;

		//------------------
		//�܂��E�F�[�u���I����ĂȂ�
		//------------------
	default:
		g_SkipUI.bShutter = false;				//�V���b�^�[�����
		break;
	}
}

//�V���b�^�[����邩�J���邩
void MoveSkipShutter(void)
{
	//���݂̃E�F�[�u���擾
	WAVECamera *pWave = GetWaveCamera();

	//�V���b�^�[���J����
	if (g_SkipUI.bShutter)
	{
		g_SkipUI.pos[1].y -= SHUTTER_SPEED;//��̃V���b�^�[����ɏグ��

		if (g_SkipUI.pos[1].y <= -SKIP_SHUTTER_HEIGHT)
		{//��̃V���b�^�[����ʂ��������
			g_SkipUI.pos[1].y = -SKIP_SHUTTER_HEIGHT;		//��ʂ̏�ɖ߂�
		}

		g_SkipUI.pos[2].y += SHUTTER_SPEED;//���̃V���b�^�[�����ɉ�����

		if (g_SkipUI.pos[2].y >= SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT)
		{//���̃V���b�^�[����ʂ��������
			g_SkipUI.pos[2].y = SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT;//��ʂ̉��ɖ߂�
		}
	}

	//�V���b�^�[�����
	else
	{
		//��̃V���b�^�[�����ɉ�����			//���̃V���b�^�[����ɏグ��
		g_SkipUI.pos[1].y += SHUTTER_SPEED;		g_SkipUI.pos[2].y -= SHUTTER_SPEED;

		//�ǂ��炩�̃V���b�^�[����ʂ̔����܂ŗ���
		if (g_SkipUI.pos[1].y >= HALF_SCREEN_HEIGHT - SKIP_SHUTTER_HEIGHT || 
			g_SkipUI.pos[2].y <= HALF_SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT)
		{
			//��ʂ̔����̈ʒu�ɖ߂�
			g_SkipUI.pos[1].y = HALF_SCREEN_HEIGHT - SKIP_SHUTTER_HEIGHT;
			g_SkipUI.pos[2].y = HALF_SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT;

			//�V���b�^�[���J����
			g_SkipUI.bShutter = true;

			switch (*pWave)
			{
				//-----------------
				//	�\����V�[��
				//-----------------
			case WAVECamera_LAST_VictoryStand:
				SkipVicStd_Player();		//�\����̓o����I��点�āA�v���C���[��\����ɗ�������
				*pWave = WAVECamera_FINISH;	//�E�F�[�u�I��
				StopSoundBGM(SOUND_LABEL_BGM_RESULT_01);	//�J�n���U���gBGM��~
				PlaySoundBGM(SOUND_LABEL_BGM_RESULT_02);	//�]�C���U���gBGM�Đ�
				break;

				//------------------
				//	�E�F�[�u�r��
				//------------------
			default:
				*pWave = WAVECamera_LAST_VictoryStand;	//�Ō�̃E�F�[�u��
				SetNextWave(*pWave);					//�Ō�̃E�F�[�u�ݒ��
				SpecialWave();							//���ꉉ�o�ݒ�
				break;
			}
		}
	}
}

//�X�L�b�vUI�̒��_���̍X�V����
void UpdateSkipUIVtx(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntShutter = 0; nCntShutter < NUM_RESULT_UI; nCntShutter++, pVtx += VTX_MAX)
	{
		if (nCntShutter != 0)
		{
			//���_���W�̐ݒ�i���������ς��Ȃ��̂ŁA�x���W�̐ݒ肾��
			pVtx[0].pos.y = g_SkipUI.pos[nCntShutter].y - SKIP_SHUTTER_HEIGHT;
			pVtx[1].pos.y = g_SkipUI.pos[nCntShutter].y - SKIP_SHUTTER_HEIGHT;
			pVtx[2].pos.y = g_SkipUI.pos[nCntShutter].y + SKIP_SHUTTER_HEIGHT;
			pVtx[3].pos.y = g_SkipUI.pos[nCntShutter].y + SKIP_SHUTTER_HEIGHT;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffResult->Unlock();
}

//------------------------------------------------
//		���U���g�p�I�u�W�F�N�g�̍X�V����
//------------------------------------------------
void UpdateResultObject(void)
{
	UpdatePlayer_RESULT();	//�v���C���[�̍X�V����
	UpdateResultCamera();	//�J�����̍X�V����
	UpdateVictoryStand();	//�\����̍X�V����
	UpdateResultCylinder();	//�w�i�̍X�V����
	UpdateMeshfield();		//�t�B�[���h�̍X�V����
}

//************************************************
//				���U���g�̕`�揈��
//MEMO:2DUI��`�悷��ꍇ�̓��e�X�g��Y�ꂸ�ɁI
//************************************************
void DrawResult(void)
{
	//�f�o�C�X�̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntResult = 0; nCntResult < NUM_RESULT_UI; nCntResult++)
	{
		//�X�L�b�v�\��UI�̓E�F�[�u�������̎��̂ݕ\������
		if (nCntResult == 0)
		{
			if (*GetWaveCamera() == WAVECamera_01_SideWays)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureResult);

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * VTX_MAX, 2);
			}
		}
		else
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * VTX_MAX, 2);
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

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
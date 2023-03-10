//==========================================
//
//�w�i�\���v���O����[SelPlayer_bg.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "bg.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureBg = {};	//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBg = NULL;	//���_�o�b�t�@�|�C���^

//========================
//�w�i����������
//========================
void InitSelPlayer_Bg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�w�i�ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\title000.png",
		&g_apTextureBg);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBg,
		NULL);

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 200.0f);
	pVtx[1].pos = D3DXVECTOR3((float)SCREEN_WIDTH, 0.0f, 200.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT, 200.0f);
	pVtx[3].pos = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 200.0f);

	//���W�ϊ��p�W���ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffBg->Unlock();
}

//========================
//�w�i�I������
//========================
void UninitSelPlayer_Bg(void)
{
	int nCntTexBG;	//�J�E���^

	//�e�N�X�`���̔j��
	if (g_apTextureBg != NULL)
	{
		g_apTextureBg->Release();
		g_apTextureBg = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffBg != NULL)
	{
		g_pVtxbuffBg->Release();
		g_pVtxbuffBg = NULL;
	}
}

//========================
//�w�i�X�V����
//========================
void UpdateSelPlayer_Bg(void)
{
	
}

//========================
//�w�i�`�揈��
//========================
void DrawSelPlayer_Bg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_apTextureBg);
	
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
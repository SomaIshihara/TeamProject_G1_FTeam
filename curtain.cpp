/*==========================================================================================================================================================

�J�������g���Ă��Ȃ������Ƃ��̃J�[�e��[curtain.h]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "curtain.h"
#include "color.h"

#define NUM_CURTAIN		(4)	//�J�[�e���̐�
#define PVP_CURTAIN_TEX	(6)	//PVP�̃J�[�e���e�N�X�`��
#define HDR_CURTAIN_TEX	(5)	//HDR�̃J�[�e���e�N�X�`��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureCurtain[NUM_CURTAIN] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCurtain = NULL;			//���_�o�b�t�@�ւ̃|�C���^

// PVP�̃J�[�e���e�N�X�`���̃t�@�C���p�X
const char *c_apPVPCurtainTex[PVP_CURTAIN_TEX] = {
	"data\\TEXTURE\\PVP_Curtain000.png",
	"data\\TEXTURE\\PVP_Curtain001.png",
	"data\\TEXTURE\\PVP_Curtain002.png",
	"data\\TEXTURE\\PVP_Curtain003.png",
	"data\\TEXTURE\\PVP_Curtain004.png",
	"data\\TEXTURE\\PVP_Curtain005.png",
};

// HDR�̃J�[�e���e�N�X�`���̃t�@�C���p�X
const char *c_apHDRCurtainTex[HDR_CURTAIN_TEX] = {
	"data\\TEXTURE\\HDR_Curtain000.png",
	"data\\TEXTURE\\HDR_Curtain001.png",
	"data\\TEXTURE\\HDR_Curtain002.png",
	"data\\TEXTURE\\HDR_Curtain003.png",
	"data\\TEXTURE\\HDR_Curtain004.png",
};

//-----------------------------------
//		PVP�̃J�[�e������������
//-----------------------------------
void InitSelModePvPCurtain(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_CURTAIN, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCurtain, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCurtain->Lock(0, 0, (void* *)&pVtx, 0);

	//========================
	//		���_���ݒ�
	//========================
	for (int nCntCurtain = 0; nCntCurtain < NUM_CURTAIN; nCntCurtain++, pVtx += VTX_MAX)
	{
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apPVPCurtainTex[rand() % PVP_CURTAIN_TEX], &g_pTextureCurtain[nCntCurtain]);

		//�e��ʂ̍���̌��_���W���Z�o
		float Pos_X = (SCREEN_WIDTH / 2)  * (nCntCurtain % EVENPARITY);
		float Pos_Y = (SCREEN_HEIGHT / 2) * (nCntCurtain / EVENPARITY);

		for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
		{
			pVtx[nCntVtx].pos.x = Pos_X + (SCREEN_WIDTH / 2)  * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].pos.y = Pos_Y + (SCREEN_HEIGHT / 2) * (nCntVtx / EVENPARITY);
			pVtx[nCntVtx].pos.z = 0.0f;

			pVtx[nCntVtx].tex.x = 1.0f * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].tex.y = 1.0f * (nCntVtx / EVENPARITY);

			D3DXVECTOR2 pos = D3DXVECTOR2(pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y);
			D3DXVECTOR2 tex = pVtx[nCntVtx].tex;

			pVtx[nCntVtx].rhw = RHW;
			pVtx[nCntVtx].col = RGBA_WHITE;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCurtain->Unlock();
}

//-----------------------------------
//		HDR�̃J�[�e������������
//-----------------------------------
void InitSelModeHDRCurtain(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_CURTAIN, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCurtain, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCurtain->Lock(0, 0, (void* *)&pVtx, 0);

	//========================
	//		���_���ݒ�
	//========================
	for (int nCntCurtain = 0; nCntCurtain < NUM_CURTAIN; nCntCurtain++, pVtx += VTX_MAX)
	{
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apHDRCurtainTex[rand() % HDR_CURTAIN_TEX], &g_pTextureCurtain[nCntCurtain]);

		//�e��ʂ̍���̌��_���W���Z�o
		float Pos_X = (SCREEN_WIDTH / 4) * nCntCurtain;

		for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
		{
			pVtx[nCntVtx].pos.x = Pos_X + (SCREEN_WIDTH / 4) * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].pos.y = SCREEN_HEIGHT * (nCntVtx / EVENPARITY);
			pVtx[nCntVtx].pos.z = 0.0f;

			pVtx[nCntVtx].tex.x = 1.0f * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].tex.y = 1.0f * (nCntVtx / EVENPARITY);

			D3DXVECTOR2 pos = D3DXVECTOR2(pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y);
			D3DXVECTOR2 tex = pVtx[nCntVtx].tex;

			pVtx[nCntVtx].rhw = RHW;
			pVtx[nCntVtx].col = RGBA_WHITE;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCurtain->Unlock();
}

//----------------------------
//		�J�[�e���I������
//----------------------------
void UninitCurtain(void)
{
	//�e�N�X�`���S�j��
	for (int nCntTex = 0; nCntTex < NUM_CURTAIN; nCntTex++)
	{
		if (g_pTextureCurtain[nCntTex] != NULL)
		{
			g_pTextureCurtain[nCntTex]->Release();
			g_pTextureCurtain[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCurtain != NULL)
	{
		g_pVtxBuffCurtain->Release();
		g_pVtxBuffCurtain = NULL;
	}
}

//----------------------------
//		�J�[�e���X�V����
//----------------------------
void UpdateCurtain(void)
{

}

//----------------------------
//		�J�[�e���`�揈��
//----------------------------
void DrawCurtain(int nCntCurtain)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCurtain, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCurtain[nCntCurtain]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCurtain * VTX_MAX, 2);
}
//--------------------------------------------------------------------------------------------------------
//
//���C�g����[light.cpp]
//Author:�e�n�z�l
//
//--------------------------------------------------------------------------------------------------------

#include "main.h"
#include "light.h"
#include "input.h"

//�}�N����`
#define	NUM_LIGHT		(3)		//���C�g�̐�

//�O���[�o���ϐ�
D3DLIGHT9 g_aLight[NUM_LIGHT];

//--------------------------------------------------------------------------------------------------------
//���C�g�̏���������
//--------------------------------------------------------------------------------------------------------
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	D3DXVECTOR3 aVecDir[NUM_LIGHT];		//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//���C�g�̏����N���A����
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));
	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	//���C�g�̎�ނ�ݒ�
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�̕�����ݒ�
	aVecDir[0] = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&aVecDir[0], &aVecDir[0]);		//�x�N�g���𐳋K������
	g_aLight[0].Direction = aVecDir[0];

	aVecDir[1] = D3DXVECTOR3(-0.2f, 0.8f, -0.4f);
	D3DXVec3Normalize(&aVecDir[1], &aVecDir[1]);		//�x�N�g���𐳋K������
	g_aLight[1].Direction = aVecDir[1];

	aVecDir[2] = D3DXVECTOR3(0.8f, -0.1f, 0.4f);
	D3DXVec3Normalize(&aVecDir[2], &aVecDir[2]);		//�x�N�g���𐳋K������
	g_aLight[2].Direction = aVecDir[2];

	//���C�g��ݒ肷��
	pDevice->SetLight(0, &g_aLight[0]);
	pDevice->SetLight(1, &g_aLight[1]);
	pDevice->SetLight(2, &g_aLight[2]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);
}



//--------------------------------------------------------------------------------------------------------
//���C�g�̏I������
//--------------------------------------------------------------------------------------------------------
void UninitLight(void)
{

}



//--------------------------------------------------------------------------------------------------------
//���C�g�̍X�V����
//--------------------------------------------------------------------------------------------------------
void UpdateLight(void)
{

}
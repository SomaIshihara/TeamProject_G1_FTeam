//--------------------------------------------------------------------------------------------------------
//
//���C�g����[light.cpp]
//Author:�e�n�z�l
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "light.h"
#include "input.h"
#include "color.h"

//�}�N����`
#define	NUM_LIGHT		(4)		//���C�g�̐�

//�O���[�o���ϐ�
D3DLIGHT9 g_aLight[NUM_LIGHT];

////���C�g�̌����i�[
//const D3DXVECTOR3 c_apLightVec[NUM_LIGHT] = {
//	D3DXVECTOR3(0.2f, -0.8f, 0.4f),
//	D3DXVECTOR3(-0.2f, 0.8f, -0.4f),
//	D3DXVECTOR3(0.8f, -0.1f, 0.4f),
//};

//--------------------------------------------------------------------------------------------------------
//���C�g�̏���������
//--------------------------------------------------------------------------------------------------------
void InitLight(void)
{
#if 0
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 aVecDir[NUM_LIGHT];		//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̕�����ݒ�
		aVecDir[nCntLight] = c_apLightVec[nCntLight];

		//���C�g�̏����N���A����
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		g_aLight[nCntLight].Diffuse = XCOL_WHITE;

		D3DXVec3Normalize(&aVecDir[nCntLight], &aVecDir[nCntLight]);		//�x�N�g���𐳋K������
		g_aLight[nCntLight].Direction = aVecDir[nCntLight];

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
#endif
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

//--------------------------------------------------------------------------------------------------------
//���C�g�̐ݒ菈��
//Author:�Ό��D�n
//--------------------------------------------------------------------------------------------------------
void SetLight(int nLightNum, D3DLIGHT9 light)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C�g�̏����N���A����
	ZeroMemory(&g_aLight[nLightNum], sizeof(D3DLIGHT9));

	//�����̃��C�g������
	g_aLight[nLightNum] = light;

	//���C�g�̎�ނ�ݒ�
	g_aLight[nLightNum].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g��ݒ肷��
	pDevice->SetLight(nLightNum, &g_aLight[nLightNum]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(nLightNum, TRUE);
}
//========================================
// 
// EFF:�Ռ��g[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_shock-wave_00.h ***
//========================================
#ifndef _EFF_SHOCKWAVE_00_H_		//���̃}�N����`������ĂȂ�������
#define _EFF_SHOCKWAVE_00_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************
// �v���g�^�C�v�錾
//*****************************************
// EFF:�Ռ��g[00] �̏���������
void InitEff_shockWave_00(void);
// EFF:�Ռ��g[00] �̏I������
void UninitEff_shockWave_00(void);
// EFF:�Ռ��g[00] �̍X�V����
void UpdateEff_shockWave_00(void);
// EFF:�Ռ��g[00] �̕`�揈��
void DrawEff_shockWave_00(void);

// EFF:�Ռ��g[00] �̐ݒ菈��
void SetEff_shockWave_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fThickness, float fEdgeHeight, Color inCol, Color outCol, int nLife);

#endif

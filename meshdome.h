//--------------------------------------------------------------------------------------------------------
//
//���b�V���h�[���̉����E�ޗ�[meshdome.h]
//Author:���V����
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHDOME_H_	//���̃}�N����`������ĂȂ�������
#define _MESHDOME_H_	//2�d�C���N���[�h�h�~�̃}�N�����`����

//�v���g�^�C�v�錾
void InitMeshDome(void);
void SetMeshDomeVertexBuffer(void);		//���_���W�̐ݒ�
 void SetMeshDomeIndexBuffer(void);		//�C���f�b�N�X�ԍ��̐ݒ�
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);

#endif
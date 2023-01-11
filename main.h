/*==========================================================================================================================================================

���C���̏���[main.h]
Author:��{����

============================================================================================================================================================*/

#ifndef _MAIN_H_						//���̃}�N����`����x������Ă��Ȃ�������
#define _MAIN_H_

#include <Windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"
#include <stdlib.h>
#include <stdio.h>

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

//�}�N����`
#define SCREEN_WIDTH	(1280)			   //�E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)	           //�E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//���_���(2D)�\����
typedef struct
{
	D3DXVECTOR3 pos; //���_���W
	float rhw;		 //���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;	 //���_�J���[
	D3DXVECTOR2 tex; //�e�N�X�`�����W
} VERTEX_2D;

//���_���(3D)�\����
typedef struct
{
	D3DXVECTOR3 pos; //���_���W
	D3DXVECTOR3 nor; //�@���x�N�g��
	D3DCOLOR col;	 //���_�J���[
	D3DXVECTOR2 tex; //�e�N�X�`�����W
} VERTEX_3D;

//���(���[�h)�̎��
typedef enum
{
	MODE_NONE = 0,
	MODE_TITLE,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_GAMERESULT,
	MODE_MAX
} MODE;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void); //�f�o�C�X
void SetMode(MODE mode); //��ʂ̃��[�h�ݒ�
MODE GetMode(void); //��ʃ��[�h�̎擾

#endif
#pragma once

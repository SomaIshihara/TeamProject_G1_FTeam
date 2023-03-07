//==========================================
//
//�Q�[���S�ʂ��Ǘ�����v���O�����̃w�b�_[main.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <XInput.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	//�r���h���̌x���Ώ�
#include "dinput.h"
#include "xaudio2.h"

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//���(���[�h)�̎��
typedef enum
{
	MODE_NONE = 0,
	MODE_TITLE,			//�^�C�g�����
	MODE_SELECTGAME,	//�Q�[���I��
	MODE_TUTORIAL,		//�`���[�g���A��
	MODE_SELECTPLAYER,	//�v���C���[�I�����
	MODE_PvPGAME,		//PvP�Q�[�����
	MODE_RaceGAME,		//���[�X���
	MODE_RESULT,		//���U���g���
	MODE_MAX
} MODE;

//�R���g���[���[�`�F�b�N���[�h��
typedef enum
{
	CHECKMODE_DISCONPAUSE = 0,	//�ؒf���ꂽ�狭���|�[�Y���郂�[�h
	CHECKMODE_DISCONNOPAUSE,	//�ؒf���ꂽ�������m�F���郂�[�h
	CHECKMODE_REMOVE,			//�g�p�R���g���[���[�폜���[�h
	CHECKMODE_MAX
} CHECKMODE;

//�}�N����`
#define CLASS_NAME				"WindowClass"
#define SCREEN_WIDTH			(1280)		//�N���C�A���g�̈�̕�
#define SCREEN_HEIGHT			(720)		//�N���C�A���g�̈�̍���
#define MAX_FPS					(60)		//�ő�t���[�����[�g
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//2D
#define FVF_VERTEX_3D			(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//3D
#define MAX_TEXTURE				(16)	//�e�N�X�`���ő吔
#define MAX_PATH_STR			(512)	//�ő�p�X������
#define ACCELERATION_GRAVITY	(9.8f)		//�d�͉����x

//�֗��Ȃ��
#define FIX_ROT(x)			(fmodf(x + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI)	//�p�x��-PI~PI�ɏC��
#define fatof(x)			(float)atof(x)	//�ʏ��atof����double�^�ŕԂ��Ă���̂�float�L���X�g���s���悤�ɂ�������
#define TASUKIGAKE(ax,az,bx,bz)	((az * bx) - (ax * bz))
#define PYTHAGORAS(a,b)		sqrtf(powf(a,2) + powf(b,2))
#define CONVERT_FPS(x)		(int)(x * MAX_FPS)	//�b����FPS�ɕϊ�

//�t�F�[�h�}�N��
#define END_FADE	(60)		//�I���]�C

#define RHW			(1.0f)

#define NIL_F		(0.0f)		//0.0f���W

#define ZERO_SET	(D3DXVECTOR3(NIL_F, NIL_F, NIL_F))	//�ʒu������̏������Ɏg�p

//�p�x�̃}�N��
#define UP			(1.0f)		//��
#define DOWN		(0.0f)		//��
#define RIGHT		(0.5f)		//�E
#define LEFT		(-0.5f)		//��
#define UP_RIGHT	(0.75f)		//�E��
#define UP_LEFT		(-0.75f)	//����
#define DOWN_RIGHT	(0.25f)		//�E��
#define DOWN_LEFT	(-0.25f)	//����
#define ONE_LAP		(D3DX_PI * 2.0f)	//�P�����̊p�x


#define MAX_CHAR	(256)		//�O���t�@�C���ǂݍ��݂ȂǂɎg�p����i�[���镶���̍ő吔

//���_���[2D]
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��W���i1.0f�ŌŒ�j
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
} VERTEX_2D;

//���_���[3D]
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`��
} VERTEX_3D;

//4�̒��_���W�̗񋓌^
typedef enum
{
	VTX_LE_UP = 0,	//����
	VTX_RI_UP,		//�E��
	VTX_LE_DO,		//����
	VTX_RI_DO,		//�E��
	VTX_MAX
}VTX;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetShowCursor(bool bDisp);		//�J�[�\���\���E��\���؂�ւ��itrue�ŕ\���Efalse�Ŕ�\���j
void SetMode(MODE mode); //��ʂ̃��[�h�ݒ�
MODE GetMode(void); //��ʃ��[�h�̎擾
void EndProject(void);

#endif // !_MAIN_H_

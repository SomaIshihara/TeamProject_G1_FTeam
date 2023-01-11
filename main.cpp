/*==========================================================================================================================================================

���C���̏���[main.cpp]
Author:��{����

============================================================================================================================================================*/

#include "main.h"
//#include "input.h"
//#include "camera.h"
//#include "light.h"
//#include "ground.h"
//#include "Player.h"
//#include "shadow.h"
//#include "bullet.h"
#include "wall.h"
#include <Windows.h>
//#include "block.h"
//#include "billboard.h"
//#include "meshfield.h"
//#include "title.h"
//#include "game.h"
//#include "result.h"
//#include "fade.h"
//#include "sound.h"
//#include "tutorial.h"



//�}�N����`
#define CLASS_NAME  "WindowClass"			//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME "C�E�t�H�[�h�̘Z�d��"		//�E�C���h�E�̖��O

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;
int g_nCountFPS = 0;
MODE g_mode = MODE_NONE;	//���݂̃��[�h

							//===================================================
							//���C���֐�
							//===================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdshow)
{
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//�������T�C�Y
		CS_CLASSDC,						//�X�^�C��
		WindowProc,						//�E�C���h�E�v���V�[�W��                                
		0,								//0�ɂ���
		0,								//0�ɂ���
		hInstance,						//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		//�N���C�A���g�̈�̔w�i�F
		NULL,							//���j���[�o�[
		CLASS_NAME,						//�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),
	};
	HWND hWnd;	//�E�C���h�E�n���h��
	MSG  msg;	//���b�Z�[�W���i�[����ϐ�

				//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//�E�C���h�E����
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//����\�̐ݒ�
	timeBeginPeriod(1);

	//�`�揈���֘A�̕ϐ��̏�����
	dwCurrentTime = 0;					//����������
	dwExecLastTime = timeGetTime();		//���ݎ������擾(�ۑ�)

										//FPS�֘A�̕ϐ���������
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//�E�C���h�E�\��
	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if ((PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0))
		{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
			if (msg.message == WM_QUIT)
			{
				break;
			}

			else
			{
				//���b�Z�[�W�ݒ�
				TranslateMessage(&msg);	//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);	//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime(); //���ݎ������擾(�ۑ�)

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					//�t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime; //�����J�n�̎���[���ݎ���]��ۑ�

												//�X�V����
				Update();

				//�`�揈��
				Draw();

				//�t���[���J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	//�I������
	Uninit();

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;

	//����\��߂�
	timeEndPeriod(1);
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: //�E�C���h�E�j���̃��b�Z�[�W
					 //WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:		//[ESC]�L�[�������ꂽ
							//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//����̏�����Ԃ�
}

//���C������������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;



	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�B�X�v���C�̃v���[���e�[�V�����p�����[�^�[�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//DirectX3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice,
		18, 0, 0, 0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal",
		&g_pFont);

	/*=====================================================================
	�e��I�u�W�F�N�g�̏���������
	=======================================================================*/
	//�����_�[�X�e�[�g
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�L�[�{�[�h�̏�����
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//���b�V���t�B�[���h
	//InitMeshfield();		

	//InitBillboard();

	//�t�F�[�h
	//InitFade(g_mode);

	//�T�E���h
	//InitSound(hWnd);

	// ���[�h���^�C�g���ɐݒ�
	SetMode(MODE_TITLE);

	return S_OK;
}

//���C���I������
void Uninit(void)
{
	//�L�[�{�[�h
	//UninitKeyboard();

	//���b�V���t�B�[���h
	//UninitMeshfield();	

	//UninitBillboard();

	//�t�F�[�h
	//UninitFade();

	//�^�C�g����ʂ̏I������
	//UninitTitle();

	//�`���[�g���A����ʂ̏I������
	//UninitTutorial();

	//�Q�[����ʂ̏I������
	//UninitGame();

	//���U���g��ʂ̏I������
	//UninitResult();

	//�T�E���h
	//UninitSound();

	//DirectX3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//DirectX3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//���C���X�V����
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	//UpdateKeyboard();

	//���b�V���t�B�[���h
	//UpdateMeshfield();

	//UpdateBillboard();

	switch (g_mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		//UpdateTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		//UpdateTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		//UpdateGame();
		break;

	case MODE_GAMERESULT:	//���U���g���
		//UpdateResult();
		break;
	}

	//�t�F�[�h
	//UpdateFade();
}

//���C���`�揈��
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ 			

	 //DrawBillboard();

		switch (g_mode)
		{
		case MODE_TITLE:		//�^�C�g�����
			//DrawTitle();
			break;

		case MODE_TUTORIAL:		//�`���[�g���A�����
			//DrawTutorial();
			break;

		case MODE_GAME:			//�Q�[�����
			//DrawGame();
			break;

		case MODE_GAMERESULT:	//���U���g���
			//DrawResult();
			break;
		}

		//�t�F�[�h
		//DrawFade();

		//FPS�̕\��
		DrawFPS();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//FPS�̕\��
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//������ɑ��
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕\��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//��ʂ̃��[�h�ݒ�
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
	switch (g_mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		//UninitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		//UninitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		//UninitGame();
		break;

	case MODE_GAMERESULT:	//���U���g���
		//UninitResult();
		break;
	}

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:		//�^�C�g�����
		//InitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		//InitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		//InitGame();
		break;

	case MODE_GAMERESULT:	//���U���g���
		//InitResult();
		break;
	}

	g_mode = mode;			//���݂̉��(���[�h)��؂�ւ���
}

//��ʃ��[�h�̎擾
MODE GetMode(void)
{
	return g_mode;
}
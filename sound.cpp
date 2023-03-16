//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : �Ό��D�n  ���V����
// �قƂ�ǂ̃v���O�������炪������ˁ@�ǂ����Ă��낤�ˁH << �Ό��N��
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SOUND_LOOP		(-1)		//���[�v����
#define SOUND_ONCE		(0)			//�P�񂩂���

//*****************************************************************************
// �T�E���h���̍\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g
	float Volume;		// �{�����[��
	bool bPlay;			//�Đ����邩�ǂ���
} SOUNDINFO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

HRESULT InitSoundBGM(HWND hWnd);
HRESULT InitSoundSE(HWND hWnd);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//BGM
IXAudio2 *g_pXAudio2BGM = NULL;										// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoiceBGM = NULL;				// �}�X�^�[�{�C�X
IXAudio2SourceVoice *g_apSourceVoiceBGM[SOUND_LABEL_BGM_MAX] = {};	// �\�[�X�{�C�X
BYTE *g_apDataAudioBGM[SOUND_LABEL_BGM_MAX] = {};	// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudioBGM[SOUND_LABEL_BGM_MAX] = {};	// �I�[�f�B�I�f�[�^�T�C�Y

//SE
IXAudio2 *g_pXAudio2SE = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoiceSE = NULL;			// �}�X�^�[�{�C�X
IXAudio2SourceVoice *g_apSourceVoiceSE[SOUND_LABEL_SE_MAX][NUM_DUPLICATION] = { {} };	// �\�[�X�{�C�X
BYTE *g_apDataAudioSE[SOUND_LABEL_SE_MAX][NUM_DUPLICATION] = { {} };	// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudioSE[SOUND_LABEL_SE_MAX][NUM_DUPLICATION] = { {} };	// �I�[�f�B�I�f�[�^�T�C�Y

//���̑�
bool g_bAllPlaySound = true;	// �Đ����邩�ǂ���

// BGM�T�E���h�̏��
SOUNDINFO g_aSoundInfoBGM[SOUND_LABEL_BGM_MAX] =
{
	{ "data/SOUND/BGM/Title.wav",			SOUND_LOOP, 0.7f,true },		// �^�C�g����
	{ "data/SOUND/BGM/Tutorial.wav",		SOUND_LOOP, 0.7f,true },		// �`���[�g���A����
	{ "data/SOUND/BGM/PvP_Game_Mixed.wav",	SOUND_LOOP, 0.7f,true },		// �Q�[����
	{ "data/SOUND/BGM/Game000.wav",			SOUND_LOOP, 0.7f,true },		// �Q�[����
	{ "data/SOUND/BGM/Result_01.wav",		SOUND_LOOP, 0.7f,true },		// ���U���g��1�i�J�n
	{ "data/SOUND/BGM/Result_02.wav",		SOUND_LOOP, 0.7f,true },		// ���U���g��2�i�]�C
};

// SE�T�E���h�̏��
SOUNDINFO g_aSoundInfoSE[SOUND_LABEL_SE_MAX] =
{
	{ "data/SOUND/SE/Warning-fence.wav",		 SOUND_ONCE, 0.5f, true },	// �t�F���X�������ӌ��ʉ�
	{ "data/SOUND/SE/PvP_Game_CountDown.wav",	 SOUND_ONCE, 0.5f, true },	// �J�E���g�_�E�����ʉ�
	{ "data/SOUND/SE/Uribou.wav",				 SOUND_ONCE, 0.5f, true },	// �R���g���[���[�ڑ�SE
	{ "data/SOUND/SE/Title_Decide.wav",			 SOUND_ONCE, 0.6f, true },	// �^�C�g���̌���SE
	{ "data/SOUND/SE/Collision.wav",			 SOUND_ONCE, 0.6f, true },	// �v���C���[���m�̏Փˉ�SE
	{ "data/SOUND/SE/Energy.wav",				 SOUND_ONCE, 0.6f, true },	// �v���C���[�̃Q�[�W�[�U��
	{ "data/SOUND/SE/Drop000.wav",				 SOUND_ONCE, 0.6f, true },	// �v���C���[�̗�����
	{ "data/SOUND/SE/GrassDash000.wav",			 SOUND_ONCE, 0.6f, true },	// �v���C���[�̃_�b�V����
	{ "data/SOUND/SE/HipdropSpin_001.wav",		 SOUND_ONCE, 0.6f, true },	// �v���C���[�̃q�b�v�h���b�v�X�s����
	{ "data/SOUND/SE/HipDrop_Attack_002.wav",	 SOUND_ONCE, 0.6f, true },	// �v���C���[�̃q�b�v�h���b�v��
	{ "data/SOUND/SE/Jamp001.wav",				 SOUND_ONCE, 0.4f, true },	// �v���C���[�̃W�����v��
	{ "data/SOUND/SE/HipDropRank00_Bad.wav",	 SOUND_ONCE, 0.5f, true },	// �q�b�v�h���b�v�����N �uBad...�v�̌��ʉ�
	{ "data/SOUND/SE/HipDropRank01_Nice.wav",	 SOUND_ONCE, 0.5f, true },	//			�V			�uNICE�v
	{ "data/SOUND/SE/HipDropRank02_Good.wav",	 SOUND_ONCE, 0.5f, true },	//			�V			�uGOOD!�v
	{ "data/SOUND/SE/HipDropRank03_Great.wav",	 SOUND_ONCE, 0.5f, true },	//			�V			�u��GREAT���v
	{ "data/SOUND/SE/HipDropRank04_PERFECT.wav", SOUND_ONCE, 0.4f, true },	//			�V			�u��PERFECT���v
	{ "data/SOUND/SE/piano_00_Do.wav",			 SOUND_ONCE, 0.4f, true },	// �u���b�N�j��	�P�ڂ̌��ʉ�
	{ "data/SOUND/SE/piano_01_Re.wav",			 SOUND_ONCE, 0.4f, true },	//		�V		�Q��
	{ "data/SOUND/SE/piano_02_Mi.wav",			 SOUND_ONCE, 0.4f, true },	//		�V		�R��
	{ "data/SOUND/SE/piano_03_Fa.wav",			 SOUND_ONCE, 0.5f, true },	//		�V		�S��
	{ "data/SOUND/SE/piano_04_So.wav",			 SOUND_ONCE, 0.5f, true },	//		�V		�T��
	{ "data/SOUND/SE/piano_05_Ra.wav",			 SOUND_ONCE, 0.5f, true },	//		�V		�U��
	{ "data/SOUND/SE/piano_06_Ti.wav",			 SOUND_ONCE, 0.6f, true },	//		�V		�V��	�p��Łu�V�v�́uTi�v�Ə����炵���ł�
	{ "data/SOUND/SE/piano_07_Do.wav",			 SOUND_ONCE, 0.7f, true },	//		�V		�W��
	{ "data/SOUND/SE/PauseDecision000.wav",		 SOUND_ONCE, 0.6f, true },	// �|�[�Y�̌��艹
	{ "data/SOUND/SE/PauseSelection000.wav",	 SOUND_ONCE, 0.6f, true },	// �|�[�Y�̑I����
	{ "data/SOUND/SE/PauseTransition000.wav",	 SOUND_ONCE, 0.6f, true },	// �|�[�Y�̑J�ډ�
	{ "data/SOUND/SE/Roar000.wav",				 SOUND_ONCE, 0.6f, true },	// �|�[�Y�̙��K��
	{ "data/SOUND/SE/Cheers.wav",				 SOUND_ONCE, 0.6f, true },	// ���U���g�̊���
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	//BGM
	if (FAILED(InitSoundBGM(hWnd)))
	{
		return E_FAIL;
	}

	//SE
	if (FAILED(InitSoundSE(hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//BGM�̏���������
HRESULT InitSoundBGM(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2BGM, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2BGM->CreateMasteringVoice(&g_pMasteringVoiceBGM);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (g_pXAudio2BGM != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2BGM->Release();
			g_pXAudio2BGM = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aSoundInfoBGM[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			// �I�[�f�B�I�f�[�^�ǂݍ���
			hr = CheckChunk(hFile, 'atad', &g_aSizeAudioBGM[nCntSound], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
			g_apDataAudioBGM[nCntSound] = (BYTE*)malloc(g_aSizeAudioBGM[nCntSound]);
			hr = ReadChunkData(hFile, g_apDataAudioBGM[nCntSound], g_aSizeAudioBGM[nCntSound], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �\�[�X�{�C�X�̐���
			hr = g_pXAudio2BGM->CreateSourceVoice(&g_apSourceVoiceBGM[nCntSound], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �o�b�t�@�̒l�ݒ�
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = g_aSizeAudioBGM[nCntSound];
			buffer.pAudioData = g_apDataAudioBGM[nCntSound];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = g_aSoundInfoBGM[nCntSound].nCntLoop;

			// �I�[�f�B�I�o�b�t�@�̓o�^
			g_apSourceVoiceBGM[nCntSound]->SubmitSourceBuffer(&buffer);
		}

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//SE�̏���������
HRESULT InitSoundSE(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2SE, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2SE->CreateMasteringVoice(&g_pMasteringVoiceSE);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (g_pXAudio2SE != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2SE->Release();
			g_pXAudio2SE = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aSoundInfoSE[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			// �I�[�f�B�I�f�[�^�ǂݍ���
			hr = CheckChunk(hFile, 'atad', &g_aSizeAudioSE[nCntSound][nCntDupl], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
			g_apDataAudioSE[nCntSound][nCntDupl] = (BYTE*)malloc(g_aSizeAudioSE[nCntSound][nCntDupl]);
			hr = ReadChunkData(hFile, g_apDataAudioSE[nCntSound][nCntDupl], g_aSizeAudioSE[nCntSound][nCntDupl], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �\�[�X�{�C�X�̐���
			hr = g_pXAudio2SE->CreateSourceVoice(&g_apSourceVoiceSE[nCntSound][nCntDupl], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �o�b�t�@�̒l�ݒ�
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = g_aSizeAudioSE[nCntSound][nCntDupl];
			buffer.pAudioData = g_apDataAudioSE[nCntSound][nCntDupl];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = g_aSoundInfoSE[nCntSound].nCntLoop;

			// �I�[�f�B�I�o�b�t�@�̓o�^
			g_apSourceVoiceSE[nCntSound][nCntDupl]->SubmitSourceBuffer(&buffer);
		}

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	//BGM
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		if (g_apSourceVoiceBGM[nCntSound] != NULL)
		{
			// �ꎞ��~
			g_apSourceVoiceBGM[nCntSound]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			g_apSourceVoiceBGM[nCntSound]->DestroyVoice();
			g_apSourceVoiceBGM[nCntSound] = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudioBGM[nCntSound]);
			g_apDataAudioBGM[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoiceBGM->DestroyVoice();
	g_pMasteringVoiceBGM = NULL;
	
	// XAudio2�I�u�W�F�N�g�̊J��
	if(g_pXAudio2BGM != NULL)
	{
		g_pXAudio2BGM->Release();
		g_pXAudio2BGM = NULL;
	}


	//SE
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceSE[nCntSound] != NULL)
			{
				// �ꎞ��~
				g_apSourceVoiceSE[nCntSound][nCntDupl]->Stop(0);

				// �\�[�X�{�C�X�̔j��
				g_apSourceVoiceSE[nCntSound][nCntDupl]->DestroyVoice();
				g_apSourceVoiceSE[nCntSound][nCntDupl] = NULL;

				// �I�[�f�B�I�f�[�^�̊J��
				free(g_apDataAudioSE[nCntSound][nCntDupl]);
				g_apDataAudioSE[nCntSound][nCntDupl] = NULL;
			}
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoiceSE->DestroyVoice();
	g_pMasteringVoiceSE = NULL;

	// XAudio2�I�u�W�F�N�g�̊J��
	if (g_pXAudio2SE != NULL)
	{
		g_pXAudio2SE->Release();
		g_pXAudio2SE = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �S�T�E���h�̒�~����
//=============================================================================
void StopAllSound(void)
{
	// BGM �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceBGM[nCntSound] != NULL)
			{
				// �ꎞ��~
				g_apSourceVoiceBGM[nCntSound]->Stop(0);
			}
		}
	}

	// SE �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceSE[nCntSound][nCntDupl] != NULL)
			{
				// �ꎞ��~
				g_apSourceVoiceSE[nCntSound][nCntDupl]->Stop(0);
			}
		}
	}
}

//BGM
//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT PlaySoundBGM(SOUND_LABEL_BGM label)
{
	//���x���̍Đ���ON		����		�Đ���ON
	if (g_aSoundInfoBGM[label].bPlay == true && g_bAllPlaySound == true)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudioBGM[label];
		buffer.pAudioData = g_apDataAudioBGM[label];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfoBGM[label].nCntLoop;

		// ��Ԏ擾
		g_apSourceVoiceBGM[label]->GetState(&xa2state);

		//���ʒ���
		g_apSourceVoiceBGM[label]->SetVolume(g_aSoundInfoBGM[label].Volume);

		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �ꎞ��~
			g_apSourceVoiceBGM[label]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_apSourceVoiceBGM[label]->FlushSourceBuffers();
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoiceBGM[label]->SubmitSourceBuffer(&buffer);

		// �Đ�
		g_apSourceVoiceBGM[label]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void StopSoundBGM(SOUND_LABEL_BGM label)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		// ��Ԏ擾
		g_apSourceVoiceBGM[label]->GetState(&xa2state);
		g_apSourceVoiceBGM[label]->SetVolume(g_aSoundInfoBGM[label].Volume);

		if (xa2state.BuffersQueued != 0)
		{// �Đ���
		 // �ꎞ��~
			g_apSourceVoiceBGM[label]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_apSourceVoiceBGM[label]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void StopSoundBGM(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceBGM[nCntSound] != NULL)
			{
				// �ꎞ��~
				g_apSourceVoiceBGM[nCntSound]->Stop(0);
			}
		}
	}
}

//=============================================================================
// �Z�O�����g�ꎞ��~(���x���w��)
//=============================================================================
void PauseSound(SOUND_LABEL_BGM label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoiceBGM[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		g_apSourceVoiceBGM[label]->Stop(0);
	}
}

//=============================================================================
// �Z�O�����g�ĊJ(���x���w��)
//=============================================================================
void RestartSound(SOUND_LABEL_BGM label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoiceBGM[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �ꎞ��~��
	 // �Đ�
		g_apSourceVoiceBGM[label]->Start(0);
	}
}

//SE
//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT PlaySoundSE(SOUND_LABEL_SE label, int nNumSound)
{
	//�T�E���h�ԍ����Z�o
	nNumSound %= NUM_DUPLICATION;

	//���x���̍Đ���ON		����		�Đ���ON
	if (g_aSoundInfoSE[label].bPlay == true && g_bAllPlaySound == true)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudioSE[label][nNumSound];
		buffer.pAudioData = g_apDataAudioSE[label][nNumSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfoSE[label].nCntLoop;

		// ��Ԏ擾
		g_apSourceVoiceSE[label][nNumSound]->GetState(&xa2state);

		//���ʒ���
		g_apSourceVoiceSE[label][nNumSound]->SetVolume(g_aSoundInfoSE[label].Volume);

		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �ꎞ��~
			g_apSourceVoiceSE[label][nNumSound]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_apSourceVoiceSE[label][nNumSound]->FlushSourceBuffers();
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoiceSE[label][nNumSound]->SubmitSourceBuffer(&buffer);

		// �Đ�
		g_apSourceVoiceSE[label][nNumSound]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void StopSoundSE(SOUND_LABEL_SE label)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		// ��Ԏ擾
		g_apSourceVoiceSE[label][nCntDupl]->GetState(&xa2state);
		g_apSourceVoiceSE[label][nCntDupl]->SetVolume(g_aSoundInfoSE[label].Volume);

		if (xa2state.BuffersQueued != 0)
		{// �Đ���
		 // �ꎞ��~
			g_apSourceVoiceSE[label][nCntDupl]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_apSourceVoiceSE[label][nCntDupl]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void StopSoundSE(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceSE[nCntSound][nCntDupl] != NULL)
			{
				// �ꎞ��~
				g_apSourceVoiceSE[nCntSound][nCntDupl]->Stop(0);
			}
		}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
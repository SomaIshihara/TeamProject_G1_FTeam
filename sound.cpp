//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : �Ό��D�n
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

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX][NUM_DUPLICATION] = { {} };	// �\�[�X�{�C�X
BYTE *g_apDataAudio[SOUND_LABEL_MAX][NUM_DUPLICATION] = { {} };	// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudio[SOUND_LABEL_MAX][NUM_DUPLICATION] = { {} };	// �I�[�f�B�I�f�[�^�T�C�Y
bool g_bPlay = true;											// �Đ����邩�ǂ���

// �T�E���h�̏��
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data/SOUND/BGM/Title.wav",				SOUND_LOOP, 0.5f,true },		// �^�C�g����
	{ "data/SOUND/BGM/Tutorial.wav",			SOUND_LOOP, 0.5f,true },		// �`���[�g���A����
	{ "data/SOUND/BGM/Game000.wav",				SOUND_LOOP, 0.5f,true },		// �Q�[����
	{ "data/SOUND/BGM/Result.wav",				SOUND_LOOP, 0.5f,true },		// ���U���g��
	{ "data/SOUND/SE/Uribou.wav",				SOUND_ONCE, 0.5f,true },		// �R���g���[���[�ڑ�SE
	{ "data/SOUND/SE/Title_Decide.wav",			SOUND_ONCE, 0.6f,true },		// �^�C�g���̌���SE
	{ "data/SOUND/SE/Collision.wav",			SOUND_ONCE, 0.6f,true },		// �v���C���[���m�̏Փˉ�SE
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// �v���C���[1�̃Q�[�W�[�U��
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// �v���C���[2�̃Q�[�W�[�U��
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// �v���C���[3�̃Q�[�W�[�U��
	{ "data/SOUND/SE/Energy.wav",				SOUND_ONCE, 0.6f,true },		// �v���C���[4�̃Q�[�W�[�U��
	{ "data/SOUND/SE/Drop000.wav",				SOUND_ONCE, 0.6f,true },		// �v���C���[�̗�����
	{ "data/SOUND/SE/GrassDash000.wav",			SOUND_ONCE, 0.6f,true },		// �v���C���[�̃_�b�V����
	{ "data/SOUND/SE/HipDrop_Attack_002.wav",	SOUND_ONCE, 0.6f,true },		// �v���C���[�̃q�b�v�h���b�v��
	{ "data/SOUND/SE/Jamp001.wav",				SOUND_ONCE, 0.6f,true },		// �v���C���[�̃W�����v��
	{ "data/SOUND/SE/PauseDecision000.wav",		SOUND_ONCE, 0.6f,true },		// �|�[�Y�̌��艹
	{ "data/SOUND/SE/PauseSelection000.wav",	SOUND_ONCE, 0.6f,true },		// �|�[�Y�̑I����
	{ "data/SOUND/SE/PauseTransition000.wav",	SOUND_ONCE, 0.6f,true },		// �|�[�Y�̑J�ډ�
	{ "data/SOUND/SE/Roar000.wav",				SOUND_ONCE, 0.6f,true },		// �|�[�Y�̙��K��
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
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
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
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
			hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound][nCntDupl], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
			g_apDataAudio[nCntSound][nCntDupl] = (BYTE*)malloc(g_aSizeAudio[nCntSound][nCntDupl]);
			hr = ReadChunkData(hFile, g_apDataAudio[nCntSound][nCntDupl], g_aSizeAudio[nCntSound][nCntDupl], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �\�[�X�{�C�X�̐���
			hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound][nCntDupl], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �o�b�t�@�̒l�ݒ�
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = g_aSizeAudio[nCntSound][nCntDupl];
			buffer.pAudioData = g_apDataAudio[nCntSound][nCntDupl];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = g_aSoundInfo[nCntSound].nCntLoop;

			// �I�[�f�B�I�o�b�t�@�̓o�^
			g_apSourceVoice[nCntSound][nCntDupl]->SubmitSourceBuffer(&buffer);
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
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// �ꎞ��~
				g_apSourceVoice[nCntSound][nCntDupl]->Stop(0);

				// �\�[�X�{�C�X�̔j��
				g_apSourceVoice[nCntSound][nCntDupl]->DestroyVoice();
				g_apSourceVoice[nCntSound][nCntDupl] = NULL;

				// �I�[�f�B�I�f�[�^�̊J��
				free(g_apDataAudio[nCntSound][nCntDupl]);
				g_apDataAudio[nCntSound][nCntDupl] = NULL;
			}
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	// XAudio2�I�u�W�F�N�g�̊J��
	if(g_pXAudio2 != NULL)
	{
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label/*, int nNumSound*/)
{
	int nNumSound = 0;

	//���x���̍Đ���ON		����		�Đ���ON
	if (g_aSoundInfo[label].bPlay == true && g_bPlay == true)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[label][nNumSound];
		buffer.pAudioData = g_apDataAudio[label][nNumSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfo[label].nCntLoop;

		// ��Ԏ擾
		g_apSourceVoice[label][nNumSound]->GetState(&xa2state);

		//���ʒ���
		g_apSourceVoice[label][nNumSound]->SetVolume(g_aSoundInfo[label].Volume);

		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �ꎞ��~
			g_apSourceVoice[label][nNumSound]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_apSourceVoice[label][nNumSound]->FlushSourceBuffers();
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[label][nNumSound]->SubmitSourceBuffer(&buffer);

		// �Đ�
		g_apSourceVoice[label][nNumSound]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		// ��Ԏ擾
		g_apSourceVoice[label][nCntDupl]->GetState(&xa2state);
		g_apSourceVoice[label][nCntDupl]->SetVolume(g_aSoundInfo[label].Volume);

		if (xa2state.BuffersQueued != 0)
		{// �Đ���
		 // �ꎞ��~
			g_apSourceVoice[label][nCntDupl]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			g_apSourceVoice[label][nCntDupl]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoice[nCntSound][nCntDupl] != NULL)
			{
				// �ꎞ��~
				g_apSourceVoice[nCntSound][nCntDupl]->Stop(0);
			}
		}
	}
}

//=============================================================================
// �Z�O�����g�ꎞ��~(���x���w��)
//=============================================================================
void PauseSound(SOUND_LABEL label) 
{
	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		XAUDIO2_VOICE_STATE xa2state;

		// ��Ԏ擾
		g_apSourceVoice[label][nCntDupl]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �Đ���
		 // �ꎞ��~
			g_apSourceVoice[label][nCntDupl]->Stop(0);
		}
	}
}

//=============================================================================
// �Z�O�����g�ĊJ(���x���w��)
//=============================================================================
void RestartSound(SOUND_LABEL label)
{
	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		XAUDIO2_VOICE_STATE xa2state;

		// ��Ԏ擾
		g_apSourceVoice[label][nCntDupl]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �ꎞ��~��
		 // �Đ�
			g_apSourceVoice[label][nCntDupl]->Start(0);
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